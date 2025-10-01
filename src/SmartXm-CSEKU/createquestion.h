#ifndef CREATEQUESTION_H
#define CREATEQUESTION_H

#include <QWidget>

namespace Ui
{
    class CreateQuestion;
}

class CreateQuestion : public QWidget
{
    Q_OBJECT

public:
    explicit CreateQuestion(QWidget* parent = nullptr);
    ~CreateQuestion();

private:
    Ui::CreateQuestion* ui;
};

#endif // CREATEQUESTION_H
