#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <teachermodule.h>
// #include <studentmodule.h>
#include <studentmodulev2.h>
// #include <ide.h>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class WelcomeWindow;
}
QT_END_NAMESPACE

class WelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

private slots:
    void on_exit_welcome_pushButton_4_clicked();

    void on_teacherWelcome_pushButton_4_clicked();

    void on_studentWelcome_pushButton_4_clicked();

    void on_nextRemoteServer_pushButton_4_clicked();

    void on_NextLocalServer_pushButton_4_clicked();

    void on_sync_remoteS_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_connect_to_local_server_pushButton_3_clicked();

    void updateStatusLabels();

private:
    Ui::WelcomeWindow *ui;
    QNetworkAccessManager *m_networkManager;
    void checkConnection();
    QTimer *statusUpdateTimer;

    TeacherModule *teacherModuleWindow;
    // StudentModule *studentModuleWindow;
    StudentModuleV2 *studentModuleV2Window;
    // IDE *ideEditor;
};
#endif // WELCOMEWINDOW_H
