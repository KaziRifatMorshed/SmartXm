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


bool Evaluation::readStudentInfo(const std::string &studentInfoFile, std::vector<std::string> &studentInformation)
{
    std::ifstream studentInfoIn(studentInfoFile);
    if (!studentInfoIn.is_open())
    {

        return false;
    }

    studentInformation.clear();
    int n;
    studentInfoIn>>n;
    studentInformation.resize(n);
    for (int i = 0; i < n; i++)
    {
        std::string s;
        if (!(studentInfoIn >> s))
        {


            return false;
        }

        studentInformation[i]=s;
    }


    return true;
}
bool Evaluation::readSubmissionFileInfo(const std::string &submissionFilePath, std::vector<std::string> &submissionFiles)
{
    int n=submissionFiles.size();
    int i;
    char ch='A';
    for(i=0;i<n;i++)
    {
        std::string submissionFile=submissionFilePath+ch+".txt";

        std::ifstream submissionFileIn(submissionFile);
        if (!submissionFileIn.is_open())
        {

            return false;
        }


        std::string s;
        submissionFileIn >> s;
        std::string solutionFile=submissionFilePath+ch+"/"+s;
        submissionFiles[i]=solutionFile;
        ch++;
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
    char ch='A';
    for(i=0;i<n;i++)
    {
        std::string judgeInfoFile=judgeInfoPath+ch+".txt";

        std::ifstream judgeInfoIn(judgeInfoFile);
        if (!judgeInfoIn.is_open())
        {

            return false;
        }

        judgeInformation.clear();
        for (int i = 0; i < n; i++)
        {
            double d;
            if (!(judgeInfoIn >> d))
            {


                return false;
            }

            judgeInformation[i].push_back(d);
        }
        ch++;
    }

    return true;
}


void Evaluation::on_evaluate_clicked()
{
    if(!evaluating)
    {
        evaluating=true;

        std::vector<std::string> studentInformation;
        std::string studentInfoFile = systemDirPath.toStdString()  + "studentInfo.txt";

        if(!readStudentInfo(studentInfoFile,studentInformation))
        {
            QMessageBox::warning(this, "Warning",
                                 "student information not found for evaluation.");
            evaluating=false;
            return;
        }
        int numOfStudents=studentInformation.size();
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
        std::string judgeInfoPath = systemDirPath.toStdString()  + "Judge/";
        if(!readJudgeInfo(judgeInfoPath,judgeInformation))
        {
            QMessageBox::warning(this, "Warning",
                                 "Judge information not found for evaluation.");
            evaluating=false;
            return;
        }

        std::vector<std::pair<std::string,std::vector<std::string>>>submissionInformation;
        int i;
        for(i=0;i<numOfStudents;i++)
        {
            std::string stID=studentInformation[i];
            std::vector<std::string>submissionsFile(numberOfProblem);
            std::string submissionFilePath=systemDirPath.toStdString()+"Submissions/"+stID+"/";
            if(!readSubmissionFileInfo(submissionFilePath,submissionsFile))
            {
                QMessageBox::warning(this, "Warning",
                                     "Submission Folder not found for evaluation.");
                evaluating=false;
                return;
            }
            submissionInformation.push_back({stID,submissionsFile});

        }
        for(i=0;i<numOfStudents;i++)
        {
            std::cout<<submissionInformation[i].first<<std::endl;
            for(auto it:submissionInformation[i].second)
            {
                std::cout<<"        "<<it<<std::endl;
            }

        }
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

