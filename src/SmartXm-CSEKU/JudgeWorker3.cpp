#include "JudgeWorker3.h"
// include the file where getFileContent is defined
#include <QString>

JudgeWorker3::JudgeWorker3(const std::vector<std::string> &testcaseData,
                           const std::vector<double> &judgeInformation,
                           int totalTestCases,
                           QObject *parent)
    : QObject(parent),
    testcaseData(testcaseData),
    judgeInformation(judgeInformation),
    totalTestCases(totalTestCases)
{}

void JudgeWorker3::process()
{
    std::vector<Verdict> verdicts;

    judge=new Judge();
    judge->setJudgeInfo(judgeInformation);
    judge->setCurrentFile(testcaseData[0]);
    judge->setPretestCasesPath(testcaseData[1]);
    judge->setCurrentProblem("Problem to Problem");

    judge->setNumberOfTotalTestCase(totalTestCases);
    verdicts = judge->runOnTestCases();



    emit finished(verdicts);
}
void JudgeWorker3::killJudge()
{
    judge->stopJudge();
}
