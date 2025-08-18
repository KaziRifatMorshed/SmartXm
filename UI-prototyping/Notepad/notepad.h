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

private slots:
    void newFile();
    void openFileNoPath();
    void openFile(QString path);
    void save();
    void saveAs();
    // void run();
    void printCurrentFile();

private:
    Ui::notepad *ui;
    QFileSystemModel *model;
    QString currentFile;
};
#endif // NOTEPAD_H
