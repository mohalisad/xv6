struct stat;
struct rtcdate;
#include "sharedm.h"

// system calls

int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int inc_num(int);
int invoked_syscalls(int);
int sort_syscalls(int);
int get_count(int,int);
int log_syscalls(void);
int ticketlockinit(void);
int ticketlocktest(void);
int rwinit(void);
int rwtest(unsigned int);
int rwinit2(void);
int rwtest2(unsigned int);
int set_luck(int tickets);
int set_fcfs();
int set_priority(int priority);
int print_process();
int shm_open(int id,int page_count,int flag);
void* shm_attach(int id);
int shm_close(int id);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
