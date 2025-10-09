#include "ide.h"
#include "toast.h"
#include "ui_ide.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontMetricsF>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QtPdf/QPdfDocument>
#include <QtPdf/QPdfPageNavigator>
#include <QtPdfWidgets/QPdfView>
#include <qmessagebox.h>
// #include <Qsci/qsciscintilla.h>
// #include <Qsci/qscilexercpp.h>
// #include <Qsci/qscilexerpython.h>
#include "codeRunner.h"
#include <QString>
#include <iostream>
#include <string>

#include <QThread>
#include <runtestcases.h>

IDE *IDE::ideInstance = nullptr;
runTestcases *testcasesWindow;

IDE::IDE(QWidget *parent) : QMainWindow(parent), ui(new Ui::IDE) {
  ui->setupUi(this);
  initialize();
}

IDE::~IDE() { delete ui; }

void IDE::initialize() {
  executionThreadFlag = false;
  forciblyKillExecutionFlag=false;
  ui->actionRun->setVisible(true);
  ui->actionTerminate->setVisible(false);
  ui->CompilerDebudOutput_textEdit->setReadOnly(true);

  ui->Editor->setFont(QFont("Monospace"));
  ui->input_textEdit->setFont(QFont("Monospace"));
  ui->output_textEdit->setFont(QFont("Monospace"));
  ui->CompilerDebudOutput_textEdit->setFont(QFont("Monospace"));

  model = new QFileSystemModel(this);
  model->setRootPath(dirPath);
  model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

  ui->treeViewFiles->setModel(model);
  ui->treeViewFiles->setRootIndex(model->index(dirPath));

  QAction *newAction = new QAction("New", this);
  QAction *saveAction = new QAction("Save", this);
  QAction *runAction = new QAction("Run", this);
  QAction *terminateAction = new QAction("Terminate Execution", this);
  QAction *loadAction = new QAction("Load Problem", this);

  connect(newAction, &QAction::triggered, this, &IDE::newFile);
  connect(saveAction, &QAction::triggered, this, &IDE::save);
  connect(runAction, &QAction::triggered, this, &IDE::run);
  connect(terminateAction, &QAction::triggered, this, &IDE::terminateExecution);
  connect(ui->treeViewFiles, &QTreeView::doubleClicked, this,
          [=](const QModelIndex &index) {
            QString path = model->filePath(index);

            if (QFileInfo(path).isFile()) {
              openFile(path);
            }
          });
  connect(loadAction, &QAction::triggered, this, &IDE::loadProblem);

  ui->menuFile->addAction(newAction);
  ui->menuFile->addAction(saveAction);
  ui->menuFile->addAction(runAction);
  ui->menuFile->addAction(terminateAction);
  ui->menuFile->addAction(loadAction);
}

void IDE::loadPdfInQuesTab(QWidget *ques_tab, std::string pdfFilePath) {
  if (ques_tab->layout()) {
    QLayoutItem *item;
    while ((item = ques_tab->layout()->takeAt(0)) != nullptr) {
      delete item->widget();
      delete item;
    }
    delete ques_tab->layout();
  }

  QPdfDocument *quesPdfDoc = new QPdfDocument(ques_tab);
  QPdfView *quesPdfView = new QPdfView(ques_tab);

  quesPdfDoc->load(QString(pdfFilePath.c_str()));
  quesPdfView->setDocument(quesPdfDoc);

  quesPdfView->setPageMode(QPdfView::PageMode::MultiPage);

  QPushButton *quesZoomInBtn = new QPushButton("Zoom In");
  QPushButton *quesZoomOutBtn = new QPushButton("Zoom Out");

  QObject::connect(quesZoomInBtn, &QPushButton::clicked, [quesPdfView]() {
    qreal zoom = quesPdfView->zoomFactor();
    quesPdfView->setZoomFactor(zoom + 0.1);
  });

  QObject::connect(quesZoomOutBtn, &QPushButton::clicked, [quesPdfView]() {
    qreal zoom = quesPdfView->zoomFactor();
    if (zoom > 0.2)
      quesPdfView->setZoomFactor(zoom - 0.1);
  });

  QHBoxLayout *quesNavLayout = new QHBoxLayout();
  quesNavLayout->addStretch();
  quesNavLayout->addWidget(quesZoomOutBtn);
  quesNavLayout->addWidget(quesZoomInBtn);

  QVBoxLayout *quesMainLayout = new QVBoxLayout(ques_tab);
  quesMainLayout->addWidget(quesPdfView);
  quesMainLayout->addLayout(quesNavLayout);

  ques_tab->setLayout(quesMainLayout);
}

void IDE::newFile() {
  ui->Editor->setText(QString());

  QString fileName = QFileDialog::getSaveFileName(this, "Save New File",
                                                  dirPath, "All Files (*)");

  if (!fileName.isEmpty()) {
    currentFile = fileName;

    QFileInfo fileInfo(fileName);
    QFileInfo dirInfo(dirPath);

    QString selectedDir = fileInfo.absolutePath();
    QString projectDir = dirInfo.absolutePath();

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream out(&file);
      out << "";
      file.close();

      model->setRootPath(dirPath);

      QModelIndex index = model->index(fileName);
      if (index.isValid()) {
        ui->treeViewFiles->expand(index.parent());
        ui->treeViewFiles->setCurrentIndex(index);
      }
    } else {
      QMessageBox::critical(this, "Error",
                            "Could not create file: " + fileName);
    }
  }
}

QString IDE::getFileContent(QString path) {
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

void IDE::save() {
  QString fileName;

  fileName = currentFile;

  if (IDE::getFileContent(fileName) == ui->Editor->toPlainText()) {
    return;
  }

  QFile file(fileName);

  if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, "Warning",
                         "Cannot save file: " + file.errorString());

    return;
  }

  QTextStream out(&file);
  QString text = ui->Editor->toPlainText();
  out << text;
  file.close();

  ToastManager::showMessage(this, "File saved as: " + currentFile);
}
void IDE::run() {
    save(); // save editor content to current file
    QFile file("input.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    out << ui->input_textEdit->toPlainText();
    file.close();
    ui->CompilerDebudOutput_textEdit->clear();
    ui->CompilerDebudOutput_textEdit->append("Compiling and executing.\n");
    ui->output_textEdit->clear();

    CodeRunnerWorker *worker = new CodeRunnerWorker(currentFile.toStdString());
    QThread *thread = new QThread();
    threadExecution=thread;
    executionThreadFlag=true;
    ui->actionRun->setVisible(false);
    ui->actionTerminate->setVisible(true);
    workerExecution=worker;
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &CodeRunnerWorker::run);

           // KEY FIX: Use Qt::QueuedConnection to ensure UI updates happen in main thread
    connect(worker, &CodeRunnerWorker::finished, this,
            [this, thread]{
                // All UI updates now safely happen in the main thread

                QString debugText = getFileContent(QString("error.txt"));

                ui->CompilerDebudOutput_textEdit->setPlainText(debugText);


                QString outputText = getFileContent(QString("output.txt"));

                ui->output_textEdit->setPlainText(outputText);

                       //ui->CompilerDebudOutput_textEdit->append("\nOutput is Displayed.");


                if(forciblyKillExecutionFlag)
                {
                    ui->CompilerDebudOutput_textEdit->append("\nExecution is forcibly terminated.");
                    ToastManager::showMessage(this, "Execution terminated.");
                    forciblyKillExecutionFlag=false;
                }
                else
                {
                    ui->CompilerDebudOutput_textEdit->append("\nExecution is finished.");
                    ToastManager::showMessage(this, "Execution complete.");
                }
                ui->actionRun->setVisible(true);
                ui->actionTerminate->setVisible(false);
                executionThreadFlag=false;
                thread->quit();
            }, Qt::QueuedConnection); // <- Important: Forces main thread execution

    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    ToastManager::showMessage(this, "Running in background...");
    thread->start();
}
void IDE::terminateExecution()
{
    if(executionThreadFlag&&threadExecution && threadExecution->isRunning() && workerExecution)
    {
        forciblyKillExecutionFlag=true;
        workerExecution->killExecution(); // directly call the slot
        executionThreadFlag=false;
    }
}

void IDE::openFile(QString path) {
  QString fileName;

  if (path == "") {
    fileName = QFileDialog::getOpenFileName(this, "Open the file");
  } else {
    fileName = path;
  }

  if (fileName.isEmpty()) {
    return;
  }

  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, "Warning",
                         "Cannot open file: " + file.errorString());

    return;
  }

  QTextStream in(&file);
  QString text = in.readAll();

  ui->Editor->setText(text);

  currentFile = fileName;

  file.close();
}

void IDE::loadInput(std::string path) {
  QString fileName = QString(path.c_str());

  if (fileName.isEmpty()) {
    return;
  }

  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, "Warning",
                         "Cannot open file: " + file.errorString());

    return;
  }

  QTextStream in(&file);
  QString text = in.readAll();

  ui->input_textEdit->setText(text);

  file.close();
}

void IDE::loadOutput(std::string path) {
  QString fileName = QString(path.c_str());

  if (fileName.isEmpty()) {
    return;
  }

  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, "Warning",
                         "Cannot open file: " + file.errorString());

    return;
  }

  QTextStream in(&file);
  QString text = in.readAll();

  ui->output_textEdit->setText(text);

  file.close();
}

void IDE::loadProblem() {
  std::string path =
      "/home/seam/Desktop/SmartXm/src/SmartXm-CSEKU/examResources/230201/";

  loadPdfInQuesTab(ui->ques_tab, path + "questions.pdf");
  loadInput(path + "sample_in.txt");
  loadOutput(path + "sample_out.txt");
}

void IDE::on_actionTestcases_triggered() {
  // QMessageBox::information(this, "run test case", "test");
  testcasesWindow = new runTestcases();
  testcasesWindow->show();
}

void IDE::on_actionRun_triggered() {
    IDE::run();

}

void IDE::on_actionTerminate_triggered() {
    IDE::terminateExecution();

}
void IDE::on_actionRun_Testcases_triggered() {}
