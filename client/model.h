#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../common/common.h"

bool is_valid_ip(char* raw_ip);
void print_stat(stat_response* stat);
void print_stats(int cnt, stat_response** stats);
void print_select_iface(char* iface);
