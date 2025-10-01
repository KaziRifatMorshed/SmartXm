CREATE TABLE Teacher (
    teacher_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    designation VARCHAR(50),
    email VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);

CREATE TABLE Student (
    student_id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    email VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

CREATE TABLE Course (
    course_code VARCHAR(50) PRIMARY KEY NOT NULL,
    course_name VARCHAR(80) NOT NULL
);

CREATE TABLE CourseTeacher(
    course_code VARCHAR(50) NOT NULL,
    assigned_teacher INT NOT NULL,
    PRIMARY KEY(course_code, assigned_teacher),
    FOREIGN KEY (assigned_teacher) REFERENCES Teacher(teacher_id),
    FOREIGN KEY (course_code) REFERENCES Course(course_code)
);

CREATE TABLE Exam (
    exam_id INT AUTO_INCREMENT PRIMARY KEY,
    status ENUM('Scheduled','Running','Finished','Evaluating','Evaluated','Postponed') DEFAULT 'Scheduled',
    date DATE NOT NULL,
    start_time TIME NOT NULL,
    end_time TIME NOT NULL,
    late_submission_allowed BOOLEAN DEFAULT FALSE,
    penalty DECIMAL(5,2) DEFAULT 0.00,
    num_ques_sets INT DEFAULT 1,
    rulebook TEXT,
    full_marks INT,
    exam_creation_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_updated_time DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    topic_name VARCHAR(255) NOT NULL,
    exam_no INT,
    course_code VARCHAR(50) NOT NULL,
    FOREIGN KEY (course_code) REFERENCES Course(course_code)
);

CREATE TABLE Participates (
    student_id VARCHAR(50),
    exam_id INT,
    set_code_no INT,
    PRIMARY KEY(student_id, exam_id),
    FOREIGN KEY (student_id) REFERENCES Student(student_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (exam_id) REFERENCES Exam(exam_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE PreviousCode (
    code_id INT AUTO_INCREMENT PRIMARY KEY,
    submitted_by VARCHAR(50), -- nullable so ON DELETE SET NULL works
    code TEXT NOT NULL,
    is_allowed BOOLEAN DEFAULT FALSE,
    submission_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_updated DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (submitted_by) REFERENCES Student(student_id)
        ON DELETE SET NULL ON UPDATE CASCADE
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

CREATE TABLE Submission (
    submission_id INT AUTO_INCREMENT PRIMARY KEY,
    student_id VARCHAR(50) NOT NULL,
    exam_id INT NOT NULL,
    set_code VARCHAR(50),
    submitted_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(student_id, exam_id), -- a student can only submit once per exam
    FOREIGN KEY (student_id) REFERENCES Student(student_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY(exam_id) REFERENCES Exam(exam_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE TeacherEvaluatesSubmission (
    teacher_id INT,
    submission_id INT,
    marks DECIMAL(5,2) DEFAULT 0.00,
    penalty DECIMAL(5,2) DEFAULT 0.00,
    total_marks INT GENERATED ALWAYS AS (CEIL(marks - penalty)) STORED,
    comment TEXT,
    PRIMARY KEY(teacher_id, submission_id),
    FOREIGN KEY (teacher_id) REFERENCES Teacher(teacher_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (submission_id) REFERENCES Submission(submission_id)
        ON DELETE CASCADE ON UPDATE CASCADE
);
