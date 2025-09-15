#include "welcomewindow.h"
#include "ui_welcomewindow.h"

WelcomeWindow::WelcomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::on_exit_welcome_pushButton_4_clicked()
{
    close();
}


void WelcomeWindow::on_teacherWelcome_pushButton_4_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}


void WelcomeWindow::on_studentWelcome_pushButton_4_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}


void WelcomeWindow::on_nextRemoteServer_pushButton_4_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}


void WelcomeWindow::on_NextLocalServer_pushButton_4_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}


void WelcomeWindow::on_pushButton_2_clicked()
{

}

