QT += quick widgets network serialport  charts

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(./DataManager/DataManager.pri)

SOURCES += \
        main.cpp \
        serialmanager.cpp \
        seriespointswaper.cpp \
        serviceprovider.cpp \
        serviceproviderprivate.cpp \
        testpointmodel.cpp \
        timeprovider.cpp \
        uiimageprovider.cpp

RESOURCES += qml.qrc

DESTDIR=./build


unix: {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lglog

    INCLUDEPATH+=/home/h/privateDev/boost1.82
}

win32: {
    INCLUDEPATH += $$PWD/lib/glog/win32/include
    INCLUDEPATH += D:/LIBARAY/boost_1_82_0
    LIBS += -L$$PWD/lib/glog/win32/lib -lglog
    LIBS += -L$$PWD/lib/glog/win32/bin -lglog
}
message("qmake arch:"$$QMAKE_TARGET.arch)
LIBS += -L$$PWD/lib/glog/arm/lib -lglog
LIBS += -L$$PWD/lib/glog/arm/bin -lglog
contains(QMAKE_TARGET.arch, arm): {
INCLUDEPATH += $$PWD/lib/glog/arm/include
LIBS += -L$$PWD/lib/glog/arm/lib -lglog
LIBS += -L$$PWD/lib/glog/arm/bin -lglog
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =.

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

CONFIG += qmltypes
QML_IMPORT_NAME = CppCore
QML_IMPORT_MAJOR_VERSION = 1

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QTPLUGIN+=qlinuxfb

HEADERS += \
    PublicDefs.hpp \
    serialmanager.hpp \
    seriespointswaper.hpp \
    serviceprovider.hpp \
    serviceproviderprivate.hpp \
    testpointmodel.hpp \
    timeprovider.hpp \
    uiimageprovider.h
