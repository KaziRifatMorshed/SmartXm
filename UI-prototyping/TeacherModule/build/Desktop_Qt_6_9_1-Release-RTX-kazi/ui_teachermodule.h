/********************************************************************************
** Form generated from reading UI file 'teachermodule.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEACHERMODULE_H
#define UI_TEACHERMODULE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TeacherModule
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTabWidget *tabWidget;
    QWidget *Dashboard_tab;
    QWidget *QB_tab;
    QWidget *Server_tab;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *serverStatus_label_2;
    QLabel *serverIP_label_3;
    QHBoxLayout *horizontalLayout;
    QToolButton *StartServer_toolButton;
    QToolButton *StopServer_toolButton_2;
    QFormLayout *formLayout;
    QLabel *ServerUptimeLabel_2;
    QLabel *label_3;
    QLabel *ConnectedStudentslabel;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QWidget *exam_tab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *AllExamGroupBox;
    QVBoxLayout *verticalLayout_3;
    QTableView *AllExamTableView;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TeacherModule)
    {
        if (TeacherModule->objectName().isEmpty())
            TeacherModule->setObjectName("TeacherModule");
        TeacherModule->resize(901, 706);
        centralwidget = new QWidget(TeacherModule);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        Dashboard_tab = new QWidget();
        Dashboard_tab->setObjectName("Dashboard_tab");
        tabWidget->addTab(Dashboard_tab, QString());
        QB_tab = new QWidget();
        QB_tab->setObjectName("QB_tab");
        tabWidget->addTab(QB_tab, QString());
        Server_tab = new QWidget();
        Server_tab->setObjectName("Server_tab");
        verticalLayout_5 = new QVBoxLayout(Server_tab);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        serverStatus_label_2 = new QLabel(Server_tab);
        serverStatus_label_2->setObjectName("serverStatus_label_2");

        verticalLayout_4->addWidget(serverStatus_label_2);

        serverIP_label_3 = new QLabel(Server_tab);
        serverIP_label_3->setObjectName("serverIP_label_3");

        verticalLayout_4->addWidget(serverIP_label_3);


        horizontalLayout_2->addLayout(verticalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        StartServer_toolButton = new QToolButton(Server_tab);
        StartServer_toolButton->setObjectName("StartServer_toolButton");

        horizontalLayout->addWidget(StartServer_toolButton);

        StopServer_toolButton_2 = new QToolButton(Server_tab);
        StopServer_toolButton_2->setObjectName("StopServer_toolButton_2");

        horizontalLayout->addWidget(StopServer_toolButton_2);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(horizontalLayout_2);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        ServerUptimeLabel_2 = new QLabel(Server_tab);
        ServerUptimeLabel_2->setObjectName("ServerUptimeLabel_2");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, ServerUptimeLabel_2);

        label_3 = new QLabel(Server_tab);
        label_3->setObjectName("label_3");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, label_3);

        ConnectedStudentslabel = new QLabel(Server_tab);
        ConnectedStudentslabel->setObjectName("ConnectedStudentslabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, ConnectedStudentslabel);

        label_5 = new QLabel(Server_tab);
        label_5->setObjectName("label_5");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, label_5);

        label_6 = new QLabel(Server_tab);
        label_6->setObjectName("label_6");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_6);

        label_7 = new QLabel(Server_tab);
        label_7->setObjectName("label_7");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, label_7);


        verticalLayout_5->addLayout(formLayout);

        tabWidget->addTab(Server_tab, QString());
        exam_tab = new QWidget();
        exam_tab->setObjectName("exam_tab");
        verticalLayout_2 = new QVBoxLayout(exam_tab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        AllExamGroupBox = new QGroupBox(exam_tab);
        AllExamGroupBox->setObjectName("AllExamGroupBox");
        verticalLayout_3 = new QVBoxLayout(AllExamGroupBox);
        verticalLayout_3->setObjectName("verticalLayout_3");
        AllExamTableView = new QTableView(AllExamGroupBox);
        AllExamTableView->setObjectName("AllExamTableView");

        verticalLayout_3->addWidget(AllExamTableView);

        pushButton = new QPushButton(AllExamGroupBox);
        pushButton->setObjectName("pushButton");

        verticalLayout_3->addWidget(pushButton);

        pushButton_2 = new QPushButton(AllExamGroupBox);
        pushButton_2->setObjectName("pushButton_2");

        verticalLayout_3->addWidget(pushButton_2);


        verticalLayout_2->addWidget(AllExamGroupBox);

        tabWidget->addTab(exam_tab, QString());

        verticalLayout->addWidget(tabWidget);

        TeacherModule->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TeacherModule);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 901, 22));
        TeacherModule->setMenuBar(menubar);
        statusbar = new QStatusBar(TeacherModule);
        statusbar->setObjectName("statusbar");
        TeacherModule->setStatusBar(statusbar);

        retranslateUi(TeacherModule);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(TeacherModule);
    } // setupUi

    void retranslateUi(QMainWindow *TeacherModule)
    {
        TeacherModule->setWindowTitle(QCoreApplication::translate("TeacherModule", "TeacherModule", nullptr));
        label->setText(QCoreApplication::translate("TeacherModule", "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">Welcome, xyz Sir</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Dashboard_tab), QCoreApplication::translate("TeacherModule", "Dashboard", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(QB_tab), QCoreApplication::translate("TeacherModule", "Questionbank", nullptr));
        serverStatus_label_2->setText(QCoreApplication::translate("TeacherModule", "<html><head/><body><p><span style=\" font-size:18pt;\">Server Status: XYZ</span></p></body></html>", nullptr));
        serverIP_label_3->setText(QCoreApplication::translate("TeacherModule", "<html><head/><body><p><span style=\" font-size:18pt;\">Server Local IP: XYZ</span></p></body></html>", nullptr));
        StartServer_toolButton->setText(QCoreApplication::translate("TeacherModule", "Start Server", nullptr));
        StopServer_toolButton_2->setText(QCoreApplication::translate("TeacherModule", "Stop Server", nullptr));
        ServerUptimeLabel_2->setText(QCoreApplication::translate("TeacherModule", "Server Uptime:", nullptr));
        label_3->setText(QCoreApplication::translate("TeacherModule", "TextLabel", nullptr));
        ConnectedStudentslabel->setText(QCoreApplication::translate("TeacherModule", "Connected Students:", nullptr));
        label_5->setText(QCoreApplication::translate("TeacherModule", "TextLabel", nullptr));
        label_6->setText(QCoreApplication::translate("TeacherModule", "TextLabel", nullptr));
        label_7->setText(QCoreApplication::translate("TeacherModule", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Server_tab), QCoreApplication::translate("TeacherModule", "Server", nullptr));
        AllExamGroupBox->setTitle(QCoreApplication::translate("TeacherModule", "All Exams", nullptr));
        pushButton->setText(QCoreApplication::translate("TeacherModule", "Create Exam", nullptr));
        pushButton_2->setText(QCoreApplication::translate("TeacherModule", "Edit Exam", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(exam_tab), QCoreApplication::translate("TeacherModule", "Exam", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TeacherModule: public Ui_TeacherModule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEACHERMODULE_H
