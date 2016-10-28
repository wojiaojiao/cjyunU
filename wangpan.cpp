#include <QWebElement>
#include <QWebFrame>
#include "wangpan.h"

//以下是文件读写的头函数
#include<QFileInfo>
#include<QTextCodec>
#include<QStringList>
#include<QDateTime>
#include<QDebug>
#include <QTimer>
//遍历文件夹的
#include <QFileInfo>
#include <QDir>
//弹窗
#include <QMessageBox>

//改u盘名字
#include <windows.h>
#include <ShellAPI.h>


QByteArray account;
QByteArray password;
QVariant var;//cookies的
QString cookieStr;

bool AutoLogin = false;//1是自动登入
int alreadyL=0;
int logintimes=0;//调用loginWP()登入函数的次数
int autoLoginCount = 6;//自动登入函数的次数，至多6遍，其实只要三遍js就生效了

bool nf_success=false;//在网盘上新建文件有错误
bool exist_inf=false;//存不存在用户信息



QString wpfixedpath="/apps/qingbiji";
QString filter_up;//过滤掉F:
QString upath;//u盘在本地的路径

int unumber=1;//目前插入u盘的数量

bool if_remove=true;//这个是判断之前有没有这个u盘的信息用的

//当前插入u盘的所有信息
QString UID;//u盘的序列号
QString uname;//U盘以及在网盘里建文件夹的名字
QString last_utime;//u盘的上次更新时间
QString update_utime;//目前插入u盘的更新时间，用到写入信息那里的
//
QString cf_name;//当前遍历到文件的名字
QString cf_path;//当前遍历到文件的路径

QString access_token;
QString Referer_url="http://www.qingbiji.cn/";

int t_pos;//往信息文件里写入时间的位置

bool autoup=true;//自动上传，若要取消自动上传，请在前面的设置按钮中设置，用来控制beforeupload函数的

bool inferro=false;//读取用户文件错误，一般是不存在


Wangpan::Wangpan(QWidget *parent):
      QWidget(parent),
      ui(new Ui::wangpan),
    network_cookie_(this)

{
    ui->setupUi(this);
    QApplication::setQuitOnLastWindowClosed(false);
    setWindowFlags(Qt::Tool);
    //下面是显示图标的
    ui->webView->settings()->setIconDatabasePath("./myico.ico");
    setWindowTitle("授权登入框");
    //QObject::connect(ui->webView,SIGNAL(iconChanged()), this, SLOT(changeIcon()));


    QWebSettings *websettings= QWebSettings::globalSettings();//登入百度云的页面加载不出来用这个
    websettings->setAttribute(QWebSettings::LocalStorageEnabled, true);//登入百度云的页面加载不出来用这个，因为百度云用了html5的本地存储特性，所以qt默认显示不出来。


    init();//一开始就先判断一下本地是否有用户信息

    //下面是获取当前网页的cookie
    ui->webView->page()->networkAccessManager()->setCookieJar(&network_cookie_);//这个一定要放构造函数里呀！


}

Wangpan::~Wangpan()
{
   delete ui;
}


void Wangpan::init()
{


    qDebug()<<"先判断一下本地是否有用户信息";
    exist_inf=QFile::exists("information.txt");
    qDebug()<<"information.txt exists:"<<exist_inf;//存在是ture对应1

    // 如果存在用戶信息, 就將用戶信息读取出来, 并设置自动登陆标志为true, 否则不自动登陆.
    // 其实可以直接通过account和password是否为空来判断是否执行自动登陆
    if( QFile::exists("information.txt")==1)
    {
        read_inf_file("l");//读取用户名和密码，还有一种情况是用户名密码为空呢，也自动登入不了，或者错误，反正得来获取错误
        AutoLogin = true;
    }
    else
    {
        //不存在用户信息说明这有可能是一个新用户，必须加以指导使用
         //QApplication::setQuitOnLastWindowClosed(false);//防止一关对话框就把整个程序给关了
          QMessageBox::information(this, tr("您还未绑定网盘"), tr("1.必须在”授权登入框“中进行登入绑定\n2.”授权登入框“中输入您百度云的用户名和密码\n3.如果没有百度云账号，请注册。点击“打开网盘\浏览器”按钮\n4.右下角有超级云的小图标，右键菜单有说明"));
        AutoLogin = false;
    }
}


void Wangpan::set_param(QString path1,DWORD VolumeSerialNumber)
{
    qDebug()<<"进入set_param函数";
    upath=path1;
    UID=QString::number(VolumeSerialNumber);
}

void Wangpan::set_param2(int a)
{
    qDebug()<<"进入set_param2函数";
   alreadyL=a;
}

void Wangpan::set_param3(QString control)
{
    qDebug()<<"进入set_param3函数";
    if(control=="noautoup")
    {
        qDebug()<<"设置autoup=false";
        autoup=false;//如果前面都设置了不自动上传的话，那么是直接不登入网盘
    }
    if(control=="autoup")
    {
        qDebug()<<"设置autoup=true";
        autoup=true;
    }
    if(control=="noautobind")
    {
        qDebug()<<"noautobind,删除用户信息文件，这样只能启动手动登入模式";
        //删除原来的u盘用户信息，这样就会导致手动登入
        QFile::remove("information.txt");
    }
}

void Wangpan::loginWP()
{
    qDebug()<<"正在调用登入网盘的函数";
    logintimes++;
    qDebug()<<"调用登入函数的次数"<<logintimes;
    autoLoginCount --;


    if(AutoLogin)//自动登入
    {
        qDebug()<<"正在自动登入程序";
        QString login_account;
        login_account = QString(account);
        //修改的地方
        //QString str1 = QString("document.getElementById('TANGRAM__PSP_4__userName').value= '%1';").arg(login_account);
        QString str1 = QString("document.getElementById('TANGRAM_3__userName').value= '%1';").arg(login_account);
        ui->webView->page()->mainFrame()->evaluateJavaScript(str1);

        QString login_password;
        login_password = QString(password);
        //修改的地方
        //QString str2 = QString("document.getElementById('TANGRAM__PSP_4__password').value= '%1';").arg(login_password);
        QString str2 = QString("document.getElementById('TANGRAM_3__password').value= '%1';").arg(login_password);
        ui->webView->page()->mainFrame()->evaluateJavaScript(str2);//执行登入函数三遍才能有效

         //修改的地方
        //ui->webView->page()->mainFrame()->findFirstElement("#TANGRAM__PSP_4__submit").evaluateJavaScript("this.click();");//这个应该用在上面两句话都起效了？才不会每次都有验证码
        ui->webView->page()->mainFrame()->findFirstElement("#TANGRAM_3__submit").evaluateJavaScript("this.click();");


    }

    else//手动登入
    {
        qDebug()<<"正在手动登入程序";

        //修改的地方
        //account=ui->webView->page()->mainFrame()->findFirstElement("#TANGRAM__PSP_4__userName").evaluateJavaScript("this.value").toByteArray();//第一次登入，获取用户名
        account=ui->webView->page()->mainFrame()->findFirstElement("#TANGRAM_3__userName").evaluateJavaScript("this.value").toByteArray();
        qDebug()<<"account:"<<account;

        //修改的地方
        //password=ui->webView->page()->mainFrame()->findFirstElement("#TANGRAM__PSP_4__password").evaluateJavaScript("this.value").toByteArray();//第一次登入，获取密码
        password=ui->webView->page()->mainFrame()->findFirstElement("#TANGRAM_3__password").evaluateJavaScript("this.value").toByteArray();
        qDebug()<<"password:"<<password;

    }


}

void Wangpan::on_webView_loadFinished(bool)//调用一遍loginWP()，会执行一遍on_webView_loadFinished？
{

    QString url=ui->webView->url().toString();
    qDebug()<<"完成加载的网址"<<url;

    ui->elementLineEdit->setText(url);//这个后期改善，因为网址太长，框里显示不完的话只要显示前面的就够了


    //匹配登入成功后的网址,只要不是这个网址都再登入
    QString pattern("http://openapi.baidu.com/oauth/2.0/login_success([^%]+)");//遇到这个符号& 就不匹配了
    QRegExp rx(pattern);
    int pos =url.indexOf(rx);
          qDebug() << pos;
          if ( pos >= 0 )                       //-1代表没匹配
          {
            qDebug()<<"登录成功";
            //修改的地方
            //获取access_token
            get_token(url);
            if(AutoLogin==0)//不需要每次登入成功都产生一个本地文件，只有是手动程序的话
            {
                create_inf_file();//要在登入成功后才能保存用户信息到本地文件，相当于验证信息的正确性，
            }
            this->hide();
            before_upload();


          }

          else
          {
              if (autoLoginCount > 0)
              {

                  qDebug()<<"autoLoginCount77777777777777777777777777777777777777777777777777"<<autoLoginCount;
                  qDebug()<<"是自动登入程序登入网盘的函数222222222222222222222222";
                  QTimer::singleShot(1000,this,SLOT(loginWP()));//只运行一次的定时器
              }
          }
}


//修改增加的地方
//获取access_token
void Wangpan::get_token(QString url)
{
    //匹配access_token

    QString pattern("access_token=([^&]+)");//遇到这个符号& 就不匹配了
    QRegExp rx(pattern);
    int pos =url.indexOf(rx);
          qDebug() << pos;
          if ( pos >= 0 )                       //-1代表没匹配
          {
             access_token=rx.cap(1);

          }


    qDebug()<<"access_token"<<access_token;



}
//这个用户信息文件在后期将弄成隐藏文件并且不可读和写
void Wangpan::create_inf_file()//创建用户登入信息文件，保存用户名和密码到本地,
{
    qDebug()<<"创建用户登入信息文件，保存用户名和密码到本地";
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    //以只写方式打开，如果文件不存在，那么会创建该文件
    QFile file("information.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug()<<file.errorString();
    QByteArray a = account.toBase64();
    QByteArray p = password.toBase64();


    file.write("account=super");
    file.write("\n");
    file.write(a);
    file.write("\n");
    qDebug()<<"a:"<<a;


    file.write("password=cloud");
    file.write("\n");
    file.write(p);
    file.write("\n");//回车非常重要，直接影响到读数据的准确性
    qDebug()<<"p:"<<p;

    file.close();

    //获取文件信息
    QFileInfo info(file);
    qDebug()<<QObject::tr("绝对路径:")<<info.absoluteFilePath()<<endl
           <<QObject::tr("文件名:")<<info.fileName()<<endl
          <<QObject::tr("基本名称:")<<info.baseName()<<endl
         <<QObject::tr("后缀:")<<info.suffix()<<endl
        <<QObject::tr("创建时间:")<<info.created()<<endl
       <<QObject::tr("大小:")<<info.size();
}

void Wangpan::before_upload()
{
    qDebug()<<"进入before_upload()函数";
    if(autoup)
    {
if(!alreadyL)
{
    //获取当前网页的cookies
    qDebug() << "获取当前网页的cookies";
    ui->webView->page()->networkAccessManager()->setCookieJar(&network_cookie_);
    QList<QNetworkCookie> cookies = network_cookie_.getCookies();
    qDebug()<<"cookies:"<<cookies;
    var.setValue(cookies);
    //以下是将QList<QNetworkCookie>转化为QString类型
    for (int i = 0; i < cookies.count(); ++i)
        cookieStr.append(cookies.at(i).toRawForm());


    //
}
    qDebug() << "过滤掉F:\\\\的那两\\\\";
    qDebug() << "upath:"<<upath;

    QString pattern("(.*)\\\\");
    QRegExp rx(pattern);
    int pos =upath.indexOf(rx);
          qDebug() << pos;
          if ( pos >= 0 )
          {
              filter_up=rx.cap(1);
              qDebug() << "filter_up"<<filter_up;
          }



    if(!read_inf_file("u"))
    {
        //还有一种情况是没有用户信息文件的
        if(!inferro)
        {
        qDebug()<<"没有读取到uid相关信息，说明用户信息并没有记载这个U盘，所以去网盘上创建";
        if_remove=false;
        newfolder(unumber);
        }
        else
        {
           qDebug()<<"不存在用户信息";
        }


    }
    else
    {
        qDebug()<<"读取到uid相关信息，那就是更新传";
        record_time();
        findFile(upath);

    }

}
    else{qDebug()<<"你设置了不自动上传按钮，所以将不会上传";}

}


void Wangpan::record_time()
{

        qDebug()<<"遍历上传完之后，获取目前的时间,写到用户信息那里去";//这个还要判断一下是否是上传成功了，成功之后再写入更新时间嘛
        QDateTime  current_time= QDateTime::currentDateTime();
        update_utime=current_time.toString("yyyy-MM-dd hh:mm:ss");
        qDebug()<<"update_utime:"<<update_utime;
        write_inf_file("t");



}


//读取用户信息
bool Wangpan::read_inf_file(QString readwhat)//读取文件信息,用户名和密码
{
    qDebug()<<"正在读取用户信息文件";
    QFile file("information.txt");//要是人家的U盘里有同名的文件呢？就不可以作为是否要自动登入的判断条件了，那这个文件的路径跟用户文件的路径不一样的吧？
    //以只读方式打开
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {

        qDebug()<<file.errorString();
        inferro=true;
        return false;
        }



    if(readwhat=="l")
    {
        qDebug()<<"readwhat=l,读取登入信息";
    //匹配用户名
    int had_a=0;
    do
    {
        qDebug()<< "正在匹配用户名";
        QString currentText=file.readLine();//file.readLine()出现一次就读了一行，种子已经往后移动了
        qDebug()<<QObject::tr("当前内容")<<endl<<currentText;
        qDebug()<<QObject::tr("当前位置:")<<file.pos();


        QString pattern("account=(.*)");
        QRegExp rx(pattern);
        int pos =currentText.indexOf(rx);
              qDebug() << pos;
              if ( pos >= 0 )                       //-1代表没匹配
              {

                  had_a=1;
                  account =QByteArray::fromBase64(file.readLine());//file.readLine()出现一次就读了一行，种子已经往后移动了
                  qDebug()<<QObject::tr("account:")<<endl<<account;
                 break;
              }



    }while(!file.atEnd());//文件是到最后
    if(!had_a)
    {
         qDebug()<<"无法匹配用户名";


    }



    //匹配密码
    file.seek(0);//把文件种子放到开头，从头开始检测
    int had_p=0;
    do
    {
        qDebug()<< "正在匹配密码";
        QString currentText=file.readLine();//file.readLine()出现一次就读了一行，种子已经往后移动了
        qDebug()<<QObject::tr("当前内容")<<endl<<currentText;
        qDebug()<<QObject::tr("当前位置:")<<file.pos();


        QString pattern("password=(.*)");
        QRegExp rx(pattern);
        int pos =currentText.indexOf(rx);

              qDebug() << pos;
              if ( pos >= 0 )                       //-1代表没匹配
              {
                  had_p=1;
                  password =QByteArray::fromBase64(file.readLine());
                  qDebug()<<QObject::tr("password:")<<endl<<password;
                 break;

              }


    }while(!file.atEnd());//文件是到最后
    if(!had_p)
    {
         qDebug()<<"无法匹配密码";

    }

     AutoLogin=1;//已保存账户和密码，下次自动登入

    }


    //下面是读取u盘的信息

    if(readwhat=="u")
    {

    file.seek(0);//把文件种子放到开头，从头开始检测，可以注释，因为U盘信息总是在用户名和密码后面就省得再循环了
    qDebug()<<"readwhat=u,读取u盘的信息";
    QString currentuid;
    bool had=false;
    int num=1;

    do
    {
        qDebug()<< "正在匹配uid";

        QString currentText=file.readLine();//file.readLine()出现一次就读了一行，种子已经往后移动了
        qDebug()<<QObject::tr("当前内容")<<endl<<currentText;
        qDebug()<<QObject::tr("当前位置:")<<file.pos();

        //匹配uid
        QString pattern("uid=(.*)");//
        QRegExp rx(pattern);
        int pos =currentText.indexOf(rx);

              qDebug() << pos;
              if ( pos >= 0 )                       //匹配uid
              {
                 currentuid=file.readLine();

                 QString pattern("(.*)\n");//过滤掉\n
                 QRegExp rx(pattern);
                 int pos =currentuid.indexOf(rx);

                       qDebug() << pos;
                       if ( pos >= 0 )                       //过滤
                       {
                           currentuid=rx.cap(1);
                       }

                       qDebug()<<"过滤之后的currentuid"<<currentuid;

                       if(currentuid==UID)//判断这个uid号是当前插入u盘的uid，匹配到了uid
                       {
                         qDebug()<<"currentuid==UID的判断，是相等的";
                          had=true;
                          qDebug()<< "当前的uid就是目前插入u盘的";


                          //获取U盘的名字
                          int current_pos=file.pos();//用current_pos暂时保存现在位置，等读完之后再返回

                          //获取到u盘名字的信息

                           QTextStream stream( &file );
                            uname = stream.readLine();
                            qDebug()<<"uname:"<<uname;
                            qDebug()<<QObject::tr("获取完u盘名字后，当前位置:")<<file.pos();
                            qDebug()<<QObject::tr("返回之前的位置")<<current_pos;
                           file.seek(current_pos);


                           currentText=file.readLine();

                          qDebug()<< "正在匹配time";
                          QString current_time;
                          QString currentText;
                         t_pos=file.pos();//得到时间往哪里写的位置
                         qDebug()<<QObject::tr("更新时间要写入的位置:")<<t_pos;//位置

                         currentText=file.readLine();//至关重要，要不然读不到时间信息


                          QString pattern("time=(.*)");//
                          QRegExp rx(pattern);
                          int pos =currentText.indexOf(rx);

                                qDebug() << pos;
                                if ( pos >= 0 )                       //匹配时间
                                {
                                   qDebug() << "匹配到了时间";
                                   current_time=file.readLine();

                                   QString pattern("(.*)\n");//过滤掉\n
                                   QRegExp rx(pattern);
                                   int pos =current_time.indexOf(rx);

                                         qDebug() << pos;
                                         if ( pos >= 0 )                       //-1代表没匹配
                                         {
                                             qDebug() << "过滤时间";
                                             current_time=rx.cap(1);
                                         }

                                    qDebug()<< "过滤掉的current_time:"<<current_time;

                                        last_utime=current_time;
                                       qDebug()<<"这个目前插入u盘的最近更新时间"<<last_utime;

                                }

                       }
                       num++;
                       qDebug()<<"num++";
                       qDebug()<<"num:"<<num;
                       unumber=num;
                       qDebug()<<"unumber:"<<unumber;
              }



    }while(!file.atEnd());//文件是到最后,文件结尾并不是文字结束，所以会浪费一点时间在这里，文字是结束了，但文件并没有结束
    if(!had)//不存在，就在网盘上创建
    {
         qDebug()<<"这个序列号的u盘不存在，要网盘上创建";
         return false;
    }
    }

    file.close();
    return true;


}



//补充用户信息文件
void Wangpan::write_inf_file(QString writewhat)//读取文件信息,用户名和密码
{

    QFile file("information.txt");
    if(!file.exists())
    {
        qDebug()<<"不存在用户信息文件";
    }
    if(!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug()<<file.errorString();
    }
    QTextStream writeData(&file);
    writeData <<endl;//以追加的方式写，所以会紧跟在屁股后面啊，所以还是弄一个回车符好隔开

    if(writewhat=="u")
    {
    qDebug()<<"往用户信息文件里写入id及名字信息";

    writeData << "uid=myu" << endl<< UID << endl;
    qDebug()<<"u盘的序列号:"<<UID;
    //往用户信息文件里写u盘名字信息
    writeData << uname << endl;

    qDebug()<<"及对应网盘里名:"<<uname;

    qDebug()<<QObject::tr("当前位置:")<<file.pos();
    t_pos=file.pos();
     qDebug()<<QObject::tr("（在write函数里）更新u盘要写的位置:")<<t_pos;//位置


    }


    //写入这个时间比较特别
    if(writewhat=="t")
    {
        //每次开始写的时候先把原来的内容删掉，或者说是修改
        qDebug()<<"写入时间";
        file.seek(t_pos);//把种子放在这个位置开始写
        qDebug()<<"写的位置"<<t_pos;
         //QString currentText=file.readLine();
         //currentText.replace("time=super"+update_utime);//修改

         writeData <<"time=myu"<<endl<< update_utime<< endl;//回车非常重要，直接影响到读数据的准确性
    }

    file.close();
}


void Wangpan::newfolder(int i)
{
   //不可重复建一样名字的文件夹
    qDebug()<<"新建文件夹";
   manager = new QNetworkAccessManager(this);
  connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(nf_replyFinish(QNetworkReply*)));

   QByteArray post_data;
   QNetworkRequest *request = new QNetworkRequest();
   QString j= QString::number(i);
   uname=j+"号u盘";
   QString newpath=wpfixedpath+"/"+uname;

   post_data.append(newpath);

  request->setUrl(QUrl("https://pcs.baidu.com/rest/2.0/pcs/file?access_token="+access_token+"&method=mkdir&path="+newpath));

  request->setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");//这是form格式访问服务器
  request->setRawHeader("Referer",Referer_url.toLatin1());
  request->setHeader(QNetworkRequest::CookieHeader, var);
  manager->post(*request,post_data);


}
void Wangpan::nf_replyFinish(QNetworkReply * reply)
{
    qDebug()<<"进入nf_replyFinish函数";


    if (reply->error() != QNetworkReply::NoError){
                qWarning() << "ERROR:" << reply->errorString();
                qDebug()<< "新建文件错误";

                //一个处理错误http响应的函数，错误则说明，有一样的路径
                //
                QString erropath;
                QString pattern("path=(.*)-");//
                QRegExp rx(pattern);
                int pos =reply->errorString().indexOf(rx);

                      qDebug() << pos;
                      if ( pos >= 0 )
                      {
                         qDebug() << "匹配到了错误路径";
                         erropath=rx.cap(1);

                      }
                //
                QApplication::setQuitOnLastWindowClosed(false);//防止一关对话框就把整个程序给关了
                QMessageBox::information(this, tr("超级云"), "错误！网盘上可能已存在"+erropath+"!请去改名或删除，否则无法上传当前U盘里的内容");

                return ;
            }


    QByteArray data = reply->readAll();
     QString str(data);

     qDebug()<<"JSON返回的所有信息"<<str;
     nf_success=true;

      //本来是要放到before_upload函数里面的，可是不行，不知道为啥
      if(nf_success)//网盘中创建成功
      {
          qDebug()<<"新建u盘文件成功"<<nf_success;

            //把新建的网盘名写进用户信息那里
           qDebug()<<"去把u盘信息写到到用户信息那去了";
            write_inf_file("u");//

            //改u盘名字
             qDebug()<<"去改u盘名字了";
            LPTSTR reName=(LPTSTR)uname.utf16();
            bool ok =SetVolumeLabel((LPTSTR)upath.utf16(),reName);
            qDebug()<<"改u盘名是否成功"<<ok;


          //遍历文件
          qDebug()<<"去遍历上传了";
          record_time();//
          findFile(upath);
      }



     reply->deleteLater();
     return;

}

//这个遍历有很大的漏洞，如果上一次上传的时候还没有遍历完这个U盘，就给打断了，那从此之后只要是之前没遍历完的文件就永远不会再上传了，所以要补多一个信息才行，U盘信息那里要多一个信息，就是有没有遍历完
//可以调用文件里自带的过滤函数根据后缀名，并不是所有都要上传啊
void Wangpan::findFile(QString path)
{
    qDebug()<<"进入遍历函数";
    float size=0;
    QDir dir(path);
    if (!dir.exists())
        return ;



    dir.setFilter(QDir::Dirs|QDir::Files); //处理目录和文件其他的过滤掉
    dir.setSorting(QDir::DirsFirst);  //优先显示目录
    QFileInfoList list = dir.entryInfoList(); //获取文件信息列表
    if(list.size()==0)//若是盘里什么都没有的话还去遍历的话，这个程序就会崩溃,所以有必要先判断一下
    {
        qDebug("这是个空u盘");//如果是空u盘，则说明主人清空了这个u盘，则返回判断一下是不是要重复利用这个u盘，如果是要重复利用这个u盘，那这个u盘的信息可能要改一下了，一个u盘会对应两个名字诶
        return ;
    }


    //这个机制只是在于一种情况，就是U盘里的文件减少了，网盘里的文件多了的情况，一定要放在上传完函数的后面因为，这样才可以在后面改成只有当网盘文件多余U盘的才遍历上传
    qDebug()<<"去查阅U盘的这个路径在网盘中的文件信息"<<path;
    information_list(path);//查询这个路径的网盘文件list信息





    int i = 0;

    do{
            QFileInfo fileInfo = list.at(i);

            if(fileInfo.fileName()=="."||fileInfo.fileName()=="..")
            {
                i++;
                continue;
            }

            bool bisDir = fileInfo.isDir();
            if(bisDir)    //如果是文件夹，就递归
            {
                qDebug()<<"这是一个文件夹啊："<<fileInfo.fileName();//显示文件夹的名字

                findFile(fileInfo.filePath());


            }
            else
            {

                 QFile file(fileInfo.fileName());


                     file.open(QIODevice::ReadOnly);//跟这个有关系，如果只是以ReadOnly方式打开是不会在本地创建一个文件的，但是如果以写的方式打开文件的话，会在本地建一个文件的

                     size = size + fileInfo.size();//文件的大小
                     QString ufilename=fileInfo.fileName();//文件名

                     QString ufilepath=fileInfo.absoluteFilePath();//文件绝对路径

                     //QString ctime=fileInfo.created().toString("yyyy-MM-dd hh:mm:ss");//u盘里的文件的创建时间

                     QDateTime m_t=fileInfo.lastModified();//u盘里的文件的最近修改的时间
                     QString mtime=m_t.toString("yyyy-MM-dd hh:mm:ss");


                     //如果是QDateTime(2016-08-23 19:12:24.661 中国标准时间 Qt::TimeSpec(LocalTime))
                     qDebug("testttttttttttttttest");
                     qDebug()<<"文件名"<<ufilename;//如果文件名有韩文的就没办法显示了
                     qDebug()<<"文件绝对路径"<<ufilepath;
                     qDebug()<<"文件大小"<<size;
                     //qDebug()<<"u盘里的文件的创建时间"<<ctime;

                     qDebug()<<"u盘里的文件的最新修改的时间"<<mtime;

                     //利用这个实现同步，判断一下，若是这个修改时间比上传的时间要后，则重新上传
                     //把读取的last_utime和文件的修改时间来比较


                     QDateTime l_t=QDateTime::fromString(last_utime,"yyyy-MM-dd hh:mm:ss");
                     qDebug()<<"m_t:"<<m_t;
                     qDebug()<<"l_t:"<<l_t;//这个还没有测试呢

                     if(m_t>l_t)//如果修改的时间大于上次更新的时间，那要等到删除返回正确才往下一步走啊
                     {
                         qDebug()<<"满足m_t>l_t去上传了";
                         if(if_remove)//这里还要再判断一下，网盘上是不是已经有这个U盘的信息，如果没有是不用删的，否则先把原来的删了再来传，这个控制放在before_upload那里
                         {

                              wpfile_remove(ufilepath,true);//如果更新过的话，就删除了再传
                              cf_name=ufilename;
                              cf_path=ufilepath;
                         }

                     }

                     upload(ufilename,ufilepath);//如果已经存在的网盘会显示传失败的



            }
            i++;
       }while(i < list.size());//这里是list.size()，也就是当前文件夹下的文件夹数和文件数的和再+2,因为要加.和..






}


//批量获取文件/目录的元信息，json格式访问服务器
void Wangpan::information_list(QString lp)
{
    qDebug()<<"进入information_list函数";
   qDebug()<<"获取文件/目录的list信息";
   manager = new QNetworkAccessManager(this);



   QNetworkRequest *request = new QNetworkRequest();



connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(list_replyFinish(QNetworkReply*)));


       QString urlpath;
       QString pattern("(.*):(.*)");
       QRegExp rx(pattern);
       int pos =lp.indexOf(rx);
             qDebug() << pos;
             if ( pos >= 0 )                       //-1代表没匹配
             {
                 if(rx.cap(2)=="\\")//这是U盘根目录
                 {
                     urlpath= wpfixedpath+"/"+uname;
                     qDebug()<<"这是U盘的根目录";
                 }
                 else
                 {
                 urlpath= wpfixedpath+"/"+uname+rx.cap(2);//过滤掉盘目录，制作QUrl中的路径,文件绝对路径 "F:/傻逼/2222.txt"
                 }
                 qDebug()<<"文件的路径urlpath:"<<urlpath;//如果是根目录的话，listpath: "F:\\",,,文件的路径urlpath: "/apps/meitu/1号u盘\\"，如果是根目录要把\\也要过滤掉啊


             }



  request->setUrl(QUrl("https://pcs.baidu.com/rest/2.0/pcs/file?access_token="+access_token+"&method=list&path="+urlpath));


 request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));//json格式访问服务器
  request->setRawHeader("Referer",Referer_url.toLatin1());
  request->setHeader(QNetworkRequest::CookieHeader, var);



  manager->get(*request);

}




void Wangpan::list_replyFinish(QNetworkReply * reply)

{
    qDebug()<<"进入list_replyFinish函数";
    QString wpath;



    if (reply->error() != QNetworkReply::NoError){
                qWarning() << "ERROR:" << reply->errorString();
                qDebug()<< "获取目录的元信息错误";
                return;
            }


    QByteArray data = reply->readAll();
     QString str(data);
     qDebug()<<"JSON返回的所有信息"<<str<<endl;


qDebug()<<"下面是一个一个的path了";

     QJsonParseError jsonError;  //用QJsonParseError检测json解析过程中的错误，并返回错误。

     QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//buf为要解析的数据，在此buf为QByteArray.QJsonDocument类读写json

     QString title;



     if(jsonError.error == QJsonParseError::NoError)//

       {        if(json.isObject())        //先判断读出来的json是否为对象

                {
                       QJsonObject obj = json.object();//封装json对象

                       if(obj.contains("list"))
                       {    //判断是否含有“list”这个key

                       qDebug() << "====list:"<<obj.value("list");
                       QJsonArray jsonArray = obj["list"].toArray();

                       qDebug() << "jsonArray:"<<jsonArray.size();      //得到这个数组中元素个数,这个路径下面有多少个东西
                       if(jsonArray.size() == 0)
                       {
                       qDebug() << "这个路径下面没东西";

                      }


                       else//不为空，遍历这个数组
                       {


                       foreach (const QJsonValue & value, jsonArray)
                       {
                        bool remove_wf=true;
                       QJsonObject obj = value.toObject();     //封装数组中的值
                       QFileInfo fi(obj["path"].toString());
                       title = fi.fileName();
                       wpath=obj["path"].toString();
                      qDebug() << "path:"<<wpath;  //取key所对应的value
                      qDebug() << "title:"<<title;


                      //过滤法则：过滤掉固定路径加u盘名字路径换成盘符之后取后面的所有，然后在u盘里查找这个文件的绝对路径去掉这个这个文件的名字

                          QString ulistpath;
                          QString pattern(wpfixedpath+"/"+uname+"(.*)"+"/"+title);
                          QRegExp rx(pattern);
                          int pos =wpath.indexOf(rx);
                                qDebug() << pos;
                                if ( pos >= 0 )                       //-1代表没匹配
                                {

                                   ulistpath =filter_up+rx.cap(1);//过滤掉盘目录，制作QUrl中的路径,文件绝对路径 "F:/傻逼/2222.txt"

                                    qDebug()<<"在u盘里对应此网盘的路径是："<<ulistpath;


                                }


                       qDebug()<<"去遍历这个u盘路径了";
                          QDir dir(ulistpath);//里面这个路径是要检视的路径，会因为时间差导致不一样
                              dir.setFilter(QDir::Dirs|QDir::Files); //处理目录和文件其他的过滤掉
                              dir.setSorting(QDir::DirsFirst);  //优先显示目录
                              QFileInfoList list = dir.entryInfoList(); //获取文件信息列表


                              qDebug()<<"U盘里文件的个数"<<list.size();
                               qDebug()<<"网盘里文件的个数"<<jsonArray.size();
                              //因为有时间差所以这个数字对不上，dir里变的很快
                              //if(jsonArray.size()>list.size())//"不要求网盘里这个路径的文件数量多于U盘里文件数量才遍历u盘，因为存在一种情况就是，网盘文件少于u盘的，但是要更新啊
                              //{
                                  //qDebug()<<"网盘里这个路径的文件数量，多于U盘里文件数量，所以要求遍历，检查是哪个文件多了出来";
                                  int i = 0;

                                  do{

                                              QFileInfo fileInfo = list.at(i);
                                              if(fileInfo.fileName()=="."||fileInfo.fileName()=="..")
                                              {
                                                  i++;
                                                  continue;
                                              }

                                              qDebug()<<"这是一个文件夹或文件的名字："<<fileInfo.fileName();//显示文件夹的名字

                                                   if(fileInfo.fileName()==title)
                                                   {
                                                       qDebug()<<"fileInfo.fileName()==title,终于找到跟网盘同名的文件了，网盘里的文件与U盘里也有，所以不用删";
                                                       remove_wf=false;
                                                       qDebug()<<"是否要删除"<<remove_wf;
                                                       break;
                                                   }

                                          i++;
                                     }while(i < list.size());//这里是list.size()，也就是当前文件夹下的文件夹数和文件数




                              if(remove_wf)
                              {
                                  qDebug()<<"遍历完U盘里的文件也没找到网盘里的这个文件，所以要删";
                                  qDebug()<<"是否要删除"<<remove_wf;
                                  qDebug()<<"这个文件是:"<<wpath;
                                  wpfile_remove(wpath,false);//删除网盘里多余的文件
                              }

                              //}
                              //else
                              //{
                                   //qDebug()<<"网盘里这个路径的文件数量并没多于U盘里文件数量";


                              //}



                       }
                       }
                       }
         }
     }


     reply->deleteLater();

     return ;

}


//删除网盘里的文件

void Wangpan::wpfile_remove(QString ufilepath,bool update_up)//删除文件
{



    //
    qDebug()<<"正在删除原来的文件";

    manager = new QNetworkAccessManager(this);

   connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(re_replyFinish(QNetworkReply*)));
    QNetworkRequest *request = new QNetworkRequest();




//请暂时把meitu看成超级云u盘
    QString urlpath;
    if(update_up)//如果是更新传，就要过滤一下
    {

    QString pattern("(.*):(.*)");
    QRegExp rx(pattern);
    int pos =ufilepath.indexOf(rx);
          qDebug() << pos;
          if ( pos >= 0 )                       //-1代表没匹配
          {
              urlpath= wpfixedpath+"/"+uname+rx.cap(2);//过滤掉盘目录，制作QUrl中的路径,文件绝对路径 "F:/傻逼/2222.txt"
              qDebug()<<"这是要删除文件在网盘里的文件的路径urlpath:"<<urlpath;
          }
    }
    else
    {
        urlpath=ufilepath;
    }

   request->setUrl(QUrl("https://pcs.baidu.com/rest/2.0/pcs/file?access_token="+access_token+"&method=delete&path="+urlpath));
  QByteArray sendData;
   sendData.append(QString("path="+urlpath).toLatin1());

  request->setRawHeader("Referer",Referer_url.toLatin1());

    request->setHeader(QNetworkRequest::CookieHeader, var);
    request->setHeader(QNetworkRequest::ContentLengthHeader, QVariant(sendData.size()).toString());

    qDebug()<<"删除文件post出去的数据sendData："<<sendData;
    manager->post(*request,sendData);

}


void Wangpan::re_replyFinish(QNetworkReply * reply)
{

    qDebug()<<"进入re_replyFinish函数";


    if (reply->error() != QNetworkReply::NoError){
                qWarning() << "ERROR:" << reply->errorString();
                return;
            }


    QByteArray data = reply->readAll();
     QString str(data);

     qDebug()<<"JSON返回的所有信息"<<str;



         qDebug()<<"这是更新传，所以需要删除文件之后再传多一遍";
         qDebug()<<"为了解决线程问题：所以审视一下这重新传的文件是否跟要删的文件是同一个"<<cf_name<<cf_path;
         //觉得这更新传最好也过滤一下，跟删除的那个一样，还有在u盘里没有的话，就不会上传啊！
         upload(cf_name,cf_path);//删除成功后，再传多一遍




     reply->deleteLater();
     return ;
}



void Wangpan::upload(QString ufilename,QString ufilepath)//遍历上传
{
    qDebug()<<"正在往网盘里上传U盘里的文件";


    //

    //QString uppath=ufilepath;//创建文件在网盘上的路径，（文件的绝对路径把前面的盘符改成u盘名字）
    manager = new QNetworkAccessManager(this);
   connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(up_replyFinish(QNetworkReply*)));


    QNetworkRequest *request = new QNetworkRequest();

//图片，视频都可以传

    QString m_sTaskPlistPath=ufilepath;
    QFile file(m_sTaskPlistPath);
    if(file.exists())
    {
        if(!file.open(QIODevice::ReadOnly))
        {
             qDebug()<<"打开文件失败";
            return;
        }
    QByteArray fileContent = file.readAll();
    file.close();

    QString sCrlf="\r\n";
    qsrand(QDateTime::currentDateTime().toTime_t());
    QString b=QVariant(qrand()).toString()+QVariant(qrand()).toString()+QVariant(qrand()).toString();
    QString sBoundary="---------------------------"+b;
    QString sEndBoundary=sCrlf+"--"+sBoundary+"--"+sCrlf;
    QString sContentType="multipart/form-data; boundary="+sBoundary;
    sBoundary="--"+sBoundary+sCrlf;

    QByteArray boundary=sBoundary.toLatin1();


    QByteArray sendData;


    sBoundary = sCrlf + sBoundary;
    boundary = sBoundary.toLatin1();
    sendData.append(boundary);
    sendData.append(QString("Content-Disposition: form-data; name=\"filename\""+sCrlf+sCrlf+ufilename).toLatin1());

    sendData.append(boundary);

    sendData.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\"" + ufilename + "\"").toLatin1());

    sendData.append(QString(sCrlf+"Content-Type: text/plain").toLatin1());
    sendData.append(sCrlf+sCrlf.toLatin1());
    sendData.append(fileContent);

    sendData.append(boundary);
    sendData.append(QString("Content-Disposition: form-data; name=\"Upload\""+sCrlf+sCrlf+"Submit Query").toLatin1());

    sendData.append(sEndBoundary.toLatin1());




//请暂时把meitu看成超级云u盘
    QString urlpath;
    QString pattern("(.*):(.*)");
    QRegExp rx(pattern);
    int pos =ufilepath.indexOf(rx);
          qDebug() << pos;
          if ( pos >= 0 )                       //-1代表没匹配
          {
              urlpath= wpfixedpath+"/"+uname+rx.cap(2);//过滤掉盘目录，制作QUrl中的路径,文件绝对路径 "F:/傻逼/2222.txt"
              qDebug()<<"上传到网盘里的文件的路径urlpath:"<<urlpath;


          }


   request->setUrl(QUrl("https://pcs.baidu.com/rest/2.0/pcs/file?access_token="+access_token+"&method=upload&path="+urlpath));



  request->setRawHeader("Referer",Referer_url.toLatin1());


  request->setHeader(QNetworkRequest::ContentTypeHeader,sContentType.toLatin1());//
    request->setHeader(QNetworkRequest::CookieHeader, var);
    request->setHeader(QNetworkRequest::ContentLengthHeader, QVariant(sendData.size()).toString());



    //qDebug()<<"上传文件的第一步post出去的数据sendData："<<sendData;
    manager->post(*request,sendData);


    }

    else
    {
         qDebug()<<"文件不存在";
    }






}


void Wangpan::up_replyFinish(QNetworkReply * reply)
{

    qDebug()<<"进入up1_replyFinish函数";


    if (reply->error() != QNetworkReply::NoError){
                qWarning() << "ERROR:" << reply->errorString();
                return;
            }


    QByteArray data = reply->readAll();
     QString str(data);

     qDebug()<<"JSON返回的所有信息"<<str;
/*
     QScriptEngine engine;
     QScriptValue sc = engine.evaluate("value=" + str);

     if(sc.property("errno").toString()!= "0")
     {
      qDebug()<< "错误";
      if(sc.property("errmsg").toString()== "ticket invalid")
      {
          qDebug()<<"";

      }

     }

*/

     reply->deleteLater();
     return ;



}



void Wangpan::setUrl(const QUrl &url)
{

    ui->webView->setUrl(url);//完成后调用一遍on_webView_loadFinished

}

void Wangpan::changeIcon()
{
    setWindowIcon(ui->webView->icon());
}



