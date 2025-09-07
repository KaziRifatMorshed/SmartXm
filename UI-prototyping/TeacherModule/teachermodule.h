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

private:
    Ui::TeacherModule *ui;
};
#endif // TEACHERMODULE_H
