#include "network.h"

Connector::Connector() {
  Shot_Radio();
  thd_Client = std::thread(Client,this);
}
Connector::~Connector() {
  thd_Client.join();
}

void Connector::Client(void* C) {
  Connector *connector = (Connector *)C;
  auto PL = new ThreadPool(4);
  while (true) {
   printf("1\n");
   sleep(2);
  } 
  //
  delete PL;
}

void Connector::Shot_Radio() {
  std::thread A = std::thread(Radio,this);
  A.detach();
}

void Connector::Radio(void* C) {
  Connector *connector = (Connector *)C;
  if(connector->radio_clicked_times <= 3){
    ++connector->radio_clicked_times;
    sleep(10);
    --connector->radio_clicked_times;
  }
}
