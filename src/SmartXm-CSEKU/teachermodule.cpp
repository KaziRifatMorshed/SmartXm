#include "teachermodule.h"
#include "networking/server/Server.h"
#include "ui_teachermodule.h"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <fstream>


Server *server;
QString instructionFileName = "";

TeacherModule::TeacherModule(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TeacherModule) {
  ui->setupUi(this);
  ui->serverStatus_label_2->setText(
      "<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: "
      "NOT STARTED</span></p></body></html>");
  ui->serverIP_label_3->setText(
      "<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: "
      "NOT STARTED</span></p></body></html>");

// ServerConnectedPC_Table:
  ui->connectedPCwithServer_tableWidget->setRowCount(100);
  for (int var = 0; var < 20; ++var) {
    ui->connectedPCwithServer_tableWidget->setItem(
        var, 0, new QTableWidgetItem(QString("a")));
    ui->connectedPCwithServer_tableWidget->setItem(
        var, 1, new QTableWidgetItem(QString("b")));
    ui->connectedPCwithServer_tableWidget->setItem(
        var, 2, new QTableWidgetItem(QString("c")));
  }
}

TeacherModule::~TeacherModule() { delete ui; }

void TeacherModule::on_StartServer_toolButton_clicked() {
  std::cout << "Start Server button clicked" << std::endl;
  if (server == nullptr) {
    server = Server::createServer();
    ui->serverStatus_label_2->setText(
        "<html><head/><body><p><span style=\" font-size:18pt;\">Server "
        "Status: " +
        QString::fromStdString(server->getStatus() +
                               "</span></p></body></html>"));
    ui->serverIP_label_3->setText("<html><head/><body><p><span style=\" "
                                  "font-size:18pt;\">Server Local IP: " +
                                  QString::fromStdString(server->getLocalIP()) +
                                  "</span></p></body></html>");
  } else {
    std::cout << "One server instance should be running already." << std::endl;
  }
}

void TeacherModule::on_StopServer_toolButton_2_clicked() {
  std::cout << "Stop Server button clicked" << std::endl;
  if (server != nullptr && Server::isRunning()) {
  // if (server != nullptr) {
    std::cout << "trying to stop server..." << std::endl;
    server->stop();
    server = nullptr;
    ui->serverStatus_label_2->setText(
        "<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: "
        "STOPPED</span></p></body></html>");
    ui->serverIP_label_3->setText(
        "<html><head/><body><p><span style=\" font-size:18pt;\">Server Local "
        "IP: STOPPED</span></p></body></html>");
  } else {
    std::cout << "No server instance should be running..." << std::endl;
  }
}

/*
void updateStudentInfo(const QString& name, const QString& id, const QString&
localIp) {
    // Check if the student already exists (by ID for example)
    int row = -1;
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        if (tableWidget->item(i, 1)->text() == id) {
            row = i;
            break;
        }
    }
    if (row == -1) {
        // New student, add a new row
        row = tableWidget->rowCount();
        tableWidget->insertRow(row);
    }

    tableWidget->setItem(row, 0, new QTableWidgetItem(name));
    tableWidget->setItem(row, 1, new QTableWidgetItem(id));
    tableWidget->setItem(row, 2, new QTableWidgetItem(localIp));
}
 */

void TeacherModule::on_selectFile_pushButton_clicked() {
  QString filter =
      "PDF (*.pdf) ;; Text (.*txt) ;; DOCX (*.docx) ;; Tar (*.tar)";
  instructionFileName =
      QFileDialog::getOpenFileName(this,
                                   "Select a file containing instructions to "
                                   "sent it to all connected clients",
                                   QDir::homePath(), filter);
  QFile instructionFile(instructionFileName);
  if (!instructionFile.open(QFile::ReadOnly)) {
    QMessageBox::warning(this, "File failed to select !!!",
                         "File failed to select!");
  }
  ui->selectedFileToSendInstruction_label_2->setText(
      QString("Selected File: " + instructionFileName));
}

void TeacherModule::on_instruction_send_pushButton_clicked() {
  if (instructionFileName.length() <= 0) {
    QMessageBox::warning(this, "No File Selected!", "No File Selected!!!");
  } else {
      // Prepare file
      std::ifstream file(instructionFileName.toStdString(), std::ios::binary);
      if (!file.is_open()) {
          QMessageBox::warning(this, "failed!", "Rulebook File Open Failed!!!");
          return;
      }
      file.seekg(0, std::ios::end);
      size_t sz = file.tellg();
      file.seekg(0, std::ios::beg);
      std::vector<char> filedata(sz);
      file.read(filedata.data(), sz);

      std::string fname = QFileInfo(instructionFileName).fileName().toStdString();
      std::string ext = QFileInfo(instructionFileName).suffix().toStdString();
      std::string msg = "rulebook"; // or "question", etc.

      FileMeta meta(fname, ext, std::time(nullptr), std::move(filedata), msg);
      bool t = server->sendFileToAllClients(meta);
      if (t) {
          QMessageBox::information(this, "Success", "Rulebook sent to all clients.");
      } else {
          QMessageBox::warning(this, "failed!", "Rulebook File Send Failed!!!");
      }
  }
}

void TeacherModule::on_testExam_pushButton_3_clicked()
{
    QString filter = "Tar (*.tar)";

    QString quesFilePath = QFileDialog::getOpenFileName(this,
                                                        "Select a tar file containing question to "
                                                        "sent it to all connected clients",
                                                        QDir::homePath(), filter);
    std::ifstream file(quesFilePath.toStdString(), std::ios::binary);
    if (!file.is_open()) {
        QMessageBox::warning(this, "failed!", "Questions File Open Failed!!!");
        return;
    }
    file.seekg(0, std::ios::end);
    size_t sz = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> filedata(sz);
    file.read(filedata.data(), sz);

    std::string fname = QFileInfo(quesFilePath).fileName().toStdString();
    std::string ext = QFileInfo(quesFilePath).suffix().toStdString();
    std::string msg = "questions.tar"; // or "question", etc.

    FileMeta meta(fname, ext, std::time(nullptr), std::move(filedata), msg);
    bool t = server->sendFileToAllClients(meta);
    if (t) {
        QMessageBox::information(this, "Success", "Questions tar file sent to all clients.");
    } else {
        QMessageBox::warning(this, "failed!", "Questions File Send Failed!!!");
    }
}


void TeacherModule::on_createXm_pushButton_clicked()
{
    // Exam creation features will be here

    if(!createOrModifyXm){
        createOrModifyXm = new CreateOrModifyExam();
    }
    createOrModifyXm->show();
    createOrModifyXm->raise();
    createOrModifyXm->activateWindow();
}


void TeacherModule::on_editExam_pushButon_2_clicked()
{
    // Exam entry modification

    if(!createOrModifyXm){
        createOrModifyXm = new CreateOrModifyExam(); // may need to pass ExamId (PrimaryKey)
    }
    createOrModifyXm->show();
    createOrModifyXm->raise();
    createOrModifyXm->activateWindow();
}


void TeacherModule::on_createQues_pushButton_clicked()
{
    if (!createQuesWidgetWindow){
        createQuesWidgetWindow = new CreateQuestion();
    }
    createQuesWidgetWindow->show();
    createQuesWidgetWindow->raise();
    createQuesWidgetWindow->activateWindow();
}

