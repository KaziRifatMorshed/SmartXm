#include "createquestion.h"
#include "ui_createquestion.h"
#include <filesystem>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <toast.h>
#include <QWebEnginePage>
#include <string>
#include <QWheelEvent>
#include <QtPdf/QPdfDocument>
#include <QtPdf/QPdfPageNavigator>
#include <QtPdfWidgets/QPdfView>

CreateQuestion::CreateQuestion(QWidget* parent) : QWidget(parent), ui(new Ui::CreateQuestion) {
    ui->setupUi(this);

    ui->sampleTestCase_radioButton6->setChecked(true);

    ui->typeEditManually_radioButton_3->setChecked(true);

    ui->typeQuestionManually_radioButton->setChecked(true);
}

CreateQuestion::~CreateQuestion() { delete ui; }

bool CreateQuestion::createFolder()
{
    QString questionName = ui->quesTitle_lineEdit->text().replace(' ', '_');

    if (questionName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please write question name first.");

        return false;
    }

    qDebug() << questionName << "\n";

    path = QString(std::filesystem::current_path().string().c_str());
    path += "/" + questionName + "/";

    if (!std::filesystem::is_directory(path.toStdString())) {
        std::filesystem::create_directory(questionName.toStdString());
    }
    else {
        qDebug() << path << " already exists\n";
    }

    qDebug() << path << "\n";

    return true;
}

void CreateQuestion::writeQuestionToHTML()
{
    QString questionName = ui->quesTitle_lineEdit->text();
    QString statement = ui->QuesBody_plainTextEdit->toPlainText();
    QString input = ui->input_textEdit->toPlainText();
    QString output = ui->output_textEdit->toPlainText();
    QString notes = ui->notes_textEdit->toPlainText();

    QString document;

    document = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>" + questionName + "</title></head><body>\n";

    document += "<h1>" + questionName + "</h1>\n";

    document += "<h2>Statement</h2>\n";
    document += "<pre>" + statement + "</pre>\n";

    document += "<h2>Input</h2>\n";
    document += "<pre>" + input + "</pre>\n";

    document += "<h2>Output</h2>\n";
    document += "<pre>" + output + "</pre>\n";

    document += "<h2>Notes</h2>\n";
    document += "<pre>" + notes + "</pre>\n";

    document += "</body></html>\n";

    QFile file(path + questionName + ".html");

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());

        return;
    }

    QTextStream out(&file);
    out << document;
    file.close();

    ToastManager::showMessage(this, "Statement saved as: " + questionName + ".html");
}

void CreateQuestion::convertHtmlToPdf(QString source, QString destination)
{
    QWebEnginePage *page = new QWebEnginePage;

   QFile htmlFile(source);
   if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qDebug() << "Failed to open HTML file:" << source;
       return;
   }

   QString htmlContent = htmlFile.readAll();

   page->setHtml(htmlContent);

   QObject::connect(page, &QWebEnginePage::loadFinished, [=](bool ok) {
       if (ok) {
           page->printToPdf(destination);
       } else {
           qDebug() << "Failed to load HTML content from file:" << source;
       }
   });
}

void CreateQuestion::on_save_pushButton_clicked()
{
    // Create Folder with Question Name
    if (!createFolder()) {
        return;
    }

    // Write complete question to a HTML
    if (ui->typeQuestionManually_radioButton->isChecked()) {
        writeQuestionToHTML();

        // Convert html to pdf
        QString questionName = ui->quesTitle_lineEdit->text();

        convertHtmlToPdf(path + questionName + ".html", path + questionName + ".pdf");
    }
    else {
        QMessageBox::warning(this, "Warning", "Saving pdf is not implemented yet");
    }
    
    // Save checker
    // Save limits
    on_saveLimits_pushButton_2_clicked();

    // Save solutions
    on_soluSrcCodeSaveBtn_pushButton_2_clicked();

    // Save editorial
    on_saveEditorial_pushButton_2_clicked();
}

QString CreateQuestion::getFileContent(QString path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot read file: " + file.errorString());

        return QString();
    }

    QString text = file.readAll();

    file.close();

    return text;
}

void CreateQuestion::saveToFile(QString path, QString& textToSave) {
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot write to file: " + file.errorString());

        return;
    }

    QTextStream out(&file);
    out << textToSave;

    file.close();
}

void CreateQuestion::on_soluSrcCodeSaveBtn_pushButton_2_clicked()
{
    if (!createFolder()) {
        return;
    }

    QString fileName = "Solution";

    if (ui->solutionLanguage_comboBox->currentText() == "C/C++") {
        fileName += ".cpp";
    }
    else if (ui->solutionLanguage_comboBox->currentText() == "Python") {
        fileName += ".py";
    }
    else {
        fileName += ".java";
    }

    QString solution = ui->soluSourceCode_textEdit->toPlainText();

    saveToFile(path + fileName, solution);

    ToastManager::showMessage(this, "Saved as: " + fileName);
}


void CreateQuestion::on_saveEditorial_pushButton_2_clicked()
{
    if (!createFolder()) {
        return;
    }

    QString fileName = "Editorial.html";
    QString outFileName = "Editorial.pdf";

    QString questionName = ui->quesTitle_lineEdit->text();
    QString document;

    document = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>" + questionName + "</title></head><body>\n";

    document += "<pre>" + ui->editorialTextBox_textEdit->toPlainText() + "</pre>\n";

    document += "</body></html>\n";

    saveToFile(path + fileName, document);

    convertHtmlToPdf(path + fileName, path + outFileName);

    ToastManager::showMessage(this, "Saved as: " + outFileName);
}


void CreateQuestion::on_radioButton_clicked()
{
    ui->UpQuesStatePDF_pushButton_2->setEnabled(false);
    ui->groupBox_typeQuestionManually->setEnabled(true);
}


void CreateQuestion::on_typeQuestionManually_radioButton_clicked()
{
    ui->UpQuesStatePDF_pushButton_2->setEnabled(false);
    ui->groupBox_typeQuestionManually->setEnabled(true);
}


void CreateQuestion::on_uploadPdf_radioButton_clicked()
{
    ui->groupBox_typeQuestionManually->setEnabled(false);
    ui->UpQuesStatePDF_pushButton_2->setEnabled(true);
}


void CreateQuestion::on_inputTypeManually_radioButton_4_clicked()
{
    ui->inoutSelectFile_pushButton_2->setEnabled(false);
    ui->inputOfTestCase___textEdit->setEnabled(true);
}


void CreateQuestion::on_inputSeleceFile_radioButton_4_clicked()
{
    ui->inoutSelectFile_pushButton_2->setEnabled(true);
    ui->inputOfTestCase___textEdit->setEnabled(false);
}



void CreateQuestion::on_RunSolution_execute_pushButton_2_clicked()
{
    if(!judging)
    {
        judging=true;
        ui->maxCpuTimeUses->setText(QString::fromStdString("**")+" s");
        ui->maxMemoryUses->setText(QString::fromStdString("**")+" MB");
        ui->finalVerdict->setText(QString::fromStdString("Execution Running"));


        on_soluSrcCodeSaveBtn_pushButton_2_clicked();
        double cpuTime = ui->RunSolution_CPUtime_lineEdit->text().toInt();
        int memoryLimit = ui->RunSolution_Memory_lineEdit->text().toInt();
        QString solutionPath = path + "Solution";
        QString testCasePath = path + "Testcases/";

        int testCaseCount=14;
        cpuTime*=1000;
        memoryLimit*=1024;
        if (ui->solutionLanguage_comboBox->currentText() == "C/C++") {
            solutionPath += ".cpp";
        }
        else if (ui->solutionLanguage_comboBox->currentText() == "Python") {
            solutionPath += ".py";
        }
        else {
            solutionPath += ".java";
        }

        std::vector<std::string>testCaseData={solutionPath.toStdString()
                                                 ,testCasePath.toStdString()};
        std::vector<double>judgeInformation={0,1,double(cpuTime),1,1,1,double(memoryLimit),1,1,1,1024*1024,1,1,1};

        QThread* thread = new QThread();
        JudgeWorker3* worker = new JudgeWorker3(testCaseData, judgeInformation,
                                                testCaseCount);
        judgeThread=thread;
        judgeWorker=worker;
        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &JudgeWorker3::process);
        connect(worker, &JudgeWorker3::finished, this, [=](std::vector<Verdict> verdicts){


                    std::string verdict;
                    double cpuUsedTimeMS=0;
                    double usedMemoryKB=0;
                    for(auto v:verdicts)
                    {
                        if(verdict.size()==0&&v.verdict!="Accepted")
                        {
                            verdict=v.verdict;
                        }
                        usedMemoryKB=fmax(usedMemoryKB,v.memory_size);
                        cpuUsedTimeMS=fmax(cpuUsedTimeMS,v.cpu_time);
                    }
                    if(verdict.size()==0)verdict="Accepted";
                    usedMemoryKB/=1024;
                    cpuUsedTimeMS/=1000;

                    ui->maxCpuTimeUses->setText(QString::number(cpuUsedTimeMS)+" s");
                    ui->maxMemoryUses->setText(QString::number(usedMemoryKB)+" MB");
                    ui->finalVerdict->setText(QString::fromStdString(verdict));

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

           // code here
}

void CreateQuestion::on_stopExecution_clicked()
{

    if(judging)
    {
        judgeWorker->killJudge();
    }
    else
    {
        QMessageBox::warning(this, "Warning", "There is no running execution.");
    }
}



void CreateQuestion::on_newTestCaseINPUT_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->inputOfTestCase___textEdit->setEnabled(true);
        ui->inoutSelectFile_pushButton_2->setEnabled(false);
        break;
    case 1:
        ui->inputOfTestCase___textEdit->setEnabled(false);
        ui->inoutSelectFile_pushButton_2->setEnabled(true);
    default:
        break;
    }
}


void CreateQuestion::on_testcaseOutput_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->outputOfTestCase__textEdit->setEnabled(true);
        ui->outputSelectFile_pushButton_3->setEnabled(false);
        break;
    case 1:
        ui->outputOfTestCase__textEdit->setEnabled(false);
        ui->outputSelectFile_pushButton_3->setEnabled(true);
    default:
        break;
    }
}

void CreateQuestion::updateTestCaseTable()
{
    ui->tableWidget->setRowCount(test_cases.size());

    for (size_t i = 0; i < test_cases.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(test_cases[i].getTestCaseName())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(test_cases[i].getCategory())));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(test_cases[i].getInputFileSize()) + " bytes"));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(test_cases[i].getOutputFileSize()) + " bytes"));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(test_cases[i].getWeight())));
    }
}

void CreateQuestion::on_saveCurrentTestCase_pushButton_2_clicked()
{
    if (!createFolder()) {
        return;
    }

    int test_case_no = ui->tableWidget->rowCount() + 1;
    std::string test_case_name = ui->testCaseTitle_lineEdit->text().toStdString();
    std::string category = "";

    if (ui->preTestCase_radioButton_4->isChecked()) {
        category = "Pre Test";
    }
    else if (ui->systemTestCase_radioButton_5->isChecked()) {
        category = "System";
    }
    else {
        category = "Sample";
    }

    std::string test_case_folder_path = path.toStdString() + "Test-Cases/";

    if (!std::filesystem::is_directory(test_case_folder_path)) {
        std::filesystem::create_directory(test_case_folder_path);
    }
    else {
        qDebug() << path << " already exists\n";
    }

    std::string input_file_path = test_case_folder_path + std::to_string(test_case_no) + ".in";
    std::string output_file_path = test_case_folder_path + std::to_string(test_case_no) + ".out";

    QString input_text = ui->inputOfTestCase___textEdit->toPlainText();
    QString output_text = ui->outputOfTestCase__textEdit->toPlainText();

    saveToFile(QString::fromStdString(input_file_path), input_text);
    saveToFile(QString::fromStdString(output_file_path), output_text);

    test_cases.push_back(test_case(test_case_no, test_case_name, category, 1, input_file_path, output_file_path));

    updateTestCaseTable();

    QString test_case_infos;

    for (auto &tc : test_cases) {
        test_case_infos += QString::number(tc.getTestCaseNumber()) + "," +
                           QString::fromStdString(tc.getTestCaseName()) + "," +
                           QString::fromStdString(tc.getCategory()) + "," +
                           QString::number(tc.getInputFileSize()) + "," +
                           QString::number(tc.getOutputFileSize()) + "," +
                           QString::number(tc.getWeight()) + "," +
                           QString::fromStdString(tc.getInputFilePath()) + "," +
                           QString::fromStdString(tc.getOutputFilePath()) + "\n";
    }

    saveToFile(QString::fromStdString(test_case_folder_path + "test_cases_info.txt"), test_case_infos);
}


void CreateQuestion::on_saveLimits_pushButton_2_clicked()
{
    if (!createFolder()) {
        return;
    }

    QString limits;

    limits += QString::number(ui->BaseMultiplier_lineEdit->text().toInt() * 1000) + "\n";
    limits += QString::number(ui->cpp_multiplier_lineEdit->text().toInt()) + "\n";
    limits += QString::number(ui->py_multiplier_lineEdit->text().toInt()) + "\n";
    limits += QString::number(ui->java_multiplier_lineEdit->text().toInt()) + "\n";
    limits += QString::number(ui->baseMemoryLimit_lineEdit_2->text().toInt() * 1024) + "\n";
    limits += QString::number(ui->cpp_mem_limit_lineEdit->text().toInt()) + "\n";
    limits += QString::number(ui->py_mem_limit_lineEdit->text().toInt()) + "\n";
    limits += QString::number(ui->java_mem_limit_lineEdit->text().toInt()) + "\n";
    limits += QString::number(ui->baseSourceLimit_lineEdit_3->text().toInt()) + "\n";
    limits += QString::number(ui->cpp_source_limit_lineEdit->text().toInt()) + "\n";
    limits += QString::number(ui->py_source_limit_lineEdit->text().toInt()) + "\n";
    limits += QString::number(ui->java_source_limit_lineEdit->text().toInt()) + "\n";

    saveToFile(path + "limits.txt", limits);

    ToastManager::showMessage(this, "Limits saved.");

    qDebug() << limits << "\n";
}

void CreateQuestion::loadPdf(QWidget *tab, std::string pdfFilePath) {
    if (tab->layout()) {
        QLayoutItem *item;
        while ((item = tab->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete tab->layout();
    }

    QPdfDocument *pdfDocument = new QPdfDocument(tab);
    QPdfView *pdfView = new QPdfView(tab);

    pdfDocument->load(QString(pdfFilePath.c_str()));
    pdfView->setDocument(pdfDocument);

    pdfView->setPageMode(QPdfView::PageMode::MultiPage);

    QPushButton *zoomInButton = new QPushButton("Zoom In");
    QPushButton *zoomOutButton = new QPushButton("Zoom Out");

    QObject::connect(zoomInButton, &QPushButton::clicked, [pdfView]() {
        qreal zoom = pdfView->zoomFactor();
        pdfView->setZoomFactor(zoom + 0.1);
    });

    QObject::connect(zoomOutButton, &QPushButton::clicked, [pdfView]() {
        qreal zoom = pdfView->zoomFactor();
        if (zoom > 0.2)
            pdfView->setZoomFactor(zoom - 0.1);
    });

    QHBoxLayout *navigationLayout = new QHBoxLayout();
    navigationLayout->addStretch();
    navigationLayout->addWidget(zoomOutButton);
    navigationLayout->addWidget(zoomInButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(tab);
    mainLayout->addWidget(pdfView);
    mainLayout->addLayout(navigationLayout);

    tab->setLayout(mainLayout);
}

void CreateQuestion::on_tabWidget_2_currentChanged(int index)
{
    if (index == 0) {
        QString questionName = ui->quesTitle_lineEdit->text();
        std::string pdfFilePath = path.toStdString() + questionName.toStdString() + ".pdf";

        loadPdf(ui->statementTab, pdfFilePath);
    }
    else if (index == 1) {
        std::string pdfFilePath = path.toStdString() + "Editorial.pdf";

        loadPdf(ui->editorialTab, pdfFilePath);
    }
}

void CreateQuestion::on_typeEditManually_radioButton_3_clicked()
{
    ui->editorialTextBox_textEdit->setEnabled(true);
    ui->editorialUploadPDF_pushButton_2->setEnabled(false);
}


void CreateQuestion::on_radioButton_3_clicked()
{
    ui->editorialTextBox_textEdit->setEnabled(false);
    ui->editorialUploadPDF_pushButton_2->setEnabled(true);
}

