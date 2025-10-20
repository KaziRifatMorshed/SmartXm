#include "evaluation.h"
#include "ui_evaluation.h"
#include <iostream>

Evaluation::Evaluation(QWidget* parent) : QMainWindow(parent), ui(new Ui::Evaluation) {
    ui->setupUi(this);
    ui->_tabWidget->setCurrentIndex(0);
}

Evaluation::~Evaluation() { delete ui; }

void Evaluation::on_tabNext_pushButton_clicked()
{
    int idx = ui->_tabWidget->currentIndex();
    int tabCount = ui->_tabWidget->count();
    idx++;
    if(idx >= tabCount - 1) {
        idx = tabCount - 1;
    }
    std::cout << "idx = " << idx << std::endl;
    ui->_tabWidget->setCurrentIndex(idx);
}


void Evaluation::on_tabPrev_pushButton_2_clicked()
{
    int idx = ui->_tabWidget->currentIndex();
    idx = (idx - 1) % ui->_tabWidget->count();
    std::cout << "idx = " << idx << std::endl;
    ui->_tabWidget->setCurrentIndex(idx);
}


void Evaluation::on_complete_pushButton_clicked()
{

}

