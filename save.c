#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "winlib.h"
#include "save.h"

record_t record[10000];
int num[6];
int achieve[20];

void save_time(const char *filename, int m, int s, int ms) {
    FILE *file = fopen(filename, "a");

    fprintf(file, "%02d:%02d.%02d\n",m,s,ms);
    fclose(file);
}

int load_times(const char *filename, record_t record[10000]) {
    FILE *file = fopen(filename, "r");

    int count = 0;
    while (fscanf(file, "%02d:%02d.%02d", &record[count].m, &record[count].s, &record[count].ms) == 3) {
        count++;
    }
    fclose(file);
    return count;
}

void sort_times(record_t record[10000], int num_record) {
    int i,j;
    for(i=0; i<num_record; i++) {
        for(j=0; j<num_record-1; j++) {
            double a=record[j].m*6000+record[j].s*100+record[j].ms, b=record[j+1].m*6000+record[j+1].s*100+record[j+1].ms;
            if(a>b) {
                int tmp_m=record[j].m, tmp_s=record[j].s, tmp_ms=record[j].ms;
                record[j].m=record[j+1].m, record[j].s=record[j+1].s, record[j].ms=record[j+1].ms;
                record[j+1].m=tmp_m, record[j+1].s=tmp_s, record[j+1].ms=tmp_ms;
            }
        }
    }
}

void to_vector(record_t record[10000], int num_record, int m[10], int s[10], int ms[10]) {
    int i;
    for(i=0; i<fmin(num_record,10); i++) {
        m[i]=record[i].m;
        s[i]=record[i].s;
        ms[i]=record[i].ms;
    }
}

void save_stats(const char *filename, int warp_remain, int left_remain, int right_remain, int wind_remain, int shield_times, int timer) {
    FILE *file = fopen(filename, "r");

    int count = 0;
    int goal,warp,rotate,wind,shield,time;
    fscanf(file, "%d,%d,%d,%d,%d,%d", &goal, &warp, &rotate, &wind, &shield, &time);
    fclose(file);

    file = fopen(filename, "w");
    fprintf(file, "%d,%d,%d,%d,%d,%d",1+goal, warp+(3-warp_remain), rotate+(6-left_remain-right_remain), wind+(3-wind_remain), shield+shield_times, time+timer);
    fclose(file);
}

void load_stats(const char *filename, int num[5]) {
    FILE *file = fopen(filename, "r");

    (fscanf(file, "%d,%d,%d,%d,%d,%d", &num[0], &num[1], &num[2], &num[3], &num[4], &num[5]));
    fclose(file);
}

void achieve_updata(const char *filename, int warp_remain, int left_remain, int right_remain, int wind_remain, int shield_times, double timer, double hit_flag, int goal) {
    FILE *file = fopen(filename, "r");

    (fscanf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &achieve[0], &achieve[1], &achieve[2], &achieve[3], &achieve[4], &achieve[5], &achieve[6], &achieve[7], &achieve[8], &achieve[9], &achieve[10], &achieve[11], &achieve[12], &achieve[13], &achieve[14], &achieve[15], &achieve[16], &achieve[17], &achieve[18], &achieve[19]));
    fclose(file);
    int magic_num=num[1]+num[2]+num[3]+num[4];
    if(num[0]>=1) {
        achieve[0]=1;
    }
    if(num[0]>=10) {
        achieve[1]=1;
    }
    if(num[0]>=50) {
        achieve[2]=1;
    }
    if(num[0]>=100) {
        achieve[3]=1;
    }
    if(num[0]>=1000) {
        achieve[4]=1;
    }
    if(magic_num>=1) {
        achieve[5]=1;
    }
    if(magic_num>=10) {
        achieve[6]=1;
    }
    if(magic_num>=50) {
        achieve[7]=1;
    }
    if(magic_num>=500) {
        achieve[8]=1;
    }
    if(magic_num>=5000) {
        achieve[9]=1;
    }
    if(magic_num>=10000) {
        achieve[10]=1;
    }
    if(goal && (3-warp_remain)+(3-left_remain)+(3-right_remain)+(3-wind_remain)+shield_times==0) {
        achieve[11]=1;
    }
    if(timer<=120) {
        achieve[12]=1;
    }
    if(timer<=60) {
        achieve[13]=1;
    }
    if(timer<=40) {
        achieve[14]=1;
    }
    if(timer<=30) {
        achieve[15]=1;
    }
    if(timer<=25) {
        achieve[16]=1;
    }
    if(timer<=20) {
        achieve[17]=1;
    }
    if(timer>=600) {
        achieve[18]=1;
    }
    if(goal && hit_flag==0) {
        achieve[19]=1;
    }
    
    file = fopen(filename, "w");
    (fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", achieve[0], achieve[1], achieve[2], achieve[3], achieve[4], achieve[5], achieve[6], achieve[7], achieve[8], achieve[9], achieve[10], achieve[11], achieve[12], achieve[13], achieve[14], achieve[15], achieve[16], achieve[17], achieve[18], achieve[19]));
    fclose(file);
}