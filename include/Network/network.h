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

#include "thread_pool.h"

struct Connecter {
  int32_t id_;
};

class Connector {
public:
  Connector();
  ~Connector();
private:
  ThreadPool *TP_ = new ThreadPool(4);
  std::list<Connector *> connecter;
};

namespace NetWork {
  void Radio_to_others();
}

#endif
