#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include "TerminalExecuter.h"
#include "dependencies/linux/Encryption/encryption.h"
#include <iostream>

WelcomeWindow::WelcomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WelcomeWindow)
    , m_networkManager(new QNetworkAccessManager(this))
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


void WelcomeWindow::checkConnection()
{
    QNetworkRequest request(QUrl("http://www.google.com"));
    m_networkManager->head(request);
    qDebug() << "Checking for internet connection...";
}

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



void WelcomeWindow::on_sync_remoteS_pushButton_2_clicked()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString pyFetcherPath;

#ifdef __linux__
    pyFetcherPath = executablePath + "/dependencies/linux/WebScrap2";
#elif _WIN32
    pyFetcherPath = executablePath + "/dependencies/win/WebScrap2"; // forgot
#endif

    DownloadDataAndSaveCSV:
    std::cout << "Downloading Data from Remote Server and Saving CSV file inside db"<< std::endl;
    std::string downCSVdataCMD = "cd " + executablePath.toStdString() + "/db && " + pyFetcherPath.toStdString();
    // std::cout << downCSVdataCMD << std::endl;
    std::string downCSVdataOutput = termiExec(downCSVdataCMD.c_str());
    // std::cout << downCSVdataOutput << std::endl;

    DecryptCSVdata:
    Encryption encryption = Encryption("./db/remoteData.csv", "./db/remoteData-dec.csv", 20);
    std::cout << "Decrypting Data" << std::endl;
    encryption.decrypt();
} // working


void WelcomeWindow::on_pushButton_clicked()
{
    bool temp = true;
    bool isTeacher = true;
    ///
    if(temp) {
        if(isTeacher){
            hide();
            teacherModuleWindow = new TeacherModule(this);
            teacherModuleWindow->show();
        } else {

        }
    }
}

