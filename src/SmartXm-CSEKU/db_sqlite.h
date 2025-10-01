#ifndef DB_SQLITE_H
#define DB_SQLITE_H

// INCOMPLETE

#include <QDebug>
#include <QString>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <csv.h>
#include <string>

/*
Database Design:
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    email TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL, -- STORED AS RAW/PLAIN TEXT (NOT HASHED)
    student_id TEXT UNIQUE,
    role TEXT NOT NULL DEFAULT 'Student' CHECK(role IN ('Teacher', 'Student')),
    created_at TEXT DEFAULT CURRENT_TIMESTAMP
);
*/

class SQliteDB {
private:
  static QSqlDatabase db;

public:
  SQliteDB() {
    if (!db.isValid()) {
      db = QSqlDatabase::addDatabase("QSQLITE");
    }
  }

  ~SQliteDB() {
    if (db.isOpen()) {
      db.close();
    }
  }

  bool initDB(const QString &dbName) {
    if (!db.isOpen()) {
      db.setDatabaseName(dbName);
      if (!db.open()) {
        qDebug() << "Error: " << db.lastError().text();
        return false;
      }
    }
    return true;
  }

  bool createTable() { // though not necessary
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY,"
                    "name TEXT NOT NULL,"
                    "email TEXT NOT NULL UNIQUE,"
                    "password TEXT NOT NULL,"
                    "student_id TEXT UNIQUE,"
                    "role TEXT NOT NULL DEFAULT 'Student' CHECK(role IN "
                    "('Teacher', 'Student')),"
                    "created_at TEXT DEFAULT CURRENT_TIMESTAMP)")) {
      qDebug() << "Create table failed:" << query.lastError().text();
      return false;
    }
    return true;
  }

  bool insertUser(int id, const QString &name, const QString &email,
                  const QString &password, const QString &student_id,
                  const QString &role, const QString &created_at) {
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO users (id, name, email, password, "
                  "student_id, role, created_at) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(name);
    query.addBindValue(email);
    query.addBindValue(password);
    query.addBindValue(student_id);
    query.addBindValue(role);
    query.addBindValue(created_at);
    if (!query.exec()) {
      qDebug() << "Insert failed:" << query.lastError().text();
      return false;
    }
    return true;
  }

  // Reads user data from CSV and inserts into DB
  bool insertDataFromCSV(const QString &csvPath) {
    try {
      io::CSVReader<7, io::trim_chars<>, io::double_quote_escape<',', '\"'>> in(
          csvPath.toStdString());
      in.read_header(io::ignore_extra_column, "id", "name", "email", "password",
                     "student_id", "role", "created_at");
      std::string id, name, email, password, student_id, role, created_at;
      int insertCount = 0;
      while (in.read_row(id, name, email, password, student_id, role,
                         created_at)) {
        bool ok = insertUser(
            QString::fromStdString(id).toInt(), QString::fromStdString(name),
            QString::fromStdString(email), QString::fromStdString(password),
            QString::fromStdString(student_id), QString::fromStdString(role),
            QString::fromStdString(created_at));
        if (ok)
          insertCount++;
      }
      qDebug() << "Inserted" << insertCount << "users from CSV.";
      return true;
    } catch (const std::exception &e) {
      qDebug() << "CSV import failed:" << e.what();
      return false;
    }
  }

  // Prints all data from the 'users' table
  void printData() {
    QSqlQuery query("SELECT id, name, email, password, student_id, role, "
                    "created_at FROM users");
    if (!query.exec()) {
      qDebug() << "Select failed:" << query.lastError().text();
      return;
    }
    while (query.next()) {
      int id = query.value(0).toInt();
      QString name = query.value(1).toString();
      QString email = query.value(2).toString();
      QString password = query.value(3).toString();
      QString student_id = query.value(4).toString();
      QString role = query.value(5).toString();
      QString created_at = query.value(6).toString();
      qDebug() << "id:" << id << "name:" << name << "email:" << email
               << "password:" << password << "student_id:" << student_id
               << "role:" << role << "created_at:" << created_at;
    }
  }
};

// Definition of the static member outside the class
QSqlDatabase SQliteDB::db;

#endif // DB_SQLITE_H
