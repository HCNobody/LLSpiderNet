#ifndef TRAY_H
#define TRAY_H

#include <gtk/gtk.h>
#include <string.h>

#include "debug_tools.h"

/**
 *
 * 拖盘class
 * 功能：托盘显示图标。点击图标可显示隐藏主窗口。
 *
 * The_Main_tray()   构造函数
 * ~The_Main_tray()  析构函数
 * //tray所使用的回调函数
 *static void TrayExit
 *static void TrayIconActivated
 *static void TrayIconPopup
 *
 *GtkWidget *window_;                // 主窗口    
 *GtkStatusIcon *trayIcon_;          // 托盘图标
 *GtkWidget *menu_,                  // 托盘选项
 *          *menuItemExit_;          // 托盘退出应用
**/
class The_Main_tray {
public:
  The_Main_tray();
  ~The_Main_tray();

  // static The_Main_tray *pThis;
  static void TrayExit(GtkMenuItem *item, gpointer user_data);
  static void TrayIconActivated(GObject *trayIcon, gpointer data);
  static void TrayIconPopup(
    GtkStatusIcon *status_icon, 
    guint button, 
    guint32 activate_time, 
    gpointer popUpMenu
  );

private:
  GtkWidget *window_;
  GtkStatusIcon *trayIcon_;
  GtkWidget *menu_, *menuItemExit_;

};


#endif
