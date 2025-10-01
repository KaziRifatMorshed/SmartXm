// // #include "studentmodule.h"
// // #include "ui_studentmodule.h"

// // #include <QDesktopServices>
// #include <QDir>
// // #include <QMessageBox>
// // #include <QPushButton>  // <-- ADD THIS LINE!
// #include <QProcess>
// #include <QTime>
// #include <QUrl>
// // #include <TerminalExecuter.h>
// #include <iostream>

// StudentModule::StudentModule(QWidget* parent) : QMainWindow(parent), ui(new Ui::StudentModule) {

//     ui->setupUi(this);
//     // ui->openRuleBook_pushButton->setDisabled(false);

//     // QPushButton *examTableActionBtn = new QPushButton("Enter");
//     // ui->exam_list_tableWidget->setCellWidget(0, 8, examTableActionBtn);
// }

// StudentModule::~StudentModule() { delete ui; }

// void StudentModule::on_exitBtn_profileTab_pushButton_2_clicked() { close(); }

// // void _showMsgBox(QWidget *parent, QString type, QString title, QString text) {
// //     if (type == "information") {
// //         QMessageBox::information(parent, title, text);
// //     } else if (type == "warning") {
// //         QMessageBox::warning(parent, title, text);
// //     } else if (type == "critical") {
// //         QMessageBox::critical(parent, title, text);
// //     }
// // }

// // void StudentModule::showMsgBox(QString type, QString title, QString text) {
// //     if (type == "information") {
// //         QMessageBox::information(this, title, text);
// //     } else if (type == "warning") {
// //         QMessageBox::warning(this, title, text);
// //     } else if (type == "critical") {
// //         QMessageBox::critical(this, title, text);
// //     }
// // }

// void StudentModule::on_openRuleBook_pushButton_clicked() {
//     std::cout << "Opening rulebook/instructions" << std::endl;
//     QString rulebookPath;

// #ifdef _WIN32
//     QProcess::startDetached("start \"\"",
//                             QStringList() << "./examResources/rulebook.pdf");
// #elif __linux__
//     QProcess::startDetached("./dependencies/qpdfjs/qpdfjs",
//                             QStringList() << "./examResources/rulebook.pdf");
// #endif
// }

// void StudentModule::ruleBookReceived() {
//     QString currentTime = QTime::currentTime().toString();
//     ui->openRuleBook_pushButton->setDisabled(false);
//     ui->roolbook_receive_status_text_label->setText(
//         "Rulebook received from server at " + currentTime);
//     std::cout << "Rulebook received from server at " << currentTime.toStdString()
//               << std::endl;
// }

// void StudentModule::on_openCodeEditor_pushButton_clicked() {
//     ideEditor = IDE::getInstance(this);
//     ideEditor->show();
// }
