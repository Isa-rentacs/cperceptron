#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "perceptron.h"

#ifndef LL
#define LL long long
#endif
#define MAX_TEACHER_SZ 5
#define MAX_DATA_SZ 1500
#define MAX_DATA_LEN 256

typedef struct perceptron Perceptron;

struct perceptron{
  int hoge;
};

int parseInt(char *arg){
  int ret=0,i;
  for(i=0;i<(int)strlen(arg);i++){
    ret *= 10;
    ret += arg[i] - '0';
  }
  return ret;
}

int main(int argc, char *argv[]){
  char teacher[MAX_TEACHER_SZ][MAX_DATA_LEN];
  char data[MAX_DATA_SZ][MAX_DATA_LEN];
  int dataNum = 0;

  char *str;
  char buf[MAX_DATA_LEN];
  int t=0;
  LL tsum = 0;
  int n;
  int ans;
  int takenAll=0;
  int takenNormal = 0;
  int gaveup = 0;
  int random = 0;
  int istaken = 0;
  LL result;
  LL e;
  int pred;
  Perceptron p;
  FILE *fp;
  int i;

  if(argc == 1){
    printf("input number of teacher. Exitting.\n");
    exit(0);
  }
  
  n = parseInt(argv[1]);
#ifdef DEBUG
  printf("history length = %d\n", n);
#endif

  //データの読み込み
  if((fp = fopen("./data.dat", "r")) == NULL){
    printf("couldn't open the file. exitting.\n");
    exit(-1);
  }

  i = 0;
  while(fgets(buf, MAX_DATA_LEN,fp) != NULL){
    printf("%d %s\n", strlen(buf), buf);
    buf[strlen(buf)-1] = '\0';
    strcpy(data[i], buf);
    printf("%s\n", data[i]);
    i++;
  }
  dataNum = i;
  /*
  for(i=0;i<dataNum;i++){
    printf("%s\n", data[i]);
  }
  */

}
