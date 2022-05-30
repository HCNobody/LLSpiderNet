#include "network.h"

pthread_mutex_t muteofconnecter_;

Connector::Connector() {
  Local_ip_char_ = Getip(); 
  inet_pton(AF_INET,Local_ip_char_.c_str(),&Local_ip_int32_);

  thd_Shot_Radio_ = std::thread(Shot_Radio,this);
  thd_UDP_Server_ = std::thread(UDP_Server,this);
  thd_TCP_Server_ = std::thread(TCP_Server,this);
}
Connector::~Connector() {
  pthread_cancel(thd_UDP_Server_.native_handle());
  pthread_cancel(thd_TCP_Server_.native_handle());
  for(auto i: Client_List_){pthread_cancel(i->native_handle());}
  for(auto i: Server_List_){pthread_cancel(i->native_handle());}
  pthread_cancel(thd_Shot_Radio_.native_handle());
}

void Connector::UDP_Server(void* C) {
  Connector *connector = (Connector *)C;

  setvbuf(stdout, NULL, _IONBF, 0); 
	fflush(stdout); 
 
	// 绑定地址
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = htonl(INADDR_ANY);
	addrto.sin_port = htons(10084);
	
	// 广播地址
	struct sockaddr_in from;
	bzero(&from, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	from.sin_addr.s_addr = htonl(INADDR_ANY);
	from.sin_port = htons(10084);
	
	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {   
		perror("ERROR[SOCKET]");	
		return ;
	}   
 
	const int opt = 1;
	//设置该套接字为广播类型，
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1) {
		perror("ERROR[SEt SOCKET]");
		return ;
	}
 
	if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1) 
	{   
		perror("ERROR[BIND]");
		return ;
	}
 
	int len = sizeof(sockaddr_in);
	char smsg[100] = {0};

  std::string Local_ip1 = "127.0.0.1";
  std::string Local_ip2 = "0.0.0.0";
 
	while(1) {
		//从广播地址接受消息
		int ret=recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&from,(socklen_t*)&len);
    if(smsg == connector->Local_ip_char_)continue;
    pthread_mutex_lock(&muteofconnecter_);
    bool exist = false;
    for(auto i:connector->connecter_) {
      if(i->ip_char_ == smsg) {exist = true;continue;}
    }
    if(smsg == Local_ip1 || smsg == Local_ip2)exist = true;
    pthread_mutex_unlock(&muteofconnecter_);
    if(exist)continue;
    std::cout << "UDP :<ip="<<smsg<<"> -- now try to connect by TCP/IP"<<std::endl;
    std::thread *CL = new std::thread(Client,connector,smsg,nullptr);
    CL->detach();
    connector->Client_List_.push_back(CL);
		sleep(1);
	}
  close(nb);
}

void Connector::TCP_Server(void *C) {
  Connector *connector = (Connector *)C;
  int32_t fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd == -1) {perror("ERROR[SOCKEt]");return;}

  sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port =htons(10084);
  saddr.sin_addr.s_addr = INADDR_ANY;
  int32_t ret = bind(fd, (sockaddr *)&saddr, sizeof(saddr));
  if(ret == -1) {perror("ERROR[BIND]");return;}

  ret = listen(fd,50);

  int32_t Local_ip1 = 16777343;
  int32_t Local_ip2 = 0;

  while (true) {
    sockaddr_in caddr;
    socklen_t caddrlen = sizeof(caddr);
    int cfd = accept(fd, (sockaddr *)&caddr,&caddrlen);
    if(cfd == -1) {perror("ERROR[ACCEPT]");return;}
    pthread_mutex_lock(&muteofconnecter_);
    bool exist = false;
    for(auto i:connector->connecter_) {
      if(i->ip_ == caddr.sin_addr.s_addr) {
        exist = true;

        continue;
      }
    }
    if(caddr.sin_addr.s_addr == Local_ip1 || caddr.sin_addr.s_addr == Local_ip2)exist = true;
    if(exist) {
      pthread_mutex_unlock(&muteofconnecter_);
      continue;
    }

    Connecter *NewFrind = new Connecter;
    NewFrind->ip_char_ = inet_ntoa( ((struct sockaddr_in*)(&caddr.sin_addr))->sin_addr);
    NewFrind->ip_      = caddr.sin_addr.s_addr;
    NewFrind->fd_      = cfd;
    connector->connecter_.push_back(NewFrind);
    std::cout << "-- TCP:New Frind<ip="<<caddr.sin_addr.s_addr<<">"<<std::endl;
    pthread_mutex_unlock(&muteofconnecter_);

    std::unique_lock<std::mutex> locker(connector->Singal.Mu_Singal_newconnect);
    connector->Date.Singal_newconnect.push(NewFrind->ip_char_);
    connector->Singal.Cond_Singal_newconnect.notify_all();
    locker.unlock();

    std::thread *SE = new std::thread(Server,connector,cfd,NewFrind->ip_char_,NewFrind);
    SE->detach();
    connector->Server_List_.push_back(SE);
  }
  close(fd);

}

void Connector::Client(void *C,std::string IP,Connecter *connecter) {
  Connector *connector = (Connector *)C;
  int32_t fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd == -1) {perror("ERROR[SOCKEt]");return;}

  sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port =htons(10084);
  inet_pton(AF_INET,IP.c_str(),&saddr.sin_addr.s_addr);
  int32_t ret = connect(fd, (sockaddr *)&saddr, sizeof(saddr));
  Connecter *NewFrind = new Connecter;
  if(ret == -1) {perror("ERROR[CONNECT]");return;}
  {
    char buff[1024];
    recv(fd,buff,sizeof(buff),0);
    pthread_mutex_lock(&muteofconnecter_);
    for(auto i:connector->connecter_) {
      if(i->ip_char_ == buff){close(fd);return;}
    }
    NewFrind->ip_char_  = buff;
    NewFrind->fd_       = fd;
    inet_pton(AF_INET,buff,&NewFrind->ip_);
    connector->connecter_.push_back(NewFrind);
    pthread_mutex_unlock(&muteofconnecter_);
    std::cout << "-- TCP:New Frind<ip="<<NewFrind->ip_<<">"<<std::endl;
    std::cout <<"Client:"<<buff<<std::endl;
    std::unique_lock<std::mutex> locker(connector->Singal.Mu_Singal_newconnect);
    connector->Date.Singal_newconnect.push(buff);
    connector->Singal.Cond_Singal_newconnect.notify_all();
    locker.unlock();
  }
  connecter = NewFrind;
  while(true) {
    char buff[1024];
    recv(fd,buff,sizeof(buff),0);
    memset(buff,0,sizeof(buff));
    sprintf(buff,"hhhh");
    send(fd,buff,strlen(buff)+1,0);
  }    
  std::unique_lock<std::mutex> locker(connector->Singal.Mu_Singal_deleteconnect);
  connector->Date.Singal_deleteconnect.push(IP);
  connector->Singal.Cond_Singal_deleteconnect.notify_all();
  locker.unlock();
  close(fd);
}
void Connector::Shot_Radio(void *C) {
  Connector *connector = (Connector *)C;
  ThreadPool PL;
  PL.init();
  PL.submit(Radio,connector);
  sleep(3);
  PL.submit(Radio,connector);
  sleep(3);
  PL.submit(Radio,connector);
  while(true) {
    TooLKit::Event_T_Callback::Shend_Singal("Radio",connector);
    PL.submit(Radio,connector);
    sleep(30);
  }
  PL.shutdown();
}

void Connector::Radio(void* C) {
  Connector *connector = (Connector *)C;
  if(connector->radio_clicked_times == 0)sleep(1);
  if(connector->radio_clicked_times <= 3) {
    ++connector->radio_clicked_times;
    {
      int32_t fd = socket(AF_INET, SOCK_DGRAM, 0);
      if(fd == -1){perror("ERROR[Socket]\n");--connector->radio_clicked_times;return;}
      int sock = -1;
      if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {   
        perror("ERROR[SOCKET]");
        return ;
      }   
      const int opt = 1;
      //设置该套接字为广播类型，
      int nb = 0;
      nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
      if(nb == -1) {
        perror("ERROR[SET_SOCKET]");
        return ;
      }
      struct sockaddr_in addrto;
      bzero(&addrto, sizeof(struct sockaddr_in));
      addrto.sin_family=AF_INET;
      addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);
      addrto.sin_port=htons(10084);
      int nlen=sizeof(addrto);
        //从广播地址发送消息
        int ret=sendto(sock, 
                       connector->Local_ip_char_.c_str(), 
                       connector->Local_ip_char_.size(), 
                       0, 
                       (sockaddr*)&addrto, nlen);
        if(ret<0) {
          perror("ERROR[UDP]");
        } else {		
          printf("SUCCESS[UDP]\n");	
        }
    }
    --connector->radio_clicked_times;
  }
}

void Connector::Server(void *C,int32_t cfd,std::string IP,Connecter *connecter) {
  Connector *connector = (Connector *)C;
  send(cfd,connector->Local_ip_char_.c_str(),connector->Local_ip_char_.size(),0);
  while (true) {
    char buff[1024];
    int32_t len = recv(cfd, buff, sizeof(buff),0);
    send(cfd,buff,sizeof(buff),0);
    if(len == 0)break;
  }
  std::unique_lock<std::mutex> locker(connector->Singal.Mu_Singal_deleteconnect);
  connector->Date.Singal_newconnect.push(IP);
  connector->Singal.Cond_Singal_deleteconnect.notify_all();
  locker.unlock();
  close(cfd);
}
