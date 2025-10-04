#include "welcomewindow.h"
#include "TerminalExecuter.h"
#include "dependencies/linux/Encryption/encryption.h"
#include "ui_welcomewindow.h"
#include <QtConcurrent/QtConcurrent> // At the top
#include <db_xampp.h>
#include <iostream>
#include <networking/client/Client.h>
#include <stdlib.h>
#include <studentmodulev2.h>

Client *client;
bool clientConnectedToLocalServer = false;

// Provide the actual storage for the extern variable
StudentModuleV2 *studentModuleV2Pointer = nullptr;

WelcomeWindow::WelcomeWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::WelcomeWindow),
      m_networkManager(new QNetworkAccessManager(this)),
      statusUpdateTimer(new QTimer(this)) {
  ui->setupUi(this);
  ui->tabWidget->setTabEnabled(1, false);
  ui->tabWidget->setTabEnabled(2, false);
  ui->tabWidget->setTabEnabled(3, false);

  connect(statusUpdateTimer, &QTimer::timeout, this,
          &WelcomeWindow::updateStatusLabels);
  connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int index) { // not working, dunno why
      if (index == 1) {
          statusUpdateTimer->start(2000);
      } else {
          statusUpdateTimer->stop();
      }
  });
}

WelcomeWindow::~WelcomeWindow() { delete ui; }

void WelcomeWindow::on_exit_welcome_pushButton_4_clicked() { close(); }

bool isXamppInstalled() {
  QString installPath;

#ifdef Q_OS_WIN
  // Check standard Windowspaths (C: and D:)
  if (QDir("C:/xampp").exists()) {
    installPath = "C:/xampp";
  } else if (QDir("D:/xampp").exists()) {
    installPath = "D:/xampp";
  }
#elif defined(Q_OS_LINUX)
  // Check standard Linux (LAMPP) path
  if (QDir("/opt/lampp").exists()) {
    installPath = "/opt/lampp";
  }
#else
  // If the OS is neither Windows nor Linux, we bypass the check.
  qDebug() << "XAMPP installation check skipped for this OS.";
  return true;
#endif

  if (!installPath.isEmpty()) {
    qDebug() << "XAMPP appears to be installed at:" << installPath;
    return true;
  } else {
    qDebug() << "XAMPP installation directory not found at common locations.";
    return false;
  }
}

bool isXamppServiceRunning(const QString &host, int port) {
  // qDebug() << "Checking if MariaDB service is running on" << host << ":" <<
  // port
  //          << "...";
  QTcpSocket socket;
  // Set a short timeout (e.g., 1 second)
  socket.connectToHost(host, port);
  bool connected = socket.waitForConnected(1000);

  if (connected) {
    socket.disconnectFromHost();
    qDebug() << "MariaDB service is running on " << host << ", port: " << port;
  } else {
    qDebug() << "MariaDB service is NOT running (Connection attempt failed).";
  }

  return connected;
}

void WelcomeWindow::on_teacherWelcome_pushButton_4_clicked() {
  ui->tabWidget->setTabEnabled(1, true);
  ui->tabWidget->setTabEnabled(2, false);
  ui->tabWidget->setTabEnabled(3, false);
  ui->tabWidget->setCurrentIndex(1);

  if (isXamppInstalled()) {
    ui->xampp_installation_label_2->setText(
        "<html><head/><body><p align=\"center\">XAMPP installation: <span "
        "style=\"color:green;\">XAMPP installation detected. "
        "✅️</span></p></body></html>");
  } else {
    ui->xampp_installation_label_2->setText(
        "<html><head/><body>"
        "<p align=\"center\">"
        "XAMPP installation: <span style=\"color:red;\">XAMPP installation not "
        "found. Please install XAMPP "
        "(<a href=\"https://www.apachefriends.org/download.html\" "
        "style=\"color:red;\">https://www.apachefriends.org/download.html</a>)"
        "</span></p></body></html>");
    ui->xampp_installation_label_2->setOpenExternalLinks(true);
  }

  //   if (isXamppServiceRunning(
  //           "127.0.0.1",
  //           3306)) { // need multithreading to ensure real time update
  //     ui->xampp_status_label->setText(
  //         "<html><head/><body><p align=\"center\">XAMPP status: <span "
  //         "style=\"color:green;\">XAMPP is running.
  //         ✅️</span></p></body></html>");
  //   } else {
  //     ui->xampp_status_label->setText(
  //         "<html><head/><body>"
  //         "<p align=\"center\">"
  //         "XAMPP installation: <span style=\"color:red;\">XAMPP DB server (or
  //         " "any MySQL/MariaDB Server in 127.0.0.1:3306) is not running. "
  //         "<br>Please start XAMPP."
  //         "</span></p></body></html>");
  //   }

  // #ifdef __linux__
  //   if (system("ping -c 1 8.8.8.8 > /dev/null 2>&1") == 0) {
  // #elif _WIN32
  //   if (system("ping -n 1") == 0) {
  // #endif
  //     ui->netConnectionStatus_label->setText(
  //         "<html><head/><body><p align=\"center\">Internet conenction status:
  //         "
  //         "<span style=\"color:green;\">This PC is connected to internet. "
  //         "✅️</span></p></body></html>");
  //   } else {
  //     ui->netConnectionStatus_label->setText(
  //         "<html><head/><body>"
  //         "<p align=\"center\">"
  //         "Internet conenction status: <span style=\"color:red;\">This PC is
  //         not " "connected to internet. <br>Please connect to internet to
  //         fetch data " "from remote server."
  //         "</span></p></body></html>");
  //   }

  // connectToXamppDB();
  // testDBchanging();
}

void WelcomeWindow::updateStatusLabels() {
  QtConcurrent::run([this]() {
    bool xamppRunning = isXamppServiceRunning("127.0.0.1", 3306);
    bool internetConnected =
#ifdef __linux__
        (system("ping -c 1 8.8.8.8 > /dev/null 2>&1") == 0);
#elif _WIN32
        (system("ping -n 1 8.8.8.8 > nul") == 0);
#endif

    // Use Qt's signal/slot to update UI in main thread
    QMetaObject::invokeMethod(this, [this, xamppRunning, internetConnected]() {
      ui->xampp_status_label->setText(
          xamppRunning
              ? "<html><head/><body><p align=\"center\">XAMPP status: <span "
                "style=\"color:green;\">XAMPP is running. "
                "✅️</span></p></body></html>"
              : "<html><head/><body><p align=\"center\">XAMPP status: <span "
                "style=\"color:red;\">XAMPP DB server (or any MySQL/MariaDB "
                "Server in 127.0.0.1:3306) is not running. <br>Please start "
                "XAMPP.</span></p></body></html>");

      ui->netConnectionStatus_label->setText(
          internetConnected
              ? "<html><head/><body><p align=\"center\">Internet connection "
                "status: <span style=\"color:green;\">This PC is connected to "
                "internet. ✅️</span></p></body></html>"
              : "<html><head/><body><p align=\"center\">Internet connection "
                "status: <span style=\"color:red;\">This PC is not connected "
                "to internet. <br>Please connect to internet to fetch data "
                "from remote server.</span></p></body></html>");
    });
  });
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
  statusUpdateTimer->stop();
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
      ui->connect_to_local_server_pushButton_3->setEnabled(false);
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
