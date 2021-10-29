#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sniffer.h"
#include "trie.h"


char *current_dev;
char **all_inet_devs;
struct trie *pkg_counter;

void counting_addresses(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    struct in_addr ip = source_extractor(arg, pkthdr, packet);
    trie_increase(pkg_counter, current_dev, &ip, 4);
    if (1) {
        static int counter = 0; ++counter;
        int x = trie_get(pkg_counter, current_dev, &ip, 4);
        printf("%d (%d) pack from %s %u\n", counter, x, inet_ntoa(ip), ip);
    }
}

void *start_counting_sniff(void *device) {
    sniff((char*) device, counting_addresses);
    pthread_exit(0);
}

void *printer(void *device) {
    char *myfifo0 = "/tmp/myfifo0";
    mkfifo(myfifo0, 0666);
    int fd0 = open(myfifo0, O_RDONLY);

    char *myfifo1 = "/tmp/myfifo1";
    mkfifo(myfifo1, 0666);
    int fd1 = open(myfifo1, O_WRONLY);

    printf("Start listening\n");
    for (;;) {
        unsigned int ip;
        char s[20];
        read(fd0, s, 20);
        printf("\nRespond for %s", s);

        struct in_addr* addr = malloc(sizeof(struct in_addr));
        memset(addr, 0, 4);
        inet_aton(s, addr);

        printf("It is %u (%X)\n", addr, addr);

        int cnt_p = trie_get(pkg_counter, device, addr, 4);
        printf("has %d packages\n\n", cnt_p);
        write(fd1, &cnt_p, 4);
    }
}

int main()
{
    pkg_counter = new_trie();

    all_inet_devs = get_device_list();
    print_inet_devices(all_inet_devs);
    current_dev = all_inet_devs[0];

    pthread_t t_sniff, t_printer;
    pthread_create(&t_sniff,NULL, start_counting_sniff, current_dev);
    pthread_create(&t_printer,NULL, printer, current_dev);

    pthread_join(t_sniff,NULL);
    pthread_join(t_printer,NULL);

}
