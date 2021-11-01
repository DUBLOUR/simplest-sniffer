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
#include "../common/common.h"

char *save_file;
char *current_dev;
char **all_inet_devs;
stat_holder *pkg_counter;
trie* current_stat_trie;


trie* load_stat(char *filename, bool verbose)
{
    if (verbose)
        fprintf(stderr, "Load statistic from %s\n", filename);

    trie_compressed* tc = new_trie_compressed();
    FILE *save = fopen(filename,"rb");
    fread(tc, sizeof(trie_compressed), 1, save);
    tc->data = malloc(sizeof(char)*tc->data_len);
    fread(tc->data, sizeof(char), tc->data_len, save);
    fclose(save);

    if (verbose)
        fprintf(stderr, "Loaded %d ip address\n", tc->cnt_point);

    trie* t = trie_load(tc);
    del_trie_compressed(tc);

    if (verbose)
        fprintf(stderr, "Loaded data successfully encoded\n");
    return t;
}


void stat_save(char *filename, trie* t, bool verbose)
{
    if (verbose)
        fprintf(stderr, "Save statistic to %s\n", filename);
    trie_compressed* tc = trie_dump(t);
    FILE *save = fopen(filename,"wb");
    fwrite(tc, sizeof(trie_compressed), 1, save);
    fwrite(tc->data, sizeof(char), tc->data_len, save);
    fclose(save);
    del_trie_compressed(tc);

    size_t total_len = sizeof(trie_compressed) + sizeof(char)*tc->data_len;
    if (verbose)
        fprintf(stderr, "%lu bytes successfully written\n",  total_len);
}


void counting_addresses(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    struct in_addr ip = source_extractor(arg, pkthdr, packet);
    trie_increase(current_stat_trie, &ip, 32);
    if (save_file)
        stat_save(save_file, current_stat_trie, false);
    if (1) {
        static int counter = 0;
        ++counter;
        int x = trie_get(current_stat_trie, &ip, 32);
        printf("%d (%d) pack from %s\n", counter, x, inet_ntoa(ip));
    }
}


void *start_counting_sniff(void *device)
{
    if (1) {
//        current_stat_trie = stat_holder_add_dev(pkg_counter, current_dev)->t;
        sniff((char *) device, counting_addresses);
        pthread_exit(0);
    }
}


void *communicator()
{
    mknod(DAEMON_IPC_LISTENER, S_IFIFO|0666, 0);
    int fd0 = open(DAEMON_IPC_LISTENER, O_RDONLY|O_CREAT);

    mknod(DAEMON_IPC_RESPONDER, S_IFIFO|0666, 0);
    int fd1 = open(DAEMON_IPC_RESPONDER, O_WRONLY|O_CREAT);

    printf("Start listening\n");
    for (;;) {
        int command = -1;
        read(fd0, &command, 4);
        if (command == -1)
            break;

        printf("\nHandle command code %d\n", command);

        if (command == DSHOW_IPADDR) {
            char *raw_ip = malloc(sizeof(char)*20);
            read(fd0, raw_ip, 20);
            printf("Received ip %s\n", raw_ip);

            struct in_addr* addr = calloc(sizeof(struct in_addr), 1);
            inet_aton(raw_ip, addr);
            stat_response* res = calloc(sizeof(stat_response), 1);
            res->ip = *addr;
            res->cnt = trie_get(current_stat_trie, addr, 32);
            strncpy(res->iface, current_dev, strlen(current_dev));

            printf("Return:\ndev: %s\nip: %s\npackets: %d\n",
                   res->iface,
                   inet_ntoa(res->ip),
                   res->cnt);
            write(fd1, res, sizeof(stat_response));
            free(raw_ip);
            free(addr);
            free(res);
            break;
        }

        if (command == DSELECT) {
            continue;
        }
    }

    close(fd0);
    close(fd1);
    pthread_exit(0);
}


int main(int argc, char **argv)
{
    if (argc == 2)
        save_file = argv[1];

    all_inet_devs = get_device_list();
    current_dev = all_inet_devs[0];

    pkg_counter = new_stat_holder();
    current_stat_trie = stat_holder_add_dev(pkg_counter, current_dev)->t;
    if (save_file) {
        current_stat_trie = load_stat(save_file, true);
        pkg_counter->current->t = current_stat_trie;
    }

    pthread_t t_sniff, t_comm;
    pthread_create(&t_sniff,NULL, start_counting_sniff, current_dev);

    for (;;) {
        pthread_create(&t_comm, NULL, communicator, NULL);
        pthread_join(t_comm, NULL);
    }

    pthread_join(t_sniff,NULL);

}
