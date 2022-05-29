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

#include "ThreadPool.h"

std::string Getip();      


struct Connecter {
  std::string ip_char_;
  int32_t ip_ = 0;
  int32_t fd_;
};
class Connector {
public:
  Connector();
  ~Connector();
  void Shot_Radio();
public:
  std::string Local_ip_char_; 
  int32_t Local_ip_int32_; 
  std::list<std::thread *> Client_List_;
  std::list<std::thread *> Server_List_;
  std::list<Connecter *> connecter_;
private:
  static void UDP_Server(void* C);
  static void Radio(void* C);
  static void Client(void *C,std::string IP);
  static void TCP_Server(void *C);
  static void Server(void *C,int32_t cfd);
private:
  int32_t radio_clicked_times = 0;
  std::thread thd_UDP_Server_;
  std::thread thd_TCP_Server_;

};


#endif
