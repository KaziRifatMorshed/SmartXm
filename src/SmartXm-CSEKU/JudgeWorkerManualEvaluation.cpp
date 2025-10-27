
#include "JudgeWorkerManualEvaluation.h"


#include <QString>

    JudgeWorkerManualEvaluation::JudgeWorkerManualEvaluation(const std::vector<std::string> &testcaseData,
                               const std::vector<double> &judgeInformation,
                               int totalTestCases,
                               QObject *parent)
    : QObject(parent),
    testcaseData(testcaseData),
    judgeInformation(judgeInformation),
    totalTestCases(totalTestCases)
{}

void JudgeWorkerManualEvaluation::process()
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
void JudgeWorkerManualEvaluation::killJudge()
{
    judge->stopJudge();
}
