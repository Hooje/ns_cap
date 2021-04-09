#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, const char * argv[]) {
    pcap_if_t *devices = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    char ntop_buf[256];

    //get all devices
    if(-1 == pcap_findalldevs(&devices, errbuf)) {
        fprintf(stderr, "pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }//end if

    //list all device
    for(pcap_if_t *d = devices ; d ; d = d->next) {
        printf("Interface: %s\n", d->name);
        if(d->description) {
            printf("\tDescription: %s\n", d->description);
        }//end if
        printf("\tLoopback: %s\n",(d->flags & PCAP_IF_LOOPBACK) ? "yes" : "no");

        //list all address
        for(struct pcap_addr *a = d->addresses ; a ; a = a->next) {
            sa_family_t family = a->addr->sa_family;

            if(family == AF_INET || family == AF_INET6) {
                if(a->addr) {
                    printf("\t\tAddress: %s\n",
                           inet_ntop(family, &((struct sockaddr_in *)a->addr)->sin_addr, ntop_buf, sizeof(ntop_buf)));
                }//end if
                if(a->netmask) {
                    printf("\t\tNetmask: %s\n",
                           inet_ntop(family, &((struct sockaddr_in *)a->netmask)->sin_addr, ntop_buf, sizeof(ntop_buf)));
                }//end if
                if(a->broadaddr) {
                    printf("\t\tBroadcast Address: %s\n",
                           inet_ntop(family, &((struct sockaddr_in *)a->broadaddr)->sin_addr, ntop_buf, sizeof(ntop_buf)));
                }//end if
                if(a->dstaddr) {
                    printf("\t\tDestination Address: %s\n",
                           inet_ntop(family, &((struct sockaddr_in *)a->dstaddr)->sin_addr, ntop_buf, sizeof(ntop_buf)));
                }//end if
            }//end else

            printf("\n");
        }//end for
    }//end for

    //free
    pcap_freealldevs(devices);

    return 0;
}
