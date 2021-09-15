#include <stdio.h> 
#include <string.h>
#include <stdbool.h> 

const int INVALID_INPUT_CODE = 127;

enum DAEMON_COMMAND
{
    DSTART_SNIFF,
    DSTOP_SNIFF,
    DSHOW_IPV6,
    DSELECT,
    DSTAT,
    DSTAT_FILTERED
};

struct ipv6
{
    unsigned char part[16];
};

struct stat_header
{
    int cnt;
};

struct stat_response 
{
    struct ipv6 ip;
    unsigned int cnt;
    char iface[16];
};

bool daemon_is_up() {return true;}
bool print_help_daemon_is_down() {return true;}
bool daemon_is_sniffed() {return true;}
bool send_command(int c) {return true;}
bool send_ip(struct ipv6 res) {return true;}
bool recive_stat_header(struct stat_header header) {return true;}
bool recive_stat(int cnt, struct stat_response response[]) {return true;}
bool send_intefrace(char* iface) {return true;}
bool recive_interface(char* iface) {return true;}


bool convert_ip(char* str, struct ipv6 res) {return true;}

void print_stat(int cnt, struct stat_response rows[]) {}

void print_select_iface(char* iface) {
    printf("%s is default now", iface);
}

void print_help_invalid_ip() {
    printf("IP-adress is invalid\n");
}

int handle_start() {
    printf("start sniff\n");
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    if (daemon_is_sniffed()) 
        return 0;

    send_command(DSTART_SNIFF);
    
    return daemon_is_sniffed();
}

int handle_stop() {
    printf("stop sniff\n");
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    if (!daemon_is_sniffed()) 
        return 0;

    send_command(DSTOP_SNIFF);
    
    return !daemon_is_sniffed();
}

int handle_show(char *raw_ip) {
    printf("show %s\n", raw_ip);

    struct ipv6 ip;
    if (!convert_ip(raw_ip, ip)) {
        print_help_invalid_ip();
        return INVALID_INPUT_CODE;
    }

    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    send_command(DSHOW_IPV6);
    send_ip(ip);

    struct stat_response response[1];
    if (!recive_stat(1, response)) 
        return 1;

    print_stat(1, response);
    return 0;
}

int handle_select(char *iface) {
    printf("select %s\n", iface);
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    send_command(DSELECT);
    send_intefrace(iface);

    char *new_iface;
    if (!recive_interface(new_iface)) 
        return 1;

    print_select_iface(new_iface);
    return !!strcmp(new_iface, iface);
}

int handle_stat(char *iface) {
    printf("stat %s\n", iface);
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    send_command(DSTAT_FILTERED);
    send_intefrace(iface);

    struct stat_header header;
    if (!recive_stat_header(header)) 
        return 1;
    
    struct stat_response response[header.cnt];
    if (!recive_stat(header.cnt, response)) 
        return 1;

    print_stat(header.cnt, response);
    return 0;
}

int handle_stat_all() {
    printf("stat all");
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    send_command(DSTAT);
    
    struct stat_header header;
    if (!recive_stat_header(header)) 
        return 1;
    
    struct stat_response response[header.cnt];
    if (!recive_stat(header.cnt, response)) 
        return 1;

    print_stat(header.cnt, response);
    return 0;
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
        if (argc == 2) 
            return handle_stat_all();
        if (argc == 3) 
            return handle_stat(argv[2]);
        print_help_stat();
        return INVALID_INPUT_CODE;
    }

    if (!strcmp(command, "--help")) {
        print_help_general();
        return 0;
    }

    print_help_general();
    return INVALID_INPUT_CODE;
}