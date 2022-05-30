#include "Event_T_Callback.h"
#include "iostream"

namespace TooLKit{
namespace Event_T_Callback{
namespace HIDE {
  std::mutex mut_The_map_;
  std::mutex mut_The_pthd__;
  ThreadPool *PL = new ThreadPool(8);
  std::map<std::string,std::list<FuncShooter*>> The_Map_;
  AutoRun autorun;
}
}
}

namespace TooLKit{
namespace Event_T_Callback{
  bool Shend_Singal(std::string Signal,void *Shooter) {
    HIDE::mut_The_map_.lock();
    auto itl = TooLKit::Event_T_Callback::HIDE::The_Map_.find(Signal);
    if(itl == TooLKit::Event_T_Callback::HIDE::The_Map_.end()) {
      HIDE::mut_The_map_.unlock();
      return false;
    }
    for (auto it:itl->second) {
      if(it->Shooter != Shooter){HIDE::mut_The_map_.unlock();return false;}
      switch(it->mode){
        case 0://use current thread
               HIDE::mut_The_map_.unlock();
               it->func();
               break;
        case 1://use thread pool
               HIDE::mut_The_map_.unlock();
               HIDE::PL->submit(it->func);
               break;
        case 2://detach a thread
               HIDE::mut_The_map_.unlock();
               HIDE::mut_The_pthd__.lock();
               auto t = new std::thread(it->func);
               t->detach();
               HIDE::autorun.pthd.push_back(t);
               HIDE::mut_The_pthd__.unlock();
               break;
      }
    }
    return true;
  }
  bool Delete_Bind(std::string Signal,void *Shooter) {
    HIDE::mut_The_map_.lock();
    auto itl = TooLKit::Event_T_Callback::HIDE::The_Map_.find(Signal);
    if(itl == TooLKit::Event_T_Callback::HIDE::The_Map_.end()) {
      HIDE::mut_The_map_.unlock();
      return false;
    }
    for(auto &i:itl->second) {
      if(i->Shooter != Shooter)continue;
      i->func.~function<void()>();
    }
    itl->second.remove_if([&Shooter](HIDE::FuncShooter* F)->bool{return F->Shooter == Shooter;});
    HIDE::mut_The_map_.unlock();
    return true;
  }

  HIDE::AutoRun::AutoRun() {
    PL->init();
  }

  HIDE::AutoRun::~AutoRun(){
    PL->shutdown();
    delete PL;
    for(auto i:pthd) {
      pthread_cancel(i->native_handle());
    }
  }
}
}
