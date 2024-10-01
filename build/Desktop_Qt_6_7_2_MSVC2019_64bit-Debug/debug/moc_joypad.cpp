/****************************************************************************
** Meta object code from reading C++ file 'joypad.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../JoypadWidget/joypad.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'joypad.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSJoyPadENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSJoyPadENDCLASS = QtMocHelpers::stringData(
    "JoyPad",
    "xChanged",
    "",
    "value",
    "yChanged",
    "setX",
    "setY",
    "removeXAnimation",
    "addXAnimation",
    "removeYAnimation",
    "addYAnimation",
    "setAlignment",
    "Qt::Alignment",
    "f",
    "x",
    "y"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSJoyPadENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       2,   87, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    3 /* Public */,
       4,    1,   71,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   74,    2, 0x0a,    7 /* Public */,
       6,    1,   77,    2, 0x0a,    9 /* Public */,
       7,    0,   80,    2, 0x0a,   11 /* Public */,
       8,    0,   81,    2, 0x0a,   12 /* Public */,
       9,    0,   82,    2, 0x0a,   13 /* Public */,
      10,    0,   83,    2, 0x0a,   14 /* Public */,
      11,    1,   84,    2, 0x0a,   15 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,

 // properties: name, type, flags
      14, QMetaType::Float, 0x00015103, uint(0), 0,
      15, QMetaType::Float, 0x00015103, uint(1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject JoyPad::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSJoyPadENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSJoyPadENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSJoyPadENDCLASS_t,
        // property 'x'
        QtPrivate::TypeAndForceComplete<float, std::true_type>,
        // property 'y'
        QtPrivate::TypeAndForceComplete<float, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<JoyPad, std::true_type>,
        // method 'xChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'yChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'setX'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'setY'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'removeXAnimation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addXAnimation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeYAnimation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addYAnimation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setAlignment'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Qt::Alignment, std::false_type>
    >,
    nullptr
} };

void JoyPad::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<JoyPad *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->xChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 1: _t->yChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 2: _t->setX((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 3: _t->setY((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 4: _t->removeXAnimation(); break;
        case 5: _t->addXAnimation(); break;
        case 6: _t->removeYAnimation(); break;
        case 7: _t->addYAnimation(); break;
        case 8: _t->setAlignment((*reinterpret_cast< std::add_pointer_t<Qt::Alignment>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (JoyPad::*)(float );
            if (_t _q_method = &JoyPad::xChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (JoyPad::*)(float );
            if (_t _q_method = &JoyPad::yChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<JoyPad *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = _t->x(); break;
        case 1: *reinterpret_cast< float*>(_v) = _t->y(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<JoyPad *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setX(*reinterpret_cast< float*>(_v)); break;
        case 1: _t->setY(*reinterpret_cast< float*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *JoyPad::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *JoyPad::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSJoyPadENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int JoyPad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void JoyPad::xChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void JoyPad::yChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
