

enum DAEMON_COMMAND {
    DSTART_SNIFF,
    DSTOP_SNIFF,
    DSHOW_IPADDR,
    DSELECT,
    DSTAT,
    DSTAT_FILTERED
};

struct StatResponse {
    unsigned int cnt;
    struct in_addr ip;
    char iface[16];
};
