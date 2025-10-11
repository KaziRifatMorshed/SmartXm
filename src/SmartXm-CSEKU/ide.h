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
    void terminateExecution();
    void loadProblem();

    void on_actionTestcases_triggered();
    void on_actionRun_triggered();
    void on_actionTerminate_triggered();

    void on_actionRun_Testcases_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionLoad_Questions_triggered();

    void on_actionExit_triggered();

private:
    Ui::IDE* ui;
    QFileSystemModel *model;
#ifdef _WIN32
    QString dirPath = "C:/SmartXM/230201/Editor/";
    QString systemDirPath = "C:/SmartXM/230201/System/";
#else
    QString dirPath = "/SmartXm/230201/Editor/";
    QString systemDirPath = "/SmartXm/230201/System/";
#endif
    QString currentFile;
    QString getFileContent(QString path);
    QThread *threadExecution;
    CodeRunnerWorker *workerExecution;
    bool executionThreadFlag;
    bool forciblyKillExecutionFlag;
    void initialize();
    void loadInput(std::string path);
    void loadOutput(std::string path);
};

#endif // IDE_H
