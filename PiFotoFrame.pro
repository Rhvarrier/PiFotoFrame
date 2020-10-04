QT += quick core network

CONFIG += c++17
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += src/
SOURCES += \
        src/Globals.cpp \
        src/Photo.cpp \
        src/Weather.cpp \
        src/main.cpp

RESOURCES += res/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =


install_config.path = /home/dietpi/QtProjects/$${TARGET}/bin/
install_config.files = Data/*


INSTALLS += \
    install_config \

# Default rules for deployment.
target.path = /home/dietpi/QtProjects/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/Globals.hpp \
    src/Photo.hpp \
    src/Weather.hpp

DISTFILES += \
    res/main.qml \
