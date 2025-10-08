
#pragma once
#include <QObject>
#include <QString>
#include <string>

class CodeRunnerWorker : public QObject
{
    Q_OBJECT
public:
    explicit CodeRunnerWorker(const std::string &file, QObject *parent = nullptr);

public slots:
    void run();  // will be called in a separate thread

signals:
    void finished();

private:
    std::string currentFile;
};
