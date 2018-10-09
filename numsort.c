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
void int2str(int input,char* output);
int get_next_int(int fd);
void print_int(int fd,int num);
void sort_iarray(int* a);
void swap_in_iarray(int* arr,int i,int j);
void swap_in_carray(char* arr,int i,int j);

int main(void){
  int a[COUNTS];
  int i;
  int file;
  write(STDOUT,"The program pid is:",19);
  print_int(STDOUT,getpid());
  for(i=0;i<COUNTS;i++){
    a[i] = get_next_int(STDIN);
  }
  sort_iarray(a);
  unlink(FILENAME);
  file = open(FILENAME,O_CREATE|O_WRONLY);
  for(i=0;i<COUNTS;i++)
    print_int(file,a[i]);
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

void int2str(int input,char* output){
  int neg = (input<0);
  int pointer = 0;
  int i;
  input = neg?-input:input;
  while (1) {
    output[pointer++] = input%10 + '0';
    input/=10;
    if(input == 0){
      if(neg)
        output[pointer++] = '-';
      output[pointer--] = '\0';
      break;
    }
  }
  for(i=0;i<(pointer+1)/2;i++){
    swap_in_carray(output,i,pointer-i);
  }
}

int get_next_int(int fd){
  read(fd,buf,250);
  return str2int(buf);
}

void print_int(int fd,int num){
  int2str(num,buf);
  write(fd,buf,strlen(buf));
  write(fd,"\n",1);
}

void sort_iarray(int* a){
  int i,j;
  for(i=0;i<COUNTS;i++)
    for(j=0;j<COUNTS-i-1;j++)
      if(a[j]>a[j+1])
        swap_in_iarray(a,j,j+1);
}

void swap_in_iarray(int* arr,int i,int j){
  arr[j] = arr[i] ^ arr[j];
  arr[i] = arr[i] ^ arr[j];
  arr[j] = arr[i] ^ arr[j];
}

void swap_in_carray(char* arr,int i,int j){
  arr[j] = arr[i] ^ arr[j];
  arr[i] = arr[i] ^ arr[j];
  arr[j] = arr[i] ^ arr[j];
}
