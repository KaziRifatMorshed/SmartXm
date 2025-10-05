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

-- 2. CRITICAL FIX: Select the database to work in
USE SmartXmRemoteServer;

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
('Alice Smith', '230201@ku.ac.bd', 'password123', '230201', 'Student'),
('Bob Johnson', '240205@ku.ac.bd', 'mypassword', '240205', 'Student'),
('Carol Williams', '210236@ku.ac.bd', 'securepass', '210236', 'Student'),
('David Brown', 'david.brown@cse.ku.ac.bd', 'letmein', 'T20001', 'Teacher'),
('Eve Davis', '250202@ku.ac.bd', 'passw0rd', '250202', 'Student'),
('Frank Miller', 'frank.miller@cse.ku.ac.bd', 'qwerty', 'T20002', 'Teacher');

*/

?>
