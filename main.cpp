#include "widget.h"
#include <QApplication>
#include "wangpan.h"//
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   Widget w;
    w.show();
    w.Readregedit();//开机程序自启动用的,要调用才行
    return a.exec();
}
