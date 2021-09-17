#include <stdbool.h> 
#include "model.h"

enum DAEMON_COMMAND
{
    DSTART_SNIFF,
    DSTOP_SNIFF,
    DSHOW_IPV6,
    DSELECT,
    DSTAT,
    DSTAT_FILTERED
};



bool daemon_is_up();
bool daemon_is_sniffed();
bool send_command(int c);
bool send_ip(struct Ipv6 res);
bool receive_stat_header(struct StatHeader header);
bool receive_stat(int cnt, struct StatResponse response[]);
bool send_interface(char* iface);
bool receive_interface(char* iface);
