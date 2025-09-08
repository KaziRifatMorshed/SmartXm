<?php
$pdo = null;
session_start();
if (!isset($_SESSION["user_id"])) {
    header("Location: login.php");
    exit();
}
require_once "../includes/db.php";

// Fetch latest user info from DB
$stmt = $pdo->prepare(
    "SELECT name, email, student_id, role FROM users WHERE id = ?",
);
$stmt->execute([$_SESSION["user_id"]]);
$user = $stmt->fetch();

if (!$user) {
    // User not found (should not happen)
    session_destroy();
    header("Location: login.php");
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Profile - SmartXm Remote Server</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <style>
        body { background: #f4f4f4; }
        .profile-label { font-weight: bold; }
        .profile-value { margin-bottom: 8px; }
        .profile-card { max-width: 500px; margin: 60px auto; }
    </style>
</head>
<body>
    <div class="container">
        <div class="card profile-card shadow">
            <div class="card-body">
                <h2 class="card-title text-center mb-4">Your Profile</h2>
                <div class="mb-3">
                    <div class="profile-label">Name:</div>
                    <div class="profile-value"><?= htmlspecialchars(
                        $user["name"],
                    ) ?></div>
                </div>
                <div class="mb-3">
                    <div class="profile-label">Email:</div>
                    <div class="profile-value"><?= htmlspecialchars(
                        $user["email"],
                    ) ?></div>
                </div>
                <div class="mb-3">
                    <div class="profile-label">Student ID:</div>
                    <div class="profile-value"><?= htmlspecialchars(
                        $user["student_id"],
                    ) ?></div>
                </div>
                <div class="mb-3">
                    <div class="profile-label">Role:</div>
                    <div class="profile-value"><?= htmlspecialchars(
                        $user["role"],
                    ) ?></div>
                </div>
                <div class="d-flex justify-content-center gap-2 mt-4">
                    <a href="edit_profile.php" class="btn btn-primary">Edit Profile</a>
                    <a href="logout.php" class="btn btn-danger">Logout</a>
                </div>
            </div>
        </div>
    </div>
</body>
</html>
