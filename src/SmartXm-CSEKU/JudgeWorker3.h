#ifndef JUDGEWORKER3_H
#define JUDGEWORKER3_H


#pragma once
#include <QObject>
#include <QString>
#include <vector>
#include "Judge.h"

// Forward declare the getFileContent function if it's global


class JudgeWorker3 : public QObject
{
    Q_OBJECT
public:
    explicit JudgeWorker3(const std::vector<std::string> &testcaseData,
                          const std::vector<double> &judgeInformation,
                          const std::string &systemDirPath,
                          const std::string &dirPath,
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
    std::string systemDirPath;
    std::string dirPath;
    int totalTestCases;
    Judge *judge;
};

#endif // JUDGEWORKER3_H
