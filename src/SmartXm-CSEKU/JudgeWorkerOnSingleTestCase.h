#ifndef JUDGEWORKERONSINGLETESTCASE_H
#define JUDGEWORKERONSINGLETESTCASE_H
#pragma once
#include <QObject>
#include <QString>
#include <vector>
#include <string>
#include "Judge.h"

class JudgeWorkerOnSingleTestCase : public QObject
{
    Q_OBJECT
public:
    explicit JudgeWorkerOnSingleTestCase(const std::vector<std::string> &testcases,
                                         const std::vector<double> &judgeInformation,
                                         QObject *parent = nullptr);
    ~JudgeWorkerOnSingleTestCase();

public slots:
    void judge();       // will be called in a separate thread
    void killJudge();   // stops the judging process

signals:
    void finished(Verdict verdict);    // emitted when judging completes or stops

private:
    std::vector<std::string> testcases;
    std::vector<double> judgeInformation;
    Judge *jjudge = nullptr;
};

#endif // JUDGEWORKERONSINGLETESTCASE_H
