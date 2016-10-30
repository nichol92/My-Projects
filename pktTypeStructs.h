/*used to store mac adresses in packet structures */

struct uint48
{
  unsigned int i;
  unsigned short s;
};

struct deauth_pkt
{
  unsigned short frame_control;
  unsigned short duration;
  struct uint48 dest_addr;
  struct uint48 src_addr;
  struct uint48 bssid;
  unsigned short seqctl;
  unsigned short reason_code;
};
