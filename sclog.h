typedef void* logdata;
struct sclog{
    int scid;
    logdata calls;
    logdata lcalls;
    logdata newcall;
    int callcount;
};

struct sclog* create_sclogs();
void call_sc(int pid,int sid,struct sclog* sclogs);
void print_sclogs_by_pid(int pid);
void sort_sclogs_by_pid(int pid);
int get_callcount(int pid,int scid);
void print_gllog();
int sumcount(struct sclog *sclogs);
