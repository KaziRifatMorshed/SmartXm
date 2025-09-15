#include "teachermodule.h"
#include "networking/server/server.cpp"
#include "ui_teachermodule.h"

Server server;

TeacherModule::TeacherModule(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TeacherModule) {
  ui->setupUi(this);
    ui->serverStatus_label_2->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: NOT STARTED</span></p></body></html>");
    ui->serverIP_label_3->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: NOT STARTED</span></p></body></html>");
}

TeacherModule::~TeacherModule() { delete ui; }

void TeacherModule::on_StartServer_toolButton_clicked() {
  if (server.getStatus() != "RUNNING") {
    server.start();
    ui->serverStatus_label_2->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: " + QString::fromStdString(server.getStatus() + "</span></p></body></html>"));
    ui->serverIP_label_3->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: " + QString::fromStdString(server.getLocalIP()) + "</span></p></body></html>");
  }
}

void TeacherModule::on_StopServer_toolButton_2_clicked() {
  if (server.getStatus() == "RUNNING") {
    server.~Server();
    ui->serverStatus_label_2->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: STOPPED</span></p></body></html>");
        ui->serverIP_label_3->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: STOPPED</span></p></body></html>");
  }
}
