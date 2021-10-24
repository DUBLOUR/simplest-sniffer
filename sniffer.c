#include <pcap.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/* IP header */
struct sniff_ip {
    u_char ip_vhl;		/* version << 4 | header length >> 2 */
    u_char ip_tos;		/* type of service */
    u_short ip_len;		/* total length */
    u_short ip_id;		/* identification */
    u_short ip_off;		/* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* don't fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
    u_char ip_ttl;		/* time to live */
    u_char ip_p;		/* protocol */
    u_short ip_sum;		/* checksum */
    struct in_addr ip_src,ip_dst; /* source and dest address */
};
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)
#define SIZE_ETHERNET 14


typedef uint32_t ip_addr;

char* str_from_ip(ip_addr addr) {
    char *str = malloc(15 * sizeof(char));
    sprintf(str, "%d.%d.%d.%d",
            addr>>24,
            (addr>>16)&255,
            (addr>>8)&255,
            addr&255);
    return str;
}


int ip_from_str(char* str, ip_addr* ip) {
    *ip = 0;
    int sz = strlen(str);
    if (sz > 15) return -1;
    int current_part = 0, parts_cnt = 0;
    for (int i=0; i<sz; ++i) {
        if (isdigit(str[i])) {
            current_part = current_part * 10 + str[i]-'0';
            if (current_part > 255)
                return -1;
        } else if (str[i] == '.') {
            ++parts_cnt;
            (*ip) = ((*ip)<<8) | current_part;
            current_part = 0;
        } else
            return -1;
    }
    (*ip) = ((*ip)<<8) | current_part;

    if (parts_cnt != 3)
        return -1;

    return 0;
}


void another_callback(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{

    static int count=0; ++count;
    const struct sniff_ip *ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    u_int size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }
    
    printf("%d-th package is from %s\n", count, inet_ntoa(ip->ip_src));

}





int main()
{
    char *dev;
    pcap_if_t *alldevsp;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr;
    struct bpf_program fp;
    bpf_u_int32 maskp;
    bpf_u_int32 netp;


    if(pcap_findalldevs(&alldevsp,errbuf)==-1) {
        printf("\nerror in pcap findall devs");
        return -1;
    }

    printf("The interfaces present on the system are:");
    for(pcap_if_t *temp=alldevsp; temp; temp = temp->next){
        static int i = 0;
        printf("\n%d : %s", i++, temp->name);
    }

    dev = alldevsp->name;

    if(dev == NULL) {
        fprintf(stderr, "%s\n", errbuf);
        exit(1);
    }

    pcap_lookupnet(dev, &netp, &maskp, errbuf);

    fprintf(stdout, "dev: %s\nnet: %0x\nmask: %x\n", dev, netp, maskp);

    descr = pcap_open_live(dev, BUFSIZ, 1,-1, errbuf);
    if(descr == NULL) {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    if(pcap_compile(descr, &fp, "ip", 0, netp) == -1) {
        fprintf(stderr, "Error calling pcap_compile\n");
        exit(1);
    }

    if(pcap_setfilter(descr, &fp) == -1) {
        fprintf(stderr, "Error setting filter\n");
        exit(1);
    }

    pcap_loop(descr, -1, another_callback, NULL);
    return 0;
}