#include "Button.h"

Button::Button(
    The_Main_tray *Tray,
    Connector *C,
    Connecter *c
):Tray_(Tray),connector_(C),connecter_(c) {
  ip_char_ = connecter_->ip_char_;
}


