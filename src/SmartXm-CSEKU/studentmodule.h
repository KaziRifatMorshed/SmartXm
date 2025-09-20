#ifndef STUDENTMODULE_H
#define STUDENTMODULE_H

#include <QMainWindow>

namespace Ui {
class StudentModule;
}

class StudentModule : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentModule(QWidget *parent = nullptr);
    ~StudentModule();
    void showMsgBox(QString type, QString title, QString text);

private slots:
    void on_exitBtn_profileTab_pushButton_2_clicked();

    void on_openRuleBook_pushButton_clicked();

private:
    Ui::StudentModule *ui;
    void ruleBookReceived();
};

#endif // STUDENTMODULE_H
