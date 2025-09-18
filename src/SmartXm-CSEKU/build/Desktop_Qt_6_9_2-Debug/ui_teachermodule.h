/********************************************************************************
** Form generated from reading UI file 'teachermodule.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEACHERMODULE_H
#define UI_TEACHERMODULE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
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
    QTableWidget *connectedPCwithServer_tableWidget;
    QWidget *exam_tab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *AllExamGroupBox;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *allXmList_tableWidget;
    QPushButton *createXm_pushButton;
    QPushButton *editExam_pushButon_2;
    QPushButton *testExam_pushButton_3;
    QGroupBox *sendInstructions_groupBox;
    QGridLayout *gridLayout;
    QPushButton *instruction_send_pushButton;
    QPushButton *selectFile_pushButton;
    QLabel *selectedFileToSendInstruction_label_2;
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

        connectedPCwithServer_tableWidget = new QTableWidget(Server_tab);
        if (connectedPCwithServer_tableWidget->columnCount() < 4)
            connectedPCwithServer_tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        connectedPCwithServer_tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        connectedPCwithServer_tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        connectedPCwithServer_tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        connectedPCwithServer_tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        connectedPCwithServer_tableWidget->setObjectName("connectedPCwithServer_tableWidget");
        connectedPCwithServer_tableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_5->addWidget(connectedPCwithServer_tableWidget);

        tabWidget->addTab(Server_tab, QString());
        exam_tab = new QWidget();
        exam_tab->setObjectName("exam_tab");
        verticalLayout_2 = new QVBoxLayout(exam_tab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        AllExamGroupBox = new QGroupBox(exam_tab);
        AllExamGroupBox->setObjectName("AllExamGroupBox");
        verticalLayout_3 = new QVBoxLayout(AllExamGroupBox);
        verticalLayout_3->setObjectName("verticalLayout_3");
        allXmList_tableWidget = new QTableWidget(AllExamGroupBox);
        if (allXmList_tableWidget->columnCount() < 7)
            allXmList_tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        allXmList_tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        allXmList_tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        allXmList_tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        allXmList_tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        allXmList_tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        allXmList_tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        allXmList_tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem10);
        if (allXmList_tableWidget->rowCount() < 3)
            allXmList_tableWidget->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        allXmList_tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        allXmList_tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(0, 0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(0, 1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(0, 2, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(0, 3, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(0, 4, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(0, 5, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(1, 0, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(1, 1, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(1, 2, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(1, 3, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(1, 4, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(1, 5, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(2, 0, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(2, 1, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(2, 2, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(2, 3, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(2, 4, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        allXmList_tableWidget->setItem(2, 5, __qtablewidgetitem30);
        allXmList_tableWidget->setObjectName("allXmList_tableWidget");
        allXmList_tableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_3->addWidget(allXmList_tableWidget);

        createXm_pushButton = new QPushButton(AllExamGroupBox);
        createXm_pushButton->setObjectName("createXm_pushButton");

        verticalLayout_3->addWidget(createXm_pushButton);

        editExam_pushButon_2 = new QPushButton(AllExamGroupBox);
        editExam_pushButon_2->setObjectName("editExam_pushButon_2");

        verticalLayout_3->addWidget(editExam_pushButon_2);

        testExam_pushButton_3 = new QPushButton(AllExamGroupBox);
        testExam_pushButton_3->setObjectName("testExam_pushButton_3");

        verticalLayout_3->addWidget(testExam_pushButton_3);


        verticalLayout_2->addWidget(AllExamGroupBox);

        tabWidget->addTab(exam_tab, QString());

        verticalLayout->addWidget(tabWidget);

        sendInstructions_groupBox = new QGroupBox(centralwidget);
        sendInstructions_groupBox->setObjectName("sendInstructions_groupBox");
        gridLayout = new QGridLayout(sendInstructions_groupBox);
        gridLayout->setObjectName("gridLayout");
        instruction_send_pushButton = new QPushButton(sendInstructions_groupBox);
        instruction_send_pushButton->setObjectName("instruction_send_pushButton");

        gridLayout->addWidget(instruction_send_pushButton, 1, 1, 1, 1);

        selectFile_pushButton = new QPushButton(sendInstructions_groupBox);
        selectFile_pushButton->setObjectName("selectFile_pushButton");

        gridLayout->addWidget(selectFile_pushButton, 1, 0, 1, 1);

        selectedFileToSendInstruction_label_2 = new QLabel(sendInstructions_groupBox);
        selectedFileToSendInstruction_label_2->setObjectName("selectedFileToSendInstruction_label_2");

        gridLayout->addWidget(selectedFileToSendInstruction_label_2, 0, 0, 1, 2);


        verticalLayout->addWidget(sendInstructions_groupBox);

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
        QTableWidgetItem *___qtablewidgetitem = connectedPCwithServer_tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("TeacherModule", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = connectedPCwithServer_tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("TeacherModule", "Stu ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = connectedPCwithServer_tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("TeacherModule", "Local IP", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = connectedPCwithServer_tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("TeacherModule", "Action", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Server_tab), QCoreApplication::translate("TeacherModule", "Server", nullptr));
        AllExamGroupBox->setTitle(QCoreApplication::translate("TeacherModule", "All Exams", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = allXmList_tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("TeacherModule", "Course Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = allXmList_tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("TeacherModule", "Course Code", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = allXmList_tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("TeacherModule", "Course Teacher", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = allXmList_tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("TeacherModule", "Exam Topic", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = allXmList_tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("TeacherModule", "Exam Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = allXmList_tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("TeacherModule", "Exam Time", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = allXmList_tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("TeacherModule", "Action", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = allXmList_tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("TeacherModule", "2", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = allXmList_tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("TeacherModule", "1", nullptr));

        const bool __sortingEnabled = allXmList_tableWidget->isSortingEnabled();
        allXmList_tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem13 = allXmList_tableWidget->item(0, 0);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("TeacherModule", "Data Structure Lab", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = allXmList_tableWidget->item(0, 1);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("TeacherModule", "CSE 2100", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = allXmList_tableWidget->item(0, 2);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("TeacherModule", "Mr. XYZ ABCD", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = allXmList_tableWidget->item(0, 3);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("TeacherModule", "Stack", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = allXmList_tableWidget->item(0, 4);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("TeacherModule", "dummy", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = allXmList_tableWidget->item(0, 5);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("TeacherModule", "dummy", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = allXmList_tableWidget->item(1, 0);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("TeacherModule", "Computer Fundamentals Lab", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = allXmList_tableWidget->item(1, 1);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("TeacherModule", "CSE 1100", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = allXmList_tableWidget->item(1, 2);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("TeacherModule", "Mr WXYZ", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = allXmList_tableWidget->item(1, 3);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("TeacherModule", "PowerPoint", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = allXmList_tableWidget->item(1, 4);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("TeacherModule", "dummy", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = allXmList_tableWidget->item(1, 5);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("TeacherModule", "dummy", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = allXmList_tableWidget->item(2, 0);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("TeacherModule", "Algorithms Lab", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = allXmList_tableWidget->item(2, 1);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("TeacherModule", "CSE 2200", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = allXmList_tableWidget->item(2, 2);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("TeacherModule", "Mr. ABC XYZ", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = allXmList_tableWidget->item(2, 3);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("TeacherModule", "Greedy", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = allXmList_tableWidget->item(2, 4);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("TeacherModule", "dummy", nullptr));
        QTableWidgetItem *___qtablewidgetitem30 = allXmList_tableWidget->item(2, 5);
        ___qtablewidgetitem30->setText(QCoreApplication::translate("TeacherModule", "dummy", nullptr));
        allXmList_tableWidget->setSortingEnabled(__sortingEnabled);

        createXm_pushButton->setText(QCoreApplication::translate("TeacherModule", "Create Exam", nullptr));
        editExam_pushButon_2->setText(QCoreApplication::translate("TeacherModule", "Edit Exam", nullptr));
        testExam_pushButton_3->setText(QCoreApplication::translate("TeacherModule", "Test Exam", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(exam_tab), QCoreApplication::translate("TeacherModule", "Exam", nullptr));
        sendInstructions_groupBox->setTitle(QCoreApplication::translate("TeacherModule", "Send Instructions to all clients", nullptr));
        instruction_send_pushButton->setText(QCoreApplication::translate("TeacherModule", "Send", nullptr));
        selectFile_pushButton->setText(QCoreApplication::translate("TeacherModule", "Select File", nullptr));
        selectedFileToSendInstruction_label_2->setText(QCoreApplication::translate("TeacherModule", "Selected File: none", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TeacherModule: public Ui_TeacherModule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEACHERMODULE_H
