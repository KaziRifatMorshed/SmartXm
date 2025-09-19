#include "teachermodule.h"
#include "networking/server/Server.h"
#include "ui_teachermodule.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>


QString instructionFileName = "";

TeacherModule::TeacherModule(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TeacherModule) {
    ui->setupUi(this);
    ui->serverStatus_label_2->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: NOT STARTED</span></p></body></html>");
    ui->serverIP_label_3->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: NOT STARTED</span></p></body></html>");

    ServerConnectedPC_Table:
    ui->connectedPCwithServer_tableWidget->setRowCount(100);
    for (int var = 0; var < 20; ++var) {
        ui->connectedPCwithServer_tableWidget->setItem(var,0, new QTableWidgetItem(QString("a")));
        ui->connectedPCwithServer_tableWidget->setItem(var,1, new QTableWidgetItem(QString("b")));
        ui->connectedPCwithServer_tableWidget->setItem(var,2, new QTableWidgetItem(QString("c")));
    }
}

TeacherModule::~TeacherModule() { delete ui; }

void TeacherModule::on_StartServer_toolButton_clicked() {
  if (Server::getStatus() != "RUNNING") {
    Server::createServer();
    ui->serverStatus_label_2->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: " + QString::fromStdString(Server::getStatus() + "</span></p></body></html>"));
    ui->serverIP_label_3->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: " + QString::fromStdString(Server::getLocalIP()) + "</span></p></body></html>");
  }
}

void TeacherModule::on_StopServer_toolButton_2_clicked() {
  if (Server::getStatus() == "RUNNING") {
    Server::stop();
    ui->serverStatus_label_2->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: STOPPED</span></p></body></html>");
    ui->serverIP_label_3->setText("<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: STOPPED</span></p></body></html>");
  }
}

/*
void updateStudentInfo(const QString& name, const QString& id, const QString& localIp) {
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

void TeacherModule::on_selectFile_pushButton_clicked(){
    QString filter = "PDF (*.pdf) ;; Text (.*txt) ;; DOCX (*.docx) ;; Tar (*.tar)";
    instructionFileName = QFileDialog::getOpenFileName(this,"Select a file containing instructions to sent it to all connected clients", QDir::homePath(), filter);
    QFile instructionFile(instructionFileName);
    if(!instructionFile.open(QFile::ReadOnly)){
        QMessageBox::warning(this, "File failed to select !!!", "File failed to select!");
    }
    ui->selectedFileToSendInstruction_label_2->setText(QString("Selected File: " + instructionFileName));
}


void TeacherModule::on_instruction_send_pushButton_clicked() {
    if(instructionFileName.length() <= 0){
        QMessageBox::warning(this, "No File Selected!", "No File Selected!!!");
    } else {
        // send file to all connected clients
        bool t = Server::sendFileToAllClients(instructionFileName.toStdString());
        // on successful, show msg
        if(t){
            QMessageBox::information(this, "Success", "Instruction sent to all connected clients.");
        } else {
            QMessageBox::warning(this, "failed!", "File Send Failed!!!");
        }
    }
}

