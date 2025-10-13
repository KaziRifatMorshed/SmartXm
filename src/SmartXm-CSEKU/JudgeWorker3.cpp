#include "JudgeWorker3.h"
// include the file where getFileContent is defined
#include <QString>

JudgeWorker3::JudgeWorker3(const std::vector<std::string> &testcaseData,
                           const std::vector<double> &judgeInformation,
                           const std::string &systemDirPath,
                           const std::string &dirPath,
                           int totalTestCases,
                           QObject *parent)
    : QObject(parent),
    testcaseData(testcaseData),
    judgeInformation(judgeInformation),
    systemDirPath(systemDirPath),
    dirPath(dirPath),
    totalTestCases(totalTestCases)
{}

void JudgeWorker3::process()
{
    std::vector<Verdict> verdicts;

    judge=new Judge();
    judge->setJudgeInfo(judgeInformation);
    judge->setCurrentFile(dirPath + testcaseData[2]);
    judge->setCurrentProblem(std::string()+testcaseData[0][0]);
    judge->setPretestCasesPath(systemDirPath + "Pretest/" + testcaseData[0][0] + "/");

    judge->setNumberOfTotalTestCase(totalTestCases);
    verdicts = judge->runOnTestCases();



    emit finished(verdicts);
}
void JudgeWorker3::killJudge()
{
    judge->stopJudge();
}
