#ifndef TEACHERMANAGEMENTWINDOW_H
#define TEACHERMANAGEMENTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class TeacherManagementWindow;
}
QT_END_NAMESPACE

class TeacherManagementWindow : public QMainWindow
{
    Q_OBJECT

public:
    TeacherManagementWindow(QWidget *parent = nullptr);
    ~TeacherManagementWindow();

private:
    Ui::TeacherManagementWindow *ui;
};
#endif // TEACHERMANAGEMENTWINDOW_H
