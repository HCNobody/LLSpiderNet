#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <list>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <string>
#include <condition_variable>

#include "../../ToolKit/heads/ThreadPool.h"
#include  "../../ToolKit/heads/Event_T_Callback.h"
std::string Getip();      


struct Connecter {
  std::string ip_char_;
  int32_t ip_ = 0;
  int32_t fd_;
  std::mutex mu;
  std::condition_variable con;
  bool operator == (const Connecter *C) {
    return this->ip_char_ == C->ip_char_;
  }
};

class Connector {
public:
  Connector();
  ~Connector();
public:
  std::string Local_ip_char_; 
  int32_t Local_ip_int32_; 
  std::list<std::thread *> Client_List_;
  std::list<std::thread *> Server_List_;
  std::list<Connecter *> connecter_;
public:
  struct Date {
    std::queue<std::string> Singal_newconnect;
    std::queue<std::string> Singal_deleteconnect;
  }Date;
  struct Singal { 
    std::mutex Mu_Singal_newconnect;
    std::condition_variable Cond_Singal_newconnect;
    std::mutex Mu_Singal_deleteconnect;
    std::condition_variable Cond_Singal_deleteconnect;
  }Singal;
private:
  static void UDP_Server(void* C);
  static void Shot_Radio(void *C);
  static void Radio(void* C);
  static void Client(void *C,std::string IP,Connecter *connecter);
  static void TCP_Server(void *C);
  static void Server(void *C,int32_t cfd,std::string IP,Connecter *connecter);
private:
  int32_t radio_clicked_times = 0;
  std::thread thd_UDP_Server_;
  std::thread thd_TCP_Server_;
  std::thread thd_Shot_Radio_;

};


#endif
