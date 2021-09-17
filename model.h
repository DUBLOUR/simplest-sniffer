#pragma once
#include <stdbool.h> 

struct Ipv6
{
    unsigned char part[16];
};

struct StatHeader
{
    int cnt;
};

struct StatResponse
{
    struct Ipv6 ip;
    unsigned int cnt;
    char iface[16];
};


bool convert_ip(char* str, struct Ipv6 res);

