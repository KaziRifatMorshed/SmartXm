#include "runtestcases.h"
#include "ui_runtestcases.h"
#include <QFile>
#include <QMessageBox>
#include <filesystem>
#include <verdict.h>

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

void runTestcases::on_runThisTestcase_pushButton_clicked()
{
    std::vector <std::string> testcaseData = getTestcaseData();

    if (testcaseData[1] == "No sample testcases") {
        QMessageBox::warning(this, "Warning",
                             "No sample testcases for this problem exists.");

        return;
    }

    if (testcaseData[2] == "No solutions") {
        QMessageBox::warning(this, "Warning",
                             "No solution file selected.");

        return;
    }

    testcaseData[1].erase(0, std::string("Sample Testcase #").size());

    ui->ProblemTitle_label_7->setText(("Problem: " + testcaseData[0]).c_str());

    QString testInput = getFileContent((systemDirPath.toStdString() + "Pretest/" + testcaseData[0][0] + "/" + testcaseData[1] + ".in").c_str());

    ui->testcaseInput_textEdit_2->setPlainText(testInput);

    // code run

    // end code run

    QString expectedOutput = getFileContent((systemDirPath.toStdString() + "Pretest/" + testcaseData[0][0] + "/" + testcaseData[1] + ".out").c_str());
    ui->testcaseExpectedOutput_textEdit->setPlainText(expectedOutput);

    QString userOutput = getFileContent((systemDirPath.toStdString() + "Pretest/" + testcaseData[0][0] + "/" + testcaseData[1] + ".output").c_str());
    ui->studentOutput_textEdit_3->setPlainText(userOutput);

    Verdict verdict = {"Memory Limit Exceeded", 200, 512};

    ui->verdictTable->clear();
    ui->verdictTable->setRowCount(0);

    ui->verdictTable->setRowCount(ui->verdictTable->rowCount() + 1);
    ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 0, new QTableWidgetItem(("Sample Testcase #" + testcaseData[1]).c_str()));
    ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 1, new QTableWidgetItem(verdict.verdict.c_str()));
    ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 2, new QTableWidgetItem(std::to_string(verdict.cpu_time).c_str()));
    ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 3, new QTableWidgetItem(std::to_string(verdict.memory_size).c_str()));
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


void runTestcases::on_runAllTestcases_pushButton_2_clicked()
{
    std::vector <Verdict> verdicts = {
                                     {"AC", 200, 400},
        {"TLE", 100, 300},
        {"MLE", 2000, 500},
        {"WA", 400, 390}
};
    int counter = 1;
    std::vector <std::string> testcaseData = getTestcaseData();
    char problem = testcaseData[0][0];

    QString testsText = getFileContent(systemDirPath + "numTests.txt");
    std::string testSelectedText = splitStringByChar(testsText.toStdString(), '\n')[problem - 'A'];
    int testCases = stoi(splitStringByChar(testSelectedText, ' ')[0]);

    ui->testcaseInput_textEdit_2->clear();
    ui->verdictTable->clear();
    ui->verdictTable->setRowCount(0);

    // run code

    // end code run

    for (auto &verdict : verdicts) {
        ui->verdictTable->setRowCount(ui->verdictTable->rowCount() + 1);
        if (counter <= testCases) {
            ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 0, new QTableWidgetItem(("Sample Testcase #" + std::to_string(counter)).c_str()));
        }
        else {
            ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 0, new QTableWidgetItem(("Pre Testcase #" + std::to_string(counter - testCases)).c_str()));
        }

        counter++;

        ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 1, new QTableWidgetItem(verdict.verdict.c_str()));
        ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 2, new QTableWidgetItem(std::to_string(verdict.cpu_time).c_str()));
        ui->verdictTable->setItem(ui->verdictTable->rowCount() - 1, 3, new QTableWidgetItem(std::to_string(verdict.memory_size).c_str()));
    }
}


void runTestcases::on_pushButton_clicked()
{


}

