#include "teachermodule.h"
#include "ui_teachermodule.h"

TeacherModule::TeacherModule(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TeacherModule)
{
    ui->setupUi(this);
}

TeacherModule::~TeacherModule()
{
    delete ui;
}
