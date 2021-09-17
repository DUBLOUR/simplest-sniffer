#include "messenger.h"
#include <stdbool.h>


bool daemon_is_up() {
    return true;
}
bool daemon_is_sniffed() {
    return true;
}
bool send_command(int c) {
    return true;
}
bool send_ip(struct Ipv6 res) {
    return true;
}
bool receive_stat_header(struct StatHeader header) {
    return true;
}
bool receive_stat(int cnt, struct StatResponse response[]) {
    return true;
}
bool send_interface(char* iface) {
    return true;
}
bool receive_interface(char* iface) {
    return true;
}
