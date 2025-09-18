#include "welcomewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(true);
    a.setWindowIcon(QIcon(":/logo/resources/logo/SmartXm-logo-mini.png"));
    WelcomeWindow w;
    w.show();
    return a.exec();
}
