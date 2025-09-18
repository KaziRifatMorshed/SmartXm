<?php
// Database configuration
$host = "localhost";
$db = "smartxmremoteserver";
$user = "root";
$pass = "";
$charset = "utf8mb4";

// Data Source Name
$dsn = "mysql:host=$host;dbname=$db;charset=$charset";

// Options for PDO
$options = [
    PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES => false,
];

// Create PDO instance
try {
    $pdo = new PDO($dsn, $user, $pass, $options);
} catch (\PDOException $e) {
    throw new \PDOException($e->getMessage(), (int) $e->getCode());
}

/*
-- SQL to create the database and user table:

CREATE DATABASE IF NOT EXISTS SmartXmRemoteServer CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL, -- STORED AS RAW/PLAIN TEXT (NOT HASHED)
    student_id VARCHAR(50) UNIQUE,
    role ENUM('Teacher', 'Student') NOT NULL DEFAULT 'Student',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL, -- STORED AS RAW/PLAIN TEXT (NOT HASHED)
    student_id VARCHAR(50) UNIQUE,
    role ENUM('Teacher', 'Student') NOT NULL DEFAULT 'Student',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Dummy data for users table

INSERT INTO users (name, email, password, student_id, role) VALUES
('Alice Smith', 'alice.smith@example.com', 'password123', 'S10001', 'Student'),
('Bob Johnson', 'bob.johnson@example.com', 'mypassword', 'S10002', 'Student'),
('Carol Williams', 'carol.williams@example.com', 'securepass', 'S10003', 'Student'),
('David Brown', 'david.brown@example.com', 'letmein', 'T20001', 'Teacher'),
('Eve Davis', 'eve.davis@example.com', 'passw0rd', 'S10004', 'Student'),
('Frank Miller', 'frank.miller@example.com', 'qwerty', 'T20002', 'Teacher');

*/

?>
