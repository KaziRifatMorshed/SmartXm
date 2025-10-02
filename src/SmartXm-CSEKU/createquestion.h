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

private slots:
    void on_save_pushButton_clicked();

private:
    Ui::CreateQuestion* ui;
    QString path;
    void createFolder();
    void writeQuestionToHTML();
    void convertHtmlToPdf();
};

#endif // CREATEQUESTION_H
