<?php
$pdo = null;
// export_users_csv.php
require_once "../includes/db.php";

// http://localhost/simple-php-site/pages/ecsv.php?keyChabi=iAmSuperSecret123&seed=7

// Only allow functionality if $_GET['keyChabi'] is equal to "iAmSuperSecret123"
if (isset($_GET["keyChabi"]) && $_GET["keyChabi"] === "iAmSuperSecret123") {

    // Use provided seed or fallback
    $seed = isset($_GET["seed"]) ? intval($_GET["seed"]) : 5;

    // Helper function to XOR-encode a string
    function xor_encode($str, $seed) {
        $out = "";
        for ($i = 0; $i < strlen($str); $i++) {
            $out .= chr(ord($str[$i]) ^ $seed);
        }
        return $out;
    }

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

    // Output headers (XOR encoded)
    $headerLine = implode(
        ",",
        array_map(function ($h) {
            return '"' . str_replace('"', '""', $h) . '"';
        }, $headers)
    );
    echo xor_encode($headerLine . "\n", $seed);

    // Fetch all users
    $stmt = $pdo->query(
        "SELECT id, name, email, password, student_id, role, created_at FROM users ORDER BY id ASC"
    );
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $line = implode(
            ",",
            array_map(function ($v) {
                return '"' . str_replace('"', '""', $v) . '"';
            }, $row)
        );
        echo xor_encode($line . "\n", $seed);
    }
}
?>
