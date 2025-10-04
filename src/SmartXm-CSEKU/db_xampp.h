#ifndef DB_XAMPP_H
#define DB_XAMPP_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <iostream>
#include <string>
const QString DB_NAME = "SmartXmRemoteServer";

class localDB {
public:
  static localDB *startDB();

protected:

private:
};


void testDBchanging() {
  // Define the database name to be created/used
  const QString DB_NAME = "SmartXmRemoteServer";

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

// void testDBchanging() {

//   QSqlDatabase db1 =
//       QSqlDatabase::addDatabase("QMARIADB"); // "QMYSQL" driver / QMARIADB
//   db1.setHostName("127.0.0.1");              // localhost / 127.0.0.1
//   db1.setPort(3306);                         // default
//   db1.setDatabaseName("test");               // Replace with your DB name
//   db1.setUserName("root");                   // Default XAMPP user
//   db1.setPassword("");                       // Default XAMPP password
//   (empty)

//   if (!db1.open()) {
//     qDebug() << "Database connection failed:" << db1.lastError().text();
//     return;
//   } else {
//     qDebug() << "Connected!";
//   }

//   // QString createDBquery = "CREATE DATABASE IF NOT EXISTS
//   SmartXmRemoteServer
//   // "
//   //                         "CHARACTER SET utf8mb4 COLLATE
//   //                         utf8mb4_unicode_ci;";
//   QString selectDB = "USE SmartXmRemoteServer;";
//   QString createTable =
//       "USE SmartXmRemoteServer CREATE TABLE IF NOT EXISTS users ("
//       "id INT AUTO_INCREMENT PRIMARY KEY,"
//       "name VARCHAR(100) NOT NULL,"
//       "email VARCHAR(100) NOT NULL UNIQUE,"
//       "password VARCHAR(255) NOT NULL, -- STORED AS RAW/PLAIN TEXT (NOT
//       HASHED)" "student_id VARCHAR(50) UNIQUE," "role ENUM('Teacher',
//       'Student') NOT NULL DEFAULT 'Student'," "created_at TIMESTAMP DEFAULT
//       CURRENT_TIMESTAMP"
//       ");";
//   QString insertData =
//       "INSERT INTO users (name, email, password, "
//       "student_id, role) VALUES "
//       "('Alice Smith', 'alice.smith@example.com', 'password123', '230201', "
//       "'Student');";

//   // --- 2. Create the Database (if it doesn't exist) ---
//   QString createDBquery =
//       QString("CREATE DATABASE IF NOT EXISTS %1 "
//               "CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;")
//           .arg(DB_NAME);
//   QSqlQuery createDB;
//   if (createDB.exec(createDBquery)) {
//     qDebug() << "Database '" << DB_NAME
//              << "' created successfully (or already existed).";
//   } else {
//     qDebug() << "Error creating database:" << createDB.lastError().text();
//     // Return is not strictly necessary as table creation will fail later,
//     but
//     // good for debugging.
//   }

//   QSqlQuery cnjDB;
//   if (cnjDB.exec(selectDB)) {
//     std::cout << "database changed successfully" << std::endl;
//   } else {
//     qDebug() << cnjDB.lastError();
//   }
//   // db1.close();
//   db1.setDatabaseName("SmartXmRemoteServer"); // Replace with your DB name
//   // db1.open();

//   // QSqlDatabase db =
//   //     QSqlDatabase::addDatabase("QMARIADB"); // "QMYSQL" driver / QMARIADB
//   // db.setHostName("127.0.0.1");               // localhost / 127.0.0.1
//   // db.setPort(3306);                          // default
//   // db.setDatabaseName("SmartXmRemoteServer"); // Replace with your DB name
//   // db.setUserName("root");                    // Default XAMPP user
//   // db.setPassword("");                        // Default XAMPP password
//   // (empty)

//   QSqlQuery createTableDB;
//   if (createTableDB.exec(createTable)) {
//     std::cout << "create table successfully" << std::endl;
//   } else {
//     qDebug() << createTableDB.lastError();
//   }

//   QSqlQuery insertIntoDB;
//   if (insertIntoDB.exec(insertData)) {
//     std::cout << "data insertion successful" << std::endl;
//   } else {
//     qDebug() << insertIntoDB.lastError();
//   }

//   QSqlQuery testPrintAllUsers;
//   if (testPrintAllUsers.exec("SELECT * FROM `users`;")) {
//     std::cout << "data printed successful" << std::endl;
//   } else {
//     qDebug() << testPrintAllUsers.lastError();
//   }

//   if (testPrintAllUsers.size() > 0) {
//     while (testPrintAllUsers.next()) {
//       std::string name =
//           testPrintAllUsers.value("name").toString().toUtf8().constData();
//       std::string email =
//           testPrintAllUsers.value("email").toString().toUtf8().constData();
//       std::string pass =
//           testPrintAllUsers.value("password").toString().toUtf8().constData();
//       std::cout << "name: " << name << " email: " << email << " pass: " <<
//       pass
//                 << std::endl;
//     }
//   } else {
//     std::cout << "empty " << std::endl;
//   }

//   db1.close();
// }

void connectToXamppDB() {
  // check all drivers
  qDebug() << QSqlDatabase::drivers();

  QSqlDatabase db =
      QSqlDatabase::addDatabase("QMARIADB"); // "QMYSQL" driver / QMARIADB
  db.setHostName("127.0.0.1");               // localhost / 127.0.0.1
  db.setPort(3306);                          // default
  db.setDatabaseName("SmartXmRemoteServer"); // Replace with your DB name
  db.setUserName("root");                    // Default XAMPP user
  db.setPassword("");                        // Default XAMPP password (empty)

  if (!db.open()) {
    qDebug() << "Database connection failed:" << db.lastError().text();
    return;
  } else {
    qDebug() << "Connected!";
  }

  QSqlQuery testPrintAllUsers;
  testPrintAllUsers.exec("SELECT * FROM `users`;");

  if (testPrintAllUsers.size() > 0) {
    while (testPrintAllUsers.next()) {
      std::string name =
          testPrintAllUsers.value("name").toString().toUtf8().constData();
      std::string email =
          testPrintAllUsers.value("email").toString().toUtf8().constData();
      std::string pass =
          testPrintAllUsers.value("password").toString().toUtf8().constData();
      std::cout << "name: " << name << " email: " << email << " pass: " << pass
                << std::endl;
    }
  } else {
    std::cout << "empty " << std::endl;
  }

  db.close();
}

#endif // DB_XAMPP_H
