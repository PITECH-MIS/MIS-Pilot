/****************************************************************************
** Meta object code from reading C++ file 'controllerwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../controllerwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controllerwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSControllerWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSControllerWindowENDCLASS = QtMocHelpers::stringData(
    "ControllerWindow",
    "debugMessage",
    "",
    "msg",
    "infoMessage",
    "errorMessage",
    "closed",
    "onPOVEvent",
    "QJoystickPOVEvent",
    "event",
    "onAxisEvent",
    "QJoystickAxisEvent",
    "onButtonEvent",
    "QJoystickButtonEvent",
    "onClickDebugEnable",
    "onSelectDebugMode",
    "onSendDebugAction",
    "onSelectDebugMotor",
    "showWindow",
    "controlLoop"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSControllerWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   92,    2, 0x06,    1 /* Public */,
       4,    1,   95,    2, 0x06,    3 /* Public */,
       5,    1,   98,    2, 0x06,    5 /* Public */,
       6,    0,  101,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    1,  102,    2, 0x0a,    8 /* Public */,
      10,    1,  105,    2, 0x0a,   10 /* Public */,
      12,    1,  108,    2, 0x0a,   12 /* Public */,
      14,    0,  111,    2, 0x0a,   14 /* Public */,
      15,    0,  112,    2, 0x0a,   15 /* Public */,
      16,    0,  113,    2, 0x0a,   16 /* Public */,
      17,    0,  114,    2, 0x0a,   17 /* Public */,
      18,    0,  115,    2, 0x0a,   18 /* Public */,
      19,    0,  116,    2, 0x0a,   19 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,    9,
    QMetaType::Void, 0x80000000 | 13,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ControllerWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSControllerWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSControllerWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSControllerWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ControllerWindow, std::true_type>,
        // method 'debugMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'infoMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'errorMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'closed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPOVEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJoystickPOVEvent &, std::false_type>,
        // method 'onAxisEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJoystickAxisEvent &, std::false_type>,
        // method 'onButtonEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJoystickButtonEvent &, std::false_type>,
        // method 'onClickDebugEnable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSelectDebugMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSendDebugAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSelectDebugMotor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'controlLoop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ControllerWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControllerWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->debugMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->infoMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->errorMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->closed(); break;
        case 4: _t->onPOVEvent((*reinterpret_cast< std::add_pointer_t<QJoystickPOVEvent>>(_a[1]))); break;
        case 5: _t->onAxisEvent((*reinterpret_cast< std::add_pointer_t<QJoystickAxisEvent>>(_a[1]))); break;
        case 6: _t->onButtonEvent((*reinterpret_cast< std::add_pointer_t<QJoystickButtonEvent>>(_a[1]))); break;
        case 7: _t->onClickDebugEnable(); break;
        case 8: _t->onSelectDebugMode(); break;
        case 9: _t->onSendDebugAction(); break;
        case 10: _t->onSelectDebugMotor(); break;
        case 11: _t->showWindow(); break;
        case 12: _t->controlLoop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ControllerWindow::*)(QString );
            if (_t _q_method = &ControllerWindow::debugMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ControllerWindow::*)(QString );
            if (_t _q_method = &ControllerWindow::infoMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ControllerWindow::*)(QString );
            if (_t _q_method = &ControllerWindow::errorMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ControllerWindow::*)();
            if (_t _q_method = &ControllerWindow::closed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *ControllerWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControllerWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSControllerWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ControllerWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ControllerWindow::debugMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControllerWindow::infoMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ControllerWindow::errorMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ControllerWindow::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
