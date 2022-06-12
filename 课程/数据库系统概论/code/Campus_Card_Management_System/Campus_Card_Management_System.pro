QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


TARGET = ccm
TEMPLATE = app
RC_FILE += version.rc
RC_ICONS = icon.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin.cpp \
    card_status.cpp \
    connect.cpp \
    deposit.cpp \
    main.cpp \
    mainwindow.cpp \
    query.cpp \
    stu_func.cpp \
    student.cpp

HEADERS += \
    admin.h \
    card_status.h \
    connect.h \
    deposit.h \
    mainwindow.h \
    query.h \
    stu_func.h \
    student.h

FORMS += \
    admin.ui \
    card_status.ui \
    deposit.ui \
    mainwindow.ui \
    query.ui \
    stu_func.ui \
    student.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
