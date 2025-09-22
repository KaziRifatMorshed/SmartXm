#include "ide.h"
#include "ui_ide.h"
#include <QtPdfWidgets/QPdfView>
#include <QtPdf/QPdfDocument>
#include <QtPdf/QPdfPageNavigator>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "toast.h"
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QTextEdit>
#include <QApplication>
#include <QTextStream>
#include <QProcess>
#include <QFontMetricsF>
#include <QWheelEvent>
// #include <Qsci/qsciscintilla.h>
// #include <Qsci/qscilexercpp.h>
// #include <Qsci/qscilexerpython.h>
#include <string>
#include <QString>

IDE::IDE(QWidget* parent) : QMainWindow(parent), ui(new Ui::IDE) {
    ui->setupUi(this);

    loadPdfInQuesTab(ui->ques_tab, "/home/seam/Desktop/SmartXm/src/SmartXm-CSEKU/examResources/questions.pdf");
}

IDE::~IDE() { delete ui; }

void IDE::loadPdfInQuesTab(QWidget* ques_tab, const QString& pdfFilePath)
{
    if (ques_tab->layout()) {
        QLayoutItem* item;
        while ((item = ques_tab->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete ques_tab->layout();
    }

    QPdfDocument* quesPdfDoc = new QPdfDocument(ques_tab);
    QPdfView* quesPdfView = new QPdfView(ques_tab);

    quesPdfDoc->load(pdfFilePath);
    quesPdfView->setDocument(quesPdfDoc);

    quesPdfView->setPageMode(QPdfView::PageMode::MultiPage);

    QPushButton* quesZoomInBtn = new QPushButton("Zoom In");
    QPushButton* quesZoomOutBtn = new QPushButton("Zoom Out");

    QObject::connect(quesZoomInBtn, &QPushButton::clicked, [quesPdfView]() {
        qreal zoom = quesPdfView->zoomFactor();
        quesPdfView->setZoomFactor(zoom + 0.1);
    });

    QObject::connect(quesZoomOutBtn, &QPushButton::clicked, [quesPdfView]() {
        qreal zoom = quesPdfView->zoomFactor();
        if (zoom > 0.2)
            quesPdfView->setZoomFactor(zoom - 0.1);
    });

    QHBoxLayout* quesNavLayout = new QHBoxLayout();
    quesNavLayout->addStretch();
    quesNavLayout->addWidget(quesZoomOutBtn);
    quesNavLayout->addWidget(quesZoomInBtn);

    QVBoxLayout* quesMainLayout = new QVBoxLayout(ques_tab);
    quesMainLayout->addWidget(quesPdfView);
    quesMainLayout->addLayout(quesNavLayout);

    ques_tab->setLayout(quesMainLayout);
}
