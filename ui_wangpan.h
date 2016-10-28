/********************************************************************************
** Form generated from reading UI file 'wangpan.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WANGPAN_H
#define UI_WANGPAN_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wangpan
{
public:
    QWebView *webView;
    QLineEdit *elementLineEdit;

    void setupUi(QWidget *wangpan)
    {
        if (wangpan->objectName().isEmpty())
            wangpan->setObjectName(QStringLiteral("wangpan"));
        wangpan->resize(1446, 997);
        webView = new QWebView(wangpan);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setGeometry(QRect(40, 100, 1391, 891));
        webView->setUrl(QUrl(QStringLiteral("https://webkit.org/")));
        elementLineEdit = new QLineEdit(wangpan);
        elementLineEdit->setObjectName(QStringLiteral("elementLineEdit"));
        elementLineEdit->setGeometry(QRect(32, 30, 1381, 51));

        retranslateUi(wangpan);

        QMetaObject::connectSlotsByName(wangpan);
    } // setupUi

    void retranslateUi(QWidget *wangpan)
    {
        wangpan->setWindowTitle(QApplication::translate("wangpan", "\347\275\221\347\233\230", 0));
        elementLineEdit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class wangpan: public Ui_wangpan {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WANGPAN_H
