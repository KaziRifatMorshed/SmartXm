/********************************************************************************
** Form generated from reading UI file 'welcomewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEWINDOW_H
#define UI_WELCOMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomeWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *welcome_label;
    QTabWidget *tabWidget;
    QWidget *welcome_tab;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_8;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *exit_welcome_pushButton_4;
    QPushButton *teacherWelcome_pushButton_4;
    QPushButton *studentWelcome_pushButton_4;
    QWidget *remoteServerTab;
    QVBoxLayout *verticalLayout_2;
    QLabel *syncWithRemoteServer_label;
    QLabel *xampp_status_label;
    QLabel *netConnectionStatus_label;
    QPushButton *sync_remoteS_pushButton_2;
    QLabel *sync_status_label_2;
    QPushButton *nextRemoteServer_pushButton_4;
    QLabel *label;
    QWidget *LocalServer_tab;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QFormLayout *formLayout_2;
    QLabel *label_5;
    QLineEdit *localServerIP_lineEdit_3;
    QPushButton *pushButton_3;
    QLabel *localServerStatus_label_6;
    QLabel *label_6;
    QPushButton *NextLocalServer_pushButton_4;
    QWidget *login_tab;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QFormLayout *formLayout;
    QLabel *email_label;
    QLineEdit *lineEdit;
    QLabel *pass_label;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QLabel *loginStatus_label_9;
    QLabel *footer_label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WelcomeWindow)
    {
        if (WelcomeWindow->objectName().isEmpty())
            WelcomeWindow->setObjectName("WelcomeWindow");
        WelcomeWindow->resize(800, 600);
        centralwidget = new QWidget(WelcomeWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        welcome_label = new QLabel(centralwidget);
        welcome_label->setObjectName("welcome_label");

        verticalLayout->addWidget(welcome_label);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        welcome_tab = new QWidget();
        welcome_tab->setObjectName("welcome_tab");
        verticalLayout_5 = new QVBoxLayout(welcome_tab);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_8 = new QLabel(welcome_tab);
        label_8->setObjectName("label_8");

        horizontalLayout_2->addWidget(label_8);

        label_7 = new QLabel(welcome_tab);
        label_7->setObjectName("label_7");

        horizontalLayout_2->addWidget(label_7);


        verticalLayout_5->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        exit_welcome_pushButton_4 = new QPushButton(welcome_tab);
        exit_welcome_pushButton_4->setObjectName("exit_welcome_pushButton_4");

        horizontalLayout_3->addWidget(exit_welcome_pushButton_4);

        teacherWelcome_pushButton_4 = new QPushButton(welcome_tab);
        teacherWelcome_pushButton_4->setObjectName("teacherWelcome_pushButton_4");

        horizontalLayout_3->addWidget(teacherWelcome_pushButton_4);

        studentWelcome_pushButton_4 = new QPushButton(welcome_tab);
        studentWelcome_pushButton_4->setObjectName("studentWelcome_pushButton_4");

        horizontalLayout_3->addWidget(studentWelcome_pushButton_4);


        verticalLayout_5->addLayout(horizontalLayout_3);

        tabWidget->addTab(welcome_tab, QString());
        remoteServerTab = new QWidget();
        remoteServerTab->setObjectName("remoteServerTab");
        verticalLayout_2 = new QVBoxLayout(remoteServerTab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        syncWithRemoteServer_label = new QLabel(remoteServerTab);
        syncWithRemoteServer_label->setObjectName("syncWithRemoteServer_label");

        verticalLayout_2->addWidget(syncWithRemoteServer_label);

        xampp_status_label = new QLabel(remoteServerTab);
        xampp_status_label->setObjectName("xampp_status_label");

        verticalLayout_2->addWidget(xampp_status_label);

        netConnectionStatus_label = new QLabel(remoteServerTab);
        netConnectionStatus_label->setObjectName("netConnectionStatus_label");

        verticalLayout_2->addWidget(netConnectionStatus_label);

        sync_remoteS_pushButton_2 = new QPushButton(remoteServerTab);
        sync_remoteS_pushButton_2->setObjectName("sync_remoteS_pushButton_2");

        verticalLayout_2->addWidget(sync_remoteS_pushButton_2);

        sync_status_label_2 = new QLabel(remoteServerTab);
        sync_status_label_2->setObjectName("sync_status_label_2");

        verticalLayout_2->addWidget(sync_status_label_2);

        nextRemoteServer_pushButton_4 = new QPushButton(remoteServerTab);
        nextRemoteServer_pushButton_4->setObjectName("nextRemoteServer_pushButton_4");

        verticalLayout_2->addWidget(nextRemoteServer_pushButton_4);

        label = new QLabel(remoteServerTab);
        label->setObjectName("label");

        verticalLayout_2->addWidget(label);

        tabWidget->addTab(remoteServerTab, QString());
        LocalServer_tab = new QWidget();
        LocalServer_tab->setObjectName("LocalServer_tab");
        verticalLayout_4 = new QVBoxLayout(LocalServer_tab);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label_4 = new QLabel(LocalServer_tab);
        label_4->setObjectName("label_4");

        verticalLayout_4->addWidget(label_4);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName("formLayout_2");
        label_5 = new QLabel(LocalServer_tab);
        label_5->setObjectName("label_5");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::LabelRole, label_5);

        localServerIP_lineEdit_3 = new QLineEdit(LocalServer_tab);
        localServerIP_lineEdit_3->setObjectName("localServerIP_lineEdit_3");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::FieldRole, localServerIP_lineEdit_3);

        pushButton_3 = new QPushButton(LocalServer_tab);
        pushButton_3->setObjectName("pushButton_3");

        formLayout_2->setWidget(1, QFormLayout::ItemRole::FieldRole, pushButton_3);


        verticalLayout_4->addLayout(formLayout_2);

        localServerStatus_label_6 = new QLabel(LocalServer_tab);
        localServerStatus_label_6->setObjectName("localServerStatus_label_6");

        verticalLayout_4->addWidget(localServerStatus_label_6);

        label_6 = new QLabel(LocalServer_tab);
        label_6->setObjectName("label_6");

        verticalLayout_4->addWidget(label_6);

        NextLocalServer_pushButton_4 = new QPushButton(LocalServer_tab);
        NextLocalServer_pushButton_4->setObjectName("NextLocalServer_pushButton_4");

        verticalLayout_4->addWidget(NextLocalServer_pushButton_4);

        tabWidget->addTab(LocalServer_tab, QString());
        login_tab = new QWidget();
        login_tab->setObjectName("login_tab");
        verticalLayout_3 = new QVBoxLayout(login_tab);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_3 = new QLabel(login_tab);
        label_3->setObjectName("label_3");

        verticalLayout_3->addWidget(label_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        email_label = new QLabel(login_tab);
        email_label->setObjectName("email_label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, email_label);

        lineEdit = new QLineEdit(login_tab);
        lineEdit->setObjectName("lineEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEdit);

        pass_label = new QLabel(login_tab);
        pass_label->setObjectName("pass_label");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, pass_label);

        lineEdit_2 = new QLineEdit(login_tab);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setEchoMode(QLineEdit::EchoMode::Password);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEdit_2);

        pushButton = new QPushButton(login_tab);
        pushButton->setObjectName("pushButton");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, pushButton);


        horizontalLayout->addLayout(formLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);
        horizontalLayout->setStretch(2, 1);

        verticalLayout_3->addLayout(horizontalLayout);

        loginStatus_label_9 = new QLabel(login_tab);
        loginStatus_label_9->setObjectName("loginStatus_label_9");

        verticalLayout_3->addWidget(loginStatus_label_9);

        tabWidget->addTab(login_tab, QString());

        verticalLayout->addWidget(tabWidget);

        footer_label = new QLabel(centralwidget);
        footer_label->setObjectName("footer_label");

        verticalLayout->addWidget(footer_label);

        WelcomeWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WelcomeWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        WelcomeWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(WelcomeWindow);
        statusbar->setObjectName("statusbar");
        WelcomeWindow->setStatusBar(statusbar);

        retranslateUi(WelcomeWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(WelcomeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *WelcomeWindow)
    {
        WelcomeWindow->setWindowTitle(QCoreApplication::translate("WelcomeWindow", "WelcomeWindow", nullptr));
        welcome_label->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">Welcome to SmartXm</span></p></body></html>", nullptr));
        label_8->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p><img src=\":/logo/resources/logo/SmartXm-logo-mid.png\"/></p></body></html>", nullptr));
        label_7->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p><span style=\" font-size:24pt; font-weight:700;\">SmartXm</span></p></body></html>", nullptr));
        exit_welcome_pushButton_4->setText(QCoreApplication::translate("WelcomeWindow", "Exit", nullptr));
        teacherWelcome_pushButton_4->setText(QCoreApplication::translate("WelcomeWindow", "Teacher", nullptr));
        studentWelcome_pushButton_4->setText(QCoreApplication::translate("WelcomeWindow", "Student", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(welcome_tab), QCoreApplication::translate("WelcomeWindow", "Welcome", nullptr));
        syncWithRemoteServer_label->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">Synchronize with Remote Server</span></p></body></html>", nullptr));
        xampp_status_label->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\">XAMPP DB Server Status: not implemented</p></body></html>", nullptr));
        netConnectionStatus_label->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\">Internet Connection Status: (not implemented yet)</p></body></html>", nullptr));
        sync_remoteS_pushButton_2->setText(QCoreApplication::translate("WelcomeWindow", "Sync", nullptr));
        sync_status_label_2->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\">Synchronization Status: (not implemented yet)</p></body></html>", nullptr));
        nextRemoteServer_pushButton_4->setText(QCoreApplication::translate("WelcomeWindow", "Next", nullptr));
        label->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\">If synchronization takes too much time, please visit <a href=\"https://cseku-smartxm.free.nf/\"><span style=\" text-decoration: underline; color:#27bf73;\">https://cseku-smartxm.free.nf/</span></a> to awake remote server</p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(remoteServerTab), QCoreApplication::translate("WelcomeWindow", "Remote Server", nullptr));
        label_4->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">Connect to Local Server</span></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("WelcomeWindow", "Local Server IP:", nullptr));
        pushButton_3->setText(QCoreApplication::translate("WelcomeWindow", "Connect", nullptr));
        localServerStatus_label_6->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:11pt;\">Local Server Connection Status: (not implemented yet)</span></p></body></html>", nullptr));
        label_6->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\">Instruction: Connect to a local server and click next</p></body></html>", nullptr));
        NextLocalServer_pushButton_4->setText(QCoreApplication::translate("WelcomeWindow", "Next", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(LocalServer_tab), QCoreApplication::translate("WelcomeWindow", "Local Server", nullptr));
        label_3->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">Enter Email ID and Password</span></p></body></html>", nullptr));
        email_label->setText(QCoreApplication::translate("WelcomeWindow", "Email:", nullptr));
        pass_label->setText(QCoreApplication::translate("WelcomeWindow", "Password:", nullptr));
        pushButton->setText(QCoreApplication::translate("WelcomeWindow", "Login", nullptr));
        loginStatus_label_9->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:11pt;\">Login Status: (not implemented yet)</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(login_tab), QCoreApplication::translate("WelcomeWindow", "Login", nullptr));
        footer_label->setText(QCoreApplication::translate("WelcomeWindow", "<html><head/><body><p align=\"center\">Made with \342\235\244\357\270\216 at CSE Discipline, Khulna University, Khulna</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WelcomeWindow: public Ui_WelcomeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEWINDOW_H
