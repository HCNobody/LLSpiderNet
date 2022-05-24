#ifndef TRAY_H
#define TRAY_H

#include <gtk/gtk.h>

#include "debug_tools.h"

/**
 *
 * 拖盘class
 *
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
  GtkWidget *menu_, *menuItemView_, *menuItemExit_;

};


#endif
