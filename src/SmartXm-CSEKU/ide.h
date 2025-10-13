#ifndef IDE_H
#define IDE_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <string>
#include "CodeRunnerWorker.h"

namespace Ui
{
    class IDE;
}

class IDE : public QMainWindow
{
    Q_OBJECT

private:
    explicit IDE(QWidget* parent = nullptr);

    ~IDE();
    void loadPdfInQuesTab(QWidget* ques_tab, std::string pdfFilePath);
    static IDE *ideInstance;

    IDE(const IDE&) = delete;
        IDE& operator = (const IDE&) = delete;

public:
    static IDE *getInstance(QWidget *parent = nullptr) {
        if (ideInstance == nullptr) {
            ideInstance = new IDE(parent);
        }

        return ideInstance;
    }



private slots:
    void newFile();
    void openFile(QString path);
    void save();
    void run();
    void loadProblem();

private:
    Ui::IDE* ui;
    QFileSystemModel *model;
    QString dirPath = QDir::homePath() + "/Desktop/Test-Notepad";
    QString currentFile;
    QString getFileContent(QString path);
    void initialize();
    void loadInput(std::string path);
    void loadOutput(std::string path);
    QThread *threadExecution;
    CodeRunnerWorker *workerExecution;
    bool executionThreadFlag;
    bool forciblyKillExecutionFlag;
};

#endif // IDE_H
