#ifndef DB_XAMPP_H
#define DB_XAMPP_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void connectToXamppDB() {
    // check all drivers
    qDebug() << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMARIADB"); // "QMYSQL" driver
    db.setHostName("localhost"); // localhost / 127.0.0.1
    db.setPort(3306); // default
    db.setDatabaseName("smartxmremoteserver"); // Replace with your DB name
    db.setUserName("root");             // Default XAMPP user
    db.setPassword("");                 // Default XAMPP password (empty)

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return;
    } else {
        qDebug() << "Connected!";
    }
}

#endif // DB_XAMPP_H
