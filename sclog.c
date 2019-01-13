#include "types.h"
#include "defs.h"
#include "sclog.h"
#include "syscall.h"
#include "date.h"

logdata fgllog,mgllog,egllog = 0;

#define PAGESIZE 4000

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
static int type12[] = {INT,VOIDS,INT};
static int type13[] = {INT,INT,INT};

static int* syscall_args[] = {
    [SYS_fork]    type1,
    [SYS_exit]    type1,
    [SYS_wait]    type1,
    [SYS_pipe]    type2,
    [SYS_read]    type12,
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
    [SYS_ticketlockinit]   type1,
    [SYS_ticketlocktest]   type1,
    [SYS_rwinit]           type1,
    [SYS_rwtest]           type3,
    [SYS_rwinit2]          type1,
    [SYS_rwtest2]          type3,
    [SYS_set_luck]         type3,
    [SYS_set_fcfs]         type1,
    [SYS_set_priority]     type3,
    [SYS_print_process]    type1,
    [SYS_shm_open]         type13,
    [SYS_shm_attch]        type3,
    [SYS_shm_close]        type3,
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
    [SYS_ticketlockinit]   "sys_ticketlockinit",
    [SYS_ticketlocktest]   "sys_ticketlocktest",
    [SYS_rwinit]           "sys_rwinit",
    [SYS_rwtest]           "sys_rwtest",
    [SYS_rwinit2]          "sys_rwinit2",
    [SYS_rwtest2]          "sys_rwtest2",
    [SYS_set_luck]         "sys_set_luck",
    [SYS_set_fcfs]         "sys_set_fcfs",
    [SYS_set_priority]     "sys_set_priority",
    [SYS_print_process]    "sys_print_process",
    [SYS_shm_open]         "sys_shm_open",
    [SYS_shm_attch]        "sys_shm_attch",
    [SYS_shm_close]        "sys_shm_close",
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
int sumcount(struct sclog *sclogs){
    int i,count;
    count = 0;
    for (i = 0;;i++){
        if(sclogs[i].scid == -1)
            return count;
        count += sclogs[i].callcount;
    }
}
void init_logdata(logdata *first,logdata *end){
    *first = kalloc();
    *(logdata*)*first = 0;
    *end = *first + sizeof(logdata);
}
void fix_logdata(logdata *mid,logdata *end){
    logdata temp;
    if(*end - *mid > PAGESIZE){
        temp = *mid;
        if((*(logdata*)temp)==0){
            init_logdata(mid,end);
        }else{
            *mid = *(logdata*)temp;
            *end = *mid + sizeof(logdata);
        }
        *(logdata*)temp = *mid;
    }
}
void add_sclog(int sid,struct sclog* sclogs){
    struct sclog *s;
    sclogs[len_sclogs(sclogs)+1].scid = -1;
    s = &(sclogs[len_sclogs(sclogs)]);
    s->scid = sid;
    s->callcount = 0;
    init_logdata(&(s->calls),&(s->newcall));
    s->lcalls = s->calls;
}
int argcounter(int* args){
    if(args == type1||args == type2||args == type3||args == type4)
        return 1;
    if(args == type8||args == type5||args == type12)
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
logdata add_call(logdata endlog,int sid){
    int i,n,argcount;
    char *temp;
    argcount = argcounter(syscall_args[sid]);
    cmostime((struct rtcdate*)endlog);
    endlog += sizeof(struct rtcdate);
    if(syscall_args[sid][0] != VOID){
        for (i = 0;i<argcount;i++){
            switch (syscall_args[sid][i]) {
                case VOIDS:
                case CHARSS:
                case INTS:
                case STATS:
                case SHORT:
                case INT:
                    argint(i,(int*)endlog);
                    endlog += sizeof(int);
                    break;
                case CVOIDS:
                    argint(2, &n);
                    argptr(i,&temp,n);
                    n = fixstr(endlog,temp,n);
                    endlog += (n+1)*sizeof(char);
                    break;
                case CHARS:
                case CCHARS:
                    n = argstr(i,&temp);
                    memmove(endlog,temp,n+1);
                    endlog += (n+1)*sizeof(char);
                    break;
            }
        }
    }
    return endlog;
}
void add_to_gllogs(int pid,int sid){
    if(egllog == 0){
        init_logdata(&fgllog,&egllog);
        mgllog = fgllog;
    }
    *(int*)egllog = pid;
    egllog += sizeof(int);
    *(int*)egllog = sid;
    egllog += sizeof(int);
    egllog = add_call(egllog,sid);
    fix_logdata(&mgllog,&egllog);
}
void call_sc(int pid,int sid,struct sclog* sclogs) {
    struct sclog *log;
    int p;
    p = find_sclog(sid,sclogs);
    if (p == -1){
        add_sclog(sid,sclogs);
        p = find_sclog(sid,sclogs);
    }
    log = &(sclogs[p]);
    log->newcall = add_call(log->newcall,sid);
    fix_logdata(&(log->lcalls),&(log->newcall));
    log->callcount++;
    add_to_gllogs(pid,sid);
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
logdata print_time(logdata call){
    struct rtcdate *t;
    t = call;
    cprintf("%d-%d-%d %d:%d:%d",t->year,t->month,t->day,t->hour,t->minute,t->second);
    return call + sizeof(struct rtcdate);
}
logdata print_call(logdata call,int scid){
    int argcount,n,i;
    argcount = argcounter(syscall_args[scid]);
    cprintf("(");
    for(i=0;i<argcount;i++){
        if(i != 0)cprintf(",");
        switch (syscall_args[scid][i]) {
            case VOIDS:
            case CHARSS:
            case INTS:
            case STATS:
            case SHORT:
            case INT:
                cprintf("%d",*((int*)call));
                call += sizeof(int);
                break;
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
    return call;
}
void print_func_calls(struct sclog* log){
    int i;
    logdata call,mid;
    mid  = log->calls;
    call = log->calls + sizeof(logdata);
    for(i=0;i<log->callcount;i++){
        cprintf("  ");
        call = print_time(call);
        call = print_call(call,log->scid);
        fix_logdata(&mid,&call);
    }
}
void print_sclog(struct sclog* sclog){
    print_func_disc(sclog);
    print_func_calls(sclog);
}
void print_sclogs(struct sclog* sclogs){
    int i,len;
    len = len_sclogs(sclogs);
    for(i=0;i<len;i++){
        print_sclog(&(sclogs[i]));
    }
}
void print_sclogs_by_pid(int pid){
    struct sclog *logs;
    if((logs = get_proclog_by_id(pid))){
        print_sclogs(logs);
    }else cprintf("There is no process with %d pid\n",pid);
}
void print_gllog(){
    int sid;
    logdata call,mid;
    mid  = fgllog;
    call = fgllog + sizeof(logdata);
    while(call!=egllog){
        cprintf("pid:%d ",*(int*)call);
        call += sizeof(int);
        sid = *(int*)call;
        call += sizeof(int);
        cprintf("sid:%d ",sid);
        call = print_time(call);
        cprintf(" %s",syscall_names[sid]);
        call = print_call(call,sid);
        fix_logdata(&mid,&call);
    }
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
int get_callcount(int pid,int scid){
    int i;
    struct sclog *logs;
    if((logs = get_proclog_by_id(pid))){
        i = find_sclog(scid,logs);
        if(i!=-1){
            return logs[i].callcount;
        }else cprintf("There is no call of %d for %d\n",scid,pid);
    }else cprintf("There is no process with %d pid\n",pid);
    return -1;
}
