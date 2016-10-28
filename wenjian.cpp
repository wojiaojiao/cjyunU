
#include <QFileInfo>
#include <QDir>
#include "wenjian.h"
wenjian::wenjian(QWidget *parent)
    : QWidget(parent)
{

    setupUi(this);
}


void wenjian::findFile(QString path)//若是盘里什么都没有的话这个程序就崩溃了//
{
/*
    QDir dir(path);
    if (!dir.exists())
        return ;
    dir.setFilter(QDir::Dirs|QDir::Files); //处理目录和文件其他的过滤掉
    dir.setSorting(QDir::DirsFirst);  //优先显示目录
    QFileInfoList list = dir.entryInfoList();  //获取文件信息列表

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


                 QFile *file=new QFile(fileInfo.fileName());



                 if(file->exists("E://LTJ//"+fileInfo.fileName()))
                 {
                      //QMessageBox::warning(this,tr("上传文件"),tr("文件已经存在！"));

                 }
                 else
                 {

                     file->open(QIODevice::ReadWrite);
                     size = size + fileInfo.size();//文件的大小

                     qDebug("ffff");
                     qDebug()<<fileInfo.fileName();//如果文件名有韩文的就没办法显示了
                     qDebug()<<fileInfo.canonicalPath();
                     qDebug()<<size;

                 }
            }
            i++;
       }while(i < list.size());//这里是list.size()，也就是当前文件夹下的文件夹数和文件数的和再+2

*/
}
