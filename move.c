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
#include "move.h"

void move_view(int map[720][1280], double *player_angle, double player_v, double player_x, double player_y, int D_flag, int d_flag, int A_flag, int a_flag) {
    double pi=3.1415926535;
    double d=10;
    if(player_v>0 && D_flag==1) {
        int locate=map[(int)(player_y+d*sin(*player_angle+pi/25))][(int)(player_x+d*cos(*player_angle+pi/25))];
        if(locate!=0 && locate!=2 && locate!=3) {
            *player_angle+=pi/50;
        }
    }else if(player_v>0 && (d_flag==1 || D_flag==1)) {
        int locate=map[(int)(player_y+d*sin(*player_angle+pi/50))][(int)(player_x+d*cos(*player_angle+pi/50))];
        if(locate!=0 && locate!=2 && locate!=3) {
            *player_angle+=pi/100;
        }
    }

    if(player_v>0 && A_flag==1) {
        int locate=map[(int)(player_y+d*sin(*player_angle-pi/25))][(int)(player_x+d*cos(*player_angle-pi/25))];
        if(locate!=0 && locate!=2 && locate!=3) {
            *player_angle-=pi/50;
        }
    }else if(player_v>0 && (a_flag==1 || A_flag==1)) {
        int locate=map[(int)(player_y+d*sin(*player_angle-pi/50))][(int)(player_x+d*cos(*player_angle-pi/50))];
        if(locate!=0 && locate!=2 && locate!=3) {
            *player_angle-=pi/100;
        }
    }
}

void change_v(double *player_v, double *v_weight, int w_flag, int s_flag, double timer) {
    double max_player_v=3;
    //スタートダッシュが成功していて1秒経ったら加速を解除
    if(*v_weight==3 && timer>=1) {
        *v_weight=1;
    }

    if(w_flag) {
        if(*player_v<max_player_v) {
            *player_v+=*v_weight*0.01;
        }
    }
        
    if(s_flag){
        if(*player_v>-max_player_v/4) {
            *player_v-=0.02;
        }
    }

    if((w_flag==0 && s_flag==0) || (w_flag==1 && s_flag==1)) {
        if(*player_v>0) {
            *player_v-=0.01;
        }else if(*player_v<0) {
            *player_v+=0.01;
        }

        if(*player_v<0.01 && *player_v>-0.01) {
            *player_v=0;
        }
    }
}

void move_player(int map[720][1280], double *player_x, double *player_y, double *player_v, double player_angle, int *hit_flag) {
    int width=1280,height=720;
    double d=10;
    int locate = map[(int)(*player_y+d*sin(player_angle)+*player_v*sin(player_angle))][(int)(*player_x+d*cos(player_angle)+*player_v*cos(player_angle))];
    if(locate!=0 && locate!=2 && locate!=3) {
        *player_x+=*player_v*cos(player_angle);
        *player_y+=*player_v*sin(player_angle);
    }else {
        *player_v/=3;
        *hit_flag=1;
    }
}

void warp(int map[720][1280], double player_angle, double *player_x, double *player_y) {
    int i;
    for(i=40; i<=65; i+=5) {
        if(map[(int)(*player_y+i*sin(player_angle))][(int)(*player_x+i*cos(player_angle))]==1) {
            *player_x+=i*cos(player_angle);
            *player_y+=i*sin(player_angle);
            break;
        }
    }
}

void rotate_angle(int map[720][1280], int angle, double *player_angle, double *player_x, double *player_y, int *left_remain, int *right_remain) {
    double d=10;
    double pi=3.1415926535;
    if(angle==0) {       //右回転
        //int locate = map[(int)(*player_y+sqrt(2)*d*sin(*player_angle-pi/4)+d*sin(*player_angle+pi/2))][(int)(*player_x+sqrt(2)*d*cos(*player_angle-pi/4)+d*sin(*player_angle+pi/2))];
        //if(locate!=0 && locate!=2 && locate!=3) {
            *player_x+=sqrt(2)*d*cos(*player_angle-pi/4);
            *player_y+=sqrt(2)*d*sin(*player_angle-pi/4);
            *player_angle+=pi/2;
        //}
    }else if(angle==1) { //左回転
        //int locate = map[(int)(*player_y+sqrt(2)*d*sin(*player_angle+pi/4)+d*sin(*player_angle-pi/2))][(int)(*player_x+sqrt(2)*d*cos(*player_angle+pi/4)+d*sin(*player_angle-pi/2))];
        //if(locate!=0 && locate!=2 && locate!=3) {
            *player_x+=sqrt(2)*d*cos(*player_angle+pi/4);
            *player_y+=sqrt(2)*d*sin(*player_angle+pi/4);
            *player_angle-=pi/2;
        //}
    }
}

void wind_accelerate(double timer_start_wind, double *v_weight, int *wind_flag) {
    if(*wind_flag==1) {
        double timer_end_wind = clock()/1000000.;
        double timer_wind = timer_end_wind-timer_start_wind;
        if(timer_wind<1) {
            *v_weight=4;
        }else {
            *v_weight=1;
            *wind_flag=0;
        }
    }
}