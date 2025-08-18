#include "notepad.h"
#include "ui_notepad.h"
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QTextStream>

notepad::notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::notepad)
{
    ui->setupUi(this);

    this->setWindowTitle("SmartXam: Code Editor");

    QString dirPath = QDir::homePath() + "/Desktop";

    model = new QFileSystemModel(this);
    model->setRootPath(dirPath);
    model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    ui->treeViewFiles->setModel(model);
    ui->treeViewFiles->setRootIndex(model->index(dirPath));

    // Action buttons
    QAction *newAction = new QAction("New", this);
    QAction *openAction = new QAction("Open", this);
    QAction *saveAction = new QAction("Save", this);
    QAction *currentFileAction = new QAction("Print", this);
    QAction *saveAsAction = new QAction("Save As", this);

    connect(newAction, &QAction::triggered, this, &notepad::newFile);
    connect(openAction, &QAction::triggered, this, &notepad::openFileNoPath);
    connect(saveAction, &QAction::triggered, this, &notepad::save);
    connect(ui->treeViewFiles, &QTreeView::doubleClicked, this, [=] (const QModelIndex &index) {
        QString path = model->filePath(index);

        if (QFileInfo(path).isFile()) {
            openFile(path);
        }
    });
    connect(currentFileAction, &QAction::triggered, this, &notepad::printCurrentFile);
    connect(saveAsAction, &QAction::triggered, this, &notepad::saveAs);

    ui->menuFile->addAction(newAction);
    ui->menuFile->addAction(openAction);
    ui->menuFile->addAction(saveAction);
    ui->menuFile->addAction(currentFileAction);
    ui->menuFile->addAction(saveAsAction);
}

notepad::~notepad()
{
    delete ui;
}

void notepad::newFile()
{
    currentFile.clear();
    ui->Editor->setText(QString());
}

void notepad::openFileNoPath()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());

        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();

    ui->Editor->setText(text);

    currentFile = fileName;

    file.close();
}

void notepad::openFile(QString path)
{
    QString fileName;

    if (path == "") {
        fileName = QFileDialog::getOpenFileName(this, "Open the file");
    }
    else {
        fileName = path;
    }

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());

        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();

    ui->Editor->setText(text);

    currentFile = fileName;

    file.close();
}

void notepad::saveAs()
{
    QString fileName;

    fileName = QFileDialog::getSaveFileName(this, "Save the file");

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());

        return;
    }

    QTextStream out(&file);

    QString text = file.readAll();

    out << text;

    file.close();

    currentFile = fileName;
}

void notepad::save()
{
    QString fileName;

    if (currentFile.isEmpty()) {
        notepad::saveAs();

        return;
    }
    else {
        fileName = currentFile;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());

        return;
    }

    QTextStream out(&file);
    QString text = ui->Editor->toPlainText();
    out << text;
    file.close();
}

void notepad::printCurrentFile()
{
    QMessageBox::information(this, "Filename", currentFile);
}
