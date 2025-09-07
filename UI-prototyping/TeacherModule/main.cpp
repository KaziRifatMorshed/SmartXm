#include "teachermodule.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TeacherModule w;
    w.show();
    return a.exec();
}
