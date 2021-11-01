#define DAEMON_IPC_LISTENER    "/tmp/ssniffer0"
#define DAEMON_IPC_RESPONDER   "/tmp/ssniffer1"

enum DAEMON_COMMAND {
    DSTART_SNIFF,
    DSTOP_SNIFF,
    DSHOW_IPADDR,
    DSELECT,
    DSTAT,
    DSTAT_FILTERED
};

struct stat_header_t {
    int cnt;
};

struct stat_response_t {
    unsigned int cnt;
    struct in_addr ip;
    char iface[16];
};

typedef struct stat_header_t stat_header;
typedef struct stat_response_t stat_response;
