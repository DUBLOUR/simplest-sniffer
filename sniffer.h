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
    u_char ip_ttl;		/* time to live */
    u_char ip_p;		/* protocol */
    u_short ip_sum;		/* checksum */
    struct in_addr ip_src,ip_dst; /* source and dest address */
};
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)
#define SIZE_ETHERNET 14

typedef uint32_t ip_addr;

void handle_ip(struct in_addr ip);
struct in_addr source_extractor(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet);
//void *new_packet_handler(void()*);
char** get_device_list();
void print_inet_devices(char** devs);
void sniff(char* device, pcap_handler handler);
