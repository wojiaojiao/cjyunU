#ifndef WENJIAN_H
#define WENJIAN_H

#include <QUrl>
#include <QWidget>
#include "ui_wangpan.h"



class wenjian : public QWidget, private Ui::wangpan
{
    Q_OBJECT

public:
    wenjian(QWidget *parent = 0);
    void setUrl(const QUrl &url);

    float size;//


public slots:

    void findFile(QString path);//

};


#endif
