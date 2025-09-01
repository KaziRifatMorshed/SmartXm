#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class notepad;
}
QT_END_NAMESPACE

class notepad : public QMainWindow
{
    Q_OBJECT

public:
    notepad(QWidget *parent = nullptr);
    ~notepad();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void newFile();
    void openFileNoPath();
    void openFile(QString path);
    void save();
    void saveAs();
    void run();
    void printCurrentFile();

private:
    Ui::notepad *ui;
    QFileSystemModel *model;
    QString currentFile;
    QString shortenFileName(QString fileName);
    QString getFileContent(QString path);
    void setEditorName(QString name);
    QString getFileExtension(QString fileName);
    void setLexer();
};
#endif // NOTEPAD_H
