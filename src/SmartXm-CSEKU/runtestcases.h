#ifndef RUNTESTCASES_H
#define RUNTESTCASES_H

#include <QWidget>

namespace Ui
{
    class runTestcases;
}

class runTestcases : public QWidget
{
    Q_OBJECT

public:
    explicit runTestcases(QWidget* parent = nullptr);
    ~runTestcases();

private:
    Ui::runTestcases* ui;
};

#endif // RUNTESTCASES_H
