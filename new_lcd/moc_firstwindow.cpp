/****************************************************************************
** Meta object code from reading C++ file 'firstwindow.h'
**
** Created: Wed May 1 15:58:44 2019
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "firstwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firstwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FirstWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      27,   12,   12,   12, 0x08,
      43,   12,   12,   12, 0x08,
      59,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FirstWindow[] = {
    "FirstWindow\0\0MyTimerSlot()\0update_score1()\0"
    "update_score2()\0on_pushButton_2_clicked()\0"
};

const QMetaObject FirstWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FirstWindow,
      qt_meta_data_FirstWindow, 0 }
};

const QMetaObject *FirstWindow::metaObject() const
{
    return &staticMetaObject;
}

void *FirstWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FirstWindow))
        return static_cast<void*>(const_cast< FirstWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int FirstWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: MyTimerSlot(); break;
        case 1: update_score1(); break;
        case 2: update_score2(); break;
        case 3: on_pushButton_2_clicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
