/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLineEdit *userInput;
    QLineEdit *passInput;
    QLabel *message;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(800, 600);
        loginButton = new QPushButton(Login);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(290, 370, 201, 41));
        registerButton = new QPushButton(Login);
        registerButton->setObjectName("registerButton");
        registerButton->setGeometry(QRect(290, 420, 201, 41));
        userInput = new QLineEdit(Login);
        userInput->setObjectName("userInput");
        userInput->setGeometry(QRect(200, 210, 391, 41));
        passInput = new QLineEdit(Login);
        passInput->setObjectName("passInput");
        passInput->setGeometry(QRect(200, 270, 391, 41));
        message = new QLabel(Login);
        message->setObjectName("message");
        message->setGeometry(QRect(200, 330, 391, 20));

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Form", nullptr));
        loginButton->setText(QCoreApplication::translate("Login", "Login", nullptr));
        registerButton->setText(QCoreApplication::translate("Login", "Register", nullptr));
        userInput->setPlaceholderText(QCoreApplication::translate("Login", "Username", nullptr));
        passInput->setPlaceholderText(QCoreApplication::translate("Login", "Password", nullptr));
        message->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
