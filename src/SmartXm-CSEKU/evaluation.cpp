#include "evaluation.h"
#include "ui_evaluation.h"
#include <iostream>
#include<fstream>
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


bool Evaluation::readSubmissionInfo(const std::string &submissionInfoFile, std::vector<int> &submissionInformation)
{
    std::ifstream submissionInfoIn(submissionInfoFile);
    if (!submissionInfoIn.is_open())
    {

        return false;
    }

    submissionInformation.clear();
    int n;
    submissionInfoIn>>n;
    submissionInformation.resize(n);
    for (int i = 0; i < n; i++)
    {
        int d;
        if (!(submissionInfoIn >> d))
        {


            return false;
        }

        submissionInformation[i]=d;
    }


    return true;
}
bool Evaluation::readTestCaseInfo(const std::string &testCaseInfoFile, std::vector<int> &testCaseInformation)
{
    std::ifstream testCaseInfoIn(testCaseInfoFile);
    if (!testCaseInfoIn.is_open())
    {

        return false;
    }

    testCaseInformation.clear();
    int n;
    testCaseInfoIn>>n;
    testCaseInformation.resize(n);
    for (int i = 0; i < n; i++)
    {
        int d;
        if (!(testCaseInfoIn >> d))
        {


            return false;
        }

        testCaseInformation[i]=d;
    }


    return true;
}
bool Evaluation::readJudgeInfo(const std::string &judgeInfoPath, std::vector<std::vector<double>> &judgeInformation)
{
    int n=judgeInformation.size();
    int i;
    for(char ch='A',i=0;i<n,ch++;i++)
    {
        std::string judgeInfoFile=judgeInfoPath+"/"+ch+".txt";
        std::ifstream judgeInfoIn(judgeInfoFile);
        if (!judgeInfoIn.is_open())
        {

            return false;
        }

        judgeInformation.clear();
        for (int i = 0; i < 14; i++)
        {
            double d;
            if (!(judgeInfoIn >> d))
            {


                return false;
            }

            judgeInformation[i].push_back(d);
        }
    }

    return true;
}


void Evaluation::on_evaluate_clicked()
{
    if(!evaluating)
    {
        evaluating=true;
        std::cout<<"OKay"<<std::endl;
        std::vector<int> submissionInformation;
        std::string submissionInfoFile = systemDirPath.toStdString()  + "submissionInfo.txt";

        if(!readSubmissionInfo(submissionInfoFile,submissionInformation))
        {
            QMessageBox::warning(this, "Warning",
                                 "Submission information not found for evaluation.");
            evaluating=false;
            return;
        }
        std::vector<int> testCaseInformation;
        std::string testCaseInfoFile = systemDirPath.toStdString()  + "testCaseInfo.txt";

        if(!readTestCaseInfo(testCaseInfoFile,testCaseInformation))
        {
            QMessageBox::warning(this, "Warning",
                                 "Test Case information not found for evaluation.");
            evaluating=false;
            return;
        }
        int numberOfProblem=testCaseInformation.size();
        std::vector<std::vector<double>> judgeInformation(numberOfProblem);
         std::string judgeInfoPath = systemDirPath.toStdString()  + "/Judge";
        if(!readJudgeInfo(judgeInfoPath,judgeInformation))
        {
            QMessageBox::warning(this, "Warning",
                                 "Judge information not found for evaluation.");
            evaluating=false;
            return;
        }
        std::cout<<numberOfProblem<<std::endl;
        evaluating=false;

    }

}


void Evaluation::on_reEvaluate_clicked()
{

}


void Evaluation::on_stopEvaluation_clicked()
{
    if(evaluating)
    {
        std::cout<<"Nothing is evalutaing"<<std::endl;
    }
}

