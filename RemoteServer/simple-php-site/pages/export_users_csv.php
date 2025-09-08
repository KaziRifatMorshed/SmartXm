<?php
$pdo = null;
// export_users_csv.php
require_once "../includes/db.php";

// No file operations, just output CSV syntax to the browser

// Prepare CSV column headers
$headers = [
    "id",
    "name",
    "email",
    "password",
    "student_id",
    "role",
    "created_at",
];

// Output headers as CSV line
echo implode(
    ",",
    array_map(function ($h) {
        // Escape double quotes and wrap in quotes
        return '"' . str_replace('"', '""', $h) . '"';
    }, $headers),
) . "<br>";

// Fetch all users
$stmt = $pdo->query(
    "SELECT id, name, email, password, student_id, role, created_at FROM users ORDER BY id ASC",
);
while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
    // Output each row as CSV line
    echo implode(
        ",",
        array_map(function ($v) {
            return '"' . str_replace('"', '""', $v) . '"';
        }, $row),
    ) . "<br>";
}
?>
