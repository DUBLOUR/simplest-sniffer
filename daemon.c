#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "sniffer.h"
#include "trie.h"


/*
void simple_handle_ip(struct in_addr ip) {
    static int count=0; ++count;
    printf("%d-th package is from %s\n", count, inet_ntoa(ip));
}

void f(int x, int y) {
    printf("__ %d __ (%d) __\n", x, y);
}

void (*g())(int, int) {
    void (*x)(int, int) = &f;
//    x(10);
    return x;
}

void deco(void (*g())(int, int)) {
    g()(9, 7);
}

void (*gen(void (*a())(int, int)))(int, int) {

    return a;
}
*/
//
//void (*p(int a))(int) {
//    void (*x)(int, int) = &f;
////    x(10);
//    return x(100, a);
//}
//void (*xxx())(u_char*, const struct pcap_pkthdr*, const u_char*) {
//
//    return &source_extractor;
//}

void printer(struct in_addr ip) {
    static int count=0; ++count;
    printf("%d package is from %s\n", count, inet_ntoa(ip));
}

char *inet_dev;
char **inet_devs;
struct trie *pkg_counter, *pkg_counter2;

void fff(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
//    printf("%s", arg);
    struct in_addr ip = source_extractor(arg, pkthdr, packet);
    static int counter = 0; ++counter;
    trie_increase(pkg_counter, inet_dev, &ip, 4);
    int x = trie_get(pkg_counter, inet_dev, &ip, 4);
//    printf("%d (%d) pack from %s\n", counter, x, inet_ntoa(ip));

//    if (counter % 10 == 0)
//        trie_traversing(pkg_counter);
}

void *xxx(void *d) {
    sniff(inet_dev, fff);
    pthread_exit(0);
}

void *printerr(void *d) {
//    printf("!!!!!!!!!!!!!!!!!!!!!!!1 %s\n", d);
    for (;;) {
        sleep(5);
//        printf("!\n");
        unsigned int ip = 0x1010101;
//        int x = trie_get(pkg_counter2, inet_dev, &ip, 4);
//        printf("=== %d\n", x);

        int x = trie_get(pkg_counter, inet_dev, &ip, 4);
        printf("======== %d\n", x);
        char ex_dev[] = "eth0";
//        printf("======== %d\n", trie_get(pkg_counter2, (char*) d, &ip, 4));
    }
}

int main()
{
//    g()(10, 5);
//    deco(g);
//    f(2,3);
//    gen(f)(2,3);
//    return 0;

    char ex_dev[] = "eth0";
    pkg_counter = malloc(sizeof(struct trie));
    pkg_counter2 = malloc(sizeof(struct trie));
    unsigned ip;
    ip = 100; trie_increase(pkg_counter2, ex_dev, &ip, 4);
    printf("%d\n", trie_get(pkg_counter2, ex_dev, &ip, 4));



    inet_devs = get_device_list();
    print_inet_devices(inet_devs);
    inet_dev = inet_devs[0];
    if (inet_dev == NULL) {
        printf("PCap can't find devices");
        return 1;
    }

    pthread_t t_sniff, t_printer; /* идентификатор потока */
    pthread_create(&t_sniff,NULL,xxx,NULL);
    pthread_create(&t_printer,NULL, printerr,inet_dev);
    pthread_join(t_sniff,NULL);
    pthread_join(t_printer,NULL);
//    xxx();


    trie_traversing(pkg_counter);
}
