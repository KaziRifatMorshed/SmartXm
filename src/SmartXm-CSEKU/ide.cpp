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

           // For navigation and zoom, cast QPdfView to QPdfPageNavigation
    QPdfPageNavigator* navigation = qobject_cast<QPdfPageNavigator*>(quesPdfView);

    QPushButton* quesPrevBtn = new QPushButton("Previous");
    QPushButton* quesNextBtn = new QPushButton("Next");
    QLabel* quesPageLabel = new QLabel();

    QPushButton* quesZoomInBtn = new QPushButton("Zoom In");
    QPushButton* quesZoomOutBtn = new QPushButton("Zoom Out");

    auto updateQuesPageLabel = [navigation, quesPdfDoc, quesPageLabel]() {
        if (!navigation)
            return;
        int currentPage = navigation->currentPage();
        int pageCount = quesPdfDoc->pageCount();
        quesPageLabel->setText(QString("Page %1 / %2").arg(currentPage + 1).arg(pageCount));
    };

    QObject::connect(quesPrevBtn, &QPushButton::clicked, [navigation, updateQuesPageLabel]() {
        if (!navigation)
            return;
        int current = navigation->currentPage();
        if (current > 0) {
            // navigation->setPage(current - 1);
            updateQuesPageLabel();
        }
    });

    QObject::connect(quesNextBtn, &QPushButton::clicked, [navigation, quesPdfDoc, updateQuesPageLabel]() {
        if (!navigation)
            return;
        int current = navigation->currentPage();
        if (current + 1 < quesPdfDoc->pageCount()) {
            // navigation->setPage(current + 1);
            updateQuesPageLabel();
        }
    });

           // Zoom: QPdfView uses setZoomFactor() and zoomFactor()
    QObject::connect(quesZoomInBtn, &QPushButton::clicked, [quesPdfView]() {
        qreal zoom = quesPdfView->zoomFactor();
        quesPdfView->setZoomFactor(zoom + 0.1);
    });

    QObject::connect(quesZoomOutBtn, &QPushButton::clicked, [quesPdfView]() {
        qreal zoom = quesPdfView->zoomFactor();
        if (zoom > 0.2)
            quesPdfView->setZoomFactor(zoom - 0.1);
    });

    updateQuesPageLabel();

    QHBoxLayout* quesNavLayout = new QHBoxLayout();
    quesNavLayout->addWidget(quesPrevBtn);
    quesNavLayout->addWidget(quesPageLabel);
    quesNavLayout->addWidget(quesNextBtn);
    quesNavLayout->addStretch();
    quesNavLayout->addWidget(quesZoomOutBtn);
    quesNavLayout->addWidget(quesZoomInBtn);

    QVBoxLayout* quesMainLayout = new QVBoxLayout(ques_tab);
    quesMainLayout->addWidget(quesPdfView);
    quesMainLayout->addLayout(quesNavLayout);

    ques_tab->setLayout(quesMainLayout);
}
