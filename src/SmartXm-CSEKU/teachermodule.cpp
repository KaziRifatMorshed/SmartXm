#include "teachermodule.h"
#include "networking/server/Server.h"
#include "ui_teachermodule.h"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QTimer>
#include <Users.h>
#include <db_xampp.h>
#include <fstream>
#include <iostream>

Server *server;
QString instructionFileName = "";
Users &currentUser = Users::getInstance();

TeacherModule::TeacherModule(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TeacherModule) {
  ui->setupUi(this);
  ui->serverStatus_label_2->setText(
      "<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: "
      "NOT STARTED</span></p></body></html>");
  ui->serverIP_label_3->setText(
      "<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: "
      "NOT STARTED</span></p></body></html>");
  ui->label->setText("<html><head/><body><p align=\"center\"><span style=\" "
                     "font-size:20pt;\">Welcome, " +
                     QString::fromStdString(currentUser.getName()) +
                     " Sir</span></p></body></html>");
  ui->dashboard_teacherName->setText(
      QString::fromStdString(currentUser.getName()));
  ui->dashboard_teacherEmail->setText(
      QString::fromStdString(currentUser.getEmail()));
  ui->dashboard_TeacherDesignation->setText(
      QString::fromStdString(currentUser.getId()));
  ui->tabWidget->setCurrentIndex(0);

  // Initial table setup (keeps headers visible)
  ui->connectedPCwithServer_tableWidget->setColumnCount(4);
  ui->connectedPCwithServer_tableWidget->setHorizontalHeaderLabels(QStringList()
                                                                   << "Name"
                                                                   << "Stu ID"
                                                                   << "Local IP"
                                                                   << "Action");
  ui->connectedPCwithServer_tableWidget->horizontalHeader()
      ->setStretchLastSection(false);
  ui->connectedPCwithServer_tableWidget->horizontalHeader()
      ->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->connectedPCwithServer_tableWidget->horizontalHeader()
      ->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->connectedPCwithServer_tableWidget->horizontalHeader()
      ->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->connectedPCwithServer_tableWidget->horizontalHeader()
      ->setSectionResizeMode(3, QHeaderView::ResizeToContents);

  // Start a timer that updates the connected clients list every 1 second.
  clientsUpdateTimer = new QTimer(this);
  clientsUpdateTimer->setInterval(1000); // 1 second
  connect(clientsUpdateTimer, &QTimer::timeout, this,
          &TeacherModule::showConnectedStudentInfo);
  clientsUpdateTimer->start();
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
  ui->StartServer_toolButton->setEnabled(false);
  ui->StopServer_toolButton_2->setEnabled(true);
}

void TeacherModule::on_StopServer_toolButton_2_clicked() {
  std::cout << "Stop Server button clicked" << std::endl;
  if (server != nullptr && Server::isRunning()) {
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
  ui->StartServer_toolButton->setEnabled(true);
  ui->StopServer_toolButton_2->setEnabled(false);
}

void TeacherModule::showConnectedStudentInfo() {
  // This function populates the connectedPCwithServer_tableWidget with
  // currently connected clients from the server instance.
  // It tolerantly handles the absence of a server instance.

  if (!ui)
    return; // defensive
  if (server == nullptr) {
    // No server running: clear the table
    ui->connectedPCwithServer_tableWidget->setRowCount(0);
    return;
  }

  // Get a snapshot of connected clients in a thread-safe way.
  std::vector<ClientInfo> clients;
  try {
    clients = server->getClients();
  } catch (...) {
    ui->connectedPCwithServer_tableWidget->setRowCount(0);
    return;
  }

  const int connectedClientsNo = static_cast<int>(clients.size());

  // Clear existing rows (this deletes any cell widgets too) and set new row
  // count.
  ui->connectedPCwithServer_tableWidget->setRowCount(0);
  ui->connectedPCwithServer_tableWidget->setRowCount(connectedClientsNo);

  for (int row = 0; row < connectedClientsNo; ++row) {
    const ClientInfo &ci = clients[row];

    // Column 0: Name (use clientName if present)
    QString name = QString::fromStdString(ci.clientName);
    if (name.isEmpty())
      name = QString("Client %1").arg(row + 1);
    ui->connectedPCwithServer_tableWidget->setItem(row, 0,
                                                   new QTableWidgetItem(name));

    // Column 1: Stu ID (ClientInfo does not have explicit student id; use
    // socket fd as identifier)
    ui->connectedPCwithServer_tableWidget->setItem(
        row, 1, new QTableWidgetItem(QString::number(00)));

    // Column 2: Local IP
    ui->connectedPCwithServer_tableWidget->setItem(
        row, 2, new QTableWidgetItem(QString::fromStdString(ci.ip)));

    // Column 3: Action - Create a container widget and a layout
    QWidget *actionWidget = new QWidget();
    QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(0, 0, 0, 0); // Remove padding
    actionLayout->setSpacing(5);                  // Space between buttons

    // Create Button 1: Info
    QPushButton *infoBtn = new QPushButton("Info");
    infoBtn->setProperty("client_name", name);
    infoBtn->setProperty("client_ip", QString::fromStdString(ci.ip));
    infoBtn->setProperty("client_socfd", ci.socfd);

    // When clicked, show a simple info dialog.
    connect(infoBtn, &QPushButton::clicked, this, [this, infoBtn]() {
      QString n = infoBtn->property("client_name").toString();
      QString ip = infoBtn->property("client_ip").toString();
      int fd = infoBtn->property("client_socfd").toInt();
      QMessageBox::information(this, "Connected Client",
                               QString("Name: %1\nLocal IP: %2\nSocket FD: %3")
                                   .arg(n)
                                   .arg(ip)
                                   .arg(fd));
    });

    // Create Button 2: Test Connection
    QPushButton *testClientConnectionBtn = new QPushButton("Test Connection");
    testClientConnectionBtn->setToolTip(
        "Test Connection"); // Tooltip for clarity
    testClientConnectionBtn->setProperty("client_name", name);
    testClientConnectionBtn->setProperty("client_ip",
                                         QString::fromStdString(ci.ip));
    testClientConnectionBtn->setProperty("client_socfd", ci.socfd);

    // When clicked, send a test file.
    connect(
        testClientConnectionBtn, &QPushButton::clicked, this,
        [this, testClientConnectionBtn]() {
          QString n =
              testClientConnectionBtn->property("client_name").toString();
          QString ip =
              testClientConnectionBtn->property("client_ip").toString();
          int fd = testClientConnectionBtn->property("client_socfd").toInt();
          bool t =
              server->sendFileToClient(fd, "./resources/Hello2.pdf", "HELLO",
                                       "Hello packet sent from server");
          if (t) {
            QMessageBox::information(
                this, "Test Connection with Client",
                QString(
                    "Name: %1\nLocal IP: %2\nSocket FD: %3\nHello file sent...")
                    .arg(n)
                    .arg(ip)
                    .arg(fd));
          } else {
            QMessageBox::warning(
                this, "Test Failed",
                QString("Failed to send HELLO file to %1 (Socket: %2)")
                    .arg(n)
                    .arg(fd));
          }
        });

    // Add buttons to the layout
    actionLayout->addWidget(infoBtn);
    actionLayout->addWidget(testClientConnectionBtn);
    // You could add more buttons here
    // actionLayout->addWidget(new QPushButton("Kick"));

    // Set the container widget (which holds the layout) into the cell
    ui->connectedPCwithServer_tableWidget->setCellWidget(row, 3, actionWidget);

    // --- MODIFICATION END ---
  }

  ui->connectedPCwithServer_tableWidget->resizeColumnsToContents();
}

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
    std::string msg = "rulebook";
    std::string title = "rulebook";

    FileMeta meta(title, fname, ext, std::time(nullptr), std::move(filedata),
                  msg);
    bool t = server->sendFileToAllClients(meta);
    if (t) {
      QMessageBox::information(this, "Success",
                               "Rulebook sent to all clients.");
    } else {
      QMessageBox::warning(this, "failed!", "Rulebook File Send Failed!!!");
    }
  }
}

void TeacherModule::on_testExam_pushButton_3_clicked() {
  QString filter = "Tar (*.tar)";

  QString quesFilePath =
      QFileDialog::getOpenFileName(this,
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
  std::string title = "QUESTIONS";   // or "question", etc.

  FileMeta meta(title, fname, ext, std::time(nullptr), std::move(filedata),
                msg);
  bool t = server->sendFileToAllClients(meta);
  if (t) {
    QMessageBox::information(this, "Success",
                             "Questions tar file sent to all clients.");
  } else {
    QMessageBox::warning(this, "failed!", "Questions File Send Failed!!!");
  }
}

void TeacherModule::on_createXm_pushButton_clicked() {
  // Exam creation features will be here

  if (!createOrModifyXm) {
    createOrModifyXm = new CreateOrModifyExam();
  }
  createOrModifyXm->show();
  createOrModifyXm->raise();
  createOrModifyXm->activateWindow();
}

void TeacherModule::on_editExam_pushButon_2_clicked() {
  // Exam entry modification

  if (!createOrModifyXm) {
    createOrModifyXm =
        new CreateOrModifyExam(); // may need to pass ExamId (PrimaryKey)
  }
  createOrModifyXm->show();
  createOrModifyXm->raise();
  createOrModifyXm->activateWindow();
}

void TeacherModule::on_createQues_pushButton_clicked() {
  if (!createQuesWidgetWindow) {
    createQuesWidgetWindow = new CreateQuestion();
  }
  createQuesWidgetWindow->show();
  createQuesWidgetWindow->raise();
  createQuesWidgetWindow->activateWindow();
}
