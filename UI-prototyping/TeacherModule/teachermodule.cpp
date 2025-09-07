#include "teachermodule.h"
#include "server.cpp"
#include "ui_teachermodule.h"

Server server;

TeacherModule::TeacherModule(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TeacherModule) {
  ui->setupUi(this);
}

TeacherModule::~TeacherModule() { delete ui; }

void TeacherModule::on_StartServer_toolButton_clicked() {
  if (server.getStatus() != "RUNNING") {
    server.start();
  }
}

void TeacherModule::on_StopServer_toolButton_2_clicked() {
  if (server.getStatus() == "RUNNING") {
    server.stop();
  }
}
