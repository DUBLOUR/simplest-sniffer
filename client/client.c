#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "usage_information.h"
#include "messenger.h"
const int INVALID_INPUT_CODE = 127;

int fd_write, fd_read;

void init_connection()
{
    char *myfifo0 = "/tmp/myfifo00";
    mknod(myfifo0, S_IFIFO|0666, 0);
    fd_write = open(myfifo0, O_WRONLY|O_CREAT);

    char *myfifo1 = "/tmp/myfifo11";
    mknod(myfifo1, S_IFIFO|0666, 0);
    fd_read = open(myfifo1, O_RDONLY|O_CREAT);
}


int handle_start()
{
    printf("start sniff\n");
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    if (daemon_is_sniffed())
        return 0;

    send_command(fd_write, DSTART_SNIFF);

    return daemon_is_sniffed();
}


int handle_stop()
{
    printf("stop sniff\n");
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    if (!daemon_is_sniffed())
        return 0;

    send_command(fd_write, DSTOP_SNIFF);

    return !daemon_is_sniffed();
}


int handle_show(char *raw_ip)
{
    printf("show %s\n", raw_ip);

    if (!is_valid_ip(raw_ip)) {
        print_help_invalid_ip();
        return INVALID_INPUT_CODE;
    }

    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    send_command(fd_write, DSHOW_IPADDR);
    send_ip(fd_write, raw_ip);

    struct StatResponse response[1];
    if (!receive_stat(fd_read, 1, response))
        return 1;

    print_stat(response);
    return 0;
}


int handle_select(char *iface)
{
    printf("select %s\n", iface);
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    send_command(fd_write, DSELECT);
    send_interface(fd_write, iface);

    char *new_iface;
    if (!receive_interface(fd_read, new_iface))
        return 1;

//    print_select_iface(new_iface);
    return strcmp(new_iface, iface) != 0;
}


int handle_stat(char *iface)
{
    printf("stat %s\n", iface);
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    send_command(fd_write, DSTAT_FILTERED);
    send_interface(fd_write, iface);

    struct StatHeader header;
    if (!receive_stat_header(fd_read, header))
        return 1;

    struct StatResponse response[header.cnt];
    if (!receive_stat(fd_read, header.cnt, response))
        return 1;

//    print_stat(header.cnt, response);
    return 0;
}


int handle_stat_all()
{
    printf("stat all");
    if (!daemon_is_up()) {
        print_help_daemon_is_down();
        return 1;
    }

    send_command(fd_write, DSTAT);

    struct StatHeader header;
    if (!receive_stat_header(fd_write, header))
        return 1;

    struct StatResponse response[header.cnt];
    if (!receive_stat(fd_read, header.cnt, response))
        return 1;

//    print_stat(header.cnt, response);
    return 0;
}



int main (int argc, char **argv)
{
    init_connection();


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