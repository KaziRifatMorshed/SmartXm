#include "runtestcases.h"
#include "ui_runtestcases.h"

runTestcases::runTestcases(QWidget* parent) : QWidget(parent), ui(new Ui::runTestcases) { ui->setupUi(this); }

runTestcases::~runTestcases() { delete ui; }

void runTestcases::on_runThisTestcase_pushButton_clicked()
{

}

