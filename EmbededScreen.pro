QT += quick widgets network serialport  charts sql

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG+=c++17

#1:字号
#2：时间到每个页面上
#3：左右顺序交换
#4：中英文热切换
#5：确认流程
#6 对齐

INCLUDEPATH+=./DataManager#subdirs

SOURCES += \
        DataManager/dataexporter.cpp \
        DataManager/datamanager.cpp \
        DataManager/instructgenerator.cpp \
        DataManager/modelmanagemodel.cpp \
        devicemanager.cpp \
        main.cpp \
        modelinfo.cpp \
        serialmanager.cpp \
        seriespointswaper.cpp \
        serviceprovider.cpp \
        serviceproviderprivate.cpp \
        testpointmodel.cpp \
        timeprovider.cpp

RESOURCES += qml.qrc




##in order to build:glog, boost(VERSION>=1.82)

unix: {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lglog

    INCLUDEPATH+=/home/h/privateDev/boost1.82
    TR_EXCLUDE +=  /home/h/privateDev/boost1.82/*
}

win32: {
    INCLUDEPATH += $$PWD/lib/glog/win32/include
    INCLUDEPATH += D:/LIBARAY/boost_1_82_0
    TR_EXCLUDE +=  D:/LIBARAY/boost_1_82_0/*
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



TRANSLATIONS += En_Us.ts
TRANSLATIONS += Zh_CN.ts
#CONFIG += lrelease
CONFIG += embed_translations
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QTPLUGIN+=qlinuxfb

HEADERS += \
    DataManager/ProjectInfoObject.hpp \
    DataManager/dataexporter.hpp \
    DataManager/datamanager.hpp \
    DataManager/instructgenerator.hpp \
    DataManager/modelmanagemodel.hpp \
    PublicDefs.hpp \
    devicemanager.hpp \
    modelinfo.hpp \
    serialmanager.hpp \
    seriespointswaper.hpp \
    serviceprovider.hpp \
    serviceproviderprivate.hpp \
    testpointmodel.hpp \
    timeprovider.hpp

DISTFILES += \
    En_Us.ts \
    Zh_CN.ts
