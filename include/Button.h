#ifndef BUTTON_H
#define BUTTON_H


#include "tray.h"

class Button {
public:
  Button(The_Main_tray *Tray,Connector *C,Connecter *c);
  bool operator == (const Button *b) {
    return this->ip_char_ == b->ip_char_;
  }
public:
  std::string ip_char_;
private:
  The_Main_tray *Tray_; 
  Connector *connector_;
  Connecter *connecter_;
};


#endif
