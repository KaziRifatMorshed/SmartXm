#include "createquestion.h"
#include "ui_createquestion.h"
#include <filesystem>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <toast.h>
// #include <QWebEnginePage>

CreateQuestion::CreateQuestion(QWidget* parent) : QWidget(parent), ui(new Ui::CreateQuestion) { ui->setupUi(this); }

CreateQuestion::~CreateQuestion() { delete ui; }

void CreateQuestion::createFolder()
{
    QString questionName = ui->quesTitle_lineEdit->text();

    qDebug() << questionName << "\n";

    std::filesystem::create_directory(questionName.toStdString());

    path = std::filesystem::current_path().c_str();
    path += "/" + questionName + "/";

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

// void convertHtmlToPdf(const QString &htmlFilePath, const QString &outputPdfPath)
// {
//     // Create a QWebEnginePage to handle the HTML content
//     QWebEnginePage *page = new QWebEnginePage;

//            // Read the HTML content from the file
//     QFile htmlFile(htmlFilePath);
//     if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qDebug() << "Failed to open HTML file:" << htmlFilePath;
//         return;
//     }

//     QString htmlContent = htmlFile.readAll();

//            // Load the HTML content into the QWebEnginePage
//     page->setHtml(htmlContent);

//            // Connect the loadFinished signal to the slot that generates the PDF
//     QObject::connect(page, &QWebEnginePage::loadFinished, [=](bool ok) {
//         if (ok) {
//             // Print to PDF
//             page->printToPdf(outputPdfPath, [](bool success) {
//                 if (success) {
//                     qDebug() << "PDF generated successfully at" << outputPdfPath;
//                 } else {
//                     qDebug() << "Failed to generate PDF.";
//                 }
//             });
//         } else {
//             qDebug() << "Failed to load HTML content from file:" << htmlFilePath;
//         }
//     });
// }

void CreateQuestion::on_save_pushButton_clicked()
{
    // Create Folder with Question Name
    createFolder();

    // Write complete question to a HTML
    writeQuestionToHTML();

    // Convert html to pdf
    // Save the testcases
    // Save checker
    // Save limits
    // Save solutions
    // Save editorial
}

