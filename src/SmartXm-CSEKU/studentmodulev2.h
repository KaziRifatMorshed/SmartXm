#ifndef STUDENTMODULEV2_H
#define STUDENTMODULEV2_H

#include <QMainWindow>

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

private:
    Ui::StudentModuleV2* ui;
};

#endif // STUDENTMODULEV2_H
