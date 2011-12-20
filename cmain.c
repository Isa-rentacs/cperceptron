#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "perceptron.h"

#ifndef LL
#define LL long long
#endif
#define MAX_TEACHER_SZ 64    //教師データの最大数
#define MAX_DATA_SZ 1500     //入力データの最大数
#define MAX_DATA_LEN 256     //入力データの最大長
#define LEARN_TIME 100       //学習回数
#define REPEAT_TIME 10       //テスト回数

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
    char teacher[MAX_TEACHER_SZ][MAX_DATA_LEN]; //教師データ
    char data[MAX_DATA_SZ][MAX_DATA_LEN];       //入力データ
    int dataNum = 0;                            //全データ数

    char buf[MAX_DATA_LEN];                     //入力読み込み用バッファ
    int t=0;
    int cases;
    LL tsum = 0;
    int t_size;
    int ans;
    int taken=0;
    int istaken = 0;
    LL result;
    LL e;
    int pred=0;
    Perceptron p;
    FILE *fp;
    int i,j,k;

    if(argc == 1){
        printf("input number of teacher. Exitting.\n");
        exit(0);
    }
  
    //教師データの数を読み込み
    t_size = parseInt(argv[1]);

#ifdef DEBUG
    printf("history length = %d\n", t_size);
#endif

    //データの読み込み
    if((fp = fopen("./data.dat", "r")) == NULL){
        printf("couldn't open the file. exitting.\n");
        exit(-1);
    }

    i = 0;
    while(fgets(buf, MAX_DATA_LEN,fp) != NULL){
        printf("%d %s\n", (int)strlen(buf), buf);
        buf[strlen(buf)-1] = '\0';
        strcpy(data[i], buf);
        printf("%s\n", data[i]);
        i++;
    }
    dataNum = i;

    for(cases=0;cases<REPEAT_TIME;cases++){
        for(i=t_size;i<dataNum;i++){ //i番目に対する予測を行う
            //教師データのクリア
            memset(teacher, 0, sizeof(teacher));
        
            //パーセプトロン内パラメタのリセット
            /*
             * not implemented yet
             */
        
            //教師データをteacher配列に読み込む
            for(j=0;j<t_size;j++){
                strcpy(teacher[j],data[i-1-j]);
#ifdef DEBUG
                printf("[%d]%d-th teacher:%s\n",i,j,teacher[j]);
#endif
            }

            //教師データによる学習を行う
            /*
             * not implemented yet 
             */

            //教師データを取得
            if(data[i][strlen(data[i])-1] == '0'){
                ans = 0;
            }else{
                ans = 1;
            }
#ifdef DEBUG
            printf("[%d]ans = %d\n",i,ans);
#endif
        
            //教師データを流してperceptronからの出力を得る
            /*
             * not implemented yet
             */

            //predictionの値を決める
            //ここは確率的にすべきかもしれない
            if(pred == ans){
                taken++;
                printf("%d\n",taken);
                istaken = 1;
            }else{
                istaken = 0;
            }

            printf("[%d]answer = %d, prediction = %d, istaken = %d\n", i, ans, pred, istaken);
        }
    }
    printf("taken rate = %lf(%d/%d)\n", (double)taken/((dataNum - t_size) * REPEAT_TIME), taken, (dataNum-t_size)*REPEAT_TIME);
}
