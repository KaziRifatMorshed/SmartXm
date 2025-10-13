#ifndef CREATEORMODIFYEXAM_H
#define CREATEORMODIFYEXAM_H

#include <QWidget>

namespace Ui
{
    class CreateOrModifyExam;
}

class CreateOrModifyExam : public QWidget
{
    Q_OBJECT

public:
    explicit CreateOrModifyExam(QWidget* parent = nullptr);
    ~CreateOrModifyExam();

private:
    Ui::CreateOrModifyExam* ui;
};

#endif // CREATEORMODIFYEXAM_H
