#include "sclog.h"
#include "defs.h"
#include "syscall.h"

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

int find_sclog(sclog *sclogs,int scid){
    int i;
    for (i = 0;;i++)
        if(sclogs[i].scid == scid)
            return i;
        if(sclogs[i].scid == -1)
            return -1;
}
