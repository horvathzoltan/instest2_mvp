QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += SOURCE_PATH=$$PWD
DEFINES += TARGI=$$TARGET

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dowork.cpp \
    helpers/bitmapgen.cpp \
    helpers/commandlineparserhelper.cpp \
    helpers/doublematrix.cpp \
    helpers/filenamehelper.cpp \
    helpers/htmlhelper.cpp \
    helpers/httphelper.cpp \
    helpers/meta.cpp \
    helpers/textfilehelper.cpp \
    ipscanner.cpp \
    logger.cpp \
    main.cpp \
    mainpresenter.cpp \
    mainwindow.cpp \
    models/commandlineargs.cpp \
    models/model.cpp \
    models/settings.cpp \
    workerthread.cpp \
    zshortguid.cpp

HEADERS += \
    dowork.h \
    helpers/bitmapgen.h \
    helpers/commandlineparserhelper.h \
    helpers/doublematrix.h \
    helpers/filenamehelper.h \
    helpers/htmlhelper.h \
    helpers/httphelper.h \
    helpers/meta.h \
    helpers/textfilehelper.h \
    imainview.h \
    ipscanner.h \
    logger.h \
    macro.h \
    mainpresenter.h \
    mainwindow.h \
    models/commandlineargs.h \
    models/model.h \
    models/model_meta.h \
    models/responsemodel.h \
    models/settings.h \
    models/viewmodel.h \
    workerthread.h \
    zshortguid.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    insoletypes_sqlinsert.txt \
    settings.ini \
    todo.txt
