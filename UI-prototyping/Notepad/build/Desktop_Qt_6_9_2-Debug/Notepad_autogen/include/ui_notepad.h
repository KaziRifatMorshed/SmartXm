/********************************************************************************
** Form generated from reading UI file 'notepad.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTEPAD_H
#define UI_NOTEPAD_H

#include <Qsci/qsciscintilla.h>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_notepad
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_6;
    QSplitter *splitter_3;
    QTreeView *treeViewFiles;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_5;
    QSplitter *splitter_2;
    QTabWidget *Editor_tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QLabel *EditorLabel;
    QsciScintilla *Editor;
    QWidget *tab_2;
    QGridLayout *gridLayout_7;
    QLabel *label;
    QSplitter *splitter;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QTextEdit *Input;
    QLabel *InputLabel;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_2;
    QLabel *OutputLabel;
    QTextEdit *Output;
    QGridLayout *gridLayout_4;
    QLabel *DebugOutputLabel;
    QTextEdit *DebugOutput;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuFile;

    void setupUi(QMainWindow *notepad)
    {
        if (notepad->objectName().isEmpty())
            notepad->setObjectName("notepad");
        notepad->resize(950, 627);
        centralwidget = new QWidget(notepad);
        centralwidget->setObjectName("centralwidget");
        gridLayout_6 = new QGridLayout(centralwidget);
        gridLayout_6->setObjectName("gridLayout_6");
        splitter_3 = new QSplitter(centralwidget);
        splitter_3->setObjectName("splitter_3");
        splitter_3->setOrientation(Qt::Orientation::Horizontal);
        treeViewFiles = new QTreeView(splitter_3);
        treeViewFiles->setObjectName("treeViewFiles");
        treeViewFiles->setMaximumSize(QSize(200, 16777215));
        splitter_3->addWidget(treeViewFiles);
        layoutWidget = new QWidget(splitter_3);
        layoutWidget->setObjectName("layoutWidget");
        gridLayout_5 = new QGridLayout(layoutWidget);
        gridLayout_5->setObjectName("gridLayout_5");
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(layoutWidget);
        splitter_2->setObjectName("splitter_2");
        splitter_2->setOrientation(Qt::Orientation::Horizontal);
        Editor_tabWidget = new QTabWidget(splitter_2);
        Editor_tabWidget->setObjectName("Editor_tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setObjectName("gridLayout_3");
        EditorLabel = new QLabel(tab);
        EditorLabel->setObjectName("EditorLabel");
        EditorLabel->setMaximumSize(QSize(16777215, 16));

        gridLayout_3->addWidget(EditorLabel, 0, 0, 1, 1);

        Editor = new QsciScintilla(tab);
        Editor->setObjectName("Editor");
        Editor->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        Editor->setWrapMode(QsciScintilla::WrapNone);

        gridLayout_3->addWidget(Editor, 1, 0, 1, 1);

        Editor_tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        gridLayout_7 = new QGridLayout(tab_2);
        gridLayout_7->setObjectName("gridLayout_7");
        label = new QLabel(tab_2);
        label->setObjectName("label");

        gridLayout_7->addWidget(label, 0, 0, 1, 1);

        Editor_tabWidget->addTab(tab_2, QString());
        splitter_2->addWidget(Editor_tabWidget);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Vertical);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName("layoutWidget1");
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        Input = new QTextEdit(layoutWidget1);
        Input->setObjectName("Input");
        Input->setMaximumSize(QSize(400, 800));

        gridLayout->addWidget(Input, 1, 0, 1, 1);

        InputLabel = new QLabel(layoutWidget1);
        InputLabel->setObjectName("InputLabel");

        gridLayout->addWidget(InputLabel, 0, 0, 1, 1);

        splitter->addWidget(layoutWidget1);
        layoutWidget2 = new QWidget(splitter);
        layoutWidget2->setObjectName("layoutWidget2");
        gridLayout_2 = new QGridLayout(layoutWidget2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        OutputLabel = new QLabel(layoutWidget2);
        OutputLabel->setObjectName("OutputLabel");

        gridLayout_2->addWidget(OutputLabel, 0, 0, 1, 1);

        Output = new QTextEdit(layoutWidget2);
        Output->setObjectName("Output");
        Output->setMaximumSize(QSize(400, 800));

        gridLayout_2->addWidget(Output, 1, 0, 1, 1);

        splitter->addWidget(layoutWidget2);
        splitter_2->addWidget(splitter);

        gridLayout_5->addWidget(splitter_2, 0, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        DebugOutputLabel = new QLabel(layoutWidget);
        DebugOutputLabel->setObjectName("DebugOutputLabel");
        DebugOutputLabel->setMaximumSize(QSize(16777215, 16));

        gridLayout_4->addWidget(DebugOutputLabel, 0, 0, 1, 1);

        DebugOutput = new QTextEdit(layoutWidget);
        DebugOutput->setObjectName("DebugOutput");
        DebugOutput->setMaximumSize(QSize(16777215, 200));

        gridLayout_4->addWidget(DebugOutput, 1, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 1, 0, 1, 1);

        splitter_3->addWidget(layoutWidget);

        gridLayout_6->addWidget(splitter_3, 0, 0, 1, 1);

        notepad->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(notepad);
        statusbar->setObjectName("statusbar");
        notepad->setStatusBar(statusbar);
        menubar = new QMenuBar(notepad);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 950, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        notepad->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());

        retranslateUi(notepad);

        Editor_tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(notepad);
    } // setupUi

    void retranslateUi(QMainWindow *notepad)
    {
        notepad->setWindowTitle(QCoreApplication::translate("notepad", "notepad", nullptr));
        EditorLabel->setText(QCoreApplication::translate("notepad", "Editor", nullptr));
        Editor_tabWidget->setTabText(Editor_tabWidget->indexOf(tab), QCoreApplication::translate("notepad", "Tab 1", nullptr));
        label->setText(QCoreApplication::translate("notepad", "TextLabel", nullptr));
        Editor_tabWidget->setTabText(Editor_tabWidget->indexOf(tab_2), QCoreApplication::translate("notepad", "Tab 2", nullptr));
        InputLabel->setText(QCoreApplication::translate("notepad", "Input", nullptr));
        OutputLabel->setText(QCoreApplication::translate("notepad", "Output", nullptr));
        DebugOutputLabel->setText(QCoreApplication::translate("notepad", "Compiler Output", nullptr));
        menuFile->setTitle(QCoreApplication::translate("notepad", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class notepad: public Ui_notepad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTEPAD_H
