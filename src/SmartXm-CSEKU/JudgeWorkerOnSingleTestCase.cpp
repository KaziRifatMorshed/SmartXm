#include "JudgeWorkerOnSingleTestCase.h"
#include <QThread>
#include <QDebug>
#include <chrono>
#include <thread>

JudgeWorkerOnSingleTestCase::JudgeWorkerOnSingleTestCase(const std::vector<std::string> &testcases,
                                                         const std::vector<double> &judgeInformation,
                                                         QObject *parent)
    : QObject(parent),
    testcases(testcases),
    judgeInformation(judgeInformation)
{
    jjudge = new Judge();
}

JudgeWorkerOnSingleTestCase::~JudgeWorkerOnSingleTestCase()
{
    if(jjudge) {
        delete jjudge;
        jjudge = nullptr;
    }
}

void JudgeWorkerOnSingleTestCase::judge()
{
    Verdict verdict = jjudge->runOnSingleTestCase(testcases, judgeInformation);
    std::cout<<"I am Here\n";
    emit finished(verdict);
}

void JudgeWorkerOnSingleTestCase::killJudge()
{
    if(jjudge)
        jjudge->stopJudge();
}
