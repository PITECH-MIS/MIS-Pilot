#
# Copyright (c) 2015-2017 Alex Spataru <alex_spataru@outlook.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

include ($$PWD/SDL/SDL.pri)

QT += gui
QT += core
QT += widgets

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/QJoysticks.h \
    $$PWD/JoysticksCommon.h \
    $$PWD/SDL_Joysticks.h \
    $$PWD/VirtualJoystick.h \
    $$PWD/Android_Joystick.h

SOURCES += \
    $$PWD/QJoysticks.cpp \
    $$PWD/SDL_Joysticks.cpp \
    $$PWD/VirtualJoystick.cpp \
    $$PWD/Android_Joystick.cpp

RESOURCES += \
    $$PWD/qjoysticks-res.qrc

