#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <windows.h>
#include <QString>
#include <ShellAPI.h>
#include "wangpan.h"
// 托盘化用的
#include <QSystemTrayIcon>
#include <QMenu>
//


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
     float getDiskFreeSpace(QString driver);
     void Readregedit();




private:
    Ui::Widget *ui;



protected:
 //bool winEvent(MSG *msg, long *result);
 bool nativeEvent(const QByteArray & eventType, void * message, long *result);
private:
 //UsbConfig usbConfig;
 char FirstDriveFromMask (ULONG unitmask);
 //托盘化
 QSystemTrayIcon *trayIcon;

 QAction *explainAction;
 QAction *minimizeAction;
 QAction *restoreAction;
 QAction *quitAction;
 QMenu   *trayIconMenu;

private slots:
  void openbrowser();
  void shuoming();
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_4_clicked();
  void on_checkBox_clicked();
  void on_checkBox_2_clicked();


};

#endif // WIDGET_H
