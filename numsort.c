#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define STDIN 0
#define STDOUT 1
#define COUNTS 5
#define FILENAME "result.txt"

char buf[250];

int str2int(char* inp);
int get_next_int(int fd);
void sort_iarray(int* a, int count);
void swap_in_iarray(int* arr,int i,int j);

int main(int argc, char *argv[]){
  int a[COUNTS];
  int i;
  int file;
  printf(STDOUT, "The program pid is: %d\n", getpid());
  if(argc != 1 && argc != 6){
    printf(STDOUT, "wrong input count\n");
    exit();
  }

  for(i=0;i<COUNTS;i++)
    a[i] = (argc == 1) ? get_next_int(STDIN) : str2int(argv[i + 1]);
  sort_iarray(a, COUNTS);
  unlink(FILENAME);
  file = open(FILENAME,O_CREATE|O_WRONLY);
  for(i=0;i<COUNTS;i++)
    printf(file, "%d\n", a[i]);
  close(file);
  exit();
}

int str2int(char* input){
  int negative = 0;
  int number = 0;
  int i;
  negative = (input[0]=='-');
  for(i = negative?1:0 ; ; i++){
    if(input[i]=='\0' || input[i]=='\n' || input[i]=='\r')
      break;
    if(input[i]>='0'&&input[i]<='9'){
      number=number*10+input[i]-'0';
    }else return 0;
  }
  return negative?-number:number;
}

int get_next_int(int fd){
  read(fd,buf,250);
  return str2int(buf);
}

void sort_iarray(int* a, int count){
  int i,j;
  for(i=0;i<count;i++)
    for(j=0;j<count-i-1;j++)
      if(a[j]>a[j+1])
        swap_in_iarray(a,j,j+1);
}

void swap_in_iarray(int* arr,int i,int j){
  arr[j] = arr[i] ^ arr[j];
  arr[i] = arr[i] ^ arr[j];
  arr[j] = arr[i] ^ arr[j];
}
