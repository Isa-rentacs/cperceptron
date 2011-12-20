#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "sigmoid.h"
#include "pow2.h"
//#include "perceptron.h"

#ifndef LL
#define LL long long
#endif

#define MAX_TEACHER_SZ 64    //教師データの最大数
#define MAX_DATA_SZ 1500     //入力データの最大数
#define MAX_DATA_LEN 256     //入力データの最大長
#define LEARN_TIME 100       //学習回数
#define REPEAT_TIME 1       //テスト回数
#define LOOP_MAX 100
#define GAIN 1
#define L 3
#define M 4
#define N 1
#define eta 3
#define ALPHA 16
#define BETA 6
#define GAMMA 16
#define DELTA 16


typedef struct perceptron Perceptron;

struct perceptron{
    LL wlm[L+1][M];
    LL wmn[M+1][N];
    LL dlm[L+1][M];
    LL dmn[M+1][N];
    LL Lout[L];
    LL Min[M];
    LL Mout[M];
    LL Nin[N];
    LL output;
};


//stringからL個の入力を読み取り
//perceptronのLoutに代入する
void parseLout(Perceptron *p, char *str){
    LL tmp;
    int i;
    int pos=0;
    for(i=0;i<L;i++){
        tmp = 0;
        while(str[pos] != ' '){
            tmp += str[pos] - '0';
            pos++;
            if(str[pos] == ' ') break;
            tmp *= 10;
        }
        p->Lout[i] = tmp;
        pos++;
    }
#if defined(DEBUG) || defined(LOUT)
    printf("%lld %lld %lld\n", p->Lout[0], p->Lout[1], p->Lout[2]);
#endif
}

//渡されたPerceptronのを初期化する関数
void init(Perceptron *p){
    /*
     * 各edgeの重みを[-DELTA,DELTA]の範囲で設定
     */
    int i,j;
    for(i=0;i<L+1;i++){
        for(j=0;j<M;j++){
            p->wlm[i][j] = rand() % (pow2[DELTA+1]+1) - pow2[DELTA];
#if defined(PINIT) || defined(DEBUG)
            printf("wlm[%d][%d] = %lld\n", i, j, p->wlm[i][j]);
#endif
            p->dlm[i][j] = 0;
        }
    }
    for(i=0;i<M+1;i++){
        for(j=0;j<N;j++){
            p->wmn[i][j] = rand() % (pow2[DELTA+1]+1) - pow2[DELTA];
#if defined(PINIT) || defined(DEBUG)
            printf("wmn[%d][%d] = %lld\n", i, j, p->wmn[i][j]);
#endif
            p->dmn[i][j] = 0;
        }
    }
}

LL get(Perceptron *p, char str[MAX_DATA_LEN]){
    LL modin;
    int i,j;

    //L層の出力として入力データをparseする
    parseLout(p, str);

    //M層のi-thノードに対する入力値を計算する
    for(i=0;i<M;i++){
        p->Min[i] = 0;
        //Lout * weightの和を計算
        for(j=0;j<L;j++){
            p->Min[i] += p->wlm[j][i] * p->Lout[j];
        }
        //M層のi番目ノードの閾値分を入力から減算
        p->Min[i] += p->wlm[L][i] * -1;
    }

    //M層i-thノードのoutputを計算する    
    for(i=0;i<M;i++){
        modin = (p->Min[i] >> (1+DELTA - ALPHA)) / BETA + pow2[ALPHA-1];
        if(0 <= modin && modin < (1<<ALPHA)){
            p->Mout[i] = sigmoid[modin];
        }else if(modin < 0){
            p->Mout[i] = 0;
        }else{
            p->Mout[i] = 1 << GAMMA;
        }
    }

    //N層i-thノードへの入力値を計算する
    for(i=0;i<N;i++){
        p->Nin[i] = 0;
        for(j=0;j<M;j++){
            //M層output * weightの和を計算
            p->Nin[i] += p->wmn[j][i] * p->Mout[i];
        }
        p->Nin[i] += p->wmn[M][i] * -1;
    }

    modin = (p->Nin[0] >> (1+GAMMA+DELTA-ALPHA)) / BETA + pow2[ALPHA-1];
    if(0 <= modin && modin < (1 << ALPHA)){
        return sigmoid[modin];
    }else if(modin < 0){
        return 0;
    }else{
        return 1 << GAMMA;
    }
}

void learn(Perceptron *p, char teacher[][MAX_DATA_LEN], int t_size){
    LL result, delta_k, delta_j;
    int x,i,j,k;
    int ans;

    for(x=0;x<LOOP_MAX;x++){
        //差分変数の初期化
        memset(p->dlm, 0, sizeof(p->dlm));
        memset(p->dmn, 0, sizeof(p->dmn));

        //全ての教師データに対して
        for(i=0;i<t_size;i++){
            if(teacher[i][strlen(teacher[i])-1] == '0'){
                ans = 0;
            }else{
                ans = 1;
            }

            result = get(p, teacher[i]);
            delta_k = (ans << GAMMA) - result;
            delta_k *= (1 << GAMMA) - result;
            delta_k >>= GAMMA;
            delta_k *= result;
            delta_k >>= GAMMA;

            //M->Nの偏微分値
            for(j=0;j<M+1;j++){
                for(k=0;k<N;k++){
                    if(j != M){
                        p->dmn[j][k] += (((delta_k * p->Mout[j]) >> GAMMA) << DELTA) >> GAMMA;
                    }else{
                        p->dmn[j][k] += ((delta_k * -1) << DELTA) >> GAMMA;
                    }
                }
            }

            //L->Mの偏微分値
            for(j=0;j<M;j++){
                delta_j = (delta_k * p->wmn[j][0]) >> DELTA;
                delta_j *= p->Mout[j];
                delta_j >>= GAMMA;
                delta_j *= (1<<GAMMA) - p->Mout[j];
                delta_j >>= GAMMA;
                for(k=0;k<L+1;k++){
                    if(k != L){
                        p->dlm[k][j] += (((delta_j * p->Lout[k]) >> GAMMA) << DELTA) >> GAMMA;
                    }else{
                        p->dlm[k][j] += ((delta_j * -1) << DELTA) >> GAMMA;
                    }
                }
            }
        }
        for(i=0;i<L+1;i++){
            for(j=0;j<M;j++){
                p->wlm[i][j] += p->dlm[i][j] >> eta;
            }
        }
        for(i=0;i<M+1;i++){
            for(j=0;j<N;j++){
                p->wmn[i][j] += p->dmn[i][j] >> eta;
            }
        }
    }
}

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
    int cases;
    int t_size;
    int ans;
    int taken=0;
    int istaken = 0;
    int pred=0;
    Perceptron *p = (Perceptron *)malloc(sizeof(Perceptron));
    FILE *fp;
    int i,j;
    LL result;

    if(argc == 1){
        printf("input number of teacher. Exitting.\n");
        exit(0);
    }

    srand(time(NULL));
  
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
            init(p);

            //教師データをteacher配列に読み込む
            for(j=0;j<t_size;j++){
                strcpy(teacher[j],data[i-1-j]);
#ifdef DEBUG
                printf("[%d]%d-th teacher:%s\n",i,j,teacher[j]);
#endif
            }

            //教師データによる学習を行う
            learn(p, teacher, t_size);

            //教師データを取得
            if(data[i][strlen(data[i])-1] == '0'){
                ans = 0;
            }else{
                ans = 1;
            }
#ifdef DEBUG
            printf("[%d]ans = %d\n",i,ans);
#endif
        
            //データを流してperceptronからの出力を得る
            result = get(p, data[i]);

            //predictionの値を決める
            //ここは確率的にすべきかもしれない
            if(result > (1 << (GAMMA - 1))){
                pred = 1;
            }else{
                pred = 0;
            }

            if(pred == ans){
                taken++;
                istaken = 1;
            }else{
                istaken = 0;
            }

            printf("[%d]answer = %d, prediction = %d, istaken = %d\n", i, ans, pred, istaken);
        }
    }
    printf("taken rate = %lf(%d/%d)\n", (double)taken/((dataNum - t_size) * REPEAT_TIME), taken, (dataNum-t_size)*REPEAT_TIME);
    return 0;
}
