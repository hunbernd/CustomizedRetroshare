!include("../Common/retroshare_plugin.pri"):error("Could not include file ../Common/retroshare_plugin.pri")

CONFIG += qt \
    uic \
    qrc \
    resources

SOURCES = \
    p3ExampleRS.cpp \
    RSExampleItems.cpp \
    ExampleDialog.cpp \
    ExamplePlugin.cpp

LIBS += -lsqlite3

DEFINES += ZR_TESTNET

HEADERS = \
    p3ExampleRS.h \
    RSExampleItems.h \
    ExampleDialog.h \
    ExamplePlugin.h

FORMS = \
    ExampleDialog.ui

TARGET = Example

RESOURCES = \
    Example_images.qrc
