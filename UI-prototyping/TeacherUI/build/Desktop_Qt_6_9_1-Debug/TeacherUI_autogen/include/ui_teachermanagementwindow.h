/********************************************************************************
** Form generated from reading UI file 'teachermanagementwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEACHERMANAGEMENTWINDOW_H
#define UI_TEACHERMANAGEMENTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TeacherManagementWindow
{
public:
    QAction *actionHelp;
    QAction *actionExit;
    QWidget *centralwidget;
    QListWidget *listWidget;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuMenu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TeacherManagementWindow)
    {
        if (TeacherManagementWindow->objectName().isEmpty())
            TeacherManagementWindow->setObjectName("TeacherManagementWindow");
        TeacherManagementWindow->resize(1175, 692);
        actionHelp = new QAction(TeacherManagementWindow);
        actionHelp->setObjectName("actionHelp");
        actionExit = new QAction(TeacherManagementWindow);
        actionExit->setObjectName("actionExit");
        centralwidget = new QWidget(TeacherManagementWindow);
        centralwidget->setObjectName("centralwidget");
        listWidget = new QListWidget(centralwidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(10, 100, 291, 541));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(380, 20, 411, 61));
        QFont font;
        font.setPointSize(20);
        label->setFont(font);
        TeacherManagementWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TeacherManagementWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1175, 23));
        menuMenu = new QMenu(menubar);
        menuMenu->setObjectName("menuMenu");
        TeacherManagementWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(TeacherManagementWindow);
        statusbar->setObjectName("statusbar");
        TeacherManagementWindow->setStatusBar(statusbar);

        menubar->addAction(menuMenu->menuAction());
        menuMenu->addAction(actionHelp);
        menuMenu->addAction(actionExit);

        retranslateUi(TeacherManagementWindow);

        QMetaObject::connectSlotsByName(TeacherManagementWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TeacherManagementWindow)
    {
        TeacherManagementWindow->setWindowTitle(QCoreApplication::translate("TeacherManagementWindow", "TeacherManagementWindow", nullptr));
        actionHelp->setText(QCoreApplication::translate("TeacherManagementWindow", "Help", nullptr));
        actionExit->setText(QCoreApplication::translate("TeacherManagementWindow", "Exit", nullptr));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("TeacherManagementWindow", "Overview", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("TeacherManagementWindow", "Courses", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("TeacherManagementWindow", "Problems", nullptr));
        listWidget->setSortingEnabled(__sortingEnabled);

        label->setText(QCoreApplication::translate("TeacherManagementWindow", "Teacher's Management Window", nullptr));
        menuMenu->setTitle(QCoreApplication::translate("TeacherManagementWindow", "Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TeacherManagementWindow: public Ui_TeacherManagementWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEACHERMANAGEMENTWINDOW_H
