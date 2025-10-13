#ifndef JUDGEWORKER_H
#define JUDGEWORKER_H
#pragma once
#include <QObject>
#include <QString>
#include <vector>
#include "Judge.h"

// Forward declare the getFileContent function if it's global


class JudgeWorker : public QObject
{
    Q_OBJECT
public:
    explicit JudgeWorker(const std::vector<std::string> &testcaseData,
                         const std::vector<double> &judgeInformation,
                         const std::string &systemDirPath,
                         const std::string &dirPath,
                         QObject *parent = nullptr);

signals:
    void finished(Verdict verdict);

public slots:
    void process();
    void killJudge();

private:
    std::vector<std::string> testcaseData;
    std::vector<double> judgeInformation;
    std::string systemDirPath;
    std::string dirPath;
    Judge *judge;
};

#endif // JUDGEWORKER_H
