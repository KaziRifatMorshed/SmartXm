#ifndef IDE_H
#define IDE_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <string>

namespace Ui
{
    class IDE;
}

class IDE : public QMainWindow
{
    Q_OBJECT

public:
    explicit IDE(QWidget* parent = nullptr);
    ~IDE();
    void loadPdfInQuesTab(QWidget* ques_tab, const QString& pdfFilePath);

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
};

#endif // IDE_H
