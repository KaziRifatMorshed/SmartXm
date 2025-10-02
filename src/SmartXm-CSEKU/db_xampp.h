#ifndef DB_XAMPP_H
#define DB_XAMPP_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <string>

void connectToXamppDB() {
    // check all drivers
    qDebug() << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMARIADB"); // "QMYSQL" driver / QMARIADB
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

    QSqlQuery testPrintAllUsers;
    testPrintAllUsers.exec("SELECT * FROM `users`;");

    if (testPrintAllUsers.size() > 0) {
        while(testPrintAllUsers.next()){
            std::string name = testPrintAllUsers.value("name").toString().toUtf8().constData();
            std::string email = testPrintAllUsers.value("email").toString().toUtf8().constData();
            std::string pass = testPrintAllUsers.value("password").toString().toUtf8().constData();
            std::cout << "name: " << name << " email: " << email << " pass: " << pass << std::endl;
        }
    } else {
        std::cout << "empty " << std::endl;
    }

    db.close();
}

#endif // DB_XAMPP_H
