#include "date.h"
struct sccalls{
    struct rtcdate calltimes[100];
    int args[100][3];
};
struct sclog{
    int scid;
    struct sccalls *calls;
    int callcount;
};
struct sclog* create_sclogs();
void call_sc(int sid,struct sclog* sclogs);
void print_sclogs_by_pid(int pid);
