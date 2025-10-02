#include "createquestion.h"
#include "ui_createquestion.h"
#include <filesystem>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <toast.h>
#include <QWebEnginePage>

CreateQuestion::CreateQuestion(QWidget* parent) : QWidget(parent), ui(new Ui::CreateQuestion) { ui->setupUi(this); }

CreateQuestion::~CreateQuestion() { delete ui; }

void CreateQuestion::createFolder()
{
    QString questionName = ui->quesTitle_lineEdit->text().replace(' ', '_');

    if (questionName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please write question name first.");

        return;
    }

    qDebug() << questionName << "\n";

    path = std::filesystem::current_path().c_str();
    path += "/" + questionName + "/";

    if (!std::filesystem::is_directory(path.toStdString())) {
        std::filesystem::create_directory(questionName.toStdString());
    }
    else {
        qDebug() << path << " already exists\n";
    }

    qDebug() << path << "\n";
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
    createFolder();

    // Write complete question to a HTML
    writeQuestionToHTML();

    // Convert html to pdf
    QString questionName = ui->quesTitle_lineEdit->text();

    convertHtmlToPdf(path + questionName + ".html", path + questionName + ".pdf");

    // Save the testcases
    // Save checker
    // Save limits
    // Save solutions
    on_soluSrcCodeSaveBtn_pushButton_2_clicked();

    // Save editorial
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
    createFolder();

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
    createFolder();

    QString fileName = "Editorial.html";
    QString outFileName = "Editorial.pdf";

    QString editorial = ui->editorialTextBox_textEdit->toHtml();

    saveToFile(path + fileName, editorial);

    convertHtmlToPdf(path + fileName, path + outFileName);

    ToastManager::showMessage(this, "Saved as: " + outFileName);
}

