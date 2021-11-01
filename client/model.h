#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../common/common.h"

struct StatHeader {
    int cnt;
};

bool is_valid_ip(char* raw_ip);
void print_stat(struct StatResponse* stat);
void print_stats(int cnt, struct StatResponse** stats);
void print_select_iface(char* iface);
