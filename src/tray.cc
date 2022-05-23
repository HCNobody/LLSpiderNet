#include "tray.h"

The_Main_tray::The_Main_tray(
  ) 
{
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  GtkStatusIcon *trayIcon  = gtk_status_icon_new_from_file("../doc/蜘蛛网万圣节.svg");

  gtk_widget_show (window);
}
The_Main_tray::~The_Main_tray() {

}
