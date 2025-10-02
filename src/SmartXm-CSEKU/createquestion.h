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

    void on_soluSrcCodeSaveBtn_pushButton_2_clicked();

    void on_saveEditorial_pushButton_2_clicked();

private:
    Ui::CreateQuestion* ui;
    QString path;
    void createFolder();
    void writeQuestionToHTML();
    void convertHtmlToPdf(QString source, QString destination);
    QString getFileContent(QString path);
    void saveToFile(QString path, QString& textToSave);
};

#endif // CREATEQUESTION_H
