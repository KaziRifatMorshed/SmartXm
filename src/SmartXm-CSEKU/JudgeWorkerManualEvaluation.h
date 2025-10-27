#ifndef JUDGEWORKERMANUALEVALUATION_H
#define JUDGEWORKERMANUALEVALUATION_H


#pragma once
#include <QObject>
#include <QString>
#include <vector>
#include "Judge.h"

    // Forward declare the getFileContent function if it's global


    class JudgeWorkerManualEvaluation : public QObject
{
    Q_OBJECT
public:
    explicit JudgeWorkerManualEvaluation(const std::vector<std::string> &testcaseData,
                          const std::vector<double> &judgeInformation,
                          int totalTestCases,
                          QObject *parent = nullptr);

signals:
    void finished(std::vector<Verdict> verdicts);

public slots:
    void process();
    void killJudge();

private:
    std::vector<std::string> testcaseData;
    std::vector<double> judgeInformation;

    int totalTestCases;
    Judge *judge;
};


#endif // JUDGEWORKERMANUALEVALUATION_H
