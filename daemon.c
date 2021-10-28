#include <stdio.h>

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
struct trie *pkg_counter;

void fff(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
//    printf("%s", arg);
    struct in_addr ip = source_extractor(arg, pkthdr, packet);
    static int counter = 0; ++counter;
    printf("%d pack from %s\n", counter, inet_ntoa(ip));
    trie_increase(pkg_counter, inet_dev, &ip, 4);
    if (counter % 10 == 0)
        trie_traversing(pkg_counter);
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
    unsigned ip;
    ip = 100; trie_increase(pkg_counter, ex_dev, &ip, 4);
    printf("%d\n", trie_get(pkg_counter, ex_dev, &ip, 4));



    char **inet_devs = get_device_list();
    print_inet_devices(inet_devs);
    inet_dev = inet_devs[0];
    if (inet_dev == NULL) {
        printf("PCap can't find devices");
        return 1;
    }

    sniff(inet_dev, fff);


    trie_traversing(pkg_counter);
}
