#include "defs.h"
struct sccalls{
    struct rtcdate calltimes[50];
    int args[50][3];
};
struct sclog{
    int scid;
    struct sccalls *calls;
    int callcount;
};
