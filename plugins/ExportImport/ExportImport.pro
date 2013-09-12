!include("../Common/retroshare_plugin.pri"): error("Could not include file ../Common/retroshare_plugin.pri")

INCLUDEPATH += jsoncpp

HEADERS += \
    ExportImportManager.h \
    ExportImportPlugin.h \
    ExportImportPage.h

HEADERS -= upnp/upnputil.h
SOURCES -= upnp/upnputil.c
SOURCES += \
    jsoncpp/jsoncpp.cpp \
    ExportImportManager.cpp \
    ExportImportPlugin.cpp \
    ExportImportPage.cpp

FORMS += \
    ExportImportPage.ui

RESOURCES += \
    images.qrc
