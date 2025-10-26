#include "JudgeWorker4.h"


#include <QString>

JudgeWorker4::JudgeWorker4(const std::vector<std::string> &testcaseData,
                           const std::vector<double> &judgeInformation,
                           int totalTestCases,
                           QObject *parent)
    : QObject(parent),
    testcaseData(testcaseData),
    judgeInformation(judgeInformation),
    totalTestCases(totalTestCases)
{}

void JudgeWorker4::process()
{
    std::vector<Verdict> verdicts;

    judge=new Judge();
    judge->setJudgeInfo(judgeInformation);
    judge->setCurrentFile(testcaseData[0]);
    judge->setCurrentProblem("Problem");
    judge->setPretestCasesPath(testcaseData[1]);

    judge->setNumberOfTotalTestCase(totalTestCases);
    verdicts = judge->runOnTestCases();



    emit finished(verdicts);
}
void JudgeWorker4::killJudge()
{
    judge->stopJudge();
}
