QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# 添加 qt-material-widget 库

#INCLUDEPATH += $$PWD/../qt-material-widgets/components
#DEPENDPATH += $$PWD/../qt-material-widgets/components

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qt-material-widgets/components/release/ -lcomponents
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qt-material-widgets/components/debug/ -lcomponents
#else:unix: LIBS += -L$$OUT_PWD/../qt-material-widgets/components/ -lcomponents

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt-material-widgets/components/release/libcomponents.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt-material-widgets/components/debug/libcomponents.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt-material-widgets/components/release/components.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt-material-widgets/components/debug/components.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qt-material-widgets/components/libcomponents.a


SOURCES += \
    vlcplayer.cpp \
    main.cpp \
    playcontrolpanel.cpp

RESOURCES += \
    res.qrc

FORMS += \
    playcontrolpanel.ui

HEADERS += \
    vlcplayer.hpp \
    playcontrolpanel.hpp


