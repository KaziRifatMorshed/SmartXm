#include "JudgeWorker.h"
 // include the file where getFileContent is defined
#include <QString>

JudgeWorker::JudgeWorker(const std::vector<std::string> &testcaseData,
                         const std::vector<double> &judgeInformation,
                         const std::string &systemDirPath,
                         const std::string &dirPath,
                         QObject *parent)
    : QObject(parent),
    testcaseData(testcaseData),
    judgeInformation(judgeInformation),
    systemDirPath(systemDirPath),
    dirPath(dirPath)
{}

void JudgeWorker::process()
{
    Verdict verdict;

    judge=new Judge();
    judge->setJudgeInfo(judgeInformation);
    judge->setCurrentFile(dirPath + testcaseData[2]);
    judge->setCurrentProblem(std::string()+testcaseData[0][0]);
    judge->setPretestCasesPath(systemDirPath + "Pretest/" + testcaseData[0][0] + "/");
    judge->setCurrentTestCaseNo(testcaseData[1]);

    verdict = judge->runOnSingleTestCase();



    emit finished(verdict);
}
void JudgeWorker::killJudge()
{
    judge->stopJudge();
}
