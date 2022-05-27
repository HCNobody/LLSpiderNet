#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <list>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

 #include "ThreadPool.h"

struct Connecter {
  int32_t id_;
};

class Connector {
public:
  Connector();
  ~Connector();
  void Shot_Radio();
public:
private:
  static void Client();
  static void Radio(void* C);
private:
  std::list<Connecter *> connecter_;
  std::vector<std::thread> thd_;
  std::thread thd_Client;
  ThreadPool* TP_ = new ThreadPool(4);
};


#endif
