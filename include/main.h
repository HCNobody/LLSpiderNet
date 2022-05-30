#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <string>

#include "tray.h" //拖盘头文件
#include "../ToolKit/heads/Event_T_Callback.h"

namespace Main {
  void Init() {
    std::string buffer(getcwd(NULL,0));
    if (buffer.substr(buffer.size() - 5) != "build") {
      buffer += "/build";
      chdir(buffer.c_str());
    }
  }
}

#endif
