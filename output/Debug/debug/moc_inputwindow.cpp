/****************************************************************************
** Meta object code from reading C++ file 'inputwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../windows/inputwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inputwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_InputGraphWindow_t {
    QByteArrayData data[11];
    char stringdata0[123];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InputGraphWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InputGraphWindow_t qt_meta_stringdata_InputGraphWindow = {
    {
QT_MOC_LITERAL(0, 0, 16), // "InputGraphWindow"
QT_MOC_LITERAL(1, 17, 13), // "sig_inputPara"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 4), // "name"
QT_MOC_LITERAL(4, 37, 5), // "index"
QT_MOC_LITERAL(5, 43, 4), // "mark"
QT_MOC_LITERAL(6, 48, 4), // "type"
QT_MOC_LITERAL(7, 53, 22), // "on_m_nameCom_triggered"
QT_MOC_LITERAL(8, 76, 3), // "idx"
QT_MOC_LITERAL(9, 80, 19), // "on_yesBtn_triggered"
QT_MOC_LITERAL(10, 100, 22) // "on_cancelBtn_triggered"

    },
    "InputGraphWindow\0sig_inputPara\0\0name\0"
    "index\0mark\0type\0on_m_nameCom_triggered\0"
    "idx\0on_yesBtn_triggered\0on_cancelBtn_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InputGraphWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   43,    2, 0x08 /* Private */,
       9,    0,   46,    2, 0x08 /* Private */,
      10,    0,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::Int,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void InputGraphWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InputGraphWindow *_t = static_cast<InputGraphWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_inputPara((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->on_m_nameCom_triggered((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->on_yesBtn_triggered(); break;
        case 3: _t->on_cancelBtn_triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (InputGraphWindow::*_t)(QString , int , QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InputGraphWindow::sig_inputPara)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject InputGraphWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_InputGraphWindow.data,
      qt_meta_data_InputGraphWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *InputGraphWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InputGraphWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_InputGraphWindow.stringdata0))
        return static_cast<void*>(const_cast< InputGraphWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int InputGraphWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void InputGraphWindow::sig_inputPara(QString _t1, int _t2, QString _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_InputInstsWindow_t {
    QByteArrayData data[4];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InputInstsWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InputInstsWindow_t qt_meta_stringdata_InputInstsWindow = {
    {
QT_MOC_LITERAL(0, 0, 16), // "InputInstsWindow"
QT_MOC_LITERAL(1, 17, 19), // "on_okBtn_triggerred"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 23) // "on_cancelBtn_triggerred"

    },
    "InputInstsWindow\0on_okBtn_triggerred\0"
    "\0on_cancelBtn_triggerred"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InputInstsWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void InputInstsWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InputInstsWindow *_t = static_cast<InputInstsWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_okBtn_triggerred(); break;
        case 1: _t->on_cancelBtn_triggerred(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject InputInstsWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_InputInstsWindow.data,
      qt_meta_data_InputInstsWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *InputInstsWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InputInstsWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_InputInstsWindow.stringdata0))
        return static_cast<void*>(const_cast< InputInstsWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int InputInstsWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
