#include <stdio.h>   
#include <stdlib.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <netdb.h>   
#include <arpa/inet.h>   
#include <signal.h>   
#include<string.h>   
#include <sys/ioctl.h>   
#include <linux/if.h>   
#include <unistd.h>
#include <string>

int getip(char *ip){   
  int  MAXINTERFACES=16;   
    int fd, intrface, retn = 0;   
    struct ifreq buf[MAXINTERFACES]; ///if.h   
    struct ifconf ifc; ///if.h   
     char szip[80]; 
    if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) //socket.h   
    {   
        ifc.ifc_len = sizeof buf;   
        ifc.ifc_buf = (caddr_t) buf;   
        if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) //ioctl.h   
        {   
            intrface = ifc.ifc_len / sizeof (struct ifreq);    
            while (intrface-- > 0)   
            {   
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))   
                {   
                 sprintf(ip,"%s", inet_ntoa( ((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr) );                   
                 break;   
                }   
                       
            }   
               
        }   
        close (fd);   
    }   
    return 0;   
}      

std::string Getip(){      
     char gtip[80];
     getip(gtip);         
   printf("%s\n",gtip);
 return gtip;   
}
