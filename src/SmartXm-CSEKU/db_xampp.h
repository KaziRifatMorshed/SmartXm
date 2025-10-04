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
  static localDB *dbInstance;
  static localDB *DB() {
    if (dbInstance == nullptr) {
      dbInstance = new localDB();
    }
    return dbInstance;
  }

protected:
  QSqlDatabase qSqlDB;

  QList<QString> getDriverNames() {
    QList listOfDrivers = QSqlDatabase::drivers();
    qDebug() << listOfDrivers;
    return QSqlDatabase::drivers().toList();
  }

private:
  localDB() { startDB(); }
  ~localDB();

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

  void prepareDB() {
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
  }

  void insertTestData() {
    // --- 5. Insert Data ---
    QString insertData =
        "INSERT IGNORE INTO users (name, email, password, " // Use IGNORE to
                                                            // skip existing
                                                            // unique keys
        "student_id, role) VALUES "
        "('Alice Smith tid', 'alice.smith@example.com', 'password123', "
        "'230201', "
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
      qDebug() << "Error selecting data:"
               << testPrintAllUsers.lastError().text();
    }

    if (testPrintAllUsers.isActive() && testPrintAllUsers.size() > 0) {
      while (testPrintAllUsers.next()) {
        std::string name =
            testPrintAllUsers.value("name").toString().toStdString();
        std::string email =
            testPrintAllUsers.value("email").toString().toStdString();
        std::string pass =
            testPrintAllUsers.value("password").toString().toStdString();
        std::cout << "name: " << name << " email: " << email
                  << " pass: " << pass << std::endl;
      }
    } else {
      std::cout << "No data found or query was inactive." << std::endl;
    }
  }
  void testDB() {
      prepareDB();
      insertTestData();
  }
};

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

void testDBchanging() {
  // 1. Initial Connection: Connect to a default database to gain permissions
  //    to create a new one. "test" or "" (empty string) works here.
  QSqlDatabase db1 =
      QSqlDatabase::addDatabase("QMARIADB", DB_NAME); // or "QMYSQL"
  db1.setHostName("127.0.0.1");
  db1.setPort(3306);
  // Connect initially to a database that is guaranteed to exist (e.g., "mysql"
  // or "test")
  // db1.setDatabaseName("test");
  db1.setUserName("root");
  db1.setPassword("");

  if (!db1.open()) {
    qDebug() << "Initial Database connection (to 'DB_NAME') failed:"
             << db1.lastError().text();
    return;
  } else {
    qDebug() << "Initial Connection successful (to 'DB_NAME')!";
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
  // QString selectDB = QString("USE %1;").arg(DB_NAME);
  // QSqlQuery cnjDB;
  // if (cnjDB.exec(selectDB)) {
  //   std::cout << "Successfully switched to database '" <<
  //   DB_NAME.toStdString()
  //             << "'" << std::endl;
  // } else {
  //   qDebug() << "Error switching database:" << cnjDB.lastError().text();
  //   db1.close();
  //   return;
  // }

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

#endif // DB_XAMPP_H
