#include <stdlib.h>
#include <stdio.h>
#include "model.h"


bool is_valid_ip(char* raw_ip)
{
    struct in_addr* addr = malloc(sizeof(struct in_addr));
    bool valid = inet_aton(raw_ip, addr) != 0;
    free(addr);
    return valid;
}

void print_stat(stat_response* stat)
{
    printf("dev: %s\nip: %s\npackets: %d\n",
           stat->iface,
           inet_ntoa(stat->ip),
           stat->cnt);
}

void print_stats(int cnt, stat_response** stats)
{
    for (int i=0; i<cnt; ++i)
        print_stat(stats[i]);
}


void print_select_iface(char* iface)
{
    printf("%s is default now\n", iface);
}

