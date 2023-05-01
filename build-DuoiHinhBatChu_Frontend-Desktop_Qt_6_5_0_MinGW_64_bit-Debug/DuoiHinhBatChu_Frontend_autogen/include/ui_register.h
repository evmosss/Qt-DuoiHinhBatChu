/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QPushButton *registerButton;
    QLineEdit *userInput;
    QLineEdit *passInput;
    QLabel *message;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName("Register");
        Register->resize(800, 600);
        registerButton = new QPushButton(Register);
        registerButton->setObjectName("registerButton");
        registerButton->setGeometry(QRect(290, 380, 201, 41));
        userInput = new QLineEdit(Register);
        userInput->setObjectName("userInput");
        userInput->setGeometry(QRect(200, 210, 391, 41));
        passInput = new QLineEdit(Register);
        passInput->setObjectName("passInput");
        passInput->setGeometry(QRect(200, 280, 391, 41));
        passInput->setInputMask(QString::fromUtf8(""));
        message = new QLabel(Register);
        message->setObjectName("message");
        message->setGeometry(QRect(200, 340, 391, 20));

        retranslateUi(Register);

        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QCoreApplication::translate("Register", "Form", nullptr));
        registerButton->setText(QCoreApplication::translate("Register", "Register", nullptr));
        userInput->setPlaceholderText(QCoreApplication::translate("Register", "Username", nullptr));
        passInput->setPlaceholderText(QCoreApplication::translate("Register", "Password", nullptr));
        message->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
