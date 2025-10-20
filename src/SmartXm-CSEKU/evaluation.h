#ifndef EVALUATION_H
#define EVALUATION_H

#include <QMainWindow>

namespace Ui
{
    class Evaluation;
}

class Evaluation : public QMainWindow
{
    Q_OBJECT

public:
    explicit Evaluation(QWidget* parent = nullptr);
    ~Evaluation();

private slots:
    void on_tabNext_pushButton_clicked();

    void on_tabPrev_pushButton_2_clicked();

    void on_complete_pushButton_clicked();

private:
    Ui::Evaluation* ui;
};

#endif // EVALUATION_H
