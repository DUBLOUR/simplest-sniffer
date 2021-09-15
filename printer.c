#include <stdio.h>
#include "printer.h"


void print_stat(int cnt, struct stat_response rows[]) {
    printf("Print %d rows:\n", cnt);
}

void print_select_iface(char* iface) {
    printf("%s is default now\n", iface);
}
