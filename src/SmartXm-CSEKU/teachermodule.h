#ifndef TEACHERMODULE_H
#define TEACHERMODULE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class TeacherModule;
}
QT_END_NAMESPACE

class TeacherModule : public QMainWindow
{
    Q_OBJECT

public:
    TeacherModule(QWidget *parent = nullptr);
    ~TeacherModule();

private slots:
    void on_StartServer_toolButton_clicked();

    void on_StopServer_toolButton_2_clicked();

    void on_selectFile_pushButton_clicked();

    void on_instruction_send_pushButton_clicked();

    void on_testExam_pushButton_3_clicked();

private:
    Ui::TeacherModule *ui;
};
#endif // TEACHERMODULE_H
