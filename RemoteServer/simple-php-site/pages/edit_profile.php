<?php
$pdo = null;
session_start();
require_once "../includes/db.php";

if (!isset($_SESSION["user_id"])) {
    header("Location: login.php");
    exit();
}

$user_id = $_SESSION["user_id"];
$error = "";
$success = "";

// Fetch current user data
$stmt = $pdo->prepare(
    "SELECT name, email, student_id, role FROM users WHERE id = ?",
);
$stmt->execute([$user_id]);
$user = $stmt->fetch();

if (!$user) {
    session_destroy();
    header("Location: login.php");
    exit();
}

// Handle form submission
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $name = trim($_POST["name"] ?? "");
    $email = trim($_POST["email"] ?? "");
    $student_id = trim($_POST["student_id"] ?? "");
    $role = $_POST["role"] ?? "Student";
    $password = $_POST["password"] ?? "";
    $password_confirm = $_POST["password_confirm"] ?? "";

    // Validation
    if (
        !$name ||
        !$email ||
        !$student_id ||
        !in_array($role, ["Teacher", "Student"])
    ) {
        $error = "All fields except password are required.";
    } elseif (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
        $error = "Invalid email format.";
    } elseif ($password && $password !== $password_confirm) {
        $error = "Passwords do not match.";
    } else {
        // Check for unique email and student_id (excluding current user)
        $stmt = $pdo->prepare(
            "SELECT id FROM users WHERE (email = ? OR student_id = ?) AND id != ?",
        );
        $stmt->execute([$email, $student_id, $user_id]);
        if ($stmt->fetch()) {
            $error = "Email or Student ID already in use by another account.";
        } else {
            // Update user
            if ($password) {
                $hashed_password = password_hash($password, PASSWORD_DEFAULT);
                $update = $pdo->prepare(
                    "UPDATE users SET name=?, email=?, student_id=?, role=?, password=? WHERE id=?",
                );
                $update->execute([
                    $name,
                    $email,
                    $student_id,
                    $role,
                    $hashed_password,
                    $user_id,
                ]);
            } else {
                $update = $pdo->prepare(
                    "UPDATE users SET name=?, email=?, student_id=?, role=? WHERE id=?",
                );
                $update->execute([$name, $email, $student_id, $role, $user_id]);
            }
            // Update session info
            $_SESSION["name"] = $name;
            $_SESSION["email"] = $email;
            $_SESSION["student_id"] = $student_id;
            $_SESSION["role"] = $role;
            $success = "Profile updated successfully.";
            // Refresh user data
            $user["name"] = $name;
            $user["email"] = $email;
            $user["student_id"] = $student_id;
            $user["role"] = $role;
        }
    }
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Edit Profile - SmartXm Remote Server</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <style>
        body { background: #f4f4f4; }
        .container-custom { max-width: 500px; margin: 60px auto; background: #fff; padding: 30px; border-radius: 8px; box-shadow: 0 2px 8px rgba(0,0,0,0.1);}
        .actions { text-align: center; margin-top: 20px; }
    </style>
</head>
<body>
    <div class="container container-custom">
        <h2 class="text-center mb-4">Edit Profile</h2>
        <?php if ($error): ?>
            <div class="alert alert-danger"><?= htmlspecialchars(
                $error,
            ) ?></div>
        <?php endif; ?>
        <?php if ($success): ?>
            <div class="alert alert-success"><?= htmlspecialchars(
                $success,
            ) ?></div>
        <?php endif; ?>
        <form method="post" action="edit_profile.php" autocomplete="on">
            <div class="mb-3">
                <label for="name" class="form-label">Name:</label>
                <input type="text" name="name" id="name" class="form-control" required value="<?= htmlspecialchars(
                    $user["name"],
                ) ?>">
            </div>
            <div class="mb-3">
                <label for="email" class="form-label">Email:</label>
                <input type="email" name="email" id="email" class="form-control" required value="<?= htmlspecialchars(
                    $user["email"],
                ) ?>">
            </div>
            <div class="mb-3">
                <label for="student_id" class="form-label">Student ID:</label>
                <input type="text" name="student_id" id="student_id" class="form-control" required value="<?= htmlspecialchars(
                    $user["student_id"],
                ) ?>">
            </div>
            <div class="mb-3">
                <label for="role" class="form-label">Role:</label>
                <select name="role" id="role" class="form-select" required>
                <option value="Student" <?= $user["role"] === "Student"
                    ? "selected"
                    : "" ?>>Student</option>
                <option value="Teacher" <?= $user["role"] === "Teacher"
                    ? "selected"
                    : "" ?>>Teacher</option>
                </select>
            </div>
            <div class="mb-3">
                <label for="password" class="form-label">New Password (leave blank to keep current):</label>
                <input type="password" name="password" id="password" class="form-control" autocomplete="new-password">
            </div>
            <div class="mb-3">
                <label for="password_confirm" class="form-label">Confirm New Password:</label>
                <input type="password" name="password_confirm" id="password_confirm" class="form-control" autocomplete="new-password">
            </div>
            <button type="submit" class="btn btn-primary w-100">Update Profile</button>
        </form>
        <div class="actions mt-3">
            <a href="profile.php" class="btn btn-link">Back to Profile</a>
        </div>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>
