#include "studentmodule.h"
#include "ui_studentmodule.h"
#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QTime>
#include <QUrl>
#include <TerminalExecuter.h>
#include <iostream>

StudentModule::StudentModule(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::StudentModule) {
  ui->setupUi(this);
  ui->openRuleBook_pushButton->setDisabled(false);

  // using a button
  QPushButton *examTableActionBtn = new QPushButton("Enter");
  ui->exam_list_tableWidget->setCellWidget(0, 8, examTableActionBtn);
}

StudentModule::~StudentModule() { delete ui; }

void StudentModule::on_exitBtn_profileTab_pushButton_2_clicked() { close(); }

void _showMsgBox(QWidget *parent, QString type, QString title, QString text) {
  if (type == "information") {
    QMessageBox::information(parent, title, text);
  } else if (type == "warning") {
    QMessageBox::warning(parent, title, text);
  } else if (type == "critical") {
    QMessageBox::critical(parent, title, text);
  }
}
void StudentModule::showMsgBox(QString type, QString title, QString text) {
    if (type == "information") {
        QMessageBox::information(this, title, text);
    } else if (type == "warning") {
        QMessageBox::warning(this, title, text);
    } else if (type == "critical") {
        QMessageBox::critical(this, title, text);
    }
}

void StudentModule::on_openRuleBook_pushButton_clicked() {
  // showMsgBox("information", "Rulebook", "Rulebook received! Click OK to open rulebook."); // remove later
  std::cout << "Opening rulebook/instructions" << std::endl;
  QString rulebookPath; // this needs to modify
  // get a QMsgBox and then btn will be open
  // open received rule book

  // QString filePath = QDir::homePath() + "/a.pdf";
  // std::cout << filePath.toStdString() << std::endl;
  // if (!QFile::exists(filePath)) {
  //     qDebug() << "File does not exist:" << filePath;
  // } else {
  //     QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
  // }
  QProcess pOpenFile;
#ifdef __WIN32

#elif __linux__
  QProcess::startDetached("/usr/bin/microsoft-edge-stable",
                          QStringList() << "/home/noobcod3r-rtx/a.pdf");

  /* DOES NOT WORK
      // QProcess::startDetached("/usr/bin/xdg-open", QStringList() <<
     "/home/noobcod3r-rtx/a.pdf");
      // // system("/usr/bin/xdg-open /home/noobcod3r-rtx/Documents/a.pdf");
      // pOpenFile.start("/usr/bin/xdg-open", QStringList() <<
     "/home/noobcod3r-rtx/a.mp4");
      // // pOpenFile.startDetached("ls");
      // pOpenFile.waitForFinished();
      // std::cout << "PRINTING OuTpUtS: " <<
     pOpenFile.readAllStandardOutput().toStdString() << std::endl;

      //
     QDesktopServices::openUrl(QUrl::fromLocalFile("/home/noobcod3r-rtx/a.pdf"));

      // const char* cmdd = "/usr/bin/xdg-open
     /home/noobcod3r-rtx/Documents/a.pdf";
      // std::string s = termiExec(cmdd);
   */

#endif
}

void StudentModule::ruleBookReceived() {
  QString currentTime = QTime::currentTime().toString();
  ui->openRuleBook_pushButton->setDisabled(false);
  ui->roolbook_receive_status_text_label->setText(
      "Rulebook received from server at " + currentTime);
  std::cout << "Rulebook received from server at " << currentTime.toStdString()
            << std::endl;
}
