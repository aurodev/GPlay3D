#--------------------------------------------------------------------
# output directory
#--------------------------------------------------------------------

CONFIG(debug,debug|release){
    DESTDIR = $$PWD/../../BUILD/examples
} else {
    DESTDIR = $$PWD/../../BUILD/examples
}

QMAKE_CLEAN += $$DESTDIR/$$TARGET

#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------
QT -= core gui
TARGET = examples
TEMPLATE = app
CONFIG += c++11
CONFIG -= qt

SOURCES += \
    src/SamplesGame.cpp \
    src/DrawStress.cpp \
    src/DynamicMesh.cpp \
    src/Instancing.cpp \
    src/Example.cpp \
    src/Cube.cpp \
    src/Sponza.cpp \
    src/FirstPersonCamera.cpp \
    src/RenderToTexture.cpp \
    src/ImGuiTest.cpp \
    src/SparkDemo.cpp \
    src/Transparency.cpp \
    src/ShaderReload.cpp \
    src/EventSample.cpp

HEADERS += \
    src/SamplesGame.h \
    src/DrawStress.h \
    src/DynamicMesh.h \
    src/Instancing.h \
    src/Example.h \
    src/Cube.h \
    src/Sponza.h \
    src/FirstPersonCamera.h \
    src/RenderToTexture.h \
    src/ImGuiTest.h \
    src/SparkDemo.h \

INCLUDEPATH += $$PWD/../../gameplay/src
INCLUDEPATH += $$PWD/../../external-deps/include
DEFINES += GP_USE_GAMEPAD

linux: DEFINES += __linux__
linux: QMAKE_CXXFLAGS += -lstdc++ -pthread -w
linux: INCLUDEPATH += /usr/include/gtk-2.0
linux: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
linux: INCLUDEPATH += /usr/include/atk-1.0
linux: INCLUDEPATH += /usr/include/cairo
linux: INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
linux: INCLUDEPATH += /usr/include/pango-1.0
linux: INCLUDEPATH += /usr/include/gio-unix-2.0
linux: INCLUDEPATH += /usr/include/freetype2
linux: INCLUDEPATH += /usr/include/glib-2.0
linux: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
linux: INCLUDEPATH += /usr/include/pixman-1
linux: INCLUDEPATH += /usr/include/libpng12
linux: INCLUDEPATH += /usr/include/harfbuzz
linux: PRE_TARGETDEPS += $$DESTDIR/../libgameplay.a
linux: LIBS += -L$$DESTDIR/../ -lgameplay
linux: LIBS += -L$$PWD/../../external-deps/lib/linux/x86_64/ -lgameplay-deps
linux: LIBS += -lm -lGL -lrt -ldl -lX11 -lpthread -lgtk-x11-2.0 -lglib-2.0 -lgobject-2.0 -lsndio
linux: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/res $$DESTDIR$$escape_expand(\n\t))
linux: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/game.config $$DESTDIR$$escape_expand(\n\t))
linux: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/shaders $$DESTDIR/res$$escape_expand(\n\t))
linux: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/ui $$DESTDIR/res$$escape_expand(\n\t))


macx: QMAKE_CXXFLAGS += -x c++ -stdlib=libc++ -w -arch x86_64
macx: QMAKE_OBJECTIVE_CFLAGS += -x objective-c++ -stdlib=libc++ -w -arch x86_64
macx: LIBS += -L$$PWD/../../gameplay/Debug/ -lgameplay
macx: LIBS += -L$$PWD/../../external-deps/lib/macosx/x86_64/ -lgameplay-deps
macx: LIBS += -F/System/Library/Frameworks -framework GameKit
macx: LIBS += -F/System/Library/Frameworks -framework IOKit
macx: LIBS += -F/System/Library/Frameworks -framework QuartzCore
macx: LIBS += -F/System/Library/Frameworks -framework OpenAL
macx: LIBS += -F/System/Library/Frameworks -framework OpenGL
macx: LIBS += -F/System/Library/Frameworks -framework Cocoa
macx: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/shaders ../res$$escape_expand(\n\t))
macx: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/ui ../res$$escape_expand(\n\t))
macx: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/../../gameplay/res/logo_powered_white.png ../res$$escape_expand(\n\t))
macx 
{
    icon.files = icon.png
    icon.path = Contents/Resources
    QMAKE_BUNDLE_DATA += icon

    gameconfig.files = game.config
    gameconfig.path = Contents/Resources
    QMAKE_BUNDLE_DATA += gameconfig

    res.files = res
    res.path = Contents/Resources
    QMAKE_BUNDLE_DATA += res
}

win32: CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../external-deps/lib/windows/x86_64/Debug/gameplay-deps.lib
win32: CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../external-deps/lib/windows/x86_64/Release/gameplay-deps.lib
win32: DEFINES += WIN32 _WINDOWS _UNICODE UNICODE
win32: LIBS += -L$$DESTDIR\..\ -lgameplay
win32: CONFIG(debug, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Debug/ -lgameplay-deps
win32: CONFIG(release, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Release/ -lgameplay-deps
win32: LIBS += -lopengl32 -lkernel32 -luser32 -lwinmm -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -limm32 -lole32 -loleaut32 -luuid -lversion -lodbc32 -lodbccp32
#win32: LIBS += -L$$(DXSDK_DIR)Lib\x64 -lXInput
win32: INCLUDEPATH += $$(DXSDK_DIR)Include
win32: INCLUDEPATH += $$PWD/../../external-deps/include/compat/msvc
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34100
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34189
win32: QMAKE_CXXFLAGS_WARN_ON -= -w4302
PWD_WIN = $${PWD}
PWD_WIN ~= s,/,\\,g
PWD_DEST_WIN = $$DESTDIR
PWD_DEST_WIN ~= s,/,\\,g
win32: QMAKE_POST_LINK += $$quote(xcopy ..\..\..\gameplay\res\shaders $$PWD_DEST_WIN\res\shaders\* /s /y /d$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(xcopy ..\..\..\gameplay\res\ui $$PWD_DEST_WIN\res\ui\* /s /y /d$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(copy ..\..\..\gameplay\res\logo_powered_white.png $$PWD_DEST_WIN\res$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(xcopy $$PWD_WIN\res $$PWD_DEST_WIN\res\ /s /y /d$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(xcopy $$PWD_WIN\game.config $$PWD_DEST_WIN\game.config* /y /d$$escape_expand(\n\t))
