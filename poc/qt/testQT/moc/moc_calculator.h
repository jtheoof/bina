/****************************************************************************
** Meta object code from reading C++ file 'calculator.h'
**
** Created: Sat Jul 21 20:08:30 2012
**      by: The Qt Meta Object Compiler version 67 (Qt 5.0.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../calculator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calculator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Calculator_t {
    QByteArrayData data[16];
    char stringdata[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Calculator_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Calculator_t qt_meta_stringdata_Calculator = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 12),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 20),
QT_MOC_LITERAL(4, 46, 23),
QT_MOC_LITERAL(5, 70, 29),
QT_MOC_LITERAL(6, 100, 12),
QT_MOC_LITERAL(7, 113, 12),
QT_MOC_LITERAL(8, 126, 17),
QT_MOC_LITERAL(9, 144, 16),
QT_MOC_LITERAL(10, 161, 5),
QT_MOC_LITERAL(11, 167, 8),
QT_MOC_LITERAL(12, 176, 11),
QT_MOC_LITERAL(13, 188, 10),
QT_MOC_LITERAL(14, 199, 9),
QT_MOC_LITERAL(15, 209, 11)
    },
    "Calculator\0digitClicked\0\0unaryOperatorClicked\0"
    "additiveOperatorClicked\0"
    "multiplicativeOperatorClicked\0"
    "equalClicked\0pointClicked\0changeSignClicked\0"
    "backspaceClicked\0clear\0clearAll\0"
    "clearMemory\0readMemory\0setMemory\0"
    "addToMemory\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Calculator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08,
       3,    0,   85,    2, 0x08,
       4,    0,   86,    2, 0x08,
       5,    0,   87,    2, 0x08,
       6,    0,   88,    2, 0x08,
       7,    0,   89,    2, 0x08,
       8,    0,   90,    2, 0x08,
       9,    0,   91,    2, 0x08,
      10,    0,   92,    2, 0x08,
      11,    0,   93,    2, 0x08,
      12,    0,   94,    2, 0x08,
      13,    0,   95,    2, 0x08,
      14,    0,   96,    2, 0x08,
      15,    0,   97,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Calculator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Calculator *_t = static_cast<Calculator *>(_o);
        switch (_id) {
        case 0: _t->digitClicked(); break;
        case 1: _t->unaryOperatorClicked(); break;
        case 2: _t->additiveOperatorClicked(); break;
        case 3: _t->multiplicativeOperatorClicked(); break;
        case 4: _t->equalClicked(); break;
        case 5: _t->pointClicked(); break;
        case 6: _t->changeSignClicked(); break;
        case 7: _t->backspaceClicked(); break;
        case 8: _t->clear(); break;
        case 9: _t->clearAll(); break;
        case 10: _t->clearMemory(); break;
        case 11: _t->readMemory(); break;
        case 12: _t->setMemory(); break;
        case 13: _t->addToMemory(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Calculator::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Calculator.data,
      qt_meta_data_Calculator,  qt_static_metacall, 0, 0}
};


const QMetaObject *Calculator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Calculator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Calculator.stringdata))
        return static_cast<void*>(const_cast< Calculator*>(this));
    return QWidget::qt_metacast(_clname);
}

int Calculator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
