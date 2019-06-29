/********************************************************************************
** Form generated from reading UI file 'UsermanWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMANWINDOW_H
#define UI_USERMANWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UsermanWindow
{
public:

    void setupUi(QWidget *UsermanWindow)
    {
        if (UsermanWindow->objectName().isEmpty())
            UsermanWindow->setObjectName(QString::fromUtf8("UsermanWindow"));
        UsermanWindow->resize(400, 300);

        retranslateUi(UsermanWindow);

        QMetaObject::connectSlotsByName(UsermanWindow);
    } // setupUi

    void retranslateUi(QWidget *UsermanWindow)
    {
        UsermanWindow->setWindowTitle(QApplication::translate("UsermanWindow", "UsermanWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UsermanWindow: public Ui_UsermanWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMANWINDOW_H
