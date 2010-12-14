# -------------------------------------------------
# Project created by QtCreator 2010-09-21T13:47:18
# -------------------------------------------------
QT += webkit \
    network
TARGET = SimpleApp
TEMPLATE = app
CONFIG += uitools

# Removing the line below, uncomment the post link instruction at the end
CONFIG -= app_bundle
SOURCES += main.cpp \
    mafMainWindow.cpp \
    googlechat.cpp
HEADERS += mafMainWindow.h \
    googlechat.h
FORMS += mafMainWindow.ui \
    form.ui

include(../../mafCommon.pri)
include(../../mafImportVTK.pri)

DEPENDPATH += $$DESTDIR

INCLUDEPATH += ../../mafCore \
    ../../mafEventBus \
    ../../mafApplicationLogic \
    ../../mafGUI
LIBS += -L$$DESTDIR \
    -lmafCore$$BUILD_EXT \
    -lmafEventBus$$BUILD_EXT \
    -lmafApplicationLogic$$BUILD_EXT \
    -lmafGUI$$BUILD_EXT
RESOURCES += mafAppImages.qrc

#macx {
#    QT_CORE = /Library/Frameworks/QtCore.framework
#    QT_WEBKIT = /Library/Frameworks/QtWebKit.framework
#    QT_NETWORK = /Library/Frameworks/QtNetwork.framework
#    QT_XML = /Library/Frameworks/QtXml.framework
#    QT_GUI = /Library/Frameworks/QtGui.framework

#    QMAKE_CFLAGS += -F
#    LIBS += -framework QtCore -framework QtNetwork -framework QtXml -framework QtGui

#    DEPENDPATH += $$DESTDIR/$${TARGET}.app/Contents/Frameworks

# Create the Framework directory inside the package.
#QMAKE_POST_LINK = mkdir $$DESTDIR/$${TARGET}.app/Contents/Frameworks
# install MAF libraries inside the package
#QMAKE_POST_LINK += && $$CP_CMD -R $$MAF_CORE $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD -R $$MAF_EVENTBUS $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD -R $$MAF_APPLICATIONLOGIC $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD -R $$MAF_GUI $$DESTDIR/$${TARGET}.app/Contents/Frameworks
# install Qt libraries inside the package
#QMAKE_POST_LINK += && $$CP_CMD -R $$QT_CORE $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD -R $$QT_WEBKIT $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD -R $$QT_XML $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD -R $$QT_GUI $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD -R $$QT_NETWORK $$DESTDIR/$${TARGET}.app/Contents/Frameworks
# install VTK libraries inside the package
#QMAKE_POST_LINK += && $$CP_CMD $$DESTDIR/libvtk* $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD $$DESTDIR/libQVTK* $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD $$DESTDIR/libMap* $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD $$DESTDIR/libmpistub* $$DESTDIR/$${TARGET}.app/Contents/Frameworks
# install xmlrpc, qtSOAP libraries inside the package
#QMAKE_POST_LINK += && $$CP_CMD $$DESTDIR/libqxmlrpc$${BUILD_EXT}* $$DESTDIR/$${TARGET}.app/Contents/Frameworks \
#                             && $$CP_CMD $$DESTDIR/libQtSolutions_SOAP-2.7$${BUILD_EXT}* $$DESTDIR/$${TARGET}.app/Contents/Frameworks
#}
