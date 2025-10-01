#include "studentmodulev2.h"
#include "ui_studentmodulev2.h"

StudentModuleV2::StudentModuleV2(QWidget* parent) : QMainWindow(parent), ui(new Ui::StudentModuleV2) {
    ui->setupUi(this);
}

StudentModuleV2::~StudentModuleV2() { delete ui; }
