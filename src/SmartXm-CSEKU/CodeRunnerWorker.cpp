
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
    QString outputText, debugText;

    QFile outFile("output.txt");
    if (outFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&outFile);
        outputText = in.readAll();
        outFile.close();
    }

    QFile errFile("error.txt");
    if (errFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&errFile);
        debugText = in.readAll();
        errFile.close();
    }

    emit finished(outputText, debugText);
}
