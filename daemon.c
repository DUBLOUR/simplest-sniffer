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
#include "common.h"

#define SAVE_FILE "dump.sav"

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
        printf("%d (%d) pack from %s %X\n", counter, x, inet_ntoa(ip), ip.s_addr);
    }
}


trie* load_stat(char *filename)
{
    fprintf(stderr, "Load statistic from %s\n", filename);

    trie_compressed* tc = new_trie_compressed();
    FILE *save = fopen(filename,"rb");
    fread(tc, sizeof(trie_compressed), 1, save);
    tc->data = malloc(sizeof(char)*tc->data_len);
    fread(tc->data, sizeof(char), tc->data_len, save);
    fclose(save);

    fprintf(stderr, "Loaded %d ip address\n", tc->cnt_point);
    trie* t = trie_load(tc);
    del_trie_compressed(tc);

    fprintf(stderr, "Loaded data successfully encoded\n");
    return t;
}



void stat_save(char *filename, trie* t)
{
    fprintf(stderr, "Save statistic to %s\n", filename);
    trie_compressed* tc = trie_dump(t);
    FILE *save = fopen(filename,"wb");
    fwrite(tc, sizeof(trie_compressed), 1, save);
    fwrite(tc->data, sizeof(char), tc->data_len, save);
    fclose(save);
    del_trie_compressed(tc);

    size_t total_len = sizeof(trie_compressed) + sizeof(char)*tc->data_len;
    fprintf(stderr, "%lu bytes successfully written\n",  total_len);
}



void *start_counting_sniff(void *device)
{
    if (1) {
        current_stat_trie = stat_holder_add_dev(pkg_counter, current_dev)->t;
        sniff((char *) device, counting_addresses);
        pthread_exit(0);
    }
}



void *communicator()
{
    char *myfifo0 = "/tmp/myfifo00";
    mknod(myfifo0, S_IFIFO|0666, 0);
    int fd0 = open(myfifo0, O_RDONLY|O_CREAT);

    char *myfifo1 = "/tmp/myfifo11";
    mknod(myfifo1, S_IFIFO|0666, 0);
    int fd1 = open(myfifo1, O_WRONLY|O_CREAT);


    printf("Start listening\n");
    for (;;) {
        int command = -1;;
        read(fd0, &command, 4);
        printf("\nHandle command code %d\n", command);

        if (command == DSHOW_IPADDR) {
            stat_save(SAVE_FILE, current_stat_trie);

            char *raw_ip = malloc(sizeof(char)*20);
            read(fd0, raw_ip, 20);
            printf("Received ip %s\n", raw_ip);

            struct in_addr* addr = calloc(sizeof(struct in_addr), 1);
            inet_aton(raw_ip, addr);
            struct StatResponse* res = calloc(sizeof(struct StatResponse), 1);
            res->ip = *addr;
            res->cnt = trie_get(current_stat_trie, addr, 32);
            strncpy(res->iface, current_dev, strlen(current_dev));

            printf("Return:\ndev: %s\nip: %s\npackets: %d\n",
                   res->iface,
                   inet_ntoa(res->ip),
                   res->cnt);
            write(fd1, res, sizeof(struct StatResponse));
            free(raw_ip);
            free(addr);
            free(res);
            break;
        }

        if (command == DSELECT) {
            continue;
        }

    }
}


int main()
{
    pkg_counter = new_stat_holder();
    current_stat_trie = stat_holder_add_dev(pkg_counter, current_dev)->t;

    if (0) {
        current_stat_trie = load_stat(SAVE_FILE);
        trie_traversing(current_stat_trie);
    }

    all_inet_devs = get_device_list();
    current_dev = all_inet_devs[0];

    pthread_t t_sniff, t_printer;
    pthread_create(&t_sniff,NULL, start_counting_sniff, current_dev);
    pthread_create(&t_printer,NULL, communicator, NULL);

    pthread_join(t_sniff,NULL);
    pthread_join(t_printer,NULL);

}
