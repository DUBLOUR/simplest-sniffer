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
#include "stat_holder.h"


char *current_dev;
char **all_inet_devs;
stat_holder *pkg_counter;
trie* current_stat_trie;

void counting_addresses(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    struct in_addr ip = source_extractor(arg, pkthdr, packet);
    trie_increase(current_stat_trie, &ip, 32);
    if (1) {
        static int counter = 0;
        ++counter;
        int x = trie_get(current_stat_trie, &ip, 32);
        printf("%d (%d) pack from %s %u\n", counter, x, inet_ntoa(ip), ip.s_addr);
    }
}

void *start_counting_sniff(void *device)
{
    if (1) {
        current_stat_trie = stat_holder_add_dev(pkg_counter, current_dev)->t;
        sniff((char *) device, counting_addresses);
        pthread_exit(0);
    }
}

void *printer(void *device)
{

    char *myfifo0 = "/tmp/myfifo0";
//    mkfifo(myfifo0, 0666);
    mknod(myfifo0, S_IFIFO|0666, 0);
    int fd0 = open(myfifo0, O_RDONLY|O_CREAT);

    char *myfifo1 = "/tmp/myfifo1";
    mknod(myfifo1, S_IFIFO|0666, 0);
    int fd1 = open(myfifo1, O_WRONLY|O_CREAT);


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

        int cnt_p = trie_get(current_stat_trie, addr, 32);
        printf("has %d packages\n\n", cnt_p);
        write(fd1, &cnt_p, 4);

    }
}

trie* load_stat(char *filename)
{
    trie_compressed* tc = new_trie_compressed();
    FILE *save = fopen(filename,"rb");
    fread(tc, sizeof(trie_compressed), 1, save);
    tc->data = malloc(sizeof(char)*tc->data_len);
    fread(tc->data, sizeof(char), tc->data_len, save);
    fclose(save);
//    printf("%d %d %x\n", tc->data_len, tc->cnt_point, tc->data);
    trie* t = trie_load(tc);
    del_trie_compressed(tc);
    return t;
}

void stat_save(char *filename, trie* t)
{
    trie_compressed* tc = trie_dump(t);
//    printf("%d %d %x\n", tc->data_len, tc->cnt_point, tc->data);
    FILE *save = fopen(filename,"wb");
    fwrite(tc, sizeof(trie_compressed), 1, save);
    fwrite(tc->data, sizeof(char), tc->data_len, save);
    fclose(save);
    del_trie_compressed(tc);
}

int main()
{
    pkg_counter = new_stat_holder();
    current_stat_trie = stat_holder_add_dev(pkg_counter, current_dev)->t;
//    *(&current_stat_trie) = load_stat("dump.sav");
//    trie_traversing(current_stat_trie);

    all_inet_devs = get_device_list();
    print_inet_devices(all_inet_devs);
    current_dev = all_inet_devs[0];

    pthread_t t_sniff, t_printer;
    pthread_create(&t_sniff,NULL, start_counting_sniff, current_dev);
    pthread_create(&t_printer,NULL, printer, current_dev);

    pthread_join(t_sniff,NULL);
    pthread_join(t_printer,NULL);

}
