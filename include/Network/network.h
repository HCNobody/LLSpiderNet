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
#include <queue>

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
  static void Client(void* C);
  static void Radio(void* C);
private:
  int32_t radio_clicked_times = 0;
  std::list<Connecter *> connecter_;
  std::thread thd_Client;
};


#endif
