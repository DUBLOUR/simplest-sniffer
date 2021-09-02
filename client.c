#include <stdio.h> 
#include "string.h"

const int INVALID_INPUT_CODE = 127;

int handle_start() {
    printf("start sniff\n");
    return 0;
}

int handle_stop() {
    printf("stop sniff\n");
    return 0;
}

int handle_show(char *ip) {
    printf("show %s\n", ip);
    return 0;
}

int handle_select(char *iface) {
    printf("select %s\n", iface);
    return 0;
}

int handle_stat(char *iface) {
    printf("stat %s\n", iface);
    return 1;
}




void print_help_general() {
    printf("help_general\n");
}

void print_help_start() {
    printf("help_start\n");
}

void print_help_stop() {
    printf("help_stop\n");
}

void print_help_show() {
    printf("help_show\n");
}

void print_help_select() {
    printf("help_select\n");
}

void print_help_stat() {
    printf("help_stat\n");
}


int main (int argc, char **argv) 
{ 
    // printf("%d\n", argc);
    // for (int i=0; i<argc; ++i) 
    //     printf("%d: %s\n", i, argv[i]);


    if (argc == 1) {
        print_help_general();
        return 0;
    }

    char *command = argv[1];
    if (!strcmp(command, "start")) {
        if (argc != 2) {
            print_help_start();
            return INVALID_INPUT_CODE;
        }
        return handle_start();
    }    

    if (!strcmp(command, "stop")) {
        if (argc != 2) {
            print_help_stop();
            return INVALID_INPUT_CODE;
        }
        return handle_stop();
    }
    
    if (!strcmp(command, "show")) {
        if (argc != 4 || strcmp(argv[3], "count")) {
            print_help_show();
            return INVALID_INPUT_CODE;
        }
        return handle_show(argv[2]);
    }
    
    if (!strcmp(command, "select")) {
        if (argc != 4 || strcmp(argv[2], "iface")) {
            print_help_select();
            return INVALID_INPUT_CODE;
        }
        return handle_select(argv[3]);
    }

    if (!strcmp(command, "stat")) {
        if (argc != 3) {
            print_help_stat();
            return INVALID_INPUT_CODE;
        }
        return handle_stat(argv[2]);
    }

    if (!strcmp(command, "--help")) {
        print_help_general();
        return 0;
    }

    print_help_general();
    return INVALID_INPUT_CODE;
}