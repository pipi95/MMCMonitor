#-------------------------------------------------
#
# Project created by QtCreator 2018-03-12T16:24:03
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MMCMonitorApp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mmccontrol.cpp \
    mmcstate.cpp \
    config.cpp \
    mmcdisplaytablemodel.cpp

HEADERS += \
    mainwindow.h \
    mmccontrol.h \
    mmcstate.h \
    config.h \
    mmcdisplaytablemodel.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    log.conf

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FrameOperator/release/ -lFrameOperator
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FrameOperator/debug/ -lFrameOperator

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Log4Qt/release/ -lLog4Qt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Log4Qt/debug/ -lLog4Qt
