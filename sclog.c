#include "types.h"
#include "defs.h"
#include "sclog.h"
#include "syscall.h"

#define VOID   0
#define INT    1
#define INTS   2
#define CVOIDS 3
#define VOIDS  4
#define CHARS  5
#define CHARSS 6
#define CCHARS 7
#define SHORT  8
#define STATS  9

static int type1[]  = {VOID};
static int type2[]  = {INTS};
static int type3[]  = {INT};
static int type4[]  = {CCHARS};
static int type5[]  = {INT,CVOIDS,INT};
static int type6[]  = {INT,INT};
static int type7[]  = {CCHARS,CCHARS};
static int type8[]  = {CCHARS,SHORT,SHORT};
static int type9[]  = {CCHARS,INT};
static int type10[] = {CHARS,CHARSS};
static int type11[] = {INT,STATS};

static int* syscall_args[] = {
    [SYS_fork]    type1,
    [SYS_exit]    type1,
    [SYS_wait]    type1,
    [SYS_pipe]    type2,
    [SYS_read]    type5,
    [SYS_kill]    type3,
    [SYS_exec]    type10,
    [SYS_fstat]   type11,
    [SYS_chdir]   type4,
    [SYS_dup]     type3,
    [SYS_getpid]  type1,
    [SYS_sbrk]    type3,
    [SYS_sleep]   type3,
    [SYS_uptime]  type1,
    [SYS_open]    type9,
    [SYS_write]   type5,
    [SYS_mknod]   type8,
    [SYS_unlink]  type4,
    [SYS_link]    type7,
    [SYS_mkdir]   type4,
    [SYS_close]   type3,
    [SYS_inc_num]          type3,
    [SYS_invoked_syscalls] type3,
    [SYS_sort_syscalls]    type3,
    [SYS_get_count]        type6,
    [SYS_log_syscalls]     type1,
};
static char *type_names[] = {
    [VOID]   "void",
    [INT]    "int",
    [INTS]   "int*",
    [CVOIDS] "const void*",
    [VOIDS]  "void*",
    [CHARS]  "char*",
    [CHARSS] "char**",
    [CCHARS] "const char*",
    [SHORT]  "short",
    [STATS]  "struct stat*",
};

static char *syscall_names[] = {
    [SYS_fork]    "sys_fork",
    [SYS_exit]    "sys_exit",
    [SYS_wait]    "sys_wait",
    [SYS_pipe]    "sys_pipe",
    [SYS_read]    "sys_read",
    [SYS_kill]    "sys_kill",
    [SYS_exec]    "sys_exec",
    [SYS_fstat]   "sys_fstat",
    [SYS_chdir]   "sys_chdir",
    [SYS_dup]     "sys_dup",
    [SYS_getpid]  "sys_getpid",
    [SYS_sbrk]    "sys_sbrk",
    [SYS_sleep]   "sys_sleep",
    [SYS_uptime]  "sys_uptime",
    [SYS_open]    "sys_open",
    [SYS_write]   "sys_write",
    [SYS_mknod]   "sys_mknod",
    [SYS_unlink]  "sys_unlink",
    [SYS_link]    "sys_link",
    [SYS_mkdir]   "sys_mkdir",
    [SYS_close]   "sys_close",
    [SYS_inc_num]          "sys_inc_num",
    [SYS_invoked_syscalls] "sys_invoked_syscalls",
    [SYS_sort_syscalls]    "sys_sort_syscalls",
    [SYS_get_count]        "sys_get_count",
    [SYS_log_syscalls]     "sys_log_syscalls",
};

struct sclog* create_sclogs(){
    struct sclog* ret;
    ret = (struct sclog *)kalloc();
    ret[0].scid = -1;
    return ret;
}

int len_sclogs(struct sclog* sclogs){
    int i;
    for (i = 0;;i++)
        if(sclogs[i].scid == -1)
            return i;
}

int find_sclog(int scid,struct sclog *sclogs){
    int i;
    for (i = 0;;i++){
        if(sclogs[i].scid == scid)
            return i;
        if(sclogs[i].scid == -1)
            return -1;
    }
}

void add_sclog(int sid,struct sclog* sclogs){
    struct sclog *s;
    sclogs[len_sclogs(sclogs)+1].scid = -1;
    s = &(sclogs[len_sclogs(sclogs)]);
    s->scid = sid;
    s->callcount = 0;
    s->calls = kalloc();
    s->newcall = s->calls;
}
int argcounter(int* args){
    if(args == type1||args == type2||args == type3||args == type4)
        return 1;
    if(args == type8||args == type5)
        return 3;
    return 2;
}
int fixstr(char* dst,char* src,int n){
    int i;
    for(i=0;i<n;i++){
        if(dst[i] == 0)break;
        dst[i] = src[i];
    }
    dst[i] = 0;
    return i;
}
int lenofstr(char* src){
    int i;
    for(i=0;;i++){
        if(src[i] == 0)break;
    }
    return i;
}
void call_sc(int sid,struct sclog* sclogs) {
    struct sclog *log;
    int i,p,n;
    int argcount;
    char *temp;
    p = find_sclog(sid,sclogs);
    if (p == -1){
        add_sclog(sid,sclogs);
        p = find_sclog(sid,sclogs);
    }
    log = &(sclogs[p]);
    cmostime((struct rtcdate*)log->newcall);
    log->newcall += sizeof(struct rtcdate);
    argcount = argcounter(syscall_args[sid]);
    if(syscall_args[sid][0] != VOID){
        for (i = 0;i<argcount;i++){//
            switch (syscall_args[sid][i]) {
                case CHARSS:
                case INTS:
                case STATS:
                case SHORT:
                case INT:
                    argint(i,(int*)log->newcall);
                    log->newcall += sizeof(int);
                    break;
                case VOIDS:
                case CVOIDS:
                    argint(2, &n);
                    argptr(i,&temp,n);
                    n = fixstr(log->newcall,temp,n);
                    log->newcall += (n+1)*sizeof(char);
                    break;
                case CHARS:
                case CCHARS:
                    n = argstr(i,&temp);
                    memmove(log->newcall,temp,n+1);
                    log->newcall += (n+1)*sizeof(char);
                    break;
            }
        }
    }
    log->callcount++;
}
void print_func_disc(struct sclog* log){
    int i,argcount,scid;
    scid = log->scid;
    argcount = argcounter(syscall_args[scid]);
    cprintf("%d %s(",scid,syscall_names[scid]);
    for(i=0;i<argcount;i++){
        if(i != 0)cprintf(",");
        cprintf("%s",type_names[syscall_args[scid][i]]);
    }
    cprintf(") count:%d\n",log->callcount);
}
void print_func_calls(struct sclog* log){
    int i,j,argcount,scid,n;
    void *call;
    struct rtcdate *t;
    scid = log->scid;
    argcount = argcounter(syscall_args[scid]);
    call = log->calls;
    for(i=0;i<log->callcount;i++){
        t = call;
        cprintf("  %d-%d-%d %d:%d:%d",t->year,t->month,t->day,t->hour,t->minute,t->minute);
        call += sizeof(struct rtcdate);
        cprintf("(");
        for(j=0;j<argcount;j++){
            if(j != 0)cprintf(",");
            switch (syscall_args[scid][j]) {
                case CHARSS:
                case INTS:
                case STATS:
                case SHORT:
                case INT:
                    cprintf("%d",*((int*)call));
                    call += sizeof(int);
                    break;
                case VOIDS:
                case CVOIDS:
                case CHARS:
                case CCHARS:
                    cprintf("\"%s\"",((char*)call));
                    n = lenofstr(call);
                    call += (n+1)*sizeof(char);
                    break;
            }
        }
        cprintf(")\n");
    }
}
void print_sclogs(struct sclog* sclogs){
    int i,len;
    len = len_sclogs(sclogs);
    for(i=0;i<len;i++){
        print_func_disc(&(sclogs[i]));
        if(syscall_args[sclogs[i].scid][0] != VOID)
            print_func_calls(&(sclogs[i]));
    }
}

void print_sclogs_by_pid(int pid){
    struct sclog *logs;
    if((logs = get_proclog_by_id(pid))){
        print_sclogs(logs);
    }else cprintf("There is no process with %d pid\n",pid);
}

void xor_sclogs(struct sclog *res,struct sclog *second){
    int n;
    char *dst;
    char *src;
    n = sizeof(struct sclog);
    dst = (char*)res;
    src = (char*)second;
    while(n-- > 0){
        *dst = (*dst)^(*src);
        dst++;
        src++;
    }
}

void swap_sclogs(struct sclog *first,struct sclog *second){
    xor_sclogs(first,second);
    xor_sclogs(second,first);
    xor_sclogs(first,second);
}

void sort_sclogs(struct sclog *sclogs){
    int i,j,count;
    count = len_sclogs(sclogs);
    for(i=0;i<count;i++)
      for(j=0;j<count-i-1;j++)
        if(sclogs[j].scid>sclogs[j+1].scid)
          swap_sclogs(&(sclogs[j]),&(sclogs[j+1]));
}

void sort_sclogs_by_pid(int pid){
    struct sclog *logs;
    if((logs = get_proclog_by_id(pid))){
        sort_sclogs(logs);
    }else cprintf("There is no process with %d pid\n",pid);
}
