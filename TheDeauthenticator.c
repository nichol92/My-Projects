#include "pktTypeStructs.h"
#include "hacking.h"
#include <pcap.h>
/* This is the array for building the ieee packets ie deauth and broadcast*/
typedef unsigned char u8;

static const u8 BobTheBuilder[] = {
	0xC0, 0x00, 0x3A, 0x01,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC0, 0x05, 0xC2, 0x69, 0xDC, 0x29,
	0xC0, 0x05, 0xC2, 0x69, 0xDC, 0x29,
	0x00, 0x00, 0x07, 0x00
};
struct opt {
  unsigned char bssid[6];
  unsigned char src_addr[6];
  unsigned char dst_addr[6];
 
};
int main(int argc, unsigned char *argv[])
{
  unsigned char buff[30];
  struct opt mac;
  struct deauth_pkt pkt;
  struct deauth *data;
  unsigned int iMac[6];
  char errbuf[PCAP_ERRBUF_SIZE];
  char *device;
  int i;
   sscanf(argv[1], "%x:%x:%x:%x:%x:%x", &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]);
  for(i=0;i<6;i++)
    {
      mac.bssid[i] = (unsigned char)iMac[i];
    }
   sscanf(argv[2], "%x:%x:%x:%x:%x:%x", &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]);
  for(i=0;i<6;i++)
    {
      mac.src_addr[i] = (unsigned char)iMac[i];
    }
  device = argv[3];

  
  
  dump((const unsigned char *) BobTheBuilder, 26);/*DEBUGGING*/

  
    
}
