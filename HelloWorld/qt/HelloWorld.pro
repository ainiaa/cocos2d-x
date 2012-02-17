QT       += core gui

TARGET = HelloWorld
TEMPLATE = app

CONFIG(debug, debug|release) {
    DESTDIR = ../../Debug.qt
}

CONFIG(release, debug|release) {
    DESTDIR = ../../Release.qt
}

DEFINES += CC_UNDER_QT

LIBS += -L$(DESTDIR) -llibcocos2d

INCLUDEPATH += ../../cocos2dx\platform \
        ../../cocos2dx/include \
        ../../cocos2dx \
        ../../HelloWorld/Classes

HEADERS += \
    ../Classes/HelloWorldScene.h \
    ../Classes/AppDelegate.h

SOURCES += \
    ../Classes/HelloWorldScene.cpp \
    ../Classes/AppDelegate.cpp \
    main.cpp
