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
bool send_ip(struct ipv6 res);
bool recive_stat_header(struct stat_header header);
bool recive_stat(int cnt, struct stat_response response[]);
bool send_intefrace(char* iface);
bool recive_interface(char* iface);
