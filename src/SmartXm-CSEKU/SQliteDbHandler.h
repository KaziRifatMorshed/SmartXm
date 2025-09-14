#ifndef SQLITEDBHANDLER_H
#define SQLITEDBHANDLER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QVariant>
#include <QString>

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

    bool initDB(const QString& dbName) {
        if (!db.isOpen()) {
            db.setDatabaseName(dbName);
            if (!db.open()) {
                qDebug() << "Error: " << db.lastError().text();
                return false;
            }
        }
        return true;
    }

    bool createTable() {
        QSqlQuery query;
        if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                        "id INTEGER PRIMARY KEY,"
                        "name TEXT NOT NULL,"
                        "email TEXT NOT NULL UNIQUE,"
                        "password TEXT NOT NULL,"
                        "student_id TEXT UNIQUE,"
                        "role TEXT NOT NULL DEFAULT 'Student' CHECK(role IN ('Teacher', 'Student')),"
                        "created_at TEXT DEFAULT CURRENT_TIMESTAMP)")) {
            qDebug() << "Create table failed:" << query.lastError().text();
            return false;
        }
        return true;
    }

    // Inserts a new user into the database
    bool insertData(const QString& name) {
        QSqlQuery query;
        query.prepare("INSERT INTO users (name) VALUES (?)");
        query.addBindValue(name);
        if (!query.exec()) {
            qDebug() << "Insert failed:" << query.lastError().text();
            return false;
        }
        return true;
    }

    bool insertDataFromCSV(){

    }

    // Prints all data from the 'users' table
    void printData() {
        QSqlQuery query("SELECT id, name FROM users");
        if (!query.exec()) {
            qDebug() << "Select failed:" << query.lastError().text();
            return;
        }
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            qDebug() << "id:" << id << "name:" << name;
        }
    }
};

// Definition of the static member outside the class
QSqlDatabase SQliteDB::db;

#endif // SQLITEDBHANDLER_H
