#include <stdio.h>
#include "usage_information.h"

void print_help_general()
{
    printf("\nUsage: ssniffer [command]\n\n"
           "Available command:\n"
           "    start (NOT IMPLEMENTED)\n"
           "    stop (NOT IMPLEMENTED)\n"
           "    show [ip] count\n"
           "    select iface [iface] (NOT IMPLEMENTED)\n"
           "    stat [iface] (NOT IMPLEMENTED)\n"
           "    --help\n\n"
           );
}

void print_help_start()
{
    printf("Usage: ssniffer start\n");
}

void print_help_stop()
{
    printf("Usage: ssniffer stop\n");
}

void print_help_show()
{
    printf("Usage: ssniffer show [ip] count\n");
}

void print_help_select()
{
    printf("Usage: ssniffer select iface [iface]\n");
}

void print_help_stat()
{
    printf("Usage: ssniffer stat [iface]\n");
}

void print_help_invalid_ip()
{
    printf("IP address is invalid\n");
}

void print_help_daemon_is_down()
{
    printf("Daemon (ssnifferd) is down\n");
}