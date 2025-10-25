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

// ... (other includes)

TeacherModule::TeacherModule(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TeacherModule) {
  ui->setupUi(this);
  // ... (existing setup code for server status, labels, etc.)
  ui->tabWidget->setCurrentIndex(0);

  // --- Setup for connectedPCwithServer_tableWidget (you already have this) ---
  ui->connectedPCwithServer_tableWidget->setColumnCount(4);
  ui->connectedPCwithServer_tableWidget->setHorizontalHeaderLabels(QStringList()
                                                                   << "Name"
                                                                   << "Stu ID"
                                                                   << "Local IP"
                                                                   << "Action");
  // ... (existing header resize code for this table)

  // --- ADD THIS NEW SETUP for allXmList_tableWidget ---
  // The column headers are set in the .ui file, but we set resizing
  ui->allXmList_tableWidget->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::Stretch); // Course Name
  ui->allXmList_tableWidget->horizontalHeader()->setSectionResizeMode(
      1, QHeaderView::ResizeToContents); // Course Code
  ui->allXmList_tableWidget->horizontalHeader()->setSectionResizeMode(
      2, QHeaderView::ResizeToContents); // Course Teacher
  ui->allXmList_tableWidget->horizontalHeader()->setSectionResizeMode(
      3, QHeaderView::Stretch); // Exam Topic
  ui->allXmList_tableWidget->horizontalHeader()->setSectionResizeMode(
      4, QHeaderView::ResizeToContents); // Exam Date
  ui->allXmList_tableWidget->horizontalHeader()->setSectionResizeMode(
      5, QHeaderView::ResizeToContents); // Exam Time
  ui->allXmList_tableWidget->horizontalHeader()->setSectionResizeMode(
      6, QHeaderView::ResizeToContents); // Action

  // Call the new function to load data
  populateExamList();
  // --- END OF NEW CODE ---

  // --- Start clientsUpdateTimer (you already have this) ---
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

    // Column 1: Stu ID
    ui->connectedPCwithServer_tableWidget->setItem(
        row, 1, new QTableWidgetItem(QString::fromStdString(ci.studentID)));

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

#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidgetItem>
#include <db_xampp.h>

void TeacherModule::populateExamList() {
  // 1. Get the database instance
  localDB *db = localDB::DB();
  if (!db) {
    qWarning() << "Database not initialized!";
    return;
  }

  // 2. Define the SQL query to get all required data
  // This query joins Exam, Course, CourseTeacher, and Users to get all info
  QString sql =
      "SELECT "
      "    Course.course_name, "
      "    Exam.course_code, "
      "    Users.name AS teacher_name, "
      "    Exam.topic_name, "
      "    Exam.date, "
      "    Exam.start_time, "
      "    Exam.exam_id " // Get the ID for the action buttons
      "FROM Exam "
      "JOIN Course ON Exam.course_code = Course.course_code "
      // Use LEFT JOIN in case a teacher isn't assigned (safer)
      "LEFT JOIN CourseTeacher ON Exam.course_code = CourseTeacher.course_code "
      "LEFT JOIN Users ON CourseTeacher.assigned_teacher = Users.user_id AND "
      "Users.identity = 'teacher' "
      "ORDER BY Exam.date DESC, Exam.start_time DESC;";

  QSqlQuery query = db->execQuery(sql);
  if (!query.isActive()) {
    qWarning() << "Failed to execute exam list query:"
               << query.lastError().text();
    return;
  }

  // 3. Get the table widget and clear any existing dummy data
  auto *table = ui->allXmList_tableWidget;
  table->setRowCount(0); // Clear all rows (including dummy ones from .ui)
  table->setSortingEnabled(false); // Disable sorting while populating

  // 4. Loop through the query results and populate the table
  while (query.next()) {
    int row = table->rowCount();
    table->insertRow(row);

    // Extract data
    QString courseName = query.value("course_name").toString();
    QString courseCode = query.value("course_code").toString();
    QString teacherName = query.value("teacher_name").toString();
    QString examTopic = query.value("topic_name").toString();
    QDate examDate = query.value("date").toDate();
    QTime examTime = query.value("start_time").toTime();
    int examId = query.value("exam_id").toInt();

    // Column 0: Course Name
    table->setItem(row, 0, new QTableWidgetItem(courseName));

    // Column 1: Course Code
    table->setItem(row, 1, new QTableWidgetItem(courseCode));

    // Column 2: Course Teacher
    table->setItem(row, 2, new QTableWidgetItem(teacherName));

    // Column 3: Exam Topic
    table->setItem(row, 3, new QTableWidgetItem(examTopic));

    // Column 4: Exam Date
    table->setItem(row, 4,
                   new QTableWidgetItem(examDate.toString(Qt::ISODate)));

    // Column 5: Exam Time
    table->setItem(row, 5, new QTableWidgetItem(examTime.toString("hh:mm AP")));

    // Column 6: Action Buttons
    QWidget *actionWidget = new QWidget();
    QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(5, 0, 5, 0);
    actionLayout->setSpacing(5);

    QPushButton *editBtn = new QPushButton("Edit");
    editBtn->setProperty("exam_id", examId); // Store the ID
    connect(editBtn, &QPushButton::clicked, this, [this, examId]() {
      // This is where you would open the "Edit Exam" window
      // and pass the examId to it.
      // For now, it calls the same slot as "Create Exam"
      on_editExam_pushButon_2_clicked();
      QMessageBox::information(this, "Edit Exam",
                               QString("Would edit Exam ID: %1").arg(examId));
    });

    QPushButton *deleteBtn = new QPushButton("Delete");
    deleteBtn->setProperty("exam_id", examId);
    connect(deleteBtn, &QPushButton::clicked, this, [this, db, examId, row]() {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::warning(
          this, "Delete Exam",
          QString("Are you sure you want to delete Exam ID %1?").arg(examId),
          QMessageBox::Yes | QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        QSqlQuery deleteQuery = db->execQuery(
            QString("DELETE FROM Exam WHERE exam_id = %1").arg(examId));
        if (deleteQuery.numRowsAffected() > 0) {
          ui->allXmList_tableWidget->removeRow(row);
        } else {
          QMessageBox::critical(this, "Error",
                                "Failed to delete exam from database.");
        }
      }
    });

    actionLayout->addWidget(editBtn);
    actionLayout->addWidget(deleteBtn);
    actionWidget->setLayout(actionLayout);

    table->setCellWidget(row, 6, actionWidget);
  }

  table->setSortingEnabled(true);
  table->resizeColumnsToContents();
}
