QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

soem += $$PWD/SOEM
qjoysticks += $$PWD/QJoysticks

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += __STDC_LIMIT_MACROS

INCLUDEPATH += \
    $${soem}/Inc\
    $${soem}/wpcap/Include \
    $${soem}/wpcap/Include/pcap \
    $${qjoysticks}/ \

LIBS += \
    $${soem}/lib/soem.lib \
    $${soem}/lib/wpcap/Lib/x64/Packet.lib \
    $${soem}/lib/wpcap/Lib/x64/wpcap.lib \

SOURCES += \
    $$PWD/JoypadWidget/joypad.cpp \
    ControllerWindow.cpp \
    ECATWrapper.cpp \
    Kinematics/KinematicsAtan2.cpp \
    Kinematics/KinematicsBase.cpp \
    MainWindow.cpp \
    MotorDebugger.cpp \
    RobotArm/RobotArmWrapper.cpp \
    iFOC/Actuator3DoF.cpp \
    iFOC/Device.cpp \
    iFOC/Equipment6DoF.cpp \
    iFOC/Motor.cpp \
    main.cpp \
    utils.cpp

HEADERS += \
    $$PWD/JoypadWidget/joypad.h \
    ControllerWindow.h \
    ECATWrapper.h \
    Kinematics/KinematicsAtan2.h \
    Kinematics/KinematicsBase.h \
    MainWindow.h \
    MotorDebugger.h \
    RobotArm/RobotArmWrapper.h \
    iFOC/Actuator3DoF.h \
    iFOC/Device.h \
    iFOC/Equipment6DoF.h \
    iFOC/Motor.h \
    utils.h \
    utypes.h

FORMS += \
    ControllerWindow.ui \
    MainWindow.ui \
    MotorDebugger.ui

QT += xml
QT += core
QT += concurrent

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/soem/wpcap/Lib/x64/ -lwpcap
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/soem/wpcap/Lib/x64/wpcap.lib

include ($${qjoysticks}/QJoysticks.pri)
