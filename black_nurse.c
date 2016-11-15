#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "hacking.h"
unsigned short checksum(void *b, int len)
{	unsigned short *buf = b;
	unsigned int sum=0;
	unsigned short result;

	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++;
	if ( len == 1 )
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

struct ICMP {
  unsigned char type;
  unsigned char code;
  unsigned short check_sum;
  unsigned int  payload;
  
};
struct ip {
  
  unsigned char header_len:4, ver:4, type;
  unsigned short total_len, id, frag_off;
  unsigned char ttl, protocol;
  unsigned short check;
  unsigned int src_addr, dst_addr; 
};
struct TCP {
  unsigned short src_port;
  unsigned short dst_port;
  unsigned int seq;
  
};

int main(int argc, char *argv[])
{
  struct ip *ip1;
  struct ip *ip2;
  struct TCP *tcp;
  struct ICMP *icmp;
  char pkt[48];
  memset(pkt,0,48);
  ip1 = (struct ip *) &pkt;
  icmp = (struct ICMP *)&pkt[sizeof(struct ip)];
  ip2 = (struct ip *) &pkt[sizeof(struct ip) + sizeof(struct ICMP)];
  tcp = (struct TCP *) &pkt[sizeof(struct ip) + sizeof(struct ICMP) + sizeof(struct ip)];
  icmp->type = 3;
  icmp->code = 3;
  icmp->check_sum = checksum((void *)icmp, sizeof(struct ICMP));
  icmp->payload = 0;
  

  dump((const unsigned char *) &pkt, 48);
  
   
    
}
