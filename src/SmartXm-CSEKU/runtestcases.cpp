#include "runtestcases.h"
#include "ui_runtestcases.h"
#include <QFile>
#include <fstream>
#include <QMessageBox>
#include <filesystem>


runTestcases::runTestcases(QWidget* parent) : QWidget(parent), ui(new Ui::runTestcases) {
    ui->setupUi(this);

           // problem combo box
    QString problemsText = getFileContent(systemDirPath + "problems.txt");
    std::vector <std::string> problems = splitStringByChar(problemsText.toStdString(), '\n');

    for (auto &problem : problems) {
        ui->SelectProblem_comboBox->addItem(QString(problem.c_str()));
    }

           // select solution
    std::vector <std::string> files = listFilesInDirectory(std::filesystem::path(dirPath.toStdString()));

    if (files.empty()) {
        ui->SelectSolutionFile_comboBox->addItem("No solutions");
    }

    for (auto &file : files) {
        if (file.find("Sample Testcases") == std::string::npos && file.find("Previous Codes") == std::string::npos && file.find("Problems") == std::string::npos) {
            ui->SelectSolutionFile_comboBox->addItem(file.substr(dirPath.size()).c_str());
        }
    }

    ui->verdictTable->setColumnWidth(0, 120);
    ui->verdictTable->setColumnWidth(1, 200);
    ui->verdictTable->setColumnWidth(2, 100);
}

std::vector <std::string> runTestcases::getTestcaseData()
{
    std::vector <std::string> data(3, std::string());

    data[0] = ui->SelectProblem_comboBox->currentText().toStdString();
    data[1] = ui->SelectTestcase_comboBox->currentText().toStdString();
    data[2] = ui->SelectSolutionFile_comboBox->currentText().toStdString();

    return data;
}

std::vector <std::string> runTestcases::listFilesInDirectory(std::filesystem::path directoryPath)
{
    std::vector <std::string> files;

    try {
        for (auto &entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
            if (std::filesystem::is_regular_file(entry.status())) {
                files.emplace_back(entry.path().string());
            }
        }
    }
    catch (const std::filesystem::filesystem_error &e) {
        QMessageBox::warning(this, "Warning",
                             ("Cannot access directory. " + std::string(e.what())).c_str());
    }

    return files;
}

runTestcases::~runTestcases() { delete ui; }

std::vector <std::string> runTestcases::splitStringByChar(std::string s, char delimiter)
{
    std::vector <std::string> tokens;
    std::istringstream iss(s);
    std::string token;

    while (std::getline(iss, token, delimiter)) {
        tokens.emplace_back(token);
    }

    return tokens;
}

QString runTestcases::getFileContent(QString path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning",
                             "Cannot read file: " + file.errorString());

        return QString();
    }

    QString text = file.readAll();

    file.close();

    return text;
}

QString runTestcases::getFileContent(QString path, long long readSizeByte) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning",
                             "Cannot read file: " + file.errorString());
        return QString();
    }

           // Read limited data
    QByteArray data = file.read(readSizeByte);

    file.close();

    return QString::fromUtf8(data);
}

bool runTestcases::readJudgeInfo(const std::string &judgeInfoFile, std::vector<double> &judgeInformation)
{
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

        judgeInformation.push_back(d);
    }


    return true;
}


void runTestcases::on_runThisTestcase_pushButton_clicked()
{
    if(!judging)
    {
        judgingType=1;
        judging = true;

        ui->verdictTable->setRowCount(0);
        ui->testcaseInput_textEdit_2->clear();
        ui->testcaseExpectedOutput_textEdit->clear();
        ui->studentOutput_textEdit_3->clear();

        std::vector<std::string> testcaseData = getTestcaseData();
        std::vector<double> judgeInformation;
        std::string judgeInfoFile = systemDirPath.toStdString() + "Judge/" + testcaseData[0][0] + ".txt";

        if(!readJudgeInfo(judgeInfoFile, judgeInformation))
        {
            QMessageBox::warning(this, "Warning", "Judge information not found for this problem.");
            judging = false;
            return;
        }

        if (testcaseData[1] == "No sample testcases") {
            QMessageBox::warning(this, "Warning", "No sample testcases for this problem exists.");
            judging = false;
            return;
        }

        if (testcaseData[2] == "No solutions") {
            QMessageBox::warning(this, "Warning", "No solution file selected.");
            judging = false;
            return;
        }

        testcaseData[1].erase(0, std::string("Sample Testcase #").size());
        ui->ProblemTitle_label_7->setText(("Problem: " + testcaseData[0]).c_str());

        QString testInput;
        if(judgeInformation[1])
            testInput = getFileContent((systemDirPath.toStdString() + "Pretest/" + testcaseData[0][0] + "/" + testcaseData[1] + ".in").c_str());
        else
            testInput="Input is not required for this problem.";
        ui->testcaseInput_textEdit_2->setPlainText(testInput);

               // --- Create the thread and worker ---
        QThread* thread = new QThread();
        JudgeWorker* worker = new JudgeWorker(testcaseData, judgeInformation,
                                              systemDirPath.toStdString(),
                                              dirPath.toStdString());
        threadJudge=thread;
        judgeWorker=worker;
        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &JudgeWorker::process);
        connect(worker, &JudgeWorker::finished, this, [=](Verdict verdict){

                    QString expectedOutput = getFileContent((systemDirPath.toStdString() + "Pretest/" + testcaseData[0][0] + "/" + testcaseData[1] + ".out").c_str());
                    ui->testcaseExpectedOutput_textEdit->setPlainText(expectedOutput);

                    if(verdict.verdict=="Accepted"||verdict.verdict=="Wrong Answer")
                    {
                        QString studentOutput = getFileContent((systemDirPath.toStdString() + "Pretest/" + testcaseData[0][0] + "/" + testcaseData[1]+".000000" + ".output").c_str(),1024*1024);
                        ui->studentOutput_textEdit_3->setPlainText(studentOutput);

                    }
                    ui->testcaseExpectedOutput_textEdit->setPlainText(expectedOutput);


                    ui->verdictTable->setRowCount(ui->verdictTable->rowCount() + 1);
                    ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 0, new QTableWidgetItem(("Sample Testcase #" + QString::fromStdString(testcaseData[1]))));
                    ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 1, new QTableWidgetItem(verdict.verdict.c_str()));
                    ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 2, new QTableWidgetItem(std::to_string(verdict.cpu_time).c_str()));
                    ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 3, new QTableWidgetItem(std::to_string(verdict.memory_size).c_str()));

                    judging = false;

                    thread->quit();
                    thread->wait();
                    worker->deleteLater();
                    thread->deleteLater();
                });

        thread->start();
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Another judge is running.");

        return;
    }
}


void runTestcases::on_runAllTestcases_pushButton_2_clicked()
{


    if(!judging)
    {
        judgingType=2;
        judging=true;

        ui->verdictTable->setRowCount(0);
        ui->testcaseInput_textEdit_2->clear();
        ui->testcaseExpectedOutput_textEdit->clear();
        ui->studentOutput_textEdit_3->clear();
        std::vector <std::string> testcaseData = getTestcaseData();
        std::vector<double> judgeInformation;
        std::string judgeInfoFile = systemDirPath.toStdString() + "Judge/" + testcaseData[0][0] + ".txt";

        if(!readJudgeInfo(judgeInfoFile,judgeInformation))
        {
            QMessageBox::warning(this, "Warning",
                                 "Judge information not found for this problem.");
            judging=false;
            return;
        }



        if (testcaseData[2] == "No solutions") {
            QMessageBox::warning(this, "Warning",
                                 "No solution file selected.");
            judging=false;
            return;
        }

        testcaseData[1].erase(0, std::string("Sample Testcase #").size());

        ui->ProblemTitle_label_7->setText(("Problem: " + testcaseData[0]).c_str());

        std::vector<std::string> testcasesData = getTestcaseData();
        char problem = testcasesData[0][0];


        QString testsText = getFileContent(systemDirPath + "numTests.txt");
        std::string testSelectedText = splitStringByChar(testsText.toStdString(), '\n')[problem - 'A'];
        int testCases = stoi(splitStringByChar(testSelectedText, ' ')[0]);
        int totalTestCases=stoi(splitStringByChar(testSelectedText, ' ')[0])+stoi(splitStringByChar(testSelectedText, ' ')[1]);


        QThread* thread = new QThread();
        JudgeWorker2* worker = new JudgeWorker2(testcaseData, judgeInformation,
                                                systemDirPath.toStdString(),
                                                dirPath.toStdString(),totalTestCases);
        threadJudge=thread;
        judgeWorker2=worker;
        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &JudgeWorker2::process);
        connect(worker, &JudgeWorker2::finished, this, [=](std::vector<Verdict> verdicts){

                    int counter=1;
                    for (auto &verdict : verdicts)
                    {
                        ui->verdictTable->setRowCount(ui->verdictTable->rowCount() + 1);
                        if (counter <= testCases)
                        {
                            ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 0, new QTableWidgetItem(("Sample Testcase #" + std::to_string(counter)).c_str()));
                        }
                        else
                        {
                            ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 0, new QTableWidgetItem(("Pre Testcase #" + std::to_string(counter - testCases)).c_str()));
                        }

                        counter++;

                        ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 1, new QTableWidgetItem(verdict.verdict.c_str()));
                        ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 2, new QTableWidgetItem(std::to_string(verdict.cpu_time).c_str()));
                        ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 3, new QTableWidgetItem(std::to_string(verdict.memory_size).c_str()));
                    }
                    judging = false;

                    thread->quit();
                    thread->wait();
                    worker->deleteLater();
                    thread->deleteLater();
                });

        thread->start();
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Another judge is running.");

        return;
    }
}



void runTestcases::on_SelectProblem_comboBox_activated(int index)
{

}


void runTestcases::on_SelectProblem_comboBox_currentIndexChanged(int index)
{
    // select test case combo box
    QString testsText = getFileContent(systemDirPath + "numTests.txt");
    std::string testSelectedText = splitStringByChar(testsText.toStdString(), '\n')[index];
    int testCases = stoi(splitStringByChar(testSelectedText, ' ')[0]);

    ui->SelectTestcase_comboBox->clear();

    if (testCases == 0) {
        ui->SelectTestcase_comboBox->addItem("No sample testcases");
    }

    for (int test = 1; test <= testCases; test++) {
        ui->SelectTestcase_comboBox->addItem(("Sample Testcase #" + std::to_string(test)).c_str());
    }
}


void runTestcases::on_SelectTestcase_comboBox_currentIndexChanged(int index)
{

}




void runTestcases::on_stopJudging_clicked()
{
    if(judging)
    {

        if(judgingType==1)
        {
            judgeWorker->killJudge();
        }
        else if(judgingType==2)
        {
             judgeWorker2->killJudge();
        }
    }
    else
    {
          QMessageBox::warning(this, "Warning", "There is no running judge.");
    }
}

