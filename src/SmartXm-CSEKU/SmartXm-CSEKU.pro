QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++23

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    networking/client/client.cpp \
    networking/client/client3.cpp \
    networking/client/client_linux.cpp \
    networking/server/server.cpp \
    networking/server/server_linux.cpp \
    studentmodule.cpp \
    teachermodule.cpp \
    welcomewindow.cpp \
    dependencies/linux/Encryption/encryption.cpp

HEADERS += \
    ClientInfo.h \
    Msg.h \
    Student.h \
    TerminalExecuter.h \
    csv.h \
    db_sqlite.h \
    db_xampp.h \
    networking/client/Client.h \
    networking/server/Server.h \
    studentmodule.h \
    teachermodule.h \
    welcomewindow.h \
    dependencies/linux/Encryption/encryption.h

FORMS += \
    studentmodule.ui \
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
