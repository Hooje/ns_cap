#include <bits/stdc++.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/udp.h>
#include <pcap.h>
#include "ip_header.h"
#include "gre_header.h"
//#include "mac_address.h"
using namespace std;

int main(int argc, const char * argv[]) 
{
    pcap_if_t *devices = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    char ntop_buf[256];
    struct ether_header *eptr;
    vector<pcap_if_t*> vec;
    int h1_use=0;
    int h2_use=0;
    int num=0;

    //get all devices
    if(-1 == pcap_findalldevs(&devices, errbuf)) {
        fprintf(stderr, "pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }//end if

    //list all device
    int cnt=0;
    for(pcap_if_t *d = devices; d ; d = d->next)
    {
        vec.push_back(d);
        //cout<<vec[cnt]->name<<endl;
        cout<<cnt<<" Name: "<<d->name<<endl;
        cnt++;

    }//end for

    //free
    cout<<"Insert a number to select interface"<<endl;
    cin>>cnt;
    
    cout<<"Start listening at $"<<vec[cnt]->name<<endl;
    
    string expre;

    cout<<"Insert BPF filter expression:"<<endl;

    expre="udp";
    cout<<"filter: "<<expre<<endl;

    struct bpf_program fp;
    pcap_t *handle;
    handle = pcap_open_live(vec[cnt]->name, 65535, 1, 1, errbuf);
    if(!handle|| handle == NULL)
    {
        fprintf(stderr, "pcap_open_live(): %s\n", errbuf);
        exit(1);
    }
 
    if(-1 == pcap_compile(handle, &fp, expre.c_str(), 1, PCAP_NETMASK_UNKNOWN) )
    {
        cout<<"error"<<endl;
        pcap_perror(handle, "pkg_compile compile error\n");
        exit(1);
    }
    if(-1 == pcap_setfilter(handle, &fp)) {
        pcap_perror(handle, "set filter error\n");
        exit(1);
    }
    int ccnt=0;
    while (1) 
    {

        if(ccnt>100)
            break;
        ccnt++;

        pcap_pkthdr header;
        const unsigned char* packet = pcap_next(handle, &header);


        if (packet) 
        {
            int i;
            string src_ip;
            const struct sniff_ip *ip=new sniff_ip();
            u_char *ptr=new u_char;
            ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
            //printf("Source ip address: %s  \nDestination ip address: ",  inet_ntoa(ip->ip_src));
            //cout<<inet_ntoa(ip->ip_dst)<<endl;
            src_ip = string(inet_ntoa(ip->ip_src));

            struct udphdr *udp = (struct udphdr *)(packet + SIZE_ETHERNET + SIZE_IP );
            u_int16_t source_port = ntohs(udp->uh_sport);
            u_int16_t destination_port = ntohs(udp->uh_dport);
            string src_port = to_string(source_port);
            struct grehdr *gptr=new grehdr();
            gptr = (struct grehdr *)(packet + SIZE_ETHERNET + SIZE_IP + 8 );
            u_int32_t key = gptr->gre_opts;
            //cout<<"stop here and key new: "<<key<<endl;
            //return 0;
            //string key=string(key_c);
            //cout<<source_port<<" "<<destination_port;
            //cout<<endl;
            //cout<<"print packet in hex: ";
            //for(int i=0;i<sizeof(packet);i++) printf("%x",packet[i]);
            //cout<<endl;
            //return 0;
            string command;

            //cout<<src_ip<<endl;
            if(src_ip=="140.114.0.1")
            {
                //cout<<"here"<<endl;
                if(to_string(key)==to_string(0))
                {
                    if(h1_use==0)
                    {
                        printf("add h1\n");
                        command="ip link add GRE type gretap remote "+src_ip+"  local 140.113.0.2 \
                            key 0 encap fou encap-sport 55555 encap-dport "+src_port;

                        system(command.c_str());
                        command="ip link set GRE up";
                        system(command.c_str());
                        //command="ip fou add port 55555 ipproto 47";
                        //system(command.c_str());

                        command="brctl addif br0 GRE";
                        system(command.c_str());


                        cout<<"finish h1"<<endl;
                        h1_use=1;
                    }
                }                
                if(to_string(key)!=to_string(0))
                {
                    if(h2_use==0)
                    {
                        printf("add h2\n");
                        command="ip link add GRE2 type gretap remote "+src_ip+"  local 140.113.0.2 \
                            key 1 encap fou encap-sport  55555 encap-dport "+src_port;

                        system(command.c_str());
                        command="ip link set GRE2 up";
                        system(command.c_str());

                        command="brctl addif br0 GRE2";
                        system(command.c_str());

                        cout<<"finish h2"<<endl;

                        h2_use=1;
                    }
                }
                

            }
                

            

               

            
        }

    }
    //cout<<"end while"<<endl;

    pcap_freealldevs(devices);
    return 0;
}
