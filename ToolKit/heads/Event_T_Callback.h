#ifndef EVENT_T_CALLBACK_H
#define EVENT_T_CALLBACK_H

#include <string>
#include <mutex>
#include <queue>
#include <functional>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <list>

#include "ThreadPool.h"


namespace TooLKit{
namespace Event_T_Callback{
namespace HIDE{
  extern std::mutex mut_The_map_;
  extern std::mutex mut_The_pthd__;
  struct FuncShooter{
    std::function<void()>func;
    void * Shooter;
    int32_t mode;
  };
  extern ThreadPool *PL;
  extern std::unordered_map<std::string,std::list<FuncShooter*>> The_Map_;
  class AutoRun{
    public:
      AutoRun();
      ~AutoRun();
    public:
      std::list<std::thread*>pthd;
  };
  extern AutoRun autorun;
}
}
}

namespace TooLKit{
namespace Event_T_Callback{
  template <typename F, typename... Args>
  extern auto Bind(void *Shooter,std::string Signal, int32_t mode, F &&f, Args &&...args) -> bool;
  template <typename F, typename... Args>
  auto Bind(
      void *Shooter,
      std::string Signal, 
      int32_t mode, 
      F &&f, 
      Args &&...args
  ) -> bool {
      std::function<decltype(f(args...))()> func = 
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...);
      TooLKit::Event_T_Callback::HIDE::FuncShooter * temple 
                                                              = 
        new TooLKit::Event_T_Callback::HIDE::FuncShooter;

      temple->func = std::move(func);
      temple->mode = mode;
      temple->Shooter = Shooter;
      HIDE::mut_The_map_.lock();
      auto it = TooLKit::Event_T_Callback::HIDE::The_Map_.find(Signal);
      if(it == TooLKit::Event_T_Callback::HIDE::The_Map_.end())
      TooLKit::Event_T_Callback::HIDE::The_Map_.insert(
          std::map<
                    std::string,
                    std::list
                    <TooLKit::Event_T_Callback::HIDE::FuncShooter*>>
                    ::value_type(
                          Signal,
                          std::list
                              <TooLKit::Event_T_Callback::HIDE::FuncShooter*>
                              {temple}
                    )
      );
      else 
        it->second.push_back(temple);
      HIDE::mut_The_map_.unlock();
      return true;
  }

  extern bool Send_Signal(std::string Signal,void *Shooter);
  extern bool Delete_Bind(std::string Signal,void *Shooter);
}
}







#endif
