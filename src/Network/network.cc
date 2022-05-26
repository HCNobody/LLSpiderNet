#include "network.h"

Connector::Connector() {
  //广播
  int32_t serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  struct sockaddr_in serv_addr;
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(1234);

  bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

  listen(serv_sock,10);
  //开启监听服务
}

namespace NetWork {
}
