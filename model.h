#pragma once
#include <stdbool.h> 

struct ipv6
{
    unsigned char part[16];
};

struct stat_header
{
    int cnt;
};

struct stat_response 
{
    struct ipv6 ip;
    unsigned int cnt;
    char iface[16];
};


bool convert_ip(char* str, struct ipv6 res);

