#ifndef EVALUATION_H
#define EVALUATION_H
#include <QMessageBox>
#include <QMainWindow>
#include <QWidget>
#include <filesystem>
#include<QString>
#include<verdict.h>
#include<Judge.h>
#include <thread>          // For std::thread
#include <mutex>           // For std::mutex, std::lock_guard, std::unique_lock
#include <condition_variable>  // For std::condition_variable
#include <atomic>          // For std::atomic<bool>, std::atomic<int>
#include <queue>           // For std::queue
#include <vector>          // For std::vector
#include <string>          // For std::string
#include <iostream>        // For std::cout, std::cerr
#include<set>
#include<JudgeWorker4.h>
#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#endif

namespace Ui
{
    class Evaluation;
}

class Evaluation : public QMainWindow
{
    Q_OBJECT

public:

    explicit Evaluation(QWidget* parent = nullptr);
    ~Evaluation();

private slots:
    void on_tabNext_pushButton_clicked();

    void on_tabPrev_pushButton_2_clicked();

    void on_complete_pushButton_clicked();

    void on_evaluate_clicked();

    void on_reEvaluate_clicked();

    void on_stopEvaluation_clicked();

    void on_runThisFile_pushButton_2_clicked();

    void on_stopSingleJudge_clicked();

private:
    Ui::Evaluation* ui;
    bool evaluating=false;
    bool evaluating2=false;

    bool termination=false;

    std::set<Judge *>judges;
    bool readStudentInfo(const std::string &submissionInfoFile, std::vector<std::string> &submissionInformation);
    bool readJudgeInfo(const std::string &judgeInfoPath, std::vector<std::vector<double>> &judgeInformation);
    bool readTestCaseInfo(const std::string &testCaseInfoFile, std::vector<int> &testCaseInformation);
    bool readSubmissionFileInfo(const std::string &submissionFilePath, std::vector<std::string> &submissionFiles);
    unsigned int getPhysicalCoreCount();
    JudgeWorker4 *judgeWorker4;
    QThread *threadJudge;

#ifdef _WIN32
    QString dirPath = "C:/SmartXm/Editor/";
    QString systemDirPath = "C:/SmartXm/System/";
#else
    QString dirPath = "/SmartXm/Editor/";
    QString systemDirPath = "/SmartXm/System/";
#endif
};

#endif // EVALUATION_H
