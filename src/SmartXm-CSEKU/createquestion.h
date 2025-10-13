#ifndef CREATEQUESTION_H
#define CREATEQUESTION_H

#include <QWidget>
#include<QThread>
#include<JudgeWorker3.h>
#include<verdict.h>

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

    void on_radioButton_clicked();

    void on_typeQuestionManually_radioButton_clicked();

    void on_uploadPdf_radioButton_clicked();

    void on_inputTypeManually_radioButton_4_clicked();

    void on_inputSeleceFile_radioButton_4_clicked();

    void on_RunSolution_execute_pushButton_2_clicked();

    void on_newTestCaseINPUT_comboBox_currentIndexChanged(int index);

    void on_testcaseOutput_comboBox_currentIndexChanged(int index);

    void on_stopExecution_clicked();

private:
    Ui::CreateQuestion* ui;
    QString path;
    void createFolder();
    void writeQuestionToHTML();
    void convertHtmlToPdf(QString source, QString destination);
    QString getFileContent(QString path);
    void saveToFile(QString path, QString& textToSave);
    void loadPreviouslySavedQuestion(QString path);
    JudgeWorker3 *judgeWorker;
    QThread *judgeThread;
    bool judging =false;
};

#endif // CREATEQUESTION_H
