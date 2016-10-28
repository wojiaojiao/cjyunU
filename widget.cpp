#include "widget.h"
#include "ui_widget.h"
#include <dbt.h>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QLabel>
#include <string.h>
#include <QTextCodec>
//弹窗
#include <QMessageBox>
#define pwidth 1500//显示窗口初始横着的位置
#define pheight 50//显示窗口初始竖着的位置
QString path1="";
int Insertamount=0;
#define REG_RUM "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"//开机程序自启动用的
void Widget::Readregedit()    //开机程序自启动用的,记得调用，放哪都一样都需要调用
   {
       QString app_name = QApplication::applicationName();
       QString application_path = QApplication::applicationFilePath();
       QSettings *reg=new QSettings(REG_RUM,QSettings::NativeFormat );
       reg->setValue(app_name,QVariant(application_path.replace("/","\\")));
    }

//********************************************//
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)

{
    ui->setupUi(this);
    //下面是显示运行图标的
    setWindowIcon(QIcon("myico.ico"));
    //
    setWindowTitle("超级云");
    //setWindowFlags(Qt::Desktop);//程序不在任务栏上显示,但是后面的子窗口也跟着不见了。因为下了后面界面美化的代码，所以退出界面仍在后台，小图标还在
    setWindowFlags(Qt::Tool);

  //界面的美化代码
  //setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏
  //setAttribute(Qt::WA_TranslucentBackground);//背景透明化
    this->move(pwidth,pheight );//这可以让它显示在任何位置



    //以下是托盘化的
       //创建托盘图标
              QIcon icon = QIcon("./myico.png");
              trayIcon = new QSystemTrayIcon(this);
              trayIcon->setIcon(icon);

              trayIcon->setToolTip(tr("超级云u盘"));//这个一定要，要不然出错
              QString titlec=tr("超级云u盘");
              QString textc=tr("1.插入u盘\n2.在授权登入框中登入你的网盘\n3.在网盘的：我的应用数据/qingbiji/下查看上传好的文件");
              trayIcon->show();

              //弹出气泡提示
              trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);//后面的1000是显示多久



       //添加单/双击鼠标相应

               connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                     this,SLOT(showNormal()));


             //创建监听行为


              explainAction = new QAction(tr("说明"), this);
              connect(explainAction, SIGNAL(triggered()), this, SLOT(shuoming()));
             minimizeAction = new QAction(tr("最小化 (&I)"), this);
             connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
             restoreAction = new QAction(tr("还原 (&R)"), this);
             connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
             quitAction = new QAction(tr("退出 (&Q)"), this);
             connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

             //创建右键弹出菜单
             trayIconMenu = new QMenu(this);
             trayIconMenu->addAction(explainAction);
             trayIconMenu->addSeparator();
             trayIconMenu->addAction(minimizeAction);
             trayIconMenu->addAction(restoreAction);
             trayIconMenu->addSeparator();
             trayIconMenu->addAction(quitAction);
             trayIcon->setContextMenu(trayIconMenu);
             //**********************************************************//


             //调用外部exe，打开浏览器
             //openbrowser();



}
//调用外部exe，打开浏览器
void Widget::openbrowser()
{
 QString program = "./cjyunbrowser/cjyunbrowser.exe";
 QProcess *myProcess = new QProcess();
 myProcess->start(program);
}

//弹出说明书
void Widget::shuoming()
{

    QMessageBox::information(this, tr("说明"), tr("上传的内容在网盘的：我的应用数据/qingbiji/ 路径下。u盘及在网盘上对应的文件夹的命名规则是：~号u盘，在改名的时候避免这种规则，具体请登入www.cjyun.top"));
}


Widget::~Widget()
{
    delete ui;
}


bool Widget::nativeEvent(const QByteArray & eventType, void * message, long *result)
{
  //  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
            case DBT_DEVICETYPESPECIFIC:
            {
                break;
            }
            case DBT_DEVICEARRIVAL:
                if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                    if (lpdbv -> dbcv_flags == 0)
                    {
                         qDebug() << "有U盘 " ;
                         // 插入U盘，此处可以做你想做的事



                         //打开U盘

                         QString USBDisk = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                         qDebug() << "USB_Arrived and The USBDisk is: "<<USBDisk ;
                         path1=USBDisk+":/";//获取程序当前目录
                         path1.replace("/","\\");//将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
                         //QProcess::startDetached("explorer "+path1);//打开上面获取的目录



                         //获取U盘名字
                         QString lpRootPathName = path1;
                         LPTSTR lpVolumeNameBuffer=new TCHAR[12];//磁盘卷标
                         DWORD nVolumeNameSize=12;// 卷标的字符串长度
                         DWORD VolumeSerialNumber;//硬盘序列号
                         DWORD MaximumComponentLength;// 最大的文件长度
                         LPTSTR lpFileSystemNameBuffer=new TCHAR[10];// 存储所在盘符的分区类型的长指针变量
                         DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
                         DWORD FileSystemFlags;// 文件系统的一此标志

                         GetVolumeInformation((LPTSTR)lpRootPathName.utf16(),
                           lpVolumeNameBuffer, nVolumeNameSize,
                           &VolumeSerialNumber, &MaximumComponentLength,
                           &FileSystemFlags,
                           lpFileSystemNameBuffer, nFileSystemNameSize);
                         qDebug() <<"u盘名字:"<<lpVolumeNameBuffer ;
                         QString a;
                         a=QString::fromWCharArray(lpVolumeNameBuffer);
                         qDebug() << "U盘名字:" <<a;

                         qDebug() <<"磁盘序列号:"<< VolumeSerialNumber;
                         ui->label_2->setText(path1+a);

                         //传参

                         Insertamount++;
                         qDebug() << "Insertamount"<<Insertamount;

                         Wangpan *ww = new Wangpan;
                         ww->set_param(path1,VolumeSerialNumber);
                         if(Insertamount<2)
                         {

                         ww->setUrl(QUrl("https://openapi.baidu.com/oauth/2.0/authorize?response_type=token&client_id=WKfkEqQPBKNVTZMxYUOM3189&redirect_uri=oob&scope=netdisk"));//
                         ww->show();
                         }
                         else
                         {

                             ww->set_param2(1);
                             ww->before_upload();
                         }





                        // 获取U盘的剩余空间
                        LPCWSTR lpcwstrDriver=(LPCWSTR)path1.utf16();
                        ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
                        if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
                        //GetDiskFreeSpaceEx是Windows的系统函数
                        {
                            qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
                            return 0;
                        }
                        float freeSpace;
                        freeSpace = (float) liTotalFreeBytes.QuadPart/1024/1024/1024;
                        freeSpace = (int)(freeSpace * 10) / 10.0;
                        qDebug() << "可用:"<<freeSpace<<"GB";
                    }
                 }
                 break;
            case DBT_DEVICEREMOVECOMPLETE:
                if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                    if (lpdbv -> dbcv_flags == 0)
                    {
                        qDebug() << "USB_设备移除";
                    }
                }
                break;
            }
        }
        return false;
    }


//获取盘符
char Widget::FirstDriveFromMask (ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
        break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}

void Widget::on_pushButton_clicked()
{
  QProcess::startDetached("explorer "+path1);//打开u盘
}


void Widget::on_pushButton_2_clicked()
{
    if(Insertamount>0)
    {
   if (QMessageBox::Yes == QMessageBox::question(this,"超级云","确定清空"+path1+"U盘?",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes))
   {
       qDebug()<<"clicked yes\n";

       if (QMessageBox::Yes == QMessageBox::question(this,"超级云","在网盘上找到原u盘文件夹改个名，则清空前保存在网盘上的东西会继续保存下来",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes))
       {
           qDebug()<<"确定了当前是有插入u盘的，是删除u盘里的内容的";
           QDir dir(path1);
           dir.removeRecursively();
       }
   }
   }
    else
    {
        QMessageBox::information(this, tr("超级云"), tr("请插入u盘才能确定要清空的u盘"));
    }
}

void Widget::on_pushButton_3_clicked()
{
    //打开授权登入框
    Wangpan w;
    w.setUrl(QUrl("https://openapi.baidu.com/oauth/2.0/authorize?response_type=token&client_id=WKfkEqQPBKNVTZMxYUOM3189&redirect_uri=oob&scope=netdisk"));
}

void Widget::on_pushButton_4_clicked()
{
   openbrowser();
}

//自动上传的
void Widget::on_checkBox_clicked()
{


     if(ui->checkBox->isChecked())
     {
         qDebug("自动上传打勾勾");
         Wangpan w;
         w.set_param3("autoup");

     }
    else
    {
         qDebug("自动上传不打勾勾");
         QMessageBox::information(this, tr("超级云"), tr("设置之后插入的u盘将不会自动上传"));
         Wangpan w;
         w.set_param3("noautoup");


    }
}

//解除绑定网盘的
void Widget::on_checkBox_2_clicked()
{


     if(ui->checkBox_2->isChecked())
     {
         qDebug("绑定网盘打勾勾");
         QMessageBox::information(this, tr("超级云"), tr("若要重新绑定一个新网盘，在登入框中登入新的网盘即可"));

     }
    else
    {
         qDebug("绑定网盘不打勾勾");
         QMessageBox::information(this, tr("超级云"), tr("已解除绑定！\n若要绑定一个新的网盘，在登入框中登入新的网盘即可"));
         Wangpan w;
         w.set_param3("noautobind");

    }
}


