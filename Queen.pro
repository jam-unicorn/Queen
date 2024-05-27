# 设置项目类型
TEMPLATE = app

# 项目名称
TARGET = queen

# 使用的 Qt 模块
QT += widgets

# 设置 C++ 标准
CONFIG += c++17

# 源文件
SOURCES += \
    main.cpp \
    QueenDialog.cpp \
    Hook.cpp \
    TrayIcon.cpp

# 头文件
HEADERS += \
    QueenDialog.h \
    Hook.h \
    TrayIcon.h

# UI 文件
FORMS += queendialog.ui

# 资源文件
RESOURCES += icon.qrc
LIBS += -lgdi32

# 设置图标
RC_ICONS = deepl.ico

# 针对不同的 Qt 版本，设置不同的属性
greaterThan(QT_MAJOR_VERSION, 5): {
    QMAKE_LIBS += -ltiff -lmng.dll -ljpeg -ljbig -ldeflate -lzstd -llerc -llzma -lgraphite2 -lbz2 -lusp10 -lRpcrt4 -lsharpyuv

    # 设置 WIN32_EXECUTABLE 属性
    win32:CONFIG(release, debug|release): LIBS += -Wl,-subsystem,windows
    else:win32:CONFIG(debug, debug|release): LIBS += -Wl,-subsystem,console
}
