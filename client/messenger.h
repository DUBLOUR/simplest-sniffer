#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "model.h"


bool daemon_is_up();
bool daemon_is_sniffed();
bool send_command(int flow, int c);
bool send_ip(int flow, char* raw_ip);
bool receive_stat_header(int flow, stat_header* header);
bool receive_stat(int flow, int cnt, stat_response response[]);
bool send_interface(int flow, char* iface);
bool receive_interface(int flow, char* iface);
