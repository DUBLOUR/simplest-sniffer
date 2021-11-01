#include <pcap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "sniffer.h"

/* IP header */
struct sniff_ip {
    u_char ip_vhl;		/* version << 4 | header length >> 2 */
    u_char ip_tos;		/* type of service */
    u_short ip_len;		/* total length */
    u_short ip_id;		/* identification */
    u_short ip_off;		/* fragment offset field */
    u_char ip_ttl;		/* time to live */
    u_char ip_p;		/* protocol */
    u_short ip_sum;		/* checksum */
    struct in_addr ip_src,ip_dst; /* source and dest address */
};
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)
#define SIZE_ETHERNET 14


struct in_addr empty_addr()
{
    struct in_addr *addr = malloc(sizeof(struct in_addr));
    addr->s_addr = 0;
    return *addr;
}

struct in_addr source_extractor(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    const struct sniff_ip *ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    u_int size_ip = IP_HL(ip)*4;
    if (size_ip < 20)
        return empty_addr();
    return ip->ip_src;
}


char** get_device_list()
{
    pcap_if_t *alldevsp;
    char errbuf[PCAP_ERRBUF_SIZE];

    if(pcap_findalldevs(&alldevsp,errbuf)==-1) {
        printf("\nerror in pcap findall devs");
        exit(1);
    }

    int cnt_devs = 0;

    for(pcap_if_t *temp=alldevsp; temp; temp = temp->next)
        ++cnt_devs;

    printf("Available devices:\n");
    char **lst = (char **)malloc(cnt_devs * sizeof(char*));
    for(pcap_if_t *temp=alldevsp; temp; temp = temp->next) {
        static int i = 0;
        lst[i] = (char *)malloc(strlen(temp->name) * sizeof(char));
        strcpy(lst[i], temp->name);
        i++;
        printf("%d : %s\n", i, temp->name);
    }

    return lst;
}


void sniff(char* device, pcap_handler handler)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 maskp;
    bpf_u_int32 netp;
    pcap_lookupnet(device, &netp, &maskp, errbuf);

    fprintf(stdout, "dev: %s\nnet: %0x\nmask: %x\n", device, netp, maskp);


    pcap_t* descr = pcap_open_live(device, BUFSIZ, 1,-1, errbuf);
    if(descr == NULL) {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    struct bpf_program fp;
    if(pcap_compile(descr, &fp, "ip", 0, netp) == -1) {
        fprintf(stderr, "Error calling pcap_compile\n");
        exit(1);
    }

    if(pcap_setfilter(descr, &fp) == -1) {
        fprintf(stderr, "Error setting filter\n");
        exit(1);
    }

    pcap_loop(descr, -1, handler, NULL);
}
