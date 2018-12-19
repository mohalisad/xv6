#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    for(int i=0;i<6;i++)
        fork();
    exit();
}
