/****************************************************************************
** Meta object code from reading C++ file 'XVideoUI.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../XVideoUI.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'XVideoUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_XVideoUI_t {
    QByteArrayData data[12];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_XVideoUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_XVideoUI_t qt_meta_stringdata_XVideoUI = {
    {
QT_MOC_LITERAL(0, 0, 8), // "XVideoUI"
QT_MOC_LITERAL(1, 9, 4), // "Open"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 11), // "SliderPress"
QT_MOC_LITERAL(4, 27, 13), // "SliderRelease"
QT_MOC_LITERAL(5, 41, 6), // "SetPos"
QT_MOC_LITERAL(6, 48, 3), // "Set"
QT_MOC_LITERAL(7, 52, 12), // "BeginProcess"
QT_MOC_LITERAL(8, 65, 9), // "ExportEnd"
QT_MOC_LITERAL(9, 75, 11), // "std::string"
QT_MOC_LITERAL(10, 87, 4), // "file"
QT_MOC_LITERAL(11, 92, 15) // "ExportEndSplice"

    },
    "XVideoUI\0Open\0\0SliderPress\0SliderRelease\0"
    "SetPos\0Set\0BeginProcess\0ExportEnd\0"
    "std::string\0file\0ExportEndSplice"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_XVideoUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    1,   57,    2, 0x0a /* Public */,
       6,    0,   60,    2, 0x0a /* Public */,
       7,    0,   61,    2, 0x0a /* Public */,
       8,    1,   62,    2, 0x0a /* Public */,
      11,    1,   65,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void XVideoUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        XVideoUI *_t = static_cast<XVideoUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Open(); break;
        case 1: _t->SliderPress(); break;
        case 2: _t->SliderRelease(); break;
        case 3: _t->SetPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->Set(); break;
        case 5: _t->BeginProcess(); break;
        case 6: _t->ExportEnd((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 7: _t->ExportEndSplice((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject XVideoUI::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_XVideoUI.data,
      qt_meta_data_XVideoUI,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *XVideoUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *XVideoUI::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_XVideoUI.stringdata0))
        return static_cast<void*>(const_cast< XVideoUI*>(this));
    return QWidget::qt_metacast(_clname);
}

int XVideoUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
