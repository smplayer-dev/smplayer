/****************************************************************************
** Meta object code from reading C++ file 'filechooser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../filechooser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filechooser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FileChooser_t {
    QByteArrayData data[17];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FileChooser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FileChooser_t qt_meta_stringdata_FileChooser = {
    {
QT_MOC_LITERAL(0, 0, 11), // "FileChooser"
QT_MOC_LITERAL(1, 12, 11), // "fileChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "file"
QT_MOC_LITERAL(4, 30, 10), // "setCaption"
QT_MOC_LITERAL(5, 41, 7), // "caption"
QT_MOC_LITERAL(6, 49, 9), // "setFilter"
QT_MOC_LITERAL(7, 59, 6), // "filter"
QT_MOC_LITERAL(8, 66, 13), // "setDialogType"
QT_MOC_LITERAL(9, 80, 10), // "DialogType"
QT_MOC_LITERAL(10, 91, 4), // "type"
QT_MOC_LITERAL(11, 96, 10), // "setOptions"
QT_MOC_LITERAL(12, 107, 20), // "QFileDialog::Options"
QT_MOC_LITERAL(13, 128, 7), // "options"
QT_MOC_LITERAL(14, 136, 14), // "openFileDialog"
QT_MOC_LITERAL(15, 151, 4), // "text"
QT_MOC_LITERAL(16, 156, 10) // "dialogType"

    },
    "FileChooser\0fileChanged\0\0file\0setCaption\0"
    "caption\0setFilter\0filter\0setDialogType\0"
    "DialogType\0type\0setOptions\0"
    "QFileDialog::Options\0options\0"
    "openFileDialog\0text\0dialogType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FileChooser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       5,   60, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   47,    2, 0x0a /* Public */,
       6,    1,   50,    2, 0x0a /* Public */,
       8,    1,   53,    2, 0x0a /* Public */,
      11,    1,   56,    2, 0x0a /* Public */,
      14,    0,   59,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,

 // properties: name, type, flags
      15, QMetaType::QString, 0x00095103,
       5, QMetaType::QString, 0x00095103,
       7, QMetaType::QString, 0x00095103,
      16, 0x80000000 | 9, 0x0009510b,
      13, 0x80000000 | 12, 0x0009510b,

       0        // eod
};

void FileChooser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FileChooser *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->fileChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setCaption((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->setFilter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->setDialogType((*reinterpret_cast< DialogType(*)>(_a[1]))); break;
        case 4: _t->setOptions((*reinterpret_cast< QFileDialog::Options(*)>(_a[1]))); break;
        case 5: _t->openFileDialog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FileChooser::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileChooser::fileChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<FileChooser *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->text(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->caption(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->filter(); break;
        case 3: *reinterpret_cast< DialogType*>(_v) = _t->dialogType(); break;
        case 4: *reinterpret_cast< QFileDialog::Options*>(_v) = _t->options(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<FileChooser *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setText(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setCaption(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setFilter(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setDialogType(*reinterpret_cast< DialogType*>(_v)); break;
        case 4: _t->setOptions(*reinterpret_cast< QFileDialog::Options*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

static const QMetaObject::SuperData qt_meta_extradata_FileChooser[] = {
    QMetaObject::SuperData::link<QFileDialog::staticMetaObject>(),
    nullptr
};

QT_INIT_METAOBJECT const QMetaObject FileChooser::staticMetaObject = { {
    QMetaObject::SuperData::link<LineEditWithIcon::staticMetaObject>(),
    qt_meta_stringdata_FileChooser.data,
    qt_meta_data_FileChooser,
    qt_static_metacall,
    qt_meta_extradata_FileChooser,
    nullptr
} };


const QMetaObject *FileChooser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileChooser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FileChooser.stringdata0))
        return static_cast<void*>(this);
    return LineEditWithIcon::qt_metacast(_clname);
}

int FileChooser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LineEditWithIcon::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void FileChooser::fileChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
