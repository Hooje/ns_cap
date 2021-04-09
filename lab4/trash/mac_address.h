typedef struct mac_header{
unsigned char fc[2];
unsigned char id[2];
unsigned char add1[6];
unsigned char add2[6];
unsigned char add3[6];
unsigned char sc[2];
}mac_header;
typedef struct frame_control{
unsigned protocol:2;
unsigned type:2;
unsigned subtype:4;
unsigned to_ds:1;
unsigned from_ds:1;
unsigned more_frag:1;
unsigned retry:1;
unsigned pwr_mgt:1;
unsigned more_data:1;
unsigned wep:1;
unsigned order:1;
}frame_control;

typedef struct beacon_header{
unsigned char timestamp[8];

unsigned char beacon_interval[2];
unsigned char cap_info[2];
}beacon_header;

void procPacket( struct pcap_pkthdr *pkthdr, const u_char *packet)
{

    char * temp;
    char ssid[32];    
    struct mac_header *p= (struct mac_header *)(packet+RADIOTAP_HEADER_SIZE);
    struct frame_control *control = (struct frame_control *) p->fc;
    temp = (char *) (packet + sizeof (struct mac_header) +sizeof (struct    beacon_header)+RADIOTAP_HEADER_SIZE);
    if ((control->protocol == 0) && (control->type == 0) && (control->subtype == 8) )  // beacon frame
    {        
         memset(ssid,0,32);
         memcpy (ssid, &temp[2], temp[1]);
         printf ("Destination Add : %s\
", ether_ntoa ((struct ether_addr *)p->add1));
         printf ("Source Add : %s\
", ether_ntoa ((struct ether_addr *)p->add2));
         printf ("BSSID : %s\
", ether_ntoa ((struct ether_addr *)p->add3));
         printf ("ssid = %s\
", ssid);
    }


}