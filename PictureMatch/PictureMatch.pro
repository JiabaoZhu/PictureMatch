#-------------------------------------------------
#
# Project created by QtCreator 2015-11-18T09:29:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PictureMatch
TEMPLATE = app
RC_FILE += PictureMatch.rc


SOURCES += main.cpp\
        PictureMatch.cpp \
    MessageDialog.cpp \
    PictureMatchingCore.cpp \
    LockClass.cpp \
    RunGame.cpp \
    SelectLevel.cpp

HEADERS  += PictureMatch.h \
    MessageDialog.h \
    PictureMatchingCore.h \
    LockClass.h \
    RunGame.h \
    SelectLevel.h

RESOURCES += \
    MessageDialog.qrc \
    RunGame.qrc \
    PictureMatch.qrc

FORMS += \
    MessageDialog.ui \
    RunGame.ui \
    SelectLevel.ui

OTHER_FILES += \
    PictureMatch.rc
