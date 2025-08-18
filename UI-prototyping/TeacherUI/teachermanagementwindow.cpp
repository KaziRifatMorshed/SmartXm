#include "teachermanagementwindow.h"
#include "./ui_teachermanagementwindow.h"

TeacherManagementWindow::TeacherManagementWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TeacherManagementWindow)
{
    ui->setupUi(this);
}

TeacherManagementWindow::~TeacherManagementWindow()
{
    delete ui;
}
