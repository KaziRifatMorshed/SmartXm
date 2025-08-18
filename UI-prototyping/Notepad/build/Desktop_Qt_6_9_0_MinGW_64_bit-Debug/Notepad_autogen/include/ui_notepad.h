/********************************************************************************
** Form generated from reading UI file 'notepad.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTEPAD_H
#define UI_NOTEPAD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_notepad
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QTreeView *treeViewFiles;
    QSplitter *splitter_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_4;
    QLabel *EditorLabel;
    QTextEdit *Editor;
    QSplitter *splitter;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QTextEdit *Input;
    QLabel *InputLabel;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_2;
    QLabel *OutputLabel;
    QTextEdit *Output;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout_5;
    QLabel *DebugOutputLabel;
    QTextEdit *DebugOutput;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuFile;

    void setupUi(QMainWindow *notepad)
    {
        if (notepad->objectName().isEmpty())
            notepad->setObjectName("notepad");
        notepad->resize(869, 545);
        centralwidget = new QWidget(notepad);
        centralwidget->setObjectName("centralwidget");
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName("gridLayout_3");
        treeViewFiles = new QTreeView(centralwidget);
        treeViewFiles->setObjectName("treeViewFiles");
        treeViewFiles->setMaximumSize(QSize(200, 16777215));

        gridLayout_3->addWidget(treeViewFiles, 0, 0, 1, 1);

        splitter_2 = new QSplitter(centralwidget);
        splitter_2->setObjectName("splitter_2");
        splitter_2->setOrientation(Qt::Orientation::Vertical);
        layoutWidget = new QWidget(splitter_2);
        layoutWidget->setObjectName("layoutWidget");
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        EditorLabel = new QLabel(layoutWidget);
        EditorLabel->setObjectName("EditorLabel");
        EditorLabel->setMaximumSize(QSize(16777215, 16));

        gridLayout_4->addWidget(EditorLabel, 0, 0, 1, 1);

        Editor = new QTextEdit(layoutWidget);
        Editor->setObjectName("Editor");
        Editor->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        Editor->setLineWrapMode(QTextEdit::LineWrapMode::WidgetWidth);

        gridLayout_4->addWidget(Editor, 1, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout_4);

        splitter = new QSplitter(layoutWidget);
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

        horizontalLayout->addWidget(splitter);

        splitter_2->addWidget(layoutWidget);
        layoutWidget3 = new QWidget(splitter_2);
        layoutWidget3->setObjectName("layoutWidget3");
        gridLayout_5 = new QGridLayout(layoutWidget3);
        gridLayout_5->setObjectName("gridLayout_5");
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        DebugOutputLabel = new QLabel(layoutWidget3);
        DebugOutputLabel->setObjectName("DebugOutputLabel");
        DebugOutputLabel->setMaximumSize(QSize(16777215, 16));

        gridLayout_5->addWidget(DebugOutputLabel, 0, 0, 1, 1);

        DebugOutput = new QTextEdit(layoutWidget3);
        DebugOutput->setObjectName("DebugOutput");
        DebugOutput->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_5->addWidget(DebugOutput, 1, 0, 1, 1);

        splitter_2->addWidget(layoutWidget3);

        gridLayout_3->addWidget(splitter_2, 0, 1, 1, 1);

        notepad->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(notepad);
        statusbar->setObjectName("statusbar");
        notepad->setStatusBar(statusbar);
        menubar = new QMenuBar(notepad);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 869, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        notepad->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());

        retranslateUi(notepad);

        QMetaObject::connectSlotsByName(notepad);
    } // setupUi

    void retranslateUi(QMainWindow *notepad)
    {
        notepad->setWindowTitle(QCoreApplication::translate("notepad", "notepad", nullptr));
        EditorLabel->setText(QCoreApplication::translate("notepad", "Editor", nullptr));
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
