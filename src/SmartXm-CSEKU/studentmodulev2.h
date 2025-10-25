#ifndef STUDENTMODULEV2_H
#define STUDENTMODULEV2_H

#include <QMainWindow>
#include <ide.h>

namespace Ui
{
    class StudentModuleV2;
}

class StudentModuleV2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentModuleV2(QWidget* parent = nullptr);
    ~StudentModuleV2();
    // void showMsgBox(QString type, QString title, QString text);

private slots:
    void on_openCodeEditor_pushButton_clicked();

    void on_openRulebook_pushButton_clicked();

    void on_exit_profileTab_pushButton_clicked();

    void on_sendHello_pushButton_clicked();

    void on_dummySolution_pushButton_clicked();

private:
    Ui::StudentModuleV2* ui;
    IDE *ideEditor;
    void ruleBookReceived();
};

#endif // STUDENTMODULEV2_H
