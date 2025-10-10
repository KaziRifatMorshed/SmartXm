
#pragma once
#include <QObject>
#include <QString>
#include <string>
#include "codeRunner.h"

class CodeRunnerWorker : public QObject
{
    Q_OBJECT
public:
    explicit CodeRunnerWorker(const std::string &file, QObject *parent = nullptr);


public slots:
    void run();  // will be called in a separate thread
    void killExecution();

signals:
    void finished();

private:
    std::string currentFile;
    CodeRunner *runner=new CodeRunner();
};
