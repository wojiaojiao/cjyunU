/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QCheckBox *checkBox;
    QPushButton *pushButton_2;
    QPushButton *pushButton_4;
    QLabel *label_2;
    QCheckBox *checkBox_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(371, 115);
        Widget->setStyleSheet(QStringLiteral(""));
        checkBox = new QCheckBox(Widget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(280, 10, 91, 21));
        checkBox->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        checkBox->setChecked(true);
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(290, 70, 81, 31));
        pushButton_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        pushButton_4 = new QPushButton(Widget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(10, 40, 121, 31));
        pushButton_4->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(160, 30, 101, 41));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        checkBox_2 = new QCheckBox(Widget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(280, 40, 91, 21));
        checkBox_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        checkBox_2->setChecked(true);
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(50, 0, 81, 31));
        pushButton->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        pushButton_3 = new QPushButton(Widget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 80, 121, 31));
        pushButton_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        checkBox->setText(QApplication::translate("Widget", "\350\207\252\345\212\250\344\270\212\344\274\240", 0));
        pushButton_2->setText(QApplication::translate("Widget", "\346\270\205\347\251\272\346\255\244u\347\233\230", 0));
        pushButton_4->setText(QApplication::translate("Widget", "\346\211\223\345\274\200\347\275\221\347\233\230/\346\265\217\350\247\210\345\231\250", 0));
        label_2->setText(QApplication::translate("Widget", " \350\257\267\346\217\222\345\205\245u\347\233\230", 0));
        checkBox_2->setText(QApplication::translate("Widget", "\347\273\221\345\256\232\347\275\221\347\233\230", 0));
        pushButton->setText(QApplication::translate("Widget", "\346\211\223\345\274\200u\347\233\230", 0));
        pushButton_3->setText(QApplication::translate("Widget", "\346\211\223\345\274\200\346\216\210\346\235\203\347\231\273\345\205\245\346\241\206", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
