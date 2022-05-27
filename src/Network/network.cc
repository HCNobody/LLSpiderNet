#include "network.h"

Connector::Connector() {
  TP_->init();
  Shot_Radio();
  thd_Client = std::thread(Client);
}
Connector::~Connector() {
  thd_Client.join();
  TP_->shutdown();
  delete TP_;
}

void Connector::Client() {
}

void Connector::Shot_Radio() {
  std::thread A(Radio,this);
  A.detach();
}

void Connector::Radio(void* C) {
  Connector *connector = (Connector *)C;
  printf("11");
}
