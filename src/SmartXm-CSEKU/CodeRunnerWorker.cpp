
#include "CodeRunnerWorker.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

CodeRunnerWorker::CodeRunnerWorker(const std::string &file, QObject *parent)
    : QObject(parent), currentFile(file)
{
}

void CodeRunnerWorker::run()
{

    runner.setCurrentFile(currentFile);
    runner.run(); // blocking, safe in this thread





    emit finished();
}


