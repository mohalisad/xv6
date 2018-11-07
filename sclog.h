#include "date.h"
/*
struct sccalls{
    struct rtcdate calltimes[100];
    int args[100][3];
};
*/
struct sclog{
    int scid;
    void *calls;
    void *newcall;
    int callcount;
};
struct sclog* create_sclogs();
void call_sc(int sid,struct sclog* sclogs);
void print_sclogs_by_pid(int pid);
void sort_sclogs_by_pid(int pid);
