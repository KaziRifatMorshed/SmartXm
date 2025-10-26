#ifndef DB_XAMPP_H
#define DB_XAMPP_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <iostream>
const QString DB_NAME = "SmartXmRemoteServer";

class localDB {
public:
  static localDB *DB() {
    if (_dbInstance == nullptr) {
      _dbInstance = new localDB();
    }
    return _dbInstance;
  }

  void testDB() {
    prepareDB();
    insertTestData();
  }

  // Public method to execute arbitrary SQL queries from anywhere
  QSqlQuery execQuery(const QString &queryStr) {
    QSqlQuery query(qSqlDB);
    bool success = query.exec(queryStr);
    if (!success) {
      qDebug() << "SQL error:" << query.lastError().text();
    }
    return query;
  }

protected:
  QSqlDatabase qSqlDB;

  QList<QString> getDriverNames() {
    QList listOfDrivers = QSqlDatabase::drivers();
    qDebug() << listOfDrivers;
    return QSqlDatabase::drivers().toList();
  }

private:
  static localDB *_dbInstance;
  localDB() { startDB(); }
  ~localDB() {
    if (qSqlDB.isOpen())
      qSqlDB.close();
  }
  // Disable copy/move
  localDB(const localDB &) = delete;
  localDB &operator=(const localDB &) = delete;

  /*
   * This database is designed in such a way that, the teacher does not have to
   * login into phpMyAdmin This program itself will create database, tables and
   * insert data to do so: we need to firstly establish a connection with 'test'
   * database (which comes by default with XAMPP installation since, our desired
   * database is not yet cerated, we need to connect with an existing DB and
   * execute our desired db creation mysql query then. change to our desired DB
   */
  void startDB(const QString &host = "127.0.0.1", int port = 3306) {
    qSqlDB = QSqlDatabase::addDatabase("QMARIADB"); // or "QMYSQL"
    qSqlDB.setHostName(host);
    qSqlDB.setPort(port);
    qSqlDB.setDatabaseName("test"); // or mysql // test better
    qSqlDB.setUserName("root");
    qSqlDB.setPassword("");

    if (!qSqlDB.open()) {
      qDebug() << "Initial Database connection (to 'test') failed:"
               << qSqlDB.lastError().text();
      return;
    } else {
      qDebug() << "Initial DB Connection successful (to 'test')!";
    }

    QString dropDBquery = QString("DROP DATABASE `%1`;").arg(DB_NAME);
    QSqlQuery dropDB;
    if (dropDB.exec(dropDBquery)) {
      qDebug() << "Database '" << DB_NAME << "' dropped successfully.";
    } else {
      qDebug() << "Error dropping database:" << dropDB.lastError().text();
    }

    QString createDBquery =
        QString("CREATE DATABASE IF NOT EXISTS %1 "
                "CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;")
            .arg(DB_NAME);
    QSqlQuery createDB;
    if (createDB.exec(createDBquery)) {
      qDebug() << "Database '" << DB_NAME
               << "' created successfully (or already existed).";
    } else {
      qDebug() << "Error creating database:" << createDB.lastError().text();
    }

    QString selectDB = QString("USE %1;").arg(DB_NAME);
    QSqlQuery cnjDB;
    if (cnjDB.exec(selectDB)) {
      std::cout << "Successfully switched to database '"
                << DB_NAME.toStdString() << "'" << std::endl;
    } else {
      qDebug() << "Error switching database:" << cnjDB.lastError().text();
      return;
    }
  }

#include <QStringList> // Be sure to include this

  void prepareDB() {
    QString createUserTable =
        "CREATE TABLE IF NOT EXISTS Users ("
        "    user_id INT AUTO_INCREMENT PRIMARY KEY,"
        "    name VARCHAR(50) NOT NULL,"
        "    email VARCHAR(50) NOT NULL UNIQUE,"
        "    password VARCHAR(255) NOT NULL,"
        "    identity ENUM('teacher', 'student') NOT NULL,"
        "    id VARCHAR(50) UNIQUE,"
        "    creation_time DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,"
        "    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE "
        "CURRENT_TIMESTAMP NOT NULL"
        ");";

    QString createCourseTable =
        "CREATE TABLE IF NOT EXISTS Course ("
        "    course_code VARCHAR(50) PRIMARY KEY NOT NULL,"
        "    course_name VARCHAR(80) NOT NULL"
        ");";

    QString createCourseTeacherTable =
        "CREATE TABLE IF NOT EXISTS CourseTeacher ("
        "    course_code VARCHAR(50) NOT NULL,"
        "    assigned_teacher INT NOT NULL,"
        "    PRIMARY KEY(course_code, assigned_teacher),"
        "    FOREIGN KEY (assigned_teacher) REFERENCES Users(user_id)"
        "        ON DELETE CASCADE ON UPDATE CASCADE,"
        "    FOREIGN KEY (course_code) REFERENCES Course(course_code)"
        "        ON DELETE CASCADE ON UPDATE CASCADE,"
        "    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE "
        "CURRENT_TIMESTAMP"
        ");";

    QString createExamTable =
        "CREATE TABLE IF NOT EXISTS Exam ("
        "    exam_id INT AUTO_INCREMENT PRIMARY KEY,"
        "    status "
        "ENUM('Scheduled','Running','Finished','Evaluating','Evaluated','"
        "Postponed') DEFAULT 'Scheduled',"
        "    date DATE NOT NULL,"
        "    start_time TIME NOT NULL,"
        "    end_time TIME NOT NULL,"
        "    late_submission_allowed BOOLEAN DEFAULT FALSE,"
        "    penalty DECIMAL(5,2) DEFAULT 0.00 CHECK (penalty >= 0.00),"
        "    num_ques_sets INT DEFAULT 1 CHECK (num_ques_sets >= 1),"
        "    rulebook TEXT,"
        "    full_marks INT CHECK (full_marks > 0),"
        "    exam_creation_time DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    topic_name VARCHAR(255) NOT NULL,"
        "    exam_no INT,"
        "    course_code VARCHAR(50) NOT NULL,"
        "    show_marks BOOLEAN DEFAULT FALSE,"
        "    FOREIGN KEY (course_code) REFERENCES Course(course_code)"
        "        ON DELETE RESTRICT ON UPDATE CASCADE,"
        "    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE "
        "CURRENT_TIMESTAMP"
        ");";

    QString createParticipatesTable =
        "CREATE TABLE IF NOT EXISTS Participates ("
        "    student_id VARCHAR(50) NOT NULL,"
        "    exam_id INT NOT NULL,"
        "    set_code_no INT,"
        "    PRIMARY KEY(student_id, exam_id),"
        "    FOREIGN KEY (student_id) REFERENCES Users(id)"
        "        ON DELETE CASCADE ON UPDATE CASCADE,"
        "    FOREIGN KEY (exam_id) REFERENCES Exam(exam_id)"
        "        ON DELETE CASCADE ON UPDATE CASCADE,"
        "    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE "
        "CURRENT_TIMESTAMP"
        ");";

    QString createPreviousCodeTable =
        "CREATE TABLE IF NOT EXISTS PreviousCode ("
        "    code_id INT AUTO_INCREMENT PRIMARY KEY,"
        "    submitted_by VARCHAR(50),"
        "    code TEXT NOT NULL,"
        "    is_allowed BOOLEAN DEFAULT FALSE,"
        "    submission_time DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    FOREIGN KEY (submitted_by) REFERENCES Users(id)"
        "        ON DELETE SET NULL ON UPDATE CASCADE,"
        "    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE "
        "CURRENT_TIMESTAMP"
        ");";

    QString createUsePreviousCodeTable =
        "CREATE TABLE IF NOT EXISTS UsePreviousCode ("
        "    exam_id INT NOT NULL,"
        "    code_id INT NOT NULL,"
        "    PRIMARY KEY(exam_id, code_id),"
        "    FOREIGN KEY (exam_id) REFERENCES Exam(exam_id)"
        "        ON DELETE CASCADE ON UPDATE CASCADE,"
        "    FOREIGN KEY (code_id) REFERENCES PreviousCode(code_id)"
        "        ON DELETE CASCADE ON UPDATE CASCADE,"
        "    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE "
        "CURRENT_TIMESTAMP"
        ");";

    QString createSubmissionTable =
        "CREATE TABLE IF NOT EXISTS Submission ("
        "    submission_id INT AUTO_INCREMENT PRIMARY KEY,"
        "    student_id VARCHAR(50) NOT NULL,"
        "    exam_id INT NOT NULL,"
        "    set_code VARCHAR(50),"
        "    submitted_time DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    UNIQUE(student_id, exam_id),"
        "    FOREIGN KEY (student_id) REFERENCES Users(id)"
        "        ON DELETE RESTRICT ON UPDATE CASCADE," // Changed from CASCADE
        "    FOREIGN KEY(exam_id) REFERENCES Exam(exam_id)"
        "        ON DELETE CASCADE ON UPDATE CASCADE,"
        "    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE "
        "CURRENT_TIMESTAMP"
        ");";

    QString createTeacherEvaluatesTable =
        "CREATE TABLE IF NOT EXISTS TeacherEvaluatesSubmission ("
        "    teacher_id INT NOT NULL,"
        "    submission_id INT NOT NULL,"
        "    marks DECIMAL(5,2) DEFAULT 0.00 CHECK (marks >= 0.00),"
        "    penalty DECIMAL(5,2) DEFAULT 0.00 CHECK (penalty >= 0.00),"
        "    total_marks INT GENERATED ALWAYS AS ("
        "        CASE"
        "            WHEN (marks - penalty) < 0 THEN 0"
        "            ELSE CEIL(marks - penalty)"
        "        END"
        "    ) STORED,"
        "    comment TEXT,"
        "    PRIMARY KEY(teacher_id, submission_id),"
        "    FOREIGN KEY (teacher_id) REFERENCES Users(user_id)"
        "        ON DELETE RESTRICT ON UPDATE CASCADE," // Changed from CASCADE
        "    FOREIGN KEY (submission_id) REFERENCES Submission(submission_id)"
        "        ON DELETE CASCADE ON UPDATE CASCADE,"
        "    last_updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE "
        "CURRENT_TIMESTAMP"
        ");";

    // Put all table creation strings into a list
    QStringList tables;
    tables << createUserTable << createCourseTable << createCourseTeacherTable
           << createExamTable << createParticipatesTable
           << createPreviousCodeTable << createUsePreviousCodeTable
           << createSubmissionTable << createTeacherEvaluatesTable;

    QSqlQuery query;
    int tablesCreated = 0;

    // Loop through the list and execute each query
    foreach (const QString &tableSql, tables) {
      if (!query.exec(tableSql)) {
        // If one fails, print the error and stop
        qDebug() << "Error creating table:" << query.lastError().text();
        qDebug() << "Failing query:" << tableSql;
        return; // Exit the function
      } else {
        tablesCreated++;
      }
    }

    if (tablesCreated == tables.size()) {
      std::cout << "All tables created successfully or already exist."
                << std::endl;
    }
  }

  void insertTestData() {
    // --- 5. Insert Data ---
    // Each INSERT statement MUST be in its own string.

    QString insertUsers =
        "INSERT IGNORE INTO Users (name, email, password, identity, id) VALUES"
        "        ('Alice Smith', '230201@ku.ac.bd', 'hashed_pass_123', "
        "'student', '230201'),"
        "        ('Bob Johnson', '210236@ku.ac.bd', 'hashed_pass_abc', "
        "'student', '210236'),"
        "        ('Carol Williams', '220215@ku.ac.bd', 'hashed_pass_xyz', "
        "'student', '220215'),"
        "        ('Dr. David Brown', 'david.brown@cse.ku.ac.bd', "
        "'teacher_pass_1', 'teacher', 'T20001'),"
        "        ('Dr. Frank Miller', 'frank.miller@cse.ku.ac.bd', "
        "'teacher_pass_2', 'teacher', 'T20002'),"
        "        ('Eve Davis', '230202@ku.ac.bd', 'hashed_pass_789', "
        "'student', '230202'),"
        "        ('Charlie Brown', '230203@ku.ac.bd', 'hashed_pass_230203', "
        "'student', '230203'),"
        "        ('Diana Prince', '230204@ku.ac.bd', 'hashed_pass_230204', "
        "'student', '230204'),"
        "        ('Edward Scissorhands', '230205@ku.ac.bd', "
        "'hashed_pass_230205', 'student', '230205'),"
        "        ('Fiona Glenanne', '230206@ku.ac.bd', 'hashed_pass_230206', "
        "'student', '230206'),"
        "        ('George Kirk', '230207@ku.ac.bd', 'hashed_pass_230207', "
        "'student', '230207'),"
        "        ('Hannah Abbott', '230208@ku.ac.bd', 'hashed_pass_230208', "
        "'student', '230208'),"
        "        ('Isaac Newton', '230209@ku.ac.bd', 'hashed_pass_230209', "
        "'student', '230209'),"
        "        ('Jane Doe', '230210@ku.ac.bd', 'hashed_pass_230210', "
        "'student', '230210'),"
        "        ('Kevin McCallister', '230211@ku.ac.bd', "
        "'hashed_pass_230211', 'student', '230211'),"
        "        ('Linda Hamilton', '230212@ku.ac.bd', 'hashed_pass_230212', "
        "'student', '230212'),"
        "        ('Michael Scott', '230213@ku.ac.bd', 'hashed_pass_230213', "
        "'student', '230213'),"
        "        ('Nora Ephron', '230214@ku.ac.bd', 'hashed_pass_230214', "
        "'student', '230214'),"
        "        ('Oscar Wilde', '230215@ku.ac.bd', 'hashed_pass_230215', "
        "'student', '230215'),"
        "        ('Pam Beesly', '230216@ku.ac.bd', 'hashed_pass_230216', "
        "'student', '230216'),"
        "        ('Quentin Tarantino', '230217@ku.ac.bd', "
        "'hashed_pass_230217', 'student', '230217'),"
        "        ('Rachel Green', '230218@ku.ac.bd', 'hashed_pass_230218', "
        "'student', '230218'),"
        "        ('Steve Rogers', '230219@ku.ac.bd', 'hashed_pass_230219', "
        "'student', '230219'),"
        "        ('Tony Stark', '230220@ku.ac.bd', 'hashed_pass_230220', "
        "'student', '230220'),"
        "        ('Ursula Buffay', '230221@ku.ac.bd', 'hashed_pass_230221', "
        "'student', '230221'),"
        "        ('Victor Krum', '230222@ku.ac.bd', 'hashed_pass_230222', "
        "'student', '230222'),"
        "        ('Walter White', '230223@ku.ac.bd', 'hashed_pass_230223', "
        "'student', '230223'),"
        "        ('Xena Warrior', '230224@ku.ac.bd', 'hashed_pass_230224', "
        "'student', '230224'),"
        "        ('Yara Greyjoy', '230225@ku.ac.bd', 'hashed_pass_230225', "
        "'student', '230225'),"
        "        ('Zinedine Zidane', '230226@ku.ac.bd', 'hashed_pass_230226', "
        "'student', '230226'),"
        "        ('Arthur Weasley', '230227@ku.ac.bd', 'hashed_pass_230227', "
        "'student', '230227'),"
        "        ('Bella Swan', '230228@ku.ac.bd', 'hashed_pass_230228', "
        "'student', '230228'),"
        "        ('Carl Grimes', '230229@ku.ac.bd', 'hashed_pass_230229', "
        "'student', '230229'),"
        "        ('Dory Nemo', '230230@ku.ac.bd', 'hashed_pass_230230', "
        "'student', '230230'),"
        "        ('Elsa Frozen', '230231@ku.ac.bd', 'hashed_pass_230231', "
        "'student', '230231'),"
        "        ('Fred Weasley', '230232@ku.ac.bd', 'hashed_pass_230232', "
        "'student', '230232'),"
        "        ('Ginny Weasley', '230233@ku.ac.bd', 'hashed_pass_230233', "
        "'student', '230233'),"
        "        ('Harry Potter', '230234@ku.ac.bd', 'hashed_pass_230234', "
        "'student', '230234'),"
        "        ('Ignotus Peverell', '230235@ku.ac.bd', 'hashed_pass_230235', "
        "'student', '230235'),"
        "        ('Jack Sparrow', '230236@ku.ac.bd', 'hashed_pass_230236', "
        "'student', '230236'),"
        "        ('Kate Austen', '230237@ku.ac.bd', 'hashed_pass_230237', "
        "'student', '230237'),"
        "        ('Luke Skywalker', '230238@ku.ac.bd', 'hashed_pass_230238', "
        "'student', '230238'),"
        "        ('Mona Lisa', '230239@ku.ac.bd', 'hashed_pass_230239', "
        "'student', '230239'),"
        "        ('Ned Stark', '230240@ku.ac.bd', 'hashed_pass_230240', "
        "'student', '230240'),"
        "        ('Olivia Rodriguez', '240201@ku.ac.bd', 'hashed_pass_240201', "
        "'student', '240201'),"
        "        ('Peter Parker', '240202@ku.ac.bd', 'hashed_pass_240202', "
        "'student', '240202'),"
        "        ('Quinn Fabray', '240203@ku.ac.bd', 'hashed_pass_240203', "
        "'student', '240203'),"
        "        ('Robert Bruce', '240204@ku.ac.bd', 'hashed_pass_240204', "
        "'student', '240204'),"
        "        ('Sarah Connor', '240205@ku.ac.bd', 'hashed_pass_240205', "
        "'student', '240205'),"
        "        ('Tom Riddle', '240206@ku.ac.bd', 'hashed_pass_240206', "
        "'student', '240206'),"
        "        ('Uma Thurman', '240207@ku.ac.bd', 'hashed_pass_240207', "
        "'student', '240207'),"
        "        ('Vito Corleone', '240208@ku.ac.bd', 'hashed_pass_240208', "
        "'student', '240208'),"
        "        ('Wade Wilson', '240209@ku.ac.bd', 'hashed_pass_240209', "
        "'student', '240209'),"
        "        ('Xavier Thorpe', '240210@ku.ac.bd', 'hashed_pass_240210', "
        "'student', '240210'),"
        "        ('Yasmine Ali', '240211@ku.ac.bd', 'hashed_pass_240211', "
        "'student', '240211'),"
        "        ('Zachary Taylor', '240212@ku.ac.bd', 'hashed_pass_240212', "
        "'student', '240212'),"
        "        ('Adam Sandler', '240213@ku.ac.bd', 'hashed_pass_240213', "
        "'student', '240213'),"
        "        ('Ben Stiller', '240214@ku.ac.bd', 'hashed_pass_240214', "
        "'student', '240214'),"
        "        ('Chris Rock', '240215@ku.ac.bd', 'hashed_pass_240215', "
        "'student', '240215'),"
        "        ('Drew Barrymore', '240216@ku.ac.bd', 'hashed_pass_240216', "
        "'student', '240216'),"
        "        ('Emma Stone', '240217@ku.ac.bd', 'hashed_pass_240217', "
        "'student', '240217'),"
        "        ('Finn Wolfhard', '240218@ku.ac.bd', 'hashed_pass_240218', "
        "'student', '240218'),"
        "        ('Gal Gadot', '240219@ku.ac.bd', 'hashed_pass_240219', "
        "'student', '240219'),"
        "        ('Hugh Jackman', '240220@ku.ac.bd', 'hashed_pass_240220', "
        "'student', '240220'),"
        "        ('Ian McKellen', '240221@ku.ac.bd', 'hashed_pass_240221', "
        "'student', '240221'),"
        "        ('Jennifer Lawrence', '240222@ku.ac.bd', "
        "'hashed_pass_240222', 'student', '240222'),"
        "        ('Keanu Reeves', '240223@ku.ac.bd', 'hashed_pass_240223', "
        "'student', '240223'),"
        "        ('Leonardo DiCaprio', '240224@ku.ac.bd', "
        "'hashed_pass_240224', 'student', '240224'),"
        "        ('Margot Robbie', '240225@ku.ac.bd', 'hashed_pass_240225', "
        "'student', '240225'),"
        "        ('Natalie Portman', '240226@ku.ac.bd', 'hashed_pass_240226', "
        "'student', '240226'),"
        "        ('Owen Wilson', '240227@ku.ac.bd', 'hashed_pass_240227', "
        "'student', '240227'),"
        "        ('Paul Rudd', '240228@ku.ac.bd', 'hashed_pass_240228', "
        "'student', '240228'),"
        "        ('Ryan Reynolds', '240229@ku.ac.bd', 'hashed_pass_240229', "
        "'student', '240229'),"
        "        ('Scarlett Johansson', '240230@ku.ac.bd', "
        "'hashed_pass_240230', 'student', '240230'),"
        "        ('Tom Hanks', '240231@ku.ac.bd', 'hashed_pass_240231', "
        "'student', '240231'),"
        "        ('Vin Diesel', '240232@ku.ac.bd', 'hashed_pass_230232', "
        "'student', '230232'),"
        "        ('Will Smith', '240233@ku.ac.bd', 'hashed_pass_240233', "
        "'student', '240233'),"
        "        ('Zoe Saldana', '240234@ku.ac.bd', 'hashed_pass_240234', "
        "'student', '240234'),"
        "        ('Amy Adams', '240235@ku.ac.bd', 'hashed_pass_240235', "
        "'student', '240235'),"
        "        ('Bradley Cooper', '240236@ku.ac.bd', 'hashed_pass_240236', "
        "'student', '240236'),"
        "        ('Chris Hemsworth', '240237@ku.ac.bd', 'hashed_pass_240237', "
        "'student', '240237'),"
        "        ('Dwayne Johnson', '240238@ku.ac.bd', 'hashed_pass_240238', "
        "'student', '240238'),"
        "        ('Emily Blunt', '240239@ku.ac.bd', 'hashed_pass_240239', "
        "'student', '240239'),"
        "        ('George Clooney', '240240@ku.ac.bd', 'hashed_pass_240240', "
        "'student', '240240'),"
        "        ('Henry Cavill', '240241@ku.ac.bd', 'hashed_pass_240241', "
        "'student', '240241'),"
        "        ('Jessica Chastain', '240242@ku.ac.bd', 'hashed_pass_240242', "
        "'student', '240242');";

    QString insertCourses =
        "INSERT IGNORE INTO Course (course_code, course_name) VALUES"
        "        ('CSE 1102', 'Structured Programming Laboratory'),"
        "        ('CSE 2102', 'Data Structures Laboratory'),"
        "        ('CSE 2206', 'Database Systems Project/Fieldwork'),"
        "        ('CSE 3108', 'Operating System and Systems Programming "
        "Laboratory/Project'),"
        "        ('CSE 3202', 'Artificial Intelligence "
        "Laboratory/Project/Fieldwork');";

    QString insertCourseTeachers =
        "INSERT IGNORE INTO CourseTeacher (course_code, assigned_teacher) "
        "VALUES"
        "        ('CSE 2102', 4),"  // -- Dr. David Brown (user_id 4)
        "        ('CSE 2206', 5),"  // -- Dr. Frank Miller (user_id 5)
        "        ('CSE 3108', 4),"  // -- Dr. David Brown (user_id 4)
        "        ('CSE 3202', 5);"; // -- Dr. Frank Miller (user_id 5)

    QString insertExams =
        "INSERT IGNORE INTO Exam (status, date, start_time, end_time, "
        "full_marks, topic_name, exam_no, course_code, show_marks) VALUES"
        "        ('Evaluated', '2025-05-10', '09:00:00', '12:00:00', 100, "
        "'Final Lab Exam', 1, 'CSE 2102', TRUE),"
        "        ('Evaluating', '2025-05-15', '09:00:00', '17:00:00', 50, "
        "'Project Final Submission', 1, 'CSE 2206', FALSE),"
        "        ('Scheduled', '2025-06-01', '10:00:00', '11:30:00', 40, "
        "'Midterm Lab Test', 2, 'CSE 2102', FALSE);";

    QString insertParticipates = "INSERT IGNORE INTO Participates (student_id, "
                                 "exam_id, set_code_no) VALUES"
                                 "        ('230201', 1, 1),"
                                 "        ('210236', 1, 2),"
                                 "        ('220215', 1, 1),"
                                 "        ('230202', 1, 2),"
                                 "        ('210236', 2, 1),"
                                 "        ('220215', 2, 1),"
                                 "        ('230201', 3, 1),"
                                 "        ('230202', 3, 2);";

    QString insertPreviousCodes =
        "INSERT IGNORE INTO PreviousCode (submitted_by, code, is_allowed) "
        "VALUES"
        "        ('210236', 'public static int binarySearch(int[] arr, int "
        "target) { ... }', TRUE),"
        "        ('220215', 'private void connectDB() { ... }', FALSE),"
        "        ('230205', 'function calculateArea(radius) {\n  return "
        "Math.PI * radius * radius;\n}', TRUE),"
        "        ('240210', 'SELECT student_id, COUNT(exam_id) AS "
        "exams_taken\nFROM Participates\nGROUP BY student_id\nHAVING "
        "COUNT(exam_id) > 2;', TRUE),"
        "        ('210236', 'for i in range(10):\n    print(f\"Iteration "
        "{i+1}\")', FALSE),"
        "        ('230215', 'class Node {\n  int data;\n  Node next;\n  "
        "Node(int d) { data = d; next = NULL; }\n}', TRUE),"
        "        ('230215', 'public int factorial(int n) {\n  if (n == 0) "
        "return 1;\n  else return n * factorial(n-1);\n}', FALSE),"
        "        ('240201', 'UPDATE Exam SET status = ''Finished'' WHERE "
        "end_time < NOW();', TRUE),"
        "        ('230230', '#include <iostream>\nint main() {\n  std::cout << "
        "\"Hello World!\";\n  return 0;\n}', FALSE),"
        "        ('220215', 'const fetchData = async (url) => {\n  const "
        "response = await fetch(url);\n  return await response.json();\n}', "
        "TRUE),"
        "        ('240222', 'CREATE INDEX idx_student_exam ON Participates "
        "(student_id, exam_id);', TRUE),"
        "        ('230208', 'while (current != null) {\n  "
        "process(current.data);\n  current = current.next;\n}', FALSE);";

    // Put all insert statements into a list
    QStringList inserts;
    inserts << insertUsers << insertCourses << insertCourseTeachers
            << insertExams << insertParticipates << insertPreviousCodes;

    QSqlQuery query;
    int successfulInserts = 0;

    // Loop through the list and execute each query one by one
    foreach (const QString &sql, inserts) {
      if (!query.exec(sql)) {
        // If one fails, print the error and the failing query
        qDebug() << "Error inserting test data:" << query.lastError().text();
        qDebug() << "Failing query:" << sql;
      } else {
        successfulInserts++;
      }
    }

    if (successfulInserts == inserts.size()) {
      std::cout << "All test data inserted successfully (or already existed)."
                << std::endl;
    } else {
      qDebug() << "Executed " << successfulInserts << " out of "
               << inserts.size() << " insert statements.";
    }

    // --- 6. Test and Print Data ---
    // (This part is fine as it's commented out)
  }
};

/*
void _testDBchanging() { // BACKUP
  // 1. Initial Connection: Connect to a default database to gain permissions
  //    to create a new one. "test" or "" (empty string) works here.
  QSqlDatabase db1 = QSqlDatabase::addDatabase("QMARIADB"); // or "QMYSQL"
  db1.setHostName("127.0.0.1");
  db1.setPort(3306);
  // Connect initially to a database that is guaranteed to exist (e.g., "mysql"
  // or "test")
  db1.setDatabaseName("test");
  db1.setUserName("root");
  db1.setPassword("");

  if (!db1.open()) {
    qDebug() << "Initial Database connection (to 'test') failed:"
             << db1.lastError().text();
    return;
  } else {
    qDebug() << "Initial Connection successful (to 'test')!";
  }

  // --- 2. Drop and Create the Database (if it doesn't exist) ---
  QString dropDBquery = QString("DROP DATABASE `%1`;").arg(DB_NAME);
  QSqlQuery dropDB;
  if (dropDB.exec(dropDBquery)) {
    qDebug() << "Database '" << DB_NAME << "' dropped successfully.";
  } else {
    qDebug() << "Error dropping database:" << dropDB.lastError().text();
  }

  QString createDBquery =
      QString("CREATE DATABASE IF NOT EXISTS %1 "
              "CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;")
          .arg(DB_NAME);
  QSqlQuery createDB;
  if (createDB.exec(createDBquery)) {
    qDebug() << "Database '" << DB_NAME
             << "' created successfully (or already existed).";
  } else {
    qDebug() << "Error creating database:" << createDB.lastError().text();
    // Return is not strictly necessary as table creation will fail later, but
    // good for debugging.
  }

  // --- 3. Switch the active database for this connection ---
  // NOTE: This is necessary to target the newly created DB for table creation.
  QString selectDB = QString("USE %1;").arg(DB_NAME);
  QSqlQuery cnjDB;
  if (cnjDB.exec(selectDB)) {
    std::cout << "Successfully switched to database '" << DB_NAME.toStdString()
              << "'" << std::endl;
  } else {
    qDebug() << "Error switching database:" << cnjDB.lastError().text();
    db1.close();
    return;
  }

  // --- 4. Create the Table (MUST be run after USE) ---
  // Removed "USE SmartXmRemoteServer" from the beginning of the CREATE TABLE
  // statement.
  QString createTable =
      "CREATE TABLE IF NOT EXISTS users ("
      "id INT AUTO_INCREMENT PRIMARY KEY,"
      "name VARCHAR(100) NOT NULL,"
      "email VARCHAR(100) NOT NULL UNIQUE,"
      "password VARCHAR(255) NOT NULL, "
      "student_id VARCHAR(50) UNIQUE,"
      "role ENUM('Teacher', 'Student') NOT NULL DEFAULT 'Student',"
      "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
      ");";

  QSqlQuery createTableDB;
  if (createTableDB.exec(createTable)) {
    std::cout << "Table 'users' created successfully." << std::endl;
  } else {
    qDebug() << "Error creating table:" << createTableDB.lastError().text();
  }

  // --- 5. Insert Data ---
  QString insertData =
      "INSERT IGNORE INTO users (name, email, password, " // Use IGNORE to skip
                                                          // existing unique
                                                          // keys
      "student_id, role) VALUES "
      "('Alice Smith', 'alice.smith@example.com', 'password123', '230201', "
      "'Student');";

  QSqlQuery insertIntoDB;
  if (insertIntoDB.exec(insertData)) {
    std::cout << "Data insertion successful." << std::endl;
  } else {
    qDebug() << "Error inserting data:" << insertIntoDB.lastError().text();
  }

  // --- 6. Test and Print Data ---
  QSqlQuery testPrintAllUsers;
  if (testPrintAllUsers.exec("SELECT * FROM users;")) {
    std::cout << "Query to select data successful." << std::endl;
  } else {
    qDebug() << "Error selecting data:" << testPrintAllUsers.lastError().text();
  }

  if (testPrintAllUsers.isActive() && testPrintAllUsers.size() > 0) {
    while (testPrintAllUsers.next()) {
      std::string name =
          testPrintAllUsers.value("name").toString().toStdString();
      std::string email =
          testPrintAllUsers.value("email").toString().toStdString();
      std::string pass =
          testPrintAllUsers.value("password").toString().toStdString();
      std::cout << "name: " << name << " email: " << email << " pass: " << pass
                << std::endl;
    }
  } else {
    std::cout << "No data found or query was inactive." << std::endl;
  }

  db1.close();
}
*/
#endif // DB_XAMPP_H
