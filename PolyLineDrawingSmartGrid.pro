#-------------------------------------------------
#
# Project created by QtCreator 2018-01-11T19:52:19
#
#-------------------------------------------------

QT += widgets
QT += opengl

LIBS += -lopengl32

SOURCES += main.cpp \
    mainwindow.cpp \
    polydialog.cpp \
    draw.cpp

HEADERS += \
    mainwindow.h \
    polydialog.h \
    draw.h

RESOURCES += \
    resources.qrc

FORMS += \
    polydialog.ui \
