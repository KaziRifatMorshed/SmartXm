#include "studentmodulev2.h"
#include "ui_studentmodulev2.h"
#include <QProcess>
#include <iostream>
#include <stdlib.h>
#include <QTime>

StudentModuleV2::StudentModuleV2(QWidget* parent) : QMainWindow(parent), ui(new Ui::StudentModuleV2) {
    ui->setupUi(this);
}

StudentModuleV2::~StudentModuleV2() { delete ui; }

void StudentModuleV2::on_openCodeEditor_pushButton_clicked()
{
    ideEditor = IDE::getInstance(this);
    ideEditor->show();
}


void StudentModuleV2::on_openRulebook_pushButton_clicked()
{
    std::cout << "Opening rulebook/instructions" << std::endl;
    // QString rulebookPath;

#ifdef _WIN32
    QString filePath = QDir::toNativeSeparators(".\\examResources\\rulebook.pdf");
    QStringList args;
    system("echo %cd%");
    args << "/C" << "start" << "" << filePath;
    QProcess::startDetached("cmd", args);
/*
    23:27:35: Starting C:\Users\tiny10-RTX\Documents\GitHub\SmartXm\src\SmartXm-CSEKU\build\Desktop_Qt_6_9_3_MinGW_64_bit-Debug\debug\SmartXm-CSEKU.exe...
    Opening rulebook/instructions
    C:\Users\tiny10-RTX\Documents\GitHub\SmartXm\src\SmartXm-CSEKU\build\Desktop_Qt_6_9_3_MinGW_64_bit-Debug
*/

#elif __linux__
    QProcess::startDetached("./dependencies/qpdfjs/qpdfjs",
                            QStringList() << "./examResources/rulebook.pdf");
#endif
}

void StudentModuleV2::ruleBookReceived() {
    QString currentTime = QTime::currentTime().toString();
    ui->openRulebook_pushButton->setDisabled(false);
    ui->rulebook_recv_status_label_2->setText(
        "Rulebook received from server at " + currentTime);
    std::cout << "Rulebook received from server at " << currentTime.toStdString()
              << std::endl;
}
