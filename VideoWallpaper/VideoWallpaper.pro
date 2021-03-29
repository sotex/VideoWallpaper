QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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


# 添加 libvlc 库
libvlc_INC = $$dirname(PWD)/vlc-sdk/include
INCLUDEPATH += $$libvlc_INC
DEPENDPATH += $$libvlc_INC

libvlc_LIB = $$dirname(PWD)/vlc-sdk/lib_x64
libvlc_BIN = $$dirname(PWD)/vlc-sdk/bin_x64
contains(QMAKE_HOST.arch, x86):{
    libvlc_LIB = dirname($$PWD)/vlc-sdk/lib_x86
    libvlc_BIN = dirname($$PWD)/vlc-sdk/bin_x86
}
#message($$libvlc_INC)
#message($$libvlc_LIB)
win32: LIBS += -L$$libvlc_LIB -llibvlc -llibvlccore



win32: LIBS += -lUser32

INCLUDEPATH += $$dirname(PWD)/VideoWallpaper/interactive_buttons
INCLUDEPATH += $$dirname(PWD)/VideoWallpaper/gallery

SOURCES += \
    gallery/galleryphotowidget.cpp \
    gallery/gallerywidget.cpp \
    interactive_buttons/interactivebuttonbase.cpp \
    interactive_buttons/waterzoombutton.cpp \
    vlcplayer.cpp \
    main.cpp \
    playcontrolpanel.cpp \
    wallpaperwindow.cpp

RESOURCES += \
    res.qrc

FORMS += \
    playcontrolpanel.ui

HEADERS += \
    gallery/galleryphotowidget.h \
    gallery/gallerywidget.h \
    interactive_buttons/interactivebuttonbase.h \
    interactive_buttons/waterzoombutton.h \
    vlcplayer.hpp \
    playcontrolpanel.hpp \
    wallpaperwindow.hpp
