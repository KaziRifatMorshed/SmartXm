/********************************************************************************
** Form generated from reading UI file 'login_window.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_WINDOW_H
#define UI_LOGIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login_Window
{
public:
    QAction *actionHelp;
    QAction *actionExit;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QGridLayout *gridLayout;
    QLabel *SmartXm_logo;
    QLabel *SmartXm_label;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout_2;
    QLineEdit *emailTextField;
    QLabel *label_pass;
    QLineEdit *passTextField;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_email;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *loginButton;
    QPushButton *pushButton_2;
    QPushButton *exitButton;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_3;
    QMenuBar *menubar;
    QMenu *menuMenu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Login_Window)
    {
        if (Login_Window->objectName().isEmpty())
            Login_Window->setObjectName("Login_Window");
        Login_Window->resize(800, 600);
        actionHelp = new QAction(Login_Window);
        actionHelp->setObjectName("actionHelp");
        actionExit = new QAction(Login_Window);
        actionExit->setObjectName("actionExit");
        centralwidget = new QWidget(Login_Window);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(110, 30, 611, 491));
        gridLayout_3 = new QGridLayout(layoutWidget);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");

        gridLayout_3->addWidget(label_2, 5, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        SmartXm_logo = new QLabel(layoutWidget);
        SmartXm_logo->setObjectName("SmartXm_logo");
        SmartXm_logo->setTextFormat(Qt::TextFormat::RichText);
        SmartXm_logo->setPixmap(QPixmap(QString::fromUtf8("../../logo/SmartXm-logo-mid.png")));
        SmartXm_logo->setScaledContents(false);

        gridLayout->addWidget(SmartXm_logo, 0, 0, 2, 1);

        SmartXm_label = new QLabel(layoutWidget);
        SmartXm_label->setObjectName("SmartXm_label");
        QFont font;
        font.setFamilies({QString::fromUtf8("Prototype")});
        font.setPointSize(36);
        SmartXm_label->setFont(font);

        gridLayout->addWidget(SmartXm_label, 0, 1, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setHorizontalSpacing(6);
        emailTextField = new QLineEdit(layoutWidget);
        emailTextField->setObjectName("emailTextField");

        gridLayout_2->addWidget(emailTextField, 1, 2, 1, 1);

        label_pass = new QLabel(layoutWidget);
        label_pass->setObjectName("label_pass");

        gridLayout_2->addWidget(label_pass, 2, 1, 1, 1);

        passTextField = new QLineEdit(layoutWidget);
        passTextField->setObjectName("passTextField");

        gridLayout_2->addWidget(passTextField, 2, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 3, 2, 1);

        label_email = new QLabel(layoutWidget);
        label_email->setObjectName("label_email");

        gridLayout_2->addWidget(label_email, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 2, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 0, 0, 1, 4);


        gridLayout_3->addLayout(gridLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        loginButton = new QPushButton(layoutWidget);
        loginButton->setObjectName("loginButton");

        horizontalLayout->addWidget(loginButton);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);

        exitButton = new QPushButton(layoutWidget);
        exitButton->setObjectName("exitButton");

        horizontalLayout->addWidget(exitButton);


        gridLayout_3->addLayout(horizontalLayout, 3, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 4, 0, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(720, 120, 81, 91));
        label_3->setPixmap(QPixmap(QString::fromUtf8("../../logo/SmartXm-logo-mini.png")));
        Login_Window->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Login_Window);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuMenu = new QMenu(menubar);
        menuMenu->setObjectName("menuMenu");
        Login_Window->setMenuBar(menubar);
        statusbar = new QStatusBar(Login_Window);
        statusbar->setObjectName("statusbar");
        Login_Window->setStatusBar(statusbar);

        menubar->addAction(menuMenu->menuAction());
        menuMenu->addAction(actionHelp);
        menuMenu->addAction(actionExit);

        retranslateUi(Login_Window);

        QMetaObject::connectSlotsByName(Login_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Login_Window)
    {
        Login_Window->setWindowTitle(QCoreApplication::translate("Login_Window", "Login_Window", nullptr));
        actionHelp->setText(QCoreApplication::translate("Login_Window", "Help", nullptr));
        actionExit->setText(QCoreApplication::translate("Login_Window", "Exit", nullptr));
        label_2->setText(QCoreApplication::translate("Login_Window", "<html><head/><body><p align=\"center\">Status: ...</p></body></html>", nullptr));
        SmartXm_logo->setText(QString());
        SmartXm_label->setText(QCoreApplication::translate("Login_Window", "SmartXm", nullptr));
        label->setText(QCoreApplication::translate("Login_Window", "Version: xxx", nullptr));
        label_pass->setText(QCoreApplication::translate("Login_Window", "Password:", nullptr));
        label_email->setText(QCoreApplication::translate("Login_Window", "Email:", nullptr));
        loginButton->setText(QCoreApplication::translate("Login_Window", "Login", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Login_Window", "Continue Without Remote Server", nullptr));
        exitButton->setText(QCoreApplication::translate("Login_Window", "Exit", nullptr));
        label_3->setText(QString());
        menuMenu->setTitle(QCoreApplication::translate("Login_Window", "Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login_Window: public Ui_Login_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_WINDOW_H
