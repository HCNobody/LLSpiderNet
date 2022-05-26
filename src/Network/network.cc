#include "network.h"

Connector::Connector() {
  TP_->init();
  TP_->submit(NetWork::Radio_to_others);
}
Connector::~Connector() {
  TP_->shutdown();
  delete TP_;
}
namespace NetWork {
  void Radio_to_others() {
    printf("1");
  }
}
