#include "tray.h"
#include "Button.h"

std::list<Button *>Button_List;

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

  g_signal_connect(                                                       //信号绑定：退出窗口
    G_OBJECT(menuItemExit_), 
    "activate", 
    G_CALLBACK(TrayExit), 
    NULL
  );
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
                                                                          //
  
  std::thread(NewConnect,this).detach();
  std::thread(DeleteConnect,this).detach();
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
void The_Main_tray::NewConnect(void *T) {
  The_Main_tray *temple_Tray = (The_Main_tray *)T;
  while (true) {  
    std::unique_lock<std::mutex> locker(temple_Tray->backend_->Singal.Mu_Singal_newconnect);
    while(temple_Tray->backend_->Date.Singal_newconnect.empty())temple_Tray->backend_->Singal.Cond_Singal_newconnect.wait(locker);
    while(!temple_Tray->backend_->Date.Singal_newconnect.empty()){
      std::string aim_ip = temple_Tray->backend_->Date.Singal_newconnect.front();
      Connecter *temple_connecter = nullptr;
      for(auto i:temple_Tray->backend_->connecter_) {
        temple_connecter = i;
        if(i->ip_char_ == aim_ip)break;
      }
      Button *button_ = new Button(temple_Tray,temple_Tray->backend_,temple_connecter);
      Button_List.push_back(button_);
      temple_Tray->backend_->Date.Singal_newconnect.pop();
    }
    locker.unlock();
  }
    
  return;
}


void The_Main_tray::DeleteConnect(void *T) {
  The_Main_tray *temple_Tray = (The_Main_tray *)T;
  while (true) {  
    std::unique_lock<std::mutex> locker(temple_Tray->backend_->Singal.Mu_Singal_deleteconnect);
    while(temple_Tray->backend_->Date.Singal_newconnect.empty())temple_Tray->backend_->Singal.Cond_Singal_deleteconnect.wait(locker);
    while(!temple_Tray->backend_->Date.Singal_deleteconnect.empty()){
      std::string aim_ip = temple_Tray->backend_->Date.Singal_deleteconnect.front();
      Connecter *temple_connecter = nullptr;
      for(auto i:temple_Tray->backend_->connecter_) {
        temple_connecter = i;
        if(i->ip_char_ == aim_ip)break;
      }
      temple_Tray->backend_->connecter_.remove(temple_connecter);
      delete[] temple_connecter;

      Button *temple_button = nullptr;
      for(auto i:Button_List) {
        temple_button = i;
        if(i->ip_char_ == aim_ip)break;
      }
      Button_List.remove(temple_button);
      delete[] temple_button;

      temple_Tray->backend_->Date.Singal_deleteconnect.pop();
    }
    locker.unlock();
  }
    
  return;
}
