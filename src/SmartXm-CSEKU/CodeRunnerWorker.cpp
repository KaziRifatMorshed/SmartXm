
#include "CodeRunnerWorker.h"
#include "codeRunner.h"
#include <QFile>
#include <QTextStream>

CodeRunnerWorker::CodeRunnerWorker(const std::string &file, QObject *parent)
    : QObject(parent), currentFile(file)
{
}

void CodeRunnerWorker::run()
{
    CodeRunner runner;
    runner.setCurrentFile(currentFile);
    runner.run(); // blocking, safe in this thread

           // Read output and error files



    emit finished();
}
