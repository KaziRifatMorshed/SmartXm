-- Sample data for Users table
-- user_id (INT) is auto-incremented (1, 2, 3, 4, 5, 6)
-- id (VARCHAR) is the public university ID
INSERT INTO Users (name, email, password, identity, id) VALUES
('Alice Smith', '230201@ku.ac.bd', 'hashed_pass_123', 'student', '230201'),
('Bob Johnson', '210236@ku.ac.bd', 'hashed_pass_abc', 'student', '210236'),
('Carol Williams', '220215@ku.ac.bd', 'hashed_pass_xyz', 'student', '220215'),
('Dr. David Brown', 'david.brown@cse.ku.ac.bd', 'teacher_pass_1', 'teacher', 'T20001'),
('Dr. Frank Miller', 'frank.miller@cse.ku.ac.bd', 'teacher_pass_2', 'teacher', 'T20002'),
('Eve Davis', '230202@ku.ac.bd', 'hashed_pass_789', 'student', '230202');

-- Additional students for 23-batch (230203 to 230240)
INSERT INTO Users (name, email, password, identity, id) VALUES
('Charlie Brown', '230203@ku.ac.bd', 'hashed_pass_230203', 'student', '230203'),
('Diana Prince', '230204@ku.ac.bd', 'hashed_pass_230204', 'student', '230204'),
('Edward Scissorhands', '230205@ku.ac.bd', 'hashed_pass_230205', 'student', '230205'),
('Fiona Glenanne', '230206@ku.ac.bd', 'hashed_pass_230206', 'student', '230206'),
('George Kirk', '230207@ku.ac.bd', 'hashed_pass_230207', 'student', '230207'),
('Hannah Abbott', '230208@ku.ac.bd', 'hashed_pass_230208', 'student', '230208'),
('Isaac Newton', '230209@ku.ac.bd', 'hashed_pass_230209', 'student', '230209'),
('Jane Doe', '230210@ku.ac.bd', 'hashed_pass_230210', 'student', '230210'),
('Kevin McCallister', '230211@ku.ac.bd', 'hashed_pass_230211', 'student', '230211'),
('Linda Hamilton', '230212@ku.ac.bd', 'hashed_pass_230212', 'student', '230212'),
('Michael Scott', '230213@ku.ac.bd', 'hashed_pass_230213', 'student', '230213'),
('Nora Ephron', '230214@ku.ac.bd', 'hashed_pass_230214', 'student', '230214'),
('Oscar Wilde', '230215@ku.ac.bd', 'hashed_pass_230215', 'student', '230215'),
('Pam Beesly', '230216@ku.ac.bd', 'hashed_pass_230216', 'student', '230216'),
('Quentin Tarantino', '230217@ku.ac.bd', 'hashed_pass_230217', 'student', '230217'),
('Rachel Green', '230218@ku.ac.bd', 'hashed_pass_230218', 'student', '230218'),
('Steve Rogers', '230219@ku.ac.bd', 'hashed_pass_230219', 'student', '230219'),
('Tony Stark', '230220@ku.ac.bd', 'hashed_pass_230220', 'student', '230220'),
('Ursula Buffay', '230221@ku.ac.bd', 'hashed_pass_230221', 'student', '230221'),
('Victor Krum', '230222@ku.ac.bd', 'hashed_pass_230222', 'student', '230222'),
('Walter White', '230223@ku.ac.bd', 'hashed_pass_230223', 'student', '230223'),
('Xena Warrior', '230224@ku.ac.bd', 'hashed_pass_230224', 'student', '230224'),
('Yara Greyjoy', '230225@ku.ac.bd', 'hashed_pass_230225', 'student', '230225'),
('Zinedine Zidane', '230226@ku.ac.bd', 'hashed_pass_230226', 'student', '230226'),
('Arthur Weasley', '230227@ku.ac.bd', 'hashed_pass_230227', 'student', '230227'),
('Bella Swan', '230228@ku.ac.bd', 'hashed_pass_230228', 'student', '230228'),
('Carl Grimes', '230229@ku.ac.bd', 'hashed_pass_230229', 'student', '230229'),
('Dory Nemo', '230230@ku.ac.bd', 'hashed_pass_230230', 'student', '230230'),
('Elsa Frozen', '230231@ku.ac.bd', 'hashed_pass_230231', 'student', '230231'),
('Fred Weasley', '230232@ku.ac.bd', 'hashed_pass_230232', 'student', '230232'),
('Ginny Weasley', '230233@ku.ac.bd', 'hashed_pass_230233', 'student', '230233'),
('Harry Potter', '230234@ku.ac.bd', 'hashed_pass_230234', 'student', '230234'),
('Ignotus Peverell', '230235@ku.ac.bd', 'hashed_pass_230235', 'student', '230235'),
('Jack Sparrow', '230236@ku.ac.bd', 'hashed_pass_230236', 'student', '230236'),
('Kate Austen', '230237@ku.ac.bd', 'hashed_pass_230237', 'student', '230237'),
('Luke Skywalker', '230238@ku.ac.bd', 'hashed_pass_230238', 'student', '230238'),
('Mona Lisa', '230239@ku.ac.bd', 'hashed_pass_230239', 'student', '230239'),
('Ned Stark', '230240@ku.ac.bd', 'hashed_pass_230240', 'student', '230240');

-- Students for 24-batch (240201 to 240242)
INSERT INTO Users (name, email, password, identity, id) VALUES
('Olivia Rodriguez', '240201@ku.ac.bd', 'hashed_pass_240201', 'student', '240201'),
('Peter Parker', '240202@ku.ac.bd', 'hashed_pass_240202', 'student', '240202'),
('Quinn Fabray', '240203@ku.ac.bd', 'hashed_pass_240203', 'student', '240203'),
('Robert Bruce', '240204@ku.ac.bd', 'hashed_pass_240204', 'student', '240204'),
('Sarah Connor', '240205@ku.ac.bd', 'hashed_pass_240205', 'student', '240205'),
('Tom Riddle', '240206@ku.ac.bd', 'hashed_pass_240206', 'student', '240206'),
('Uma Thurman', '240207@ku.ac.bd', 'hashed_pass_240207', 'student', '240207'),
('Vito Corleone', '240208@ku.ac.bd', 'hashed_pass_240208', 'student', '240208'),
('Wade Wilson', '240209@ku.ac.bd', 'hashed_pass_240209', 'student', '240209'),
('Xavier Thorpe', '240210@ku.ac.bd', 'hashed_pass_240210', 'student', '240210'),
('Yasmine Ali', '240211@ku.ac.bd', 'hashed_pass_240211', 'student', '240211'),
('Zachary Taylor', '240212@ku.ac.bd', 'hashed_pass_240212', 'student', '240212'),
('Adam Sandler', '240213@ku.ac.bd', 'hashed_pass_240213', 'student', '240213'),
('Ben Stiller', '240214@ku.ac.bd', 'hashed_pass_240214', 'student', '240214'),
('Chris Rock', '240215@ku.ac.bd', 'hashed_pass_240215', 'student', '240215'),
('Drew Barrymore', '240216@ku.ac.bd', 'hashed_pass_240216', 'student', '240216'),
('Emma Stone', '240217@ku.ac.bd', 'hashed_pass_240217', 'student', '240217'),
('Finn Wolfhard', '240218@ku.ac.bd', 'hashed_pass_240218', 'student', '240218'),
('Gal Gadot', '240219@ku.ac.bd', 'hashed_pass_240219', 'student', '240219'),
('Hugh Jackman', '240220@ku.ac.bd', 'hashed_pass_240220', 'student', '240220'),
('Ian McKellen', '240221@ku.ac.bd', 'hashed_pass_240221', 'student', '240221'),
('Jennifer Lawrence', '240222@ku.ac.bd', 'hashed_pass_240222', 'student', '240222'),
('Keanu Reeves', '240223@ku.ac.bd', 'hashed_pass_240223', 'student', '240223'),
('Leonardo DiCaprio', '240224@ku.ac.bd', 'hashed_pass_240224', 'student', '240224'),
('Margot Robbie', '240225@ku.ac.bd', 'hashed_pass_240225', 'student', '240225'),
('Natalie Portman', '240226@ku.ac.bd', 'hashed_pass_240226', 'student', '240226'),
('Owen Wilson', '240227@ku.ac.bd', 'hashed_pass_240227', 'student', '240227'),
('Paul Rudd', '240228@ku.ac.bd', 'hashed_pass_240228', 'student', '240228'),
('Ryan Reynolds', '240229@ku.ac.bd', 'hashed_pass_240229', 'student', '240229'),
('Scarlett Johansson', '240230@ku.ac.bd', 'hashed_pass_240230', 'student', '240230'),
('Tom Hanks', '240231@ku.ac.bd', 'hashed_pass_240231', 'student', '240231'),
('Vin Diesel', '240232@ku.ac.bd', 'hashed_pass_240232', 'student', '240232'),
('Will Smith', '240233@ku.ac.bd', 'hashed_pass_240233', 'student', '240233'),
('Zoe Saldana', '240234@ku.ac.bd', 'hashed_pass_240234', 'student', '240234'),
('Amy Adams', '240235@ku.ac.bd', 'hashed_pass_240235', 'student', '240235'),
('Bradley Cooper', '240236@ku.ac.bd', 'hashed_pass_240236', 'student', '240236'),
('Chris Hemsworth', '240237@ku.ac.bd', 'hashed_pass_240237', 'student', '240237'),
('Dwayne Johnson', '240238@ku.ac.bd', 'hashed_pass_240238', 'student', '240238'),
('Emily Blunt', '240239@ku.ac.bd', 'hashed_pass_240239', 'student', '240239'),
('George Clooney', '240240@ku.ac.bd', 'hashed_pass_240240', 'student', '240240'),
('Henry Cavill', '240241@ku.ac.bd', 'hashed_pass_240241', 'student', '240241'),
('Jessica Chastain', '240242@ku.ac.bd', 'hashed_pass_240242', 'student', '240242');

-- Sample data for Course table, based on sessional/lab courses from the PDF
INSERT INTO Course (course_code, course_name) VALUES
('CSE 1102', 'Structured Programming Laboratory'),
('CSE 2102', 'Data Structures Laboratory'),
('CSE 2206', 'Database Systems Project/Fieldwork'),
('CSE 3108', 'Operating System and Systems Programming Laboratory/Project'),
('CSE 3202', 'Artificial Intelligence Laboratory/Project/Fieldwork');

-- Sample data for CourseTeacher table
-- Assigns teachers (by user_id) to courses
INSERT INTO CourseTeacher (course_code, assigned_teacher) VALUES
('CSE 2102', 4), -- Dr. David Brown (user_id 4) teaches Data Structures Lab
('CSE 2206', 5), -- Dr. Frank Miller (user_id 5) teaches Database Project
('CSE 3108', 4), -- Dr. David Brown (user_id 4) also teaches OS Lab
('CSE 3202', 5); -- Dr. Frank Miller (user_id 5) also teaches AI Lab

-- Sample data for Exam table
-- exam_id is auto-incremented (1, 2, 3)
INSERT INTO Exam (status, date, start_time, end_time, full_marks, topic_name, exam_no, course_code, show_marks) VALUES
('Evaluated', '2025-05-10', '09:00:00', '12:00:00', 100, 'Final Lab Exam', 1, 'CSE 2102', TRUE),
('Evaluating', '2025-05-15', '09:00:00', '17:00:00', 50, 'Project Final Submission', 1, 'CSE 2206', FALSE),
('Scheduled', '2025-06-01', '10:00:00', '11:30:00', 40, 'Midterm Lab Test', 2, 'CSE 2102', FALSE);

-- Sample data for Participates table
-- Enrolls students (by Users.id) into exams
INSERT INTO Participates (student_id, exam_id, set_code_no) VALUES
('230201', 1, 1), -- Alice in Exam 1 (DS Lab Final), Set 1
('210236', 1, 2), -- Bob in Exam 1 (DS Lab Final), Set 2
('220215', 1, 1), -- Carol in Exam 1 (DS Lab Final), Set 1
('230202', 1, 2), -- Eve in Exam 1 (DS Lab Final), Set 2
('210236', 2, 1), -- Bob in Exam 2 (DB Project)
('220215', 2, 1), -- Carol in Exam 2 (DB Project)
('230201', 3, 1), -- Alice in Exam 3 (DS Midterm)
('230202', 3, 2); -- Eve in Exam 3 (DS Midterm)

-- Sample data for PreviousCode table
-- code_id is auto-incremented (1, 2)
INSERT INTO PreviousCode (submitted_by, code, is_allowed) VALUES
('210236', 'public static int binarySearch(int[] arr, int target) { ... }', TRUE),
('220215', 'private void connectDB() { ... }', FALSE);

-- Adding 10 more entries to PreviousCode table
INSERT INTO PreviousCode (submitted_by, code, is_allowed) VALUES
('230205',
'function calculateArea(radius) {\n  return Math.PI * radius * radius;\n}',
TRUE),

('240210',
'SELECT student_id, COUNT(exam_id) AS exams_taken\nFROM Participates\nGROUP BY student_id\nHAVING COUNT(exam_id) > 2;',
TRUE),

('210236',
'for i in range(10):\n    print(f"Iteration {i+1}")',
FALSE),

('230215',
'class Node {\n  int data;\n  Node next;\n  Node(int d) { data = d; next = NULL; }\n}',
TRUE),

('230215',
'public int factorial(int n) {\n  if (n == 0) return 1;\n  else return n * factorial(n-1);\n}',
FALSE),

('240201',
'UPDATE Exam SET status = ''Finished'' WHERE end_time < NOW();',
TRUE),

('230230',
'#include <iostream>\nint main() {\n  std::cout << "Hello World!";\n  return 0;\n}',
FALSE),

('220215',
'const fetchData = async (url) => {\n  const response = await fetch(url);\n  return await response.json();\n}',
TRUE),

('240222',
'CREATE INDEX idx_student_exam ON Participates (student_id, exam_id);',
TRUE),

('230208',
'while (current != null) {\n  process(current.data);\n  current = current.next;\n}',
FALSE);

