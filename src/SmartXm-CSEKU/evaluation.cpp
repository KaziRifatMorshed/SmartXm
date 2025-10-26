#include "evaluation.h"
#include <fstream>
#include <iostream>
#include "ui_evaluation.h"
Evaluation::Evaluation(QWidget* parent) : QMainWindow(parent), ui(new Ui::Evaluation) {
    ui->setupUi(this);

    ui->_tabWidget->setCurrentIndex(0);

    listOfFiles = listFilesInDirectory(std::filesystem::path(dirPath.toStdString()));

    qDebug() << readStudentInfo(systemDirPath.toStdString() + "studentinfo.txt", studentIDs) << "\n";

    for (auto &s_ID : studentIDs) {
        qDebug() << "Student ID: " << QString(s_ID.c_str()) << "\n";

        ui->stuListcomboBox->addItem(QString(s_ID.c_str()));
    }

    ui->selectProblem_comboBox->setToolTip("Select Problem");

    int problemCount = std::stoi(splitStringByChar(getFileContent(systemDirPath + "testCaseInfo.txt").toStdString(), '\n')[0]);

    for (int i = 0; i < problemCount; i++) {
        ui->selectProblem_comboBox->addItem(("Problem " + std::string(1, 'A' + i)).c_str());
    }
}

Evaluation::~Evaluation() { delete ui; }

QString Evaluation::getFileContent(QString path) {
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


std::vector <std::string> Evaluation::splitStringByChar(std::string s, char delimiter)
{
    std::vector <std::string> tokens;
    std::istringstream iss(s);
    std::string token;

    while (std::getline(iss, token, delimiter)) {
        tokens.emplace_back(token);
    }

    return tokens;
}

void Evaluation::on_tabNext_pushButton_clicked() {
    int idx = ui->_tabWidget->currentIndex();
    int tabCount = ui->_tabWidget->count();
    idx++;
    if (idx >= tabCount - 1) {
        idx = tabCount - 1;
    }
    std::cout << "idx = " << idx << std::endl;
    ui->_tabWidget->setCurrentIndex(idx);
}


void Evaluation::on_tabPrev_pushButton_2_clicked() {
    int idx = ui->_tabWidget->currentIndex();
    idx = (idx - 1) % ui->_tabWidget->count();
    std::cout << "idx = " << idx << std::endl;
    ui->_tabWidget->setCurrentIndex(idx);
}


void Evaluation::on_complete_pushButton_clicked() {}


unsigned int Evaluation::getPhysicalCoreCount() {
    unsigned int physicalCores = 0;

#if defined(__linux__)
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open())
        return 0;

    std::string line;
    std::set<std::pair<int, int>> cores; // (physical_id, core_id)
    int physicalId = -1;
    int coreId = -1;

    while (std::getline(cpuinfo, line)) {
        if (line.substr(0, 10) == "physical id") {
            physicalId = std::stoi(line.substr(line.find(":") + 1));
        }
        if (line.substr(0, 7) == "core id") {
            coreId = std::stoi(line.substr(line.find(":") + 1));
            cores.insert({physicalId, coreId});
        }
    }
    physicalCores = static_cast<unsigned int>(cores.size());

#elif defined(_WIN32) || defined(_WIN64)
    DWORD len = 0;
    if (!GetLogicalProcessorInformation(nullptr, &len) && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        std::cerr << "Failed to query processor info." << std::endl;
        return 1;
    }

    std::vector<uint8_t> buf(len);
    auto info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>(buf.data());

    if (!GetLogicalProcessorInformation(info, &len)) {
        std::cerr << "Failed to get processor info." << std::endl;
        return 1;
    }

    unsigned int cores = 0;
    size_t count = len / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);

    for (size_t i = 0; i < count; ++i) {
        if (info[i].Relationship == RelationProcessorCore) {
            ++cores;
        }
    }
    physicalCores = cores;

#else
    // Unsupported platform
    physicalCores = 0;
#endif

    return physicalCores;
}

bool Evaluation::readStudentInfo(const std::string& studentInfoFile, std::vector<std::string>& studentInformation) {
    std::ifstream studentInfoIn(studentInfoFile);
    if (!studentInfoIn.is_open()) {

        return false;
    }

    studentInformation.clear();
    int n;
    studentInfoIn >> n;
    studentInformation.resize(n);
    for (int i = 0; i < n; i++) {
        std::string s;
        if (!(studentInfoIn >> s)) {


            return false;
        }

        studentInformation[i] = s;
    }


    return true;
}
bool Evaluation::readSubmissionFileInfo(const std::string& submissionFilePath,
                                        std::vector<std::string>& submissionFiles) {
    int n = submissionFiles.size();
    int i;
    char ch = 'A';
    for (i = 0; i < n; i++) {
        std::string submissionFile = submissionFilePath + ch + ".txt";

        std::ifstream submissionFileIn(submissionFile);
        if (!submissionFileIn.is_open()) {

            return false;
        }


        std::string s;
        submissionFileIn >> s;
        std::string solutionFile = submissionFilePath + ch + "/" + s;
        submissionFiles[i] = solutionFile;
        ch++;
    }


    return true;
}
bool Evaluation::readTestCaseInfo(const std::string& testCaseInfoFile, std::vector<int>& testCaseInformation) {
    std::ifstream testCaseInfoIn(testCaseInfoFile);
    if (!testCaseInfoIn.is_open()) {

        return false;
    }

    testCaseInformation.clear();
    int n;
    testCaseInfoIn >> n;
    testCaseInformation.resize(n);
    for (int i = 0; i < n; i++) {
        int d;
        if (!(testCaseInfoIn >> d)) {


            return false;
        }

        testCaseInformation[i] = d;
    }


    return true;
}
bool Evaluation::readJudgeInfo(const std::string& judgeInfoPath, std::vector<std::vector<double>>& judgeInformation) {
    int n = judgeInformation.size();
    int i;
    char ch = 'A';
    for (i = 0; i < n; i++) {
        std::string judgeInfoFile = judgeInfoPath + ch + ".txt";

        std::ifstream judgeInfoIn(judgeInfoFile);
        if (!judgeInfoIn.is_open()) {

            return false;
        }

        judgeInformation[i].clear();
        for (int j = 0; j < 14; j++) {
            double d;
            if (!(judgeInfoIn >> d)) {


                return false;
            }


            judgeInformation[i].push_back(d);
        }
        ch++;
    }

    return true;
}


void Evaluation::on_evaluate_clicked() {
    if (!evaluating) {
        evaluating = true;

        std::vector<std::string> studentInformation;
        std::string studentInfoFile = systemDirPath.toStdString() + "studentInfo.txt";

        if (!readStudentInfo(studentInfoFile, studentInformation)) {
            QMessageBox::warning(this, "Warning", "student information not found for evaluation.");
            evaluating = false;
            return;
        }
        static int numOfStudents = studentInformation.size();
        std::vector<int> testCaseInformation;
        std::string testCaseInfoFile = systemDirPath.toStdString() + "testCaseInfo.txt";

        if (!readTestCaseInfo(testCaseInfoFile, testCaseInformation)) {
            QMessageBox::warning(this, "Warning", "Test Case information not found for evaluation.");
            evaluating = false;
            return;
        }
        int numberOfProblem = testCaseInformation.size();
        std::vector<std::vector<double>> judgeInformation(numberOfProblem);
        std::string judgeInfoPath = systemDirPath.toStdString() + "Judge/";
        if (!readJudgeInfo(judgeInfoPath, judgeInformation)) {
            QMessageBox::warning(this, "Warning", "Judge information not found for evaluation.");
            evaluating = false;
            return;
        }

        std::vector<std::pair<std::string, std::vector<std::string>>> submissionInformation;
        int i;
        for (i = 0; i < numOfStudents; i++) {
            std::string stID = studentInformation[i];
            std::vector<std::string> submissionsFile(numberOfProblem);
            std::string submissionFilePath = systemDirPath.toStdString() + "Submissions/" + stID + "/";
            if (!readSubmissionFileInfo(submissionFilePath, submissionsFile)) {
                QMessageBox::warning(this, "Warning", "Submission Folder not found for evaluation.");
                evaluating = false;
                return;
            }
            submissionInformation.push_back({stID, submissionsFile});
        }

        int runMaxThread = 1;
        int p_core = getPhysicalCoreCount();
        runMaxThread = fmin(6, p_core - 1);
        runMaxThread = fmax(1, runMaxThread);


        // for(i=0;i<numOfStudents;i++)
        // {
        //     std::string stID=submissionInformation[i].first;
        //     std::vector<std::string> submissionsFile=submissionInformation[i].second;


        //     char ch='A';

        //     int j;
        //     int m=submissionsFile.size();
        //     std::vector<std::vector<Verdict>>verdict(m);
        //     for(j=0;j<m;j++)
        //     {
        //         if(submissionsFile[j].back()=='/')
        //         {
        //             std::vector<Verdict> v;
        //             for(int k=0;k<testCaseInformation[j];k++)
        //             {
        //                 v.push_back(Verdict("Not Submitted",0,0));
        //             }
        //             verdict[j]=v;
        //         }
        //         else
        //         {
        //             std::string testCasePath=systemDirPath.toStdString()+"Test-Cases/"+ch+"/";

        //             Judge *judge=new Judge();
        //             judge->setJudgeInfo(judgeInformation[j]);
        //             judge->setCurrentFile(submissionsFile[j]);
        //             judge->setCurrentProblem(std::to_string(ch));
        //             judge->setNumberOfTotalTestCase(testCaseInformation[j]);
        //             judge->setPretestCasesPath(testCasePath);

        //             std::vector<Verdict>v;
        //             v=judge->runOnTestCases();
        //             verdict[j]=v;

        //         }
        //         ch++;
        //     }

        //     verdicts.push_back({stID,verdict});
        // }


        // for(int i=0;i<verdicts.size();i++)
        // {
        //     std::cout<<verdicts[i].first<<std::endl;
        //     char ch='A';
        //     for(int j=0;j<verdicts[i].second.size();j++)
        //     {
        //         std::cout<<"          "<<ch<<" : \n";
        //         for(int k=0;k<verdicts[i].second[j].size();k++)
        //         {
        //             Verdict v=verdicts[i].second[j][k];
        //             std::cout<<"               "<<v.verdict<<" "<<v.cpu_time<<" "<<v.memory_size<<std::endl;
        //         }
        //         ch++;
        //     }

        // }


        static std::vector<std::thread> threads;
        static std::mutex queueMutex;
        static std::mutex verdictMutex;
        static std::condition_variable cv;
        static std::atomic<bool> stopFlag(false);
        static std::atomic<int> activeWorkers(0);
        static std::atomic<bool> completionMessageShown(false);
        static int counter;


        struct Task
        {
            std::string studentID;
            std::vector<std::string> submission;
        };

        static std::queue<Task> taskQueue;

        // Store copies to avoid dangling references
        static std::vector<int> testCaseInfoCopy;
        static std::vector<std::vector<double>> judgeInfoCopy;
        static QString systemDirPathCopy;
        static std::vector<std::pair<std::string, std::vector<std::vector<Verdict>>>> verdicts; // Add this


        // Clear previous state
        stopFlag = false;
        activeWorkers = 0;
        completionMessageShown = false;
        while (!taskQueue.empty())
            taskQueue.pop();
        verdicts.clear(); // Clear previous verdicts

        // Copy data to static storage
        testCaseInfoCopy = testCaseInformation;
        judgeInfoCopy = judgeInformation;
        systemDirPathCopy = systemDirPath;
        counter = 0;

        // Detach any previous threads
        for (auto& thread : threads) {
            if (thread.joinable())
                thread.detach();
        }
        threads.clear();

        // Fill task queue
        for (const auto& info : submissionInformation) {
            taskQueue.push({info.first, info.second});
        }


        // Create worker threads
        for (int i = 0; i < runMaxThread; ++i) {
            threads.emplace_back(
                [this, threadID = i]()
                {
                    while (!stopFlag) {
                        Task task;
                        bool hasTask = false;

                        {
                            std::unique_lock<std::mutex> lock(queueMutex);

                            // Wait for task or stop signal
                            cv.wait(lock, []() { return !taskQueue.empty() || stopFlag; });

                            if (stopFlag && taskQueue.empty())
                                break;

                            if (!taskQueue.empty()) {
                                task = taskQueue.front();
                                taskQueue.pop();
                                hasTask = true;
                                activeWorkers++;
                            }
                        }

                        if (hasTask) {


                            // JUDGING LOGIC
                            char ch = 'A';
                            int m = task.submission.size();
                            std::vector<std::vector<Verdict>> verdict(m);

                            for (int j = 0; j < m; j++) {
                                if (termination) {
                                    std::vector<Verdict> v;
                                    for (int k = 0; k < testCaseInfoCopy[j]; k++) {
                                        v.push_back(Verdict("Judge Terminated", 0, 0));
                                    }
                                    verdict[j] = v;
                                }
                                else if (task.submission[j].back() == '/') {
                                    std::vector<Verdict> v;
                                    for (int k = 0; k < testCaseInfoCopy[j]; k++) {
                                        v.push_back(Verdict("Not Submitted", 0, 0));
                                    }
                                    verdict[j] = v;
                                }
                                else {

                                    std::string testCasePath =
                                        systemDirPathCopy.toStdString() + "Test-Cases/" + ch + "/";

                                    Judge* judge = new Judge();

                                    judges.insert(judge);
                                    judge->setJudgeInfo(judgeInfoCopy[j]);


                                    judge->setCurrentFile(task.submission[j]);
                                    judge->setCurrentProblem(std::to_string(ch));
                                    judge->setNumberOfTotalTestCase(testCaseInfoCopy[j]);
                                    judge->setPretestCasesPath(testCasePath);
                                    judge->setStudentID(task.studentID);

                                    std::vector<Verdict> v = judge->runOnTestCases();
                                    verdict[j] = v;
                                    judges.erase(judge);
                                    delete judge;
                                }
                                ch++;
                            }

                            // Store the result in static verdicts
                            {
                                std::lock_guard<std::mutex> lock(verdictMutex);
                                verdicts.push_back({task.studentID, verdict});
                            }

                            // Print completion info
                            if (!termination)
                                counter++;

                            ui->autoEvalStatus_label_10->setText(QString::number(counter) + " / " +
                                                                 QString::number(numOfStudents) + " Completed");


                            // here complete one thread

                            activeWorkers--;

                            // Check if all done


                            // show verdict from here


                            {
                                std::lock_guard<std::mutex> lock(queueMutex);
                                if (taskQueue.empty() && activeWorkers == 0 && !completionMessageShown) {
                                    completionMessageShown = true;

                                    for (int i = 0; i < verdicts.size(); i++) {
                                        std::cout << verdicts[i].first << std::endl;
                                        char ch = 'A';
                                        for (int j = 0; j < verdicts[i].second.size(); j++) {
                                            std::cout << "          " << ch << " : \n";
                                            for (int k = 0; k < verdicts[i].second[j].size(); k++) {
                                                Verdict v = verdicts[i].second[j][k];
                                                std::cout << "               " << v.verdict << " " << v.cpu_time << " "
                                                          << v.memory_size << std::endl;
                                            }
                                            ch++;
                                        }
                                    }
                                    if (!termination)
                                        ui->evaluationStatusLevel->setText("Success");
                                    else
                                        ui->evaluationStatusLevel->setText("Terminated");

                                    stopFlag = true;
                                    cv.notify_all();
                                    evaluating = false;
                                    termination = false;
                                }
                            }
                        }
                    }
                });
        }

        // Detach threads so they run independently
        for (auto& thread : threads) {
            thread.detach();
        }

        // Notify threads to start
        cv.notify_all();
        ui->evaluationStatusLevel->setText("Running");
        ui->autoEvalStatus_label_10->setText(QString::number(0) + " / " + QString::number(studentInformation.size()) +
                                             " Completed");
    }
}


void Evaluation::on_reEvaluate_clicked() {}


void Evaluation::on_stopEvaluation_clicked() {
    if (evaluating) {
        termination = true;
        for (auto judge : judges)
            judge->stopJudge();
    }
    else

    {
        QMessageBox::warning(this, "Warning", "Nothing is evaluating.");
        evaluating = false;
        return;
    }
}

void Evaluation::on_prevStu_pushButton_clicked()
{
    ui->stuListcomboBox->setCurrentIndex((ui->stuListcomboBox->currentIndex()-1) % ui->stuListcomboBox->count());
}


void Evaluation::on_nextDttu_pushButton_clicked()
{
    ui->stuListcomboBox->setCurrentIndex((ui->stuListcomboBox->currentIndex()+1) % ui->stuListcomboBox->count());
}

std::vector <std::string> Evaluation::listFilesInDirectory(std::filesystem::path directoryPath)
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

void Evaluation::on_selectProblem_comboBox_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }

    ui->selectExamCode_comboBox_2->clear();

    std::string student_id = ui->stuListcomboBox->currentText().toStdString();
    std::string problem_id = ui->selectProblem_comboBox->currentText().toStdString().back() + std::string("");

    for (auto &filePath : listOfFiles) {
        if (filePath.find("/" + student_id + "/") != std::string::npos &&
            filePath.find("/" + problem_id + "/") != std::string::npos &&
            filePath.find("/Submissions/") != std::string::npos) {
            ui->selectExamCode_comboBox_2->addItem(filePath.c_str());
        }
    }

    if (ui->selectExamCode_comboBox_2->count() == 0) {
        ui->examCode_textEdit->setPlainText("");
        return;
    }

    QString content = getFileContent(ui->selectExamCode_comboBox_2->currentText());

    ui->examCode_textEdit->setPlainText(content);
}

void Evaluation::on_stuListcomboBox_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }

    ui->selectExamCode_comboBox_2->clear();

    std::string student_id = ui->stuListcomboBox->currentText().toStdString();
    std::string problem_id = ui->selectProblem_comboBox->currentText().toStdString().back() + std::string("");

    for (auto &filePath : listOfFiles) {
        if (filePath.find("/" + student_id + "/") != std::string::npos &&
            filePath.find("/" + problem_id + "/") != std::string::npos &&
            filePath.find("/Submissions/") != std::string::npos) {
            ui->selectExamCode_comboBox_2->addItem(filePath.c_str());
        }
    }

    if (ui->selectExamCode_comboBox_2->count() == 0) {
        ui->examCode_textEdit->setPlainText("");
        return;
    }

    QString content = getFileContent(ui->selectExamCode_comboBox_2->currentText());

    ui->examCode_textEdit->setPlainText(content);
}


void Evaluation::on_selectExamCode_comboBox_2_currentTextChanged(const QString &arg1)
{
    
}

