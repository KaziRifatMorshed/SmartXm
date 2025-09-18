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

private:
    Ui::StudentModule *ui;
};

#endif // STUDENTMODULE_H
