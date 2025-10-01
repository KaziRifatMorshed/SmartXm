#include "welcomewindow.h"
#include "TerminalExecuter.h"
#include "dependencies/linux/Encryption/encryption.h"
#include "ui_welcomewindow.h"
#include <QtConcurrent/QtConcurrent> // At the top
#include <db_xampp.h>
#include <iostream>
#include <networking/client/Client.h>
// #include "studentmodule.h"

Client *client;
bool clientConnectedToLocalServer = false;

// Provide the actual storage for the extern variable
StudentModule* studentModulePointer = nullptr;

WelcomeWindow::WelcomeWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::WelcomeWindow),
      m_networkManager(new QNetworkAccessManager(this)) {
  ui->setupUi(this);
  ui->tabWidget->setTabEnabled(1, false);
  ui->tabWidget->setTabEnabled(2, false);
  ui->tabWidget->setTabEnabled(3, false);
}

WelcomeWindow::~WelcomeWindow() { delete ui; }

void WelcomeWindow::on_exit_welcome_pushButton_4_clicked() { close(); }

void WelcomeWindow::on_teacherWelcome_pushButton_4_clicked() {
  ui->tabWidget->setTabEnabled(1, true);
  ui->tabWidget->setTabEnabled(2, false);
  ui->tabWidget->setTabEnabled(3, false);
  ui->tabWidget->setCurrentIndex(1);

  connectToXamppDb();
}

void WelcomeWindow::on_studentWelcome_pushButton_4_clicked() {
  ui->tabWidget->setTabEnabled(2, true);
  ui->tabWidget->setTabEnabled(1, false);
  ui->tabWidget->setTabEnabled(3, false);
  ui->tabWidget->setCurrentIndex(2);
}

void WelcomeWindow::on_nextRemoteServer_pushButton_4_clicked() {
  ui->tabWidget->setTabEnabled(3, true);
  ui->tabWidget->setCurrentIndex(3);
}

void WelcomeWindow::on_NextLocalServer_pushButton_4_clicked() {
  ui->tabWidget->setTabEnabled(3, true);
  ui->tabWidget->setCurrentIndex(3);
}

void WelcomeWindow::checkConnection() {
  QNetworkRequest request(QUrl("http://www.google.com"));
  m_networkManager->head(request);
  qDebug() << "Checking for internet connection...";
}

/*
// void WelcomeWindow::handleConnectionCheck(QNetworkReply *reply)
// {
//     if (reply->error() == QNetworkReply::NoError) {
//         // Success
//         qDebug() << "Internet connection is available.";
//         // You could update a status label here, e.g.:
//         // ui->statusLabel->setText("Status: Connected");
//     } else {
//         // Failure
//         qDebug() << "Internet connection is not available.";
//         qDebug() << "Error:" << reply->errorString();
//         // ui->statusLabel->setText("Status: Disconnected");
//     }
//     reply->deleteLater();
// }
*/

void WelcomeWindow::on_sync_remoteS_pushButton_2_clicked() {
  QString executablePath = QCoreApplication::applicationDirPath();
  QString pyFetcherPath;

#ifdef __linux__
  pyFetcherPath = executablePath + "/dependencies/linux/WebScrap2";
#elif _WIN32
  pyFetcherPath = executablePath + "/dependencies/win/WebScrap2.exe"; // forgot
#endif

DownloadDataAndSaveCSV:
  std::cout
      << "Downloading Data from Remote Server and Saving CSV file inside db"
      << std::endl;
  std::string downCSVdataCMD = "cd " + executablePath.toStdString() +
                               "/db && " + pyFetcherPath.toStdString();
  // std::cout << downCSVdataCMD << std::endl;
  std::string downCSVdataOutput = termiExec(downCSVdataCMD.c_str());
  // std::cout << downCSVdataOutput << std::endl;

DecryptCSVdata:
  Encryption encryption =
      Encryption("./db/remoteData.csv", "./db/remoteData-dec.csv", 20);
  std::cout << "Decrypting Data" << std::endl;
  encryption.decrypt();
} // working

void WelcomeWindow::on_pushButton_clicked() {
  QString inputtedEmail = ui->email_lineEdit->text();
  QString inputtedPass = ui->pass_lineEdit_2->text();

  bool temp = true; // if login info are true
  // bool isTeacher = (inputtedEmail == "t") ? true : false;
  /// login info checking code goes here
  if (temp) {
    if (inputtedEmail == "t") {
      close();
      teacherModuleWindow = new TeacherModule();
      /*
       * ISSUE: window hides but does not closes
      hide();
      teacherModuleWindow = new TeacherModule(this);
       */
      teacherModuleWindow->show();
    } else if (inputtedEmail == "s") {
        close();
        studentModuleV2Window = new StudentModuleV2();
        studentModuleV2Window->show();
    }
  }
}

void WelcomeWindow::on_connect_to_local_server_pushButton_3_clicked() {
  QString inputtedLocalServerIP = ui->localServerIP_lineEdit_3->text();
  client = Client::createClient();

  // Run connectToServer in a background thread
  QFuture<bool> future = QtConcurrent::run([=]() {
    return client->connectToServer(inputtedLocalServerIP.toStdString());
  });

  // When done, update UI in main thread
  QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>(this);
  connect(watcher, &QFutureWatcher<bool>::finished, this, [=]() {
    bool isConnectedToLocalServer = future.result();
    if (isConnectedToLocalServer) {
      std::cout << "Connected to local server" << std::endl;
      ui->localServerStatus_label_6->setText(
          "<html><head/><body><p align=\"center\"><span style=\" "
          "font-size:11pt;\">Local Server Connection Status: <span "
          "style=\"color: green;\">CONNECTED</span></span></p></body></html>");
      clientConnectedToLocalServer = true;
    } else {
      ui->localServerStatus_label_6->setText(
          "<html><head/><body><p align=\"center\"><span style=\" "
          "font-size:11pt;\">Local Server Connection Status: <span "
          "style=\"color: red;\">FAILED</span></span></p></body></html>");
    }
    watcher->deleteLater();
  });
  watcher->setFuture(future);
}
