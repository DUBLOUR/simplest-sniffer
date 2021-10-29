#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "sniffer.h"
#include "trie.h"

char *current_dev;
char **all_inet_devs;
struct trie *pkg_counter;

void counting_addresses(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    struct in_addr ip = source_extractor(arg, pkthdr, packet);
    trie_increase(pkg_counter, current_dev, &ip, 4);
    if (0) {
        static int counter = 0; ++counter;
        int x = trie_get(pkg_counter, current_dev, &ip, 4);
        printf("%d (%d) pack from %s\n", counter, x, inet_ntoa(ip));
    }
}

void *start_counting_sniff(void *device) {
    sniff((char*) device, counting_addresses);
    pthread_exit(0);
}

void *printer(void *device) {
    for (;;) {
        sleep(3);
        unsigned int ip = 0x1010101;
        int x = trie_get(pkg_counter, device, &ip, 4);
        printf("= %d\n", x);
        trie_traversing(pkg_counter);
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
