CREATE TABLE Users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    email VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL, -- Store hashed passwords
    identity ENUM('teacher', 'student') NOT NULL, -- Made NOT NULL for clarity
    id VARCHAR(50) UNIQUE, -- Public ID (Student ID or Teacher ID)
    creation_time DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP NOT NULL
);

-- Constraint to ensure 'id' is mandatory based on 'identity'
-- (Note: This might require triggers or application logic depending on the specific SQL dialect)
-- For simplicity in basic SQL, we often rely on application logic to enforce this.

CREATE TABLE Course (
    course_code VARCHAR(50) PRIMARY KEY NOT NULL,
    course_name VARCHAR(80) NOT NULL
);

CREATE TABLE CourseTeacher(
    course_code VARCHAR(50) NOT NULL,
    assigned_teacher INT NOT NULL,
    PRIMARY KEY(course_code, assigned_teacher),
    FOREIGN KEY (assigned_teacher) REFERENCES Users(user_id) -- Refers to user_id of a teacher
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (course_code) REFERENCES Course(course_code)
        ON DELETE CASCADE ON UPDATE CASCADE,
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
-- Add check constraint if supported, to ensure assigned_teacher is actually a teacher
-- ALTER TABLE CourseTeacher ADD CONSTRAINT chk_teacher CHECK ( (SELECT identity FROM Users WHERE user_id = assigned_teacher) = 'teacher' ); -- Syntax varies

CREATE TABLE Exam (
    exam_id INT AUTO_INCREMENT PRIMARY KEY,
    status ENUM('Scheduled','Running','Finished','Evaluating','Evaluated','Postponed') DEFAULT 'Scheduled',
    date DATE NOT NULL,
    start_time TIME NOT NULL,
    end_time TIME NOT NULL,
    late_submission_allowed BOOLEAN DEFAULT FALSE,
    penalty DECIMAL(5,2) DEFAULT 0.00 CHECK (penalty >= 0.00), -- Ensure penalty is non-negative
    num_ques_sets INT DEFAULT 1 CHECK (num_ques_sets >= 1), -- Ensure at least one set
    rulebook TEXT,
    full_marks INT CHECK (full_marks > 0), -- Ensure positive full marks
    exam_creation_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_updated_time DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    topic_name VARCHAR(255) NOT NULL,
    exam_no INT,
    course_code VARCHAR(50) NOT NULL,
    show_marks BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (course_code) REFERENCES Course(course_code)
      ON DELETE RESTRICT ON UPDATE CASCADE, -- Prevent deleting a course if exams exist
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP -- Duplicate column name? kept for now.
);

CREATE TABLE Participates (
    student_id VARCHAR(50) NOT NULL, -- Changed from allowing NULL
    exam_id INT NOT NULL,           -- Changed from allowing NULL
    set_code_no INT,
    PRIMARY KEY(student_id, exam_id),
    FOREIGN KEY (student_id) REFERENCES Users(id) -- Refers to the public id of a student
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (exam_id) REFERENCES Exam(exam_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
-- Add check constraint if supported, to ensure student_id is actually a student
-- ALTER TABLE Participates ADD CONSTRAINT chk_student CHECK ( (SELECT identity FROM Users WHERE id = student_id) = 'student' ); -- Syntax varies

CREATE TABLE PreviousCode (
    code_id INT AUTO_INCREMENT PRIMARY KEY,
    submitted_by VARCHAR(50), -- Can be NULL if the student user is deleted
    code TEXT NOT NULL,
    is_allowed BOOLEAN DEFAULT FALSE,
    submission_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_updated DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, -- Consider renaming to avoid conflict with last_updated_at
    FOREIGN KEY (submitted_by) REFERENCES Users(id) -- Refers to the public id of a student
        ON DELETE SET NULL ON UPDATE CASCADE, -- Set to NULL if student is deleted
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP -- Duplicate column name? kept for now.
);

CREATE TABLE UsePreviousCode (
    exam_id INT NOT NULL, -- Changed from allowing NULL
    code_id INT NOT NULL, -- Changed from allowing NULL
    PRIMARY KEY(exam_id, code_id),
    FOREIGN KEY (exam_id) REFERENCES Exam(exam_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (code_id) REFERENCES PreviousCode(code_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

CREATE TABLE Submission (
    submission_id INT AUTO_INCREMENT PRIMARY KEY,
    student_id VARCHAR(50) NOT NULL,
    exam_id INT NOT NULL,
    set_code VARCHAR(50),
    submitted_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(student_id, exam_id), -- a student can only submit once per exam
    FOREIGN KEY (student_id) REFERENCES Users(id) -- Refers to the public id of a student
        ON DELETE CASCADE ON UPDATE CASCADE, -- Keep submission even if student is deleted? Maybe RESTRICT or SET NULL? CASCADE for now.
    FOREIGN KEY(exam_id) REFERENCES Exam(exam_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

CREATE TABLE TeacherEvaluatesSubmission (
    teacher_id INT NOT NULL,    -- Changed from allowing NULL
    submission_id INT NOT NULL, -- Changed from allowing NULL
    marks DECIMAL(5,2) DEFAULT 0.00 CHECK (marks >= 0.00), -- Ensure non-negative marks
    penalty DECIMAL(5,2) DEFAULT 0.00 CHECK (penalty >= 0.00), -- Ensure non-negative penalty
    total_marks INT GENERATED ALWAYS AS (
        CASE
            WHEN (marks - penalty) < 0 THEN 0 -- Ensure total marks are not negative
            ELSE CEIL(marks - penalty)
        END
    ) STORED,
    comment TEXT,
    PRIMARY KEY(teacher_id, submission_id),
    FOREIGN KEY (teacher_id) REFERENCES Users(user_id) -- Refers to user_id of a teacher
        ON DELETE CASCADE ON UPDATE CASCADE, -- Or maybe RESTRICT? If teacher leaves, evaluation remains? CASCADE for now.
    FOREIGN KEY (submission_id) REFERENCES Submission(submission_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
