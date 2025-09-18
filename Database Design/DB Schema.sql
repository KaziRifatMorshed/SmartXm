CREATE TABLE Teacher (
    teacher_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(500) NOT NULL,
    designation VARCHAR(50),
    email VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);

CREATE TABLE Student (
    student_id VARCHAR(50) PRIMARY KEY,  -- stu_id string PK
    name VARCHAR(100) NOT NULL,
    email VARCHAR(150) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

CREATE TABLE Exam (
    exam_id INT AUTO_INCREMENT PRIMARY KEY,
    status ENUM('scheduled','running','finished','evaluating','evaluated','postponed') DEFAULT 'scheduled',
    date DATE NOT NULL,
    start_time TIME NOT NULL,
    end_time TIME NOT NULL,
    late_submission_allowed BOOLEAN DEFAULT FALSE,
    penalty DECIMAL(5,2) DEFAULT 0.00,
    num_ques_sets INT DEFAULT 1,
    rulebook TEXT,
    marks INT,
    exam_creation_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_updated_time DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    topic_name VARCHAR(255),
    exam_no INT,
    show_marks BOOLEAN DEFAULT FALSE
);

CREATE TABLE Course (
    course_code VARCHAR(50),
    course_name VARCHAR(80),
    assigned_teacher INT FOREIGN KEY Teacher(teacher_id)
)

CREATE TABLE PreviousCode (
    code_id INT AUTO_INCREMENT PRIMARY KEY,
    submitted_by VARCHAR(50),
    code TEXT NOT NULL,
    is_allowed BOOLEAN DEFAULT TRUE,
    last_updated DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (submitted_by) REFERENCES Student(student_id)
        ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE Submission (
    submission_id INT AUTO_INCREMENT PRIMARY KEY,
    submitted_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    ques_no INT NOT NULL,
    set_code VARCHAR(50),
    code TEXT NOT NULL
);

CREATE TABLE Participates (
    student_id VARCHAR(50),
    exam_id INT,
    PRIMARY KEY(student_id, exam_id),
    FOREIGN KEY (student_id) REFERENCES Student(student_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (exam_id) REFERENCES Exam(exam_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE StudentSubmission (
    student_id VARCHAR(50),
    submission_id INT,
    PRIMARY KEY(student_id, submission_id),
    FOREIGN KEY (student_id) REFERENCES Student(student_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (submission_id) REFERENCES Submission(submission_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE SubmitPrevCode (
    submit_id INT AUTO_INCREMENT PRIMARY KEY,
    student_id VARCHAR(50),
    code_id INT,
    submission_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_updated_time DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES Student(student_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (code_id) REFERENCES PreviousCode(code_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE UsePreviousCode (
    exam_id INT,
    code_id INT,
    PRIMARY KEY(exam_id, code_id),
    FOREIGN KEY (exam_id) REFERENCES Exam(exam_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (code_id) REFERENCES PreviousCode(code_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE TeacherEvaluatesSubmission (
    teacher_id INT,
    submission_id INT,
    marks DECIMAL(5,2),
    penalty DECIMAL(5,2),
    comment TEXT,
    PRIMARY KEY(teacher_id, submission_id),
    FOREIGN KEY (teacher_id) REFERENCES Teacher(teacher_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (submission_id) REFERENCES Submission(submission_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

