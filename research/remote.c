/*
 * GoGOCam UDP Remote.
 * rpkish 9/21/16
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

// the packets...
char npw[15]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4e,0x70,0x77,0x00,0x01};
char st[13]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0x74};
char sh_str[14]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x53,0x48,0x01};
char sh_stp[14]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x53,0x48,0x00};

void die(char *s) {
  perror(s);
  exit(1);
  }

void sndpkt(char *dst_ip, char *cmd) {

  const char* hostname=dst_ip; 
  const char* portname="8484";
  struct addrinfo sock;
  memset(&sock,0,sizeof(sock));
  sock.ai_family=AF_UNSPEC;
  sock.ai_socktype=SOCK_DGRAM;
  sock.ai_protocol=0;
  sock.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;
  struct addrinfo* res=0;
  int err=getaddrinfo(hostname,portname,&sock,&res);
  if (err!=0) {
    die("socket operation failed");

  }

  int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if (fd==-1) {
    die("socket operation failed");
  }

  int broadcast=1;
  if (setsockopt(fd,SOL_SOCKET,SO_BROADCAST, &broadcast,sizeof(broadcast))==-1) {
    //die("socket");
  }

  if(strcmp(cmd, "npw") == 0) {
    //printf("Sending Npw to Camera\n"); 
    if (sendto(fd,npw,sizeof(npw),0,
      res->ai_addr,res->ai_addrlen)==-1) {
      die("socket: failed to send 'Npw'");
    }
  } else if(strcmp(cmd, "st") == 0) {
     printf("Sending st to Camera\n"); 
    if (sendto(fd,st,sizeof(st),0,
      res->ai_addr,res->ai_addrlen)==-1) {
      die("socket: failed to send st");
    }
  } else if(strcmp(cmd, "sh_str") == 0) {
     printf("Sending sh (start) to Camera\n"); 
    if (sendto(fd,sh_str,sizeof(sh_str),0,
      res->ai_addr,res->ai_addrlen)==-1) {
      die("socket: failed to send sh ");
    }
  } else if(strcmp(cmd, "sh_stp") == 0) {
     printf("Sending sh (stop) to Camera\n"); 
    if (sendto(fd,sh_stp,sizeof(sh_stp),0,
      res->ai_addr,res->ai_addrlen)==-1) {
      die("socket: failed to send sh");
    }
  } else {
      printf("Command not implemented: %s.\n", cmd);
  }

  close(fd);
}

void setup(char *broadcast) {

  char *str = broadcast, *str2;
  unsigned char ip[4] = {0};
  size_t index = 0;
  int counter;

  str2 = str; 
  while (*str) {
    if (isdigit((unsigned char)*str)) {
      ip[index] *= 10;
      ip[index] += *str - '0';
    } else {
      index++;
    }
      str++;
    }
    ip[3]=0;
    for ( counter = 0; counter < 254; ++counter ) {
      ip[3] = ( ++ ip[3] % 256 );
      if ( !ip[3] ) {
        ip[2] = ( ++ ip[2] % 256 );
      }
      char ipv[16] = {0};
      snprintf(ipv, 16, "%d.%d.%d.%d", ip[0],ip[1],ip[2],ip[3]);
      sndpkt(ipv,"npw");
    }
 }

int main( int argc, char *argv[] )  {

  char *cmd = argv[1];
  char *broadcast = argv[2];

   if( argc == 3 ) {
    setup(broadcast);
    sleep(2);
    sndpkt(broadcast,"st");
     if(strcmp(cmd, "start") == 0){
       sndpkt(broadcast,"sh_str");
     }
     else if(strcmp(cmd, "stop") == 0){
       sndpkt(broadcast,"sh_stp");
     }
   }
   else {
     printf("Usage: ./remote start|stop <class c broadcast address>\n");
   }

}
