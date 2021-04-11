#include <bits/stdc++.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
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
    //getline(cin, expre);
    cin.ignore();
    getline(cin, expre);
    //expre="ip proto gre";
    cout<<"filter: "<<expre<<endl;

    struct bpf_program fp;
    pcap_t *handle;
    handle = pcap_open_live(vec[cnt]->name, 65535, 1, 1, errbuf);
    if(!handle|| handle == NULL)
    {
        fprintf(stderr, "pcap_open_live(): %s\n", errbuf);
        exit(1);
    }
    //cout<<21<<endl;
    //int pcap_compile(pcap_t *p, struct bpf_program *fp, const char *str, int optimize, bpf_u_int32 netmask);

    //netmask 0xFF000000, 25
 
    if(-1 == pcap_compile(handle, &fp, expre.c_str(), 1, PCAP_NETMASK_UNKNOWN) )
    {
        cout<<"error"<<endl;
        pcap_perror(handle, "pkg_compile compile error\n");
        exit(1);
    }
    if(-1 == pcap_setfilter(handle, &fp)) {
        pcap_perror(handle, "set filter error\n");
        exit(1);
    }//end if
    //cout<<"before while"<<endl;
    int ccnt=0;
    while (1) 
    {

        if(ccnt>100)
            break;
        ccnt++;

        pcap_pkthdr header;
        const unsigned char* packet = pcap_next(handle, &header);

        if(packet == NULL)
        {/* dinna work *sob* */
            printf("Didn't grab packet\n");
            exit(1);
        }
        //cout<<"packet get"<<endl;


        if (packet) 
        {
            int i;
            const struct sniff_ip *ip=new sniff_ip();
            u_char *ptr=new u_char;
            ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);

            if(h1_use==1)
            {
                //cout<<"h1 have used"<<endl;
                string src_ip(inet_ntoa(ip->ip_src));
                
                string dst_ip(inet_ntoa(ip->ip_dst));
                if(src_ip=="140.114.0.1" || dst_ip=="140.114.0.1") 
                {
                    //cout<<"filter h1"<<endl;
                    continue;
                }
            }
            if(h2_use==1)
            {   
                //cout<<"h2 have used"<<endl;
                string src_ip(inet_ntoa(ip->ip_src));
                
                string dst_ip(inet_ntoa(ip->ip_dst));
                if(src_ip=="140.115.0.1" || dst_ip=="140.115.0.1")
                {
                    //cout<<"filter h2"<<endl;
                    continue;
                }
            }
            
            cout<<endl;
            cout<<"print packet in hex: ";
            for(int i=0;i<sizeof(packet);i++) printf("%x",packet[i]);
            cout<<endl;
        
            eptr = (struct ether_header *) packet;
            
            ptr = eptr->ether_dhost;
            i = ETHER_ADDR_LEN;

            printf("Destination ethernet Address:  ");
            do{
                printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
            }while(--i>0);
            printf("\n");

            ptr = eptr->ether_shost;
            i = ETHER_ADDR_LEN;
            printf("Source ethernet Address:  ");
            do{
                printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
            }while(--i>0);
            cout<<endl;
            if (ntohs (eptr->ether_type) == ETHERTYPE_IP)
            {
                cout<<"ether net type: 0800"<<endl;
            }
            else if (ntohs (eptr->ether_type) == ETHERTYPE_IPV6)
            {
                cout<<"ether net type: 08dd"<<endl;
            }
            else
                cout<<"else type"<<endl;
            /*printf("Grabbed packet of length %d\n",header.len);
            printf("Recieved at ..... %s\n",ctime((const time_t*)&header.ts.tv_sec)); 
            printf("Ethernet address length is %d\n",ETHER_HDR_LEN);*/

            //const struct sniff_ip *ip=new sniff_ip();
            u_int size_ip;
            ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
            size_ip = IP_HL(ip)*4;
            if (size_ip < 20) 
            {
                printf("   * Invalid IP header length: %u bytes\n", size_ip);
                return 0;
            }
            printf("Source ip address: %s  \nDestination ip address: ",  inet_ntoa(ip->ip_src));
            cout<<inet_ntoa(ip->ip_dst)<<endl;
            char str[]="140.113.0.1";
            
            const struct grehdr *gptr=new grehdr();
            gptr = (struct grehdr *)(packet + SIZE_ETHERNET + SIZE_IP);
            printf("gre protocal type:  %x \n",  ntohs(gptr->gre_proto));
            //cout<<dec<<size_gre<<endl;
            eptr = (struct ether_header *)(packet  + SIZE_ETHERNET + SIZE_IP + SIZE_GRE);
            ptr = eptr->ether_dhost;
            i = ETHER_ADDR_LEN;
            printf("Inner Destination ethernet Address:  ");
            do{
                printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
            }while(--i>0);
            printf("\n");

            ptr = eptr->ether_shost;
            i = ETHER_ADDR_LEN;
            printf("Inner Source ethernet Address:  ");
            do{
                printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
            }while(--i>0);
            cout<<endl;
            cout<<"Inner ethernet type : "<<hex<<ntohs (eptr->ether_type) <<endl;

            string command;
            string remote_ip(inet_ntoa(ip->ip_src));
            string source_ip(inet_ntoa(ip->ip_dst));
            if (strcmp(inet_ntoa(ip->ip_dst), str)!=0)//different    destination is not my self
            {
                remote_ip=source_ip;
                cout<<"change "<<remote_ip<<endl;
            }
            if(remote_ip=="140.114.0.1")
            {
                if(h1_use==0)
                {
                    h1_use=1;
                    //cout<<"h1"<<endl;
                    if(num==0)
                    {
                        command = "ip link add GRE type gretap remote "+remote_ip+" local 140.113.0.1";
                        system(command.c_str());
                        command = "ip link set GRE up";
                        system(command.c_str());
                        command ="ip link add br0 type bridge";
                        system(command.c_str());
                        command ="brctl addif br0 BRGr-eth0";
                        system(command.c_str());
                        command ="brctl addif br0 GRE";
                        system(command.c_str());
                        command ="ip link set br0 up";
                        system(command.c_str());
                        num++;
                    }
                    else
                    {
                        command = "ip link add GRE type gretap remote "+remote_ip+" local 140.113.0.1";
                        system(command.c_str());
                        command = "ip link set GRE up";
                        system(command.c_str());

                        command ="brctl addif br0 GRE";
                        system(command.c_str());
                        command ="ip link set br0 up";
                        system(command.c_str());
                    }
                

                    expre=expre+" and src host not 140.114.0.1 and dst host not 140.114.0.1";
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
                }
            }
            else if(remote_ip=="140.115.0.1")
            {
                if(h2_use==0)
                {
                    h2_use=1;
                    //cout<<"h2"<<endl;
                    if(num==0)
                    {
                        
                        command = "ip link add GRE2 type gretap remote "+remote_ip+" local 140.113.0.1";
                        system(command.c_str());
                        command = "ip link set GRE2 up";
                        system(command.c_str());
                        command ="ip link add br0 type bridge";
                        system(command.c_str());
                        command ="brctl addif br0 BRGr-eth0";
                        system(command.c_str());
                        command ="brctl addif br0 GRE2";
                        system(command.c_str());
                        command ="ip link set br0 up";
                        system(command.c_str());
                        num++;
                    }
                    else
                    {
                        command = "ip link add GRE2 type gretap remote "+remote_ip+" local 140.113.0.1";
                        system(command.c_str());
                        command = "ip link set GRE2 up";
                        system(command.c_str());

                        command ="brctl addif br0 GRE2";
                        system(command.c_str());
                        command ="ip link set br0 up";
                        system(command.c_str());

                    }
                    expre=expre+" and src host not 140.115.0.1 and dst host not 140.115.0.1";
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
                }

            }
            else
            {
                command = "ip link add GRE3 type gretap remote "+remote_ip+" local 140.113.0.1";
                system(command.c_str());
                command = "ip link set GRE3 up";
                system(command.c_str());

                command ="brctl addif br0 GRE3";
                system(command.c_str());
                command ="ip link set br0 up";
                system(command.c_str());
                
                expre=expre+" and src host not "+remote_ip+" and dst host not "+remote_ip;
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

            }
                

            
        }

    }
    //cout<<"end while"<<endl;

    pcap_freealldevs(devices);
    return 0;
}
