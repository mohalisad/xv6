#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    int a,b,c;
    a = shm_open(3,3,0);
    shm_attach(3);
    b = shm_open(3,3,0);
    c = shm_open(4,3,0);
    printf(1,"%d %d %d\n",a,b,c );
    exit();
}
