#include "tray.h"

The_Main_tray::The_Main_tray(
  ) 
{
  window_ = gtk_window_new(GTK_WINDOW_POPUP);                             //初始化窗口
  gtk_window_set_title(GTK_WINDOW(window_), "Window");                    //设置窗口标题
  gtk_widget_set_size_request(window_, 200, -1);                          //窗口大小
  g_signal_connect(window_, "destroy", G_CALLBACK(gtk_main_quit), NULL);  //
  gtk_window_set_default_size(GTK_WINDOW(window_),700,400);               //窗口默认大小
  
  gtk_window_set_position(GTK_WINDOW(window_),GTK_WIN_POS_CENTER_ALWAYS); //窗口居中

  //set tray icon file
  std::string icon_path = "resources/蜘蛛网万圣节.svg";
#ifndef INSTALLED
  icon_path = "../" + icon_path;
#else
  icon_path = INSTALLED + icon_path;
#endif
  trayIcon_ = gtk_status_icon_new_from_file(icon_path.c_str());

  //set popup menu for tray icon
  menu_ = gtk_menu_new();
  menuItemExit_ = gtk_menu_item_new_with_label("Exit");
  menuItemRadio_ = gtk_menu_item_new_with_label("广播");

  g_signal_connect(                                                       //信号绑定：退出窗口
    G_OBJECT(menuItemExit_), 
    "activate", 
    G_CALLBACK(TrayExit), 
    NULL
  );
  g_signal_connect(                                                       //信号绑定：广播
    G_OBJECT(menuItemRadio_), 
    "activate", 
    G_CALLBACK(TrayRadio), 
    this
  );

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_), menuItemRadio_);            //
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_), menuItemExit_);            //
  gtk_widget_show_all(menu_);                                             //显示托盘
  //set tooltip
  //gtk_status_icon_set_tooltip (trayIcon_, "MsgWatcherGTK");
  //connect handlers for mouse events
  g_signal_connect(
    GTK_STATUS_ICON(trayIcon_),             
    "activate",
    G_CALLBACK(TrayIconActivated), 
    window_
  );
  g_signal_connect(
    GTK_STATUS_ICON(trayIcon_), 
    "popup-menu",
    G_CALLBACK(TrayIconPopup), 
    menu_
  );
  gtk_status_icon_set_visible(trayIcon_,true);                            //显示托盘      

}
The_Main_tray::~The_Main_tray() {
  delete backend_;
}

void The_Main_tray::TrayExit(GtkMenuItem *item, gpointer user_data) 
{
  printf("exit");
  gtk_main_quit();
}

void The_Main_tray::TrayIconActivated(GObject *trayIcon, gpointer window)
{
  if(!gtk_widget_get_visible(GTK_WIDGET(window))) {                       //如果窗口正在显示则隐藏反之显示  
    gtk_widget_show(GTK_WIDGET(window));
    gtk_window_deiconify(GTK_WINDOW(window));    
  } else {
    gtk_widget_hide(GTK_WIDGET(window)); 
  } 
}

void The_Main_tray::TrayIconPopup(
  GtkStatusIcon *status_icon, 
  guint button, 
  guint32 activate_time, 
  gpointer popUpMenu
)
{
  gtk_menu_popup(
    GTK_MENU(popUpMenu), 
    NULL, 
    NULL, 
    gtk_status_icon_position_menu, 
    status_icon, 
    button, 
    activate_time
  );
}


void The_Main_tray::TrayRadio(void* T) {
  The_Main_tray *temple_Tray = (The_Main_tray *)T;
  temple_Tray->backend_->Shot_Radio();
}





