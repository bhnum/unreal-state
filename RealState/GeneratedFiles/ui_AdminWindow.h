/********************************************************************************
** Form generated from reading UI file 'AdminWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINWINDOW_H
#define UI_ADMINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <mainwindow.h>

QT_BEGIN_NAMESPACE

class Ui_AdminWindow
{
public:

    void setupUi(MainWindow *AdminWindow)
    {
        if (AdminWindow->objectName().isEmpty())
            AdminWindow->setObjectName(QString::fromUtf8("AdminWindow"));
        AdminWindow->resize(400, 300);

        retranslateUi(AdminWindow);

        QMetaObject::connectSlotsByName(AdminWindow);
    } // setupUi

    void retranslateUi(MainWindow *AdminWindow)
    {
        AdminWindow->setWindowTitle(QApplication::translate("AdminWindow", "AdminWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminWindow: public Ui_AdminWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINWINDOW_H
