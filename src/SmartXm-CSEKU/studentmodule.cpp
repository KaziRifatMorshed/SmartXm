#include "studentmodule.h"
#include "ui_studentmodule.h"

StudentModule::StudentModule(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudentModule)
{
    ui->setupUi(this);
}

StudentModule::~StudentModule()
{
    delete ui;
}
