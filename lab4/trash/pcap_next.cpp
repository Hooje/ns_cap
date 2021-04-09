int main(int argc, char** argv)
{
    int          ret;
	int		     inum;
	int		     port;
	int		     i = 0;
    char         err[PCAP_ERRBUF_SIZE];
	char         filter[32];
	char         loopback = 0;
	char        *server = NULL;
	struct       bpf_program fp;
	pcap_if_t   *d;
	pcap_addr_t *a;

    signal(SIGINT, sig_handler);

#ifndef _WIN32
    ssl_InitSniffer();
#endif
    ssl_Trace("./tracefile.txt", err);

	if (pcap_findalldevs(&alldevs, err) == -1)
		err_sys("Error in pcap_findalldevs");

	for (d = alldevs; d; d=d->next) {
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if (i == 0)
		err_sys("No interfaces found! Make sure pcap or WinPcap is installed");

	printf("Enter the interface number (1-%d): ", i);
	scanf("%d", &inum);

	if (inum < 1 || inum > i)
		err_sys("Interface number out of range");

	/* Jump to the selected adapter */
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

	pcap = pcap_create(d->name, err);

    if (pcap == NULL) printf("pcap_create failed %s\n", err);

	if (d->flags & PCAP_IF_LOOPBACK)
		loopback = 1;

	/* get an IPv4 address */
	for (a = d->addresses; a; a = a->next) {
		switch(a->addr->sa_family)
		{
			case AF_INET:
				server =iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr);
				printf("server = %s\n", server);
				break;
		}
	}
	if (server == NULL)
		err_sys("Unable to get device IPv4 address");

    ret = pcap_set_snaplen(pcap, 65536);
    if (ret != 0) printf("pcap_set_snaplen failed %s\n", pcap_geterr(pcap));

    ret = pcap_set_timeout(pcap, 1000); 
    if (ret != 0) printf("pcap_set_timeout failed %s\n", pcap_geterr(pcap));

    ret = pcap_set_buffer_size(pcap, 1000000); 
    if (ret != 0)
		printf("pcap_set_buffer_size failed %s\n", pcap_geterr(pcap));

    ret = pcap_set_promisc(pcap, 1); 
    if (ret != 0) printf("pcap_set_promisc failed %s\n", pcap_geterr(pcap));


    ret = pcap_activate(pcap);
    if (ret != 0) printf("pcap_activate failed %s\n", pcap_geterr(pcap));

	printf("Enter the port to scan: ");
	scanf("%d", &port);

	SNPRINTF(filter, sizeof(filter), "tcp and port %d", port);

	ret = pcap_compile(pcap, &fp, filter, 0, 0);
    if (ret != 0) printf("pcap_compile failed %s\n", pcap_geterr(pcap));

    ret = pcap_setfilter(pcap, &fp);
    if (ret != 0) printf("pcap_setfilter failed %s\n", pcap_geterr(pcap));

    ret = ssl_SetPrivateKey(server, port, "../../certs/server-key.pem",
                            FILETYPE_PEM, NULL, err);
    if (ret != 0)
        err_sys(err);

    while (1) {
        struct pcap_pkthdr header;
        const unsigned char* packet = pcap_next(pcap, &header);
        if (packet) {

            byte data[65535];

            if (header.caplen > 40)  { /* min ip(20) + min tcp(20) */
				int frame = ETHER_IF_FRAME_LEN;
				if (loopback)
					frame = LOCAL_IF_FRAME_LEN;
				packet        += frame;
				header.caplen -= frame;					
            }
            else
                continue;

            ret = ssl_DecodePacket(packet, header.caplen, data, err);
            if (ret < 0)
                printf("ssl_Decode ret = %d\n", ret);
            if (ret > 0) {
                data[ret] = 0;
				printf("SSL App Data:%s\n", data);
            }
        }
    }

    return 0;
}