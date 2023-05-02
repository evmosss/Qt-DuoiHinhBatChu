/****************************************************************************
** Meta object code from reading C++ file 'room.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../DuoiHinhBatChu_Frontend/room.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'room.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.0. It"
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
struct qt_meta_stringdata_CLASSRoomENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSRoomENDCLASS = QtMocHelpers::stringData(
    "Room",
    "interactError",
    "",
    "message",
    "logOutSuccessfully",
    "handleInteractError",
    "on_createRoom_clicked",
    "on_leaveRoom_clicked",
    "on_logoutButton_clicked",
    "on_joinRoom_clicked",
    "on_submitAnswer_clicked",
    "alertConnected",
    "handleDataFromServer",
    "handleSocketError",
    "QAbstractSocket::SocketError",
    "error",
    "disconnect"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSRoomENDCLASS_t {
    uint offsetsAndSizes[34];
    char stringdata0[5];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[19];
    char stringdata5[20];
    char stringdata6[22];
    char stringdata7[21];
    char stringdata8[24];
    char stringdata9[20];
    char stringdata10[24];
    char stringdata11[15];
    char stringdata12[21];
    char stringdata13[18];
    char stringdata14[29];
    char stringdata15[6];
    char stringdata16[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSRoomENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSRoomENDCLASS_t qt_meta_stringdata_CLASSRoomENDCLASS = {
    {
        QT_MOC_LITERAL(0, 4),  // "Room"
        QT_MOC_LITERAL(5, 13),  // "interactError"
        QT_MOC_LITERAL(19, 0),  // ""
        QT_MOC_LITERAL(20, 7),  // "message"
        QT_MOC_LITERAL(28, 18),  // "logOutSuccessfully"
        QT_MOC_LITERAL(47, 19),  // "handleInteractError"
        QT_MOC_LITERAL(67, 21),  // "on_createRoom_clicked"
        QT_MOC_LITERAL(89, 20),  // "on_leaveRoom_clicked"
        QT_MOC_LITERAL(110, 23),  // "on_logoutButton_clicked"
        QT_MOC_LITERAL(134, 19),  // "on_joinRoom_clicked"
        QT_MOC_LITERAL(154, 23),  // "on_submitAnswer_clicked"
        QT_MOC_LITERAL(178, 14),  // "alertConnected"
        QT_MOC_LITERAL(193, 20),  // "handleDataFromServer"
        QT_MOC_LITERAL(214, 17),  // "handleSocketError"
        QT_MOC_LITERAL(232, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(261, 5),  // "error"
        QT_MOC_LITERAL(267, 10)   // "disconnect"
    },
    "Room",
    "interactError",
    "",
    "message",
    "logOutSuccessfully",
    "handleInteractError",
    "on_createRoom_clicked",
    "on_leaveRoom_clicked",
    "on_logoutButton_clicked",
    "on_joinRoom_clicked",
    "on_submitAnswer_clicked",
    "alertConnected",
    "handleDataFromServer",
    "handleSocketError",
    "QAbstractSocket::SocketError",
    "error",
    "disconnect"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSRoomENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   86,    2, 0x06,    1 /* Public */,
       4,    0,   89,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   90,    2, 0x08,    4 /* Private */,
       6,    0,   93,    2, 0x08,    6 /* Private */,
       7,    0,   94,    2, 0x08,    7 /* Private */,
       8,    0,   95,    2, 0x08,    8 /* Private */,
       9,    0,   96,    2, 0x08,    9 /* Private */,
      10,    0,   97,    2, 0x08,   10 /* Private */,
      11,    0,   98,    2, 0x08,   11 /* Private */,
      12,    0,   99,    2, 0x08,   12 /* Private */,
      13,    1,  100,    2, 0x08,   13 /* Private */,
      16,    0,  103,    2, 0x08,   15 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Room::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSRoomENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSRoomENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSRoomENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Room, std::true_type>,
        // method 'interactError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'logOutSuccessfully'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleInteractError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_createRoom_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_leaveRoom_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_logoutButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_joinRoom_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_submitAnswer_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'alertConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleDataFromServer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleSocketError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>,
        // method 'disconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Room::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Room *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->interactError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->logOutSuccessfully(); break;
        case 2: _t->handleInteractError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->on_createRoom_clicked(); break;
        case 4: _t->on_leaveRoom_clicked(); break;
        case 5: _t->on_logoutButton_clicked(); break;
        case 6: _t->on_joinRoom_clicked(); break;
        case 7: _t->on_submitAnswer_clicked(); break;
        case 8: _t->alertConnected(); break;
        case 9: _t->handleDataFromServer(); break;
        case 10: _t->handleSocketError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 11: _t->disconnect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Room::*)(QString );
            if (_t _q_method = &Room::interactError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Room::*)();
            if (_t _q_method = &Room::logOutSuccessfully; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *Room::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Room::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSRoomENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Room::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Room::interactError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Room::logOutSuccessfully()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
