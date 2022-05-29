/**
 *作者:HC_FSJJX
 *开发时间:2022.5.4
 *版本:0.1
**/
#include "main.h"


int main(int argc,char *argv[]) {
#ifndef INSTALLED
  Main::Init();
#endif

  gtk_init(&argc,&argv);                              //GTK初始化
  //初始化托盘
  auto Main_Tray = new The_Main_tray();               //托盘+主窗口

  gtk_main ();                                        //GTK循环
  return 0;  
}
