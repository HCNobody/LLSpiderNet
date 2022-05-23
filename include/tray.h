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
private:
  GtkWidget *window;
  GtkStatusIcon* gtk_status_icon;
};

#endif
