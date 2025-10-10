#ifndef RUNTESTCASES_H
#define RUNTESTCASES_H

#include <QWidget>
#include <filesystem>
#include<iostream>
#include "verdict.h"
#include<JudgeWorkerOnSingleTestCase.h>

namespace Ui
{
    class runTestcases;
}

class runTestcases : public QWidget
{
    Q_OBJECT

public:
    explicit runTestcases(QWidget* parent = nullptr);
    ~runTestcases();

private slots:

    void on_runThisTestcase_pushButton_clicked();

    void on_SelectProblem_comboBox_activated(int index);

    void on_SelectProblem_comboBox_currentIndexChanged(int index);

    void on_SelectTestcase_comboBox_currentIndexChanged(int index);

    void on_runAllTestcases_pushButton_2_clicked();

    void on_pushButton_clicked();
private:

    QThread *threadJudgeSingle;
    JudgeWorkerOnSingleTestCase *judgeWorker;

private:
#ifdef _WIN32
    QString dirPath = "C:/SmartXM/230201/Editor/";
    QString systemDirPath = "C:/SmartXM/230201/System/";
#else
    QString dirPath = "/SmartXM/230201/Editor/";
    QString systemDirPath = "/SmartXM/230201/System/";
#endif

    std::vector <std::string> listFilesInDirectory(std::filesystem::path directoryPath);
    QString getFileContent(QString path);
    std::vector <std::string> splitStringByChar(std::string s, char delimiter);
    std::vector <std::string> getTestcaseData();
    Ui::runTestcases* ui;
    bool judging=false;
    Judge *judge;

};

#endif // RUNTESTCASES_H
