######################################################################
# Automatically generated by qmake (2.01a) Di. Aug 18 18:17:17 2009
######################################################################

TEMPLATE = app
TARGET = RemoteClipboard
DEPENDPATH += . gui
INCLUDEPATH += /home/sicus/proggen/remoteClipboard/. .
DESTDIR = bin/
QT      += network

# Input
HEADERS += MainWindow.h NetworkConnection.h global.h RemoteClient.h
FORMS += gui/ConnectDlg.ui gui/MainWindow.ui gui/StartServerDlg.ui gui/ClientTab.ui gui/About.ui
SOURCES += main.cpp MainWindow.cpp NetworkConnection.cpp RemoteClient.cpp

OBJECTS_DIR = obj/
MOC_DIR = generatedfiles/
UI_DIR  = generatedfiles/

