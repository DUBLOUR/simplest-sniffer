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
bool send_ip(struct ipv6 res) {
    return true;
}
bool recive_stat_header(struct stat_header header) {
    return true;
}
bool recive_stat(int cnt, struct stat_response response[]) {
    return true;
}
bool send_intefrace(char* iface) {
    return true;
}
bool recive_interface(char* iface) {
    return true;
}
