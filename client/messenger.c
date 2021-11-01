#include <stdbool.h>
#include "messenger.h"

bool daemon_is_up()
{
    return true;
}

bool daemon_is_sniffed()
{
    return true;
}

bool send_command(int flow, int c)
{
    return write(flow, &c, 4) != -1;
}

bool send_ip(int flow, char* raw_ip)
{
    return write(flow, raw_ip, strlen(raw_ip)) != -1;
}

bool receive_stat_header(int flow, stat_header* header)
{
    return read(flow, &header, sizeof(stat_header)) != -1;
}

bool receive_stat(int flow, int cnt, stat_response* response)
{
    return read(flow, response, sizeof(stat_response)) != -1;
}

bool send_interface(int flow, char* iface)
{
    return write(flow, &iface, strlen(iface)) != -1;
}

bool receive_interface(int flow, char* iface)
{
    return read(flow, iface, 16) != -1;
}
