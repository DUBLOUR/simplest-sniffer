#include <pcap.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>


void handle_ip(struct in_addr ip);
struct in_addr source_extractor(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet);
char** get_device_list();
void sniff(char* device, pcap_handler handler);
