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
#define STSATS 9

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
    [STSATS] "struct stat*",
};

static int *syscall_args[] = {
    [SYS_fork]    {VOID},
    [SYS_exit]    {VOID},
    [SYS_wait]    {VOID},
    [SYS_pipe]    {INTS},
    [SYS_read]    {INT,CVOIDS,INT},
    [SYS_kill]    {INT},
    [SYS_exec]    {CHARS,CHARSS},
    [SYS_fstat]   {INT,STATS},
    [SYS_chdir]   {CCHARS},
    [SYS_dup]     {INT},
    [SYS_getpid]  {VOID},
    [SYS_sbrk]    {INT},
    [SYS_sleep]   {INT},
    [SYS_uptime]  {VOID},
    [SYS_open]    {CCHARS,INT},
    [SYS_write]   {INT,CVOIDS,INT},
    [SYS_mknod]   {CCHARS,SHORT,SHORT},
    [SYS_unlink]  {CCHARS},
    [SYS_link]    {CCHARS,CCHARS},
    [SYS_mkdir]   {CCHARS},
    [SYS_close]   {INT},
    [SYS_inc_num]          {INT},
    [SYS_invoked_syscalls] {INT},
    [SYS_sort_syscalls]    {INT},
    [SYS_get_count]        {INT,INT},
    [SYS_log_syscalls]     {VOID},
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

sclog* create_sclogs(){
    sclog* ret;
    int i=0;
    ret = kalloc();
    ret[0].scid = -1;
    return ret;
}

int len_sclogs(sclog* sclogs){
    int i;
    for (i = 0;;i++)
        if(sclogs[i].scid == -1)
            return i;
}

void xor_sclogs(sclog *res,sclog *second){
    int n;
    char *dst;
    char *src;
    n = sizeof(sclog);
    dst = (char*)res;
    src = (char*)second;
    while(n-- > 0){
        *dst = (*dst)^(*src);
        dst++;
        src++;
    }
}

void swap_sclogs(sclog *first,sclog *second){
    xor_sclogs(first,second);
    xor_sclogs(second,first);
    xor_sclogs(first,second);
}

void sort_sclogs(sclog *sclogs){
    int i,j,count;
    count = len_sclogs(sclogs);
    for(i=0;i<count;i++)
      for(j=0;j<count-i-1;j++)
        if(sclogs[j]>sclogs[j+1])
          swap_sclogs(&(a[j]),&(a[j+1]));
}

int find_sclog(int scid,sclog *sclogs){
    int i;
    for (i = 0;;i++)
        if(sclogs[i].scid == scid)
            return i;
        if(sclogs[i].scid == -1)
            return -1;
}

void add_sclog(int sid,sclog* sclogs){
    struct sclog *s;
    sclogs[len_sclogs(sclogs)+1].scid = -1;
    s = &(sclogs[len_sclogs(sclogs)]);
    s->scid = sid;
    s->callcount = 0;
    s->calls = kalloc();
}

void call_sc(int sid,s->callcount) {
    struct sclog *s;
    int i,p;
    int argcount;
    p = find_sclog(sid,sclogs);
    if (p == -1){
        add_sclog(sid,sclogs);
        p = find_sclog(sid,sclogs);
    }
    s = &(sclogs[p]);
    cmostime(&(s->calls->calltimes[s->callcount]));
    argcount = NELEM(syscall_args[sid]);
    if(syscalls_args[0] != VOID){
        for (i = 0;i<argcount;i++){
            argint(i,&(s->calls->args[s->callcount][i]));
        }
    }
    s->callcount++;
}

void print_sclogs(sclog* sclogs){
    int i,j,k,len,len2,scid;
    len = len_sclogs(sclogs);
    for(i=0;i<len;i++){
        scid = sclogs[i]->scid;
        cprintf("%d %s(",scid,syscall_names[scid]);
        len2 =  NELEM(syscall_args[scid]);
        for(j=0;j<len2,j++){
            if(j != 0)cprintf(",");
            cprintf("%s",type_names[syscall_args[scid][j]]);
        }
        cprintf(")\n");
        if(syscall_args[scid][0] != VOID){
            for(j=0;j<sclogs[i]->callcount,j++){
                cprintf("(");
                for(k=0;k<len2;k++){
                    if(k != 0)cprintf(",");
                    cprintf("%d",sclogs[i]->calls->args[j][k]);
                }
                cprintf(")\n");
            }
        }
    }
}
