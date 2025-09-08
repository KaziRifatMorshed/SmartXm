#include "login_window.h"
#include "./ui_login_window.h"
#include <iostream>
#include "Client.h"

Login_Window::Login_Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login_Window)
{
    ui->setupUi(this);
}

Login_Window::~Login_Window()
{
    delete ui;
    ui->label_status->setText("Status: NOT CONNECTED");
}

void Login_Window::on_exitButton_clicked(){}


void Login_Window::on_loginButton_clicked()
{
    QString inputtedEmail = ui->emailTextField->text();
    QString inputtedIP = ui->serverIpTextField->text();
    client.setClientEmail(inputtedEmail.toStdString());
    client.setLocalIP(inputtedIP.toStdString());
    std::cout << "client email:" << inputtedEmail.toStdString() << ", trying to connect loca ip: " << inputtedIP.toStdString() << std::endl;
    client.start(inputtedIP.toStdString());
}
