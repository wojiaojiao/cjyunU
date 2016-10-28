#ifndef WANGPAN_H
#define WANGPAN_H

#include <QUrl>
#include <QWidget>
#include "ui_wangpan.h"

#include <QtNetwork/QtNetwork>//http请求的


//获取当前网页cookie的
#include "networkcookie.h"

//以下是分析返回的json的
#include <QJsonDocument>
#include <QJsonParseError>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

//改u盘名字
#include <windows.h>
#include <ShellAPI.h>

namespace Ui {
class Wangpan;
}

class Wangpan : public QWidget
{
    Q_OBJECT

public:
    explicit Wangpan(QWidget *parent = 0);//
    //Wangpan(QWidget *parent = 0);
    ~Wangpan();//


    void setUrl(const QUrl &url);



private:
    /**
     * @brief 初始化基本信息.(是否有用户账户, 是否自动登陆)
     */
    void init();
    Ui::wangpan *ui;


    QNetworkAccessManager *manager;//是http请求的
    NetworkCookie network_cookie_;//获取当前网页的cookies的


public slots:
    void loginWP();
    void changeIcon();
    void on_webView_loadFinished(bool);
    void create_inf_file();
    bool read_inf_file(QString readwhat);
    void write_inf_file(QString writewhat );
    void before_upload();
    void record_time();
     void get_token(QString url);
    void wpfile_remove(QString ufilepath,bool update_up);
    void re_replyFinish(QNetworkReply * reply);
    


    void upload(QString ufilename,QString ufilepath);
    void up_replyFinish(QNetworkReply * reply);

    void newfolder(int i);
    void nf_replyFinish(QNetworkReply * reply);
    void findFile(QString path);
    void set_param(QString path1,DWORD VolumeSerialNumber);
    void set_param2(int a);
    void set_param3(QString control);
    void information_list(QString udirpath);
    void list_replyFinish(QNetworkReply * reply);
    





};


#endif
