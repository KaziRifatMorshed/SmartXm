#include "createormodifyexam.h"
#include "ui_createormodifyexam.h"

CreateOrModifyExam::CreateOrModifyExam(QWidget* parent) : QWidget(parent), ui(new Ui::CreateOrModifyExam) {
    ui->setupUi(this);
}

CreateOrModifyExam::~CreateOrModifyExam() { delete ui; }
