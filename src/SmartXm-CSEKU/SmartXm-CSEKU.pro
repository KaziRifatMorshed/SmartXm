QT       += core gui sql network concurrent pdfwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++23

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:LIBS += -lws2_32 \
    -lShell32

SOURCES += \
    CodeRunnerWorker.cpp \
    Judge.cpp \
    Users.cpp \
    codeRunner.cpp \
    db_xampp.cpp \
    dependencies/TarHandler/tarhandler.cpp \
    ide.cpp \
    main.cpp \
    networking/client/client.cpp \
    networking/client/client_linux.cpp \
    networking/client/client_windows.cpp \
    networking/server/server.cpp \
    networking/server/server_linux.cpp \
    networking/server/server_windows.cpp \
    studentmodulev2.cpp \
    runtestcases.cpp \
    teachermodule.cpp \
    toast.cpp \
    welcomewindow.cpp \
    dependencies/linux/Encryption/encryption.cpp

HEADERS += \
    ClientInfo.h \
    CodeRunnerWorker.h \
    Judge.h \
    Message.h \
    Student.h \
    TerminalExecuter.h \
    Users.h \
    codeRunner.h \
    csv.h \
    db_sqlite.h \
    db_xampp.h \
    dependencies/TarHandler/tarhandler.h \
    ide.h \
    networking/FileMeta.h \
    networking/client/Client.h \
    networking/server/Server.h \
    studentmodulev2.h \
    runtestcases.h \
    teachermodule.h \
    toast.h \
    welcomewindow.h \
    dependencies/linux/Encryption/encryption.h

FORMS += \
    ide.ui \
    studentmodulev2.ui \
    runtestcases.ui \
    teachermodule.ui \
    welcomewindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    SmartXm-CSEKU.pro.user \
    SmartXm-CSEKU.pro.user.c7782b4 \
    networking/server/Server.md

RESOURCES += \
    qt_resources.qrc

