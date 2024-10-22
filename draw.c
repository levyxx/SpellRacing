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
#include "draw.h"
#include "turtle.h"

int m[10],s[10],ms[10];

rain_t rain[1000];
wind_t wind[1000];

void draw_title(GC gc) {
    GC gc_title=gc;
    //S
    g_line(width/2-290+50,100,width/2-290+100,100+38,gc_title);
    g_line(width/2-290+50,100,width/2-290,100+38,gc_title);
    g_line(width/2-290,100+38,width/2-290+100,100+113,gc_title);
    g_line(width/2-290+50,100+150,width/2-290+100,100+113,gc_title);
    g_line(width/2-290+50,100+150,width/2-290,100+113,gc_title);

    //P
    g_line(width/2-170,100,width/2-170+100,100,gc_title);
    g_line(width/2-170,100,width/2-170,100+150,gc_title);
    g_line(width/2-170,100+75,width/2-170+100,100+75,gc_title);
    g_line(width/2-170+100,100,width/2-170+100,100+75,gc_title);

    //E
    g_line(width/2-50,100,width/2+50,100,gc_title);
    g_line(width/2-50,100,width/2-50,100+150,gc_title);
    g_line(width/2-50,100+75,width/2+50,100+75,gc_title);
    g_line(width/2-50,100+150,width/2+50,100+150,gc_title);

    //L
    g_line(width/2+70,100,width/2+70,100+150,gc_title);
    g_line(width/2+70,100+150,width/2+70+100,100+150,gc_title);

    //L
    g_line(width/2+190,100,width/2+190,100+150,gc_title);
    g_line(width/2+190,100+150,width/2+190+100,100+150,gc_title);

    //R
    g_line(width/2-350,350,width/2-350+100,350,gc_title);
    g_line(width/2-350,350,width/2-350,350+150,gc_title);
    g_line(width/2-350+100,350,width/2-350+100,350+75,gc_title);
    g_line(width/2-350,350+75,width/2-350+100,350+75,gc_title);
    g_line(width/2-350,350+75,width/2-350+100,350+150,gc_title);

    //A
    g_line(width/2-230+50,350,width/2-230,350+150,gc_title);
    g_line(width/2-230+50,350,width/2-230+100,350+150,gc_title);
    g_line(width/2-230+25,350+75,width/2-230+75,350+75,gc_title);

    //C
    g_line(width/2-110,350,width/2-110+100,350,gc_title);
    g_line(width/2-110,350,width/2-110,350+150,gc_title);
    g_line(width/2-110,350+150,width/2-110+100,350+150,gc_title);

    //I
    g_line(width/2+10+30,350,width/2+10+70,350,gc_title);
    g_line(width/2+10+30,350+150,width/2+10+70,350+150,gc_title);
    g_line(width/2+10+50,350,width/2+10+50,350+150,gc_title);

    //N
    g_line(width/2+130,350,width/2+130,350+150,gc_title);
    g_line(width/2+130,350,width/2+130+100,350+150,gc_title);
    g_line(width/2+130+100,350,width/2+130+100,350+150,gc_title);

    //G
    g_line(width/2+250,350,width/2+250+100,350,gc_title);
    g_line(width/2+250,350,width/2+250,350+150,gc_title);
    g_line(width/2+250,350+150,width/2+250+100,350+150,gc_title);
    g_line(width/2+250+50,350+75,width/2+250+100,350+75,gc_title);
    g_line(width/2+250+100,350+75,width/2+250+100,350+150,gc_title);
}

void koch(double leng, int n, GC gc) {
	if(n==0) {
        dforward(leng, gc);
	}else {
        koch(leng/3,n-1,gc);
        turn(-60);
        koch(leng/3,n-1,gc);
        turn(120);
        koch(leng/3,n-1,gc);
        turn(-60);
        koch(leng/3,n-1,gc);
	}
}

void draw_box(double *t) {
    double pi=3.1415926535;
    *t+=pi/200;
    double box[8][3]={
            {0,-0.7,0},{0,0,0},{0.5,0,0},{0.5,-0.7,0},{0,-0.7,0.5},{0,0,0.5},{0.5,0,0.5},{0.5,-0.7,0.5}
    };
    double new_box[8][3],describe_box[8][2];
            
    int i;
    for(i=0; i<8; i++) {
        box[i][0]-=0.3;
        box[i][2]-=0.2;
        new_box[i][0]=box[i][0]*cos(*t)+box[i][2]*sin(*t);
        new_box[i][1]=box[i][1];
        new_box[i][2]=-box[i][0]*sin(*t)+box[i][2]*cos(*t);
        new_box[i][0]+=0.5;
        new_box[i][1]+=0.5;
        box[i][0]+=0.5;
        box[i][2]+=0.5;
    }
            
    for(i=0; i<8; i++) {
        new_box[i][0]-=0.5;
        new_box[i][1]+=1.3;
        new_box[i][2]+=2;
    }

    for(i=0; i<8; i++) {
        describe_box[i][0]=new_box[i][0]/new_box[i][2];
        describe_box[i][1]=new_box[i][1]/new_box[i][2];
    }

    for(i=0; i<8; i++) {
        describe_box[i][0]=width*describe_box[i][0]/2+width/2;
        describe_box[i][1]=height*describe_box[i][1]/2+height/2;
    }

    //辺の描画
    int comb_v[12][2]={
        {0,1},{1,2},{2,3},{0,3},{0,4},{1,5},{2,6},{3,7},{4,5},{5,6},{6,7},{4,7}
    };
    for(i=0; i<12; i++) {
        g_line((int)describe_box[comb_v[i][0]][0],(int)describe_box[comb_v[i][0]][1]-100,(int)describe_box[comb_v[i][1]][0],(int)describe_box[comb_v[i][1]][1]-100,gc_aqua);
    }
}

void draw_sky() {
    int i;
    for(i=0; i<=height/8; i++) {
        g_line(0,i,width,i,gc_greenscale[255*i/(height/8)]);
    }
}

void draw_car_shade() {
    int i,j;
    for(i=height*5/8; i<height*7/8; i++) {
        for(j=width*3/8; j<width*5/8; j++) {
            if((double)(j-width/2)*(j-width/2)/(width/15*width/15)+(double)(i-height*13/16)*(i-height*13/16)/(height/30*height/30)<=1) {
                dot(j,i+20,gc_gray);
            }
        }
    }
}

void draw_car(int d_flag, int D_flag, int a_flag, int A_flag, double player_v, int car[36][53]) {
    double pi=3.1415926535;
    GC color_array[14]={gc_redscale[255],gc_lightgreen,gc_redscale[150],gc_white,gc_redscale[200],gc_lightgray,gc_blue[150],gc_blue[220],gc_black,gc_blue[255],gc_black,gc_aqua,gc_pink,gc_red};
    //GC color_array[14]={gc_lightyellow,gc_lightgreen,gc_darkyellow,gc_white,gc_yellow,gc_lightgray,gc_blue[150],gc_blue[220],gc_black,gc_blue[255],gc_black,gc_aqua,gc_pink,gc_red};
    double theta=0;       //AかDが押されているなら車体をthetaだけ傾ける
    int fix_x=0,fix_y=0;  //回転によって生じたずれを補正する項
    int is_curv=1;
    if(player_v>0 && D_flag==1) {
        theta=-pi/11;
        fix_x=10, fix_y=10;
    }else if(d_flag==1 || D_flag==1) {
        theta=-pi/15;
        fix_x=10, fix_y=10;
    }else if(player_v>0 && A_flag==1) {
        theta=-pi/11;
        fix_x=-10,fix_y=10;
    }else if(a_flag==1 || A_flag==1) {
        theta=-pi/15;
        fix_x=-10,fix_y=10;
    }
    //dもaも押されてたら曲げない
    if((d_flag==1 || D_flag==1) && (a_flag==1 || A_flag==1)) {
        is_curv=0;
        theta=0;
        fix_x=0,fix_y=0;
    }

    int i,j;

    for(i=0; i<36; i++) {
        for(j=0; j<53; j++) {
            int x=-3*j,y=-3*i;
            int nx=x*cos(theta)-y*sin(theta);  //回転行列で座標を回転する
            int ny=x*sin(theta)+y*cos(theta);
            int size=4;  //sizeが小さいと回転によって生じる切り下げで車体に隙間が出来る
            if(j==52 || car[i][j+1]==0) {
                size=3;  //車体の端だけ太くならないように例外処理
            }

            if(car[i][j]!=0) {
                //回転の中心を移動させると地面についてる車輪が軸になっているように見える
                if((d_flag==1 || D_flag==1) && is_curv==1) {
                    g_fill(720+nx+fix_x,500-ny+fix_y,size,size,color_array[car[i][j]-1]);
                }else {
                    g_fill(560-nx+fix_x,500-ny+fix_y,size,size,color_array[car[i][j]-1]);
                }
            }else if(26<=i && i<=33){
                if((d_flag==1 || D_flag==1) && is_curv==1) {
                    g_fill(720+nx+fix_x,500-ny+fix_y,size,size,gc_lightgray);
                }else {
                    g_fill(560-nx+fix_x,500-ny+fix_y,size,size,gc_lightgray);
                }
            }
        }
    }
}

void draw_speed_meter(double player_v, double max_player_v) {
    int i,j;
    double di,dj;
    double pi=3.1415926535;
    char kmh[4]={'k','m','/','h'};
    //スピードメータの描画
    for(i=height*2/3; i<height; i++) {
        for(j=width*3/4; j<width; j++) {
            if((j-width*7/8)*(j-width*7/8)+(i-height*13/16)*(i-height*13/16)<=(width/14)*(width/14)) {
                dot(j,i,gc_black);
            }
            if((j-width*7/8)*(j-width*7/8)+(i-height*13/16)*(i-height*13/16)>=(width/20)*(width/20) && (j-width*7/8)*(j-width*7/8)+(i-height*13/16)*(i-height*13/16)<=(width/19)*(width/19)) {
                dot(j,i,gc_red);
            }
        }
    }

    //メーターの周りのゲージの描画
    for(di=0; di<max_player_v; di+=0.04) {
        for(dj=0; dj<0.015; dj+=0.0005) {
            if(di>=player_v) {
                g_line((int)(width*7/8+width/18*cos(2.*pi*(di+dj)/max_player_v+pi/2)),(int)(height*13/16+width/18*sin(2.*pi*(di+dj)/max_player_v+pi/2)),(int)(width*7/8+width/15*cos(2*pi*(di+dj)/max_player_v+pi/2)),(int)(height*13/16+width/15*sin(2.*pi*(di+dj)/max_player_v+pi/2)),gc_gray);
            }else if(di/max_player_v<=7./8.) {
                g_line((int)(width*7/8+width/18*cos(2.*pi*(di+dj)/max_player_v+pi/2)),(int)(height*13/16+width/18*sin(2.*pi*(di+dj)/max_player_v+pi/2)),(int)(width*7/8+width/15*cos(2*pi*(di+dj)/max_player_v+pi/2)),(int)(height*13/16+width/15*sin(2.*pi*(di+dj)/max_player_v+pi/2)),gc_lightyellow);
            }else {
                g_line((int)(width*7/8+width/18*cos(2.*pi*(di+dj)/max_player_v+pi/2)),(int)(height*13/16+width/18*sin(2.*pi*(di+dj)/max_player_v+pi/2)),(int)(width*7/8+width/15*cos(2*pi*(di+dj)/max_player_v+pi/2)),(int)(height*13/16+width/15*sin(2.*pi*(di+dj)/max_player_v+pi/2)),gc_orange[130]);
            }
        }
    }

    //スピードの数字の描画
    text(width*7/8-10,height*7/8,"km/h",gc_lightgreen);
    int converted_player_v=fmax(0.,300*player_v/max_player_v); //表示用に速度を0~300km/hに変換
    if(converted_player_v<10) {
        g_num(width*7/8,height*13/16,15,converted_player_v%10,gc_lightgreen);
    }else if(converted_player_v<100) {
        g_num(width*7/8+15,height*13/16,15,converted_player_v%10,gc_lightgreen);
        converted_player_v/=10.;
        g_num(width*7/8-15,height*13/16,15,converted_player_v%10,gc_lightgreen);
    }else {
        g_num(width*7/8+30,height*13/16,15,converted_player_v%10,gc_lightgreen);
        converted_player_v/=10.;
        g_num(width*7/8,height*13/16,15,converted_player_v%10,gc_lightgreen);
        converted_player_v/=10.;
        g_num(width*7/8-30,height*13/16,15,converted_player_v%10,gc_lightgreen);
    }
}

void draw_timer(double timer) {
    int i;
    double pi=3.1415926535;
    g_num(width-30,40,15,(int)(timer*100)%10,gc_white);
    g_num(width-55,40,15,(int)(timer*10)%10,gc_white);
    for(i=0; i<100; i++) {
            g_line(width-68,55,width-68+(int)2*cos(2*pi*i/100),55+(int)2*sin(2*pi*i/100),gc_white);
    }

    double timer_sec=timer-(int)(timer/60)*60;
    g_num(width-85,40,15,(int)(timer_sec)%10,gc_white);
    g_num(width-110,40,15,(int)(timer_sec/10)%10,gc_white);
    for(i=0; i<100; i++) {
            g_line(width-127,48,width-127+(int)2*cos(2*pi*i/100),48+(int)2*sin(2*pi*i/100),gc_white);
            g_line(width-127,32,width-127+(int)2*cos(2*pi*i/100),32+(int)2*sin(2*pi*i/100),gc_white);
    }
    g_num(width-145,40,15,(int)(timer/60)%10,gc_white);
    g_num(width-170,40,15,(int)(timer/600)%10,gc_white);
}

void draw_minimap(int map[720][1280], double player_x, double player_y, double player_angle) {
    int i,j;
    double pi=3.1415926535;
    //道の表示
    for(i=0; i<width; i++) {
        for(j=0; j<height; j++) {
            if(map[j][i]==1) {
                dot(30+i/4,j/3+450,gc_grayscale[200]);
            }
        }
    }

    //mapにゴールテープ表示
    for(i=630; i<633; i++) {
        for(j=30; j<34; j++) {
            if((i+j)%2==0) {
                g_fill(30+j/4+2*(j-30),470+i/3+30+2*(i-660),2,2,gc_white);
            }else {
                g_fill(30+j/4+2*(j-30),470+i/3+30+2*(i-660),2,2,gc_black);
            }
        }
    }

    //mapにプレイヤーの位置を表示
    for(i=0; i<100; i++) {
        g_line((int)player_x/4+30,(int)player_y/3+450,(int)player_x/4+30+(int)5*cos(2*pi*i/100),(int)player_y/3+450+(int)5*sin(2*pi*i/100),gc_blue[255]);
    }
    g_line((int)player_x/4+30,(int)player_y/3+450,(int)player_x/4+30+(int)15*cos(player_angle+pi/6),(int)player_y/3+450+(int)15*sin(player_angle+pi/6),gc_red);
    g_line((int)player_x/4+30,(int)player_y/3+450,(int)player_x/4+30+(int)15*cos(player_angle-pi/6),(int)player_y/3+450+(int)15*sin(player_angle-pi/6),gc_red);
}

void draw_countdown(int *is_race_start, double *timer_start, double *timer_end, int *ok_startdash, double *v_weight, double *n, double *player_angle, int *ground) {
    double pi=3.1415926535;
    int width=1280, height=720;
    GC gc_count=gc_orange[170];
    if(*is_race_start==0) {      //レースが始まっていなければ
        double count=(*timer_end-*timer_start)*1.6;
        *n=fmax(1.03-count/3.,0.03);
        *player_angle=fmax(-pi/6*count,-pi/2);
        if(3<=count && count<=4) {
            *ground=1;
            if(count<=0.5) {
                g_num((int)(width/2),(int)(height*3/7),35+(int)35*sin(pi*count),3,gc_count);  //0.5秒経つまでsinで数字を大きくする
            }else {
                g_num((int)(width/2),(int)(height*3/7),70,3,gc_count);    //その後は一定の大きさ
            }
            while (XPending(disp)) {
                XNextEvent(disp, &event);
                if (event.type == KeyPress) {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    if(keysym == XK_w) {
                        *ok_startdash=0; //カウントダウンが3のときにWを押したらスタートダッシュ失敗
                    }
                }
            } 
        }else if(4<count && count<=5) {
            if(count<=1.5) {
                g_num(width/2,height*3/7,35+(int)35*sin(pi*(count-1)),2,gc_count);
            }else {
                g_num(width/2,height*3/7,70,2,gc_count);
            }
            while (XPending(disp)) {
                XNextEvent(disp, &event);
                if (event.type == KeyPress) {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    if(keysym == XK_w) {
                        if(ok_startdash) {
                            *v_weight=3;  //スタートダッシュ成功時１秒間加速が3倍速くなる
                        }
                    }
                } else if (event.type == KeyRelease) {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    if(keysym == XK_w) {
                        *v_weight=1;
                    }
                }
            }   
        }else if(5<count && count<=6) {
            if(count<=2.5) {
                g_num(width/2,height*3/7,35+(int)35*sin(pi*(count-2)),1,gc_count);
            }else {
                g_num(width/2,height*3/7,70,1,gc_count);
            }
        }
        if(count>=6) {
            *timer_start = clock()/1000000.;
            *is_race_start=1;
        }
    }
}


void draw_drift(int *D_flag, int *A_flag, double player_v, int *D_count, int *A_count) {
    double pi=3.1415926535;
    double r_angle,r_length;
    int max_length;
    int i,j;
    if(*D_flag^*A_flag==1) { //DかA片方だけが押されているとき
        if(player_v>0 && *D_flag==1) {
            *D_count++;
            if(*D_count>=15) {
                max_length=15;
            }else {
                max_length=5;
            }
            for(i=0; i<10; i++) {
                r_angle=rand()%30;
                r_length=rand()%max_length;
                for(j=0; j<5; j++) {
                    g_line(700,610,700+r_length*cos(-pi*(r_angle+(j-1))/30),610+r_length*sin(-pi*(r_angle+(j-1))/30),gc_aqua);
                }
            }
        }else {
            *D_count=0;
        }

        //ドリフトの摩擦熱の描画(左)
        if(player_v>0 && *A_flag==1) {
            if(*A_count>=15) {
                max_length=15;
            }else {
                max_length=5;
            }
            for(i=0; i<10; i++) {
                r_angle=rand()%30;
                r_length=rand()%max_length;
                for(j=0; j<5; j++) {
                    g_line(580,610,580+r_length*cos(-pi*(r_angle+(j-1))/30),610+r_length*sin(-pi*(r_angle+(j-1))/30),gc_aqua);
                }
            }
        }else {
            *A_count=0;
        }
    }
}

void draw_3d(int map[720][1280], int width, int height, double view_dist, double n, double player_angle, double FOV, double player_x, double player_y, int *can_warp) {
    int rough_rate=2; //小さいほど画質は良くなるが重くなる.
    double h,v,fv,xl,yl,xr,yr,xm,ym;
    int i,j;
    double warp_check=view_dist*3/4;
    for(i=0; i<height*7/8; i+=rough_rate) {
        for(j=0; j<width; j+=rough_rate) {
            h=(double)j/width, v=(double)i/(height*15/16);
            fv=-n/(v-1);
            xl=view_dist*sqrt(2)*cos(player_angle-FOV/2);
            yl=view_dist*sqrt(2)*sin(player_angle-FOV/2);
            xr=view_dist*sqrt(2)*cos(player_angle+FOV/2);
            yr=view_dist*sqrt(2)*sin(player_angle+FOV/2);
            xm=player_x + (1-h)*fv*xl + h*fv*xr;
            ym=player_y + (1-h)*fv*yl + h*fv*yr;
            if((int)xm<0 || (int)xm>=width || (int)ym<0 || (int)ym>=height) {
                g_fill(j,height-i,rough_rate,rough_rate,gc_purplescale[210]);
                //g_fill(j,height-i,rough_rate,rough_rate,gc_softpurple);
                if(abs(height-i-height*5/24)<=3 && abs(j-width/2)<=3) {
                    *can_warp=0;
                }
            }else if(map[(int)ym][(int)xm]==0) {
                g_fill(j,height-i,rough_rate,rough_rate,gc_purplescale[210]);//紫
                //g_fill(j,height-i,rough_rate,rough_rate,gc_softpurple);
                if(abs(height-i-height*5/24)<=3 && abs(j-width/2)<=3) {
                    *can_warp=0;
                }
            }else if(map[(int)ym][(int)xm]==1){
                g_fill(j,height-i,rough_rate,rough_rate,gc_lightgray);      //灰色
                if(abs(height-i-height*5/24)<=3 && abs(j-width/2)<=3) {
                    *can_warp=1;
                }
            }else if(map[(int)ym][(int)xm]==2) {
                g_fill(j,height-i,rough_rate,rough_rate,gc_red);            //赤
                if(abs(height-i-height*5/24)<=3 && abs(j-width/2)<=3) {
                    *can_warp=0;
                }
            }else if(map[(int)ym][(int)xm]==3) {
                g_fill(j,height-i,rough_rate,rough_rate,gc_white);          //壁の白
                if(abs(height-i-height*5/24)<=3 && abs(j-width/2)<=3) {
                    *can_warp=0;
                }
            }else if(map[(int)ym][(int)xm]==4) {
                g_fill(j,height-i,rough_rate,rough_rate,gc_black);          //黒
                if(abs(height-i-height*5/24)<=3 && abs(j-width/2)<=3) {
                    *can_warp=1;
                }
            }else if(map[(int)ym][(int)xm]==5) {
                g_fill(j,height-i,rough_rate,rough_rate,gc_white);          //ゴールの白
                if(abs(height-i-height*5/24)<=3 && abs(j-width/2)<=3) {
                    *can_warp=1;
                }
            }
        }
    }
}

void generating_map(double timer, int order_i[23*41], int order_j[23*41], int *is_generating, int *gen, double *g_time, int dist) {
    int genmap_v=5000,i;
    g_fill(0,0,1280,720,gc_purplescale[210]);
    //g_fill(0,0,1280,720,gc_softpurple);
    text(width/2-50,height/10,"Generating map...",gc_lightgreen);
    //ロードのゲージ
    g_line(width/4-1,height/10+9,width*3/4+2,height/10+9,gc_black);
    g_line(width/4-1,height/10+9,width/4-1,height/10+10+21,gc_black);
    g_line(width*3/4+2,height/10+9,width*3/4+2,height/10+10+21,gc_black);
    g_line(width/4-1,height/10+10+21,width*3/4+2,height/10+10+21,gc_black);
    g_fill(width/4,height/10+10,width/2*fmin((int)(timer*genmap_v),*g_time)/dist,20,gc_lightgreen);

    for(i=0; i<fmin((int)(timer*genmap_v),*g_time); i++) {
        if(order_j[i]!=0 && order_i[i]!=0) {
            if(i==0) {
                g_fill(100+25*order_j[i],60+25*(24-order_i[i]),25,25,gc_red);
            }else if(order_j[i]==39 && order_i[i]==21) {
                g_fill(100+25*order_j[i],60+25*(24-order_i[i]),25,25,gc_blue[255]);
                if(*g_time==1e9) {
                    *g_time=(int)(timer*genmap_v);
                }
            }else {
                g_fill(100+25*order_j[i],60+25*(24-order_i[i]),25,25,gc_lightgray);
            }
        }
    }
    g_fill(0,0,100,720,gc_purplescale[210]);

    if(timer-*g_time/genmap_v>=0.02) {
        *is_generating=0;
        *gen=0;
    }
}

void draw_magic(int can_warp, int warp_remain) {
    double i,j;
    GC gc_magic;
    if(can_warp==1 && warp_remain>0) {
        gc_magic=gc_blue[255];
    }else {
        gc_magic=gc_gray;
    }
    for(i=0; i<height/3; i++) {
        for(j=width/3; j<width*2/3; j++) {
            double ellipse_check=(double)(j-width/2)*(j-width/2)/(30*30)+(double)(i-height*5/24)*(i-height*5/24)/(15*15);
            if(0.9<=ellipse_check && ellipse_check<=1.1) {
                dot(j,i,gc_magic);
            }
        }
    }

    g_line(width/2-25,height*5/24-7,width/2+25,height*5/24-7,gc_magic);
    g_line(width/2-25,height*5/24-7,width/2,height*5/24+15,gc_magic);
    g_line(width/2+25,height*5/24-7,width/2,height*5/24+15,gc_magic);
    g_line(width/2-25,height*5/24+7,width/2+25,height*5/24+7,gc_magic);
    g_line(width/2-25,height*5/24+7,width/2,height*5/24-15,gc_magic);
    g_line(width/2+25,height*5/24+7,width/2,height*5/24-15,gc_magic);
}

void draw_magic_icon(int warp_remain, int left_remain, int right_remain, int wind_remain) {
    double i,j;
    double pi=3.1415926535;
    //ワープ
    for(i=0; i<100; i++) {
        for(j=0; j<100; j++) {
            double circle_check=(double)(i-50)*(i-50)/(30*30)+(double)(j-50)*(j-50)/(30*30);
            if(0.9<=circle_check && circle_check<=1.1) {
                dot(i,j,gc_red);
            }
        }
    }
    g_line(25,40,75,40,gc_red);
    g_line(25,40,50,80,gc_red);
    g_line(75,35,50,80,gc_red);
    g_line(25,65,75,65,gc_red);
    g_line(25,65,50,20,gc_red);
    g_line(75,65,50,20,gc_red);
    g_num(75,75,10,warp_remain,gc_black);
    text(50,91,"K",gc_black);

    //左回転
    for(i=80; i<200; i++) {
        for(j=0; j<100; j++) {
            double circle_check=(double)(i-130)*(i-130)/(30*30)+(double)(j-50)*(j-50)/(30*30);
            if(0.9<=circle_check && circle_check<=1.1) {
                dot(i,j,gc_yellow);
            }
        }
    }
    g_line(110,50,150,50,gc_yellow);
    g_line(110,50,120,40,gc_yellow);
    g_line(110,50,120,60,gc_yellow);
    g_num(155,75,10,left_remain,gc_black);
    text(130,91,"J",gc_black);

    //右回転
    for(i=160; i<250; i++) {
        for(j=0; j<100; j++) {
            double circle_check=(double)(i-210)*(i-210)/(30*30)+(double)(j-50)*(j-50)/(30*30);
            if(0.9<=circle_check && circle_check<=1.1) {
                dot(i,j,gc_orange[155]);
            }
        }
    }
    g_line(190,50,230,50,gc_orange[155]);
    g_line(230,50,220,40,gc_orange[155]);
    g_line(230,50,220,60,gc_orange[155]);
    g_num(234,75,10,right_remain,gc_black);
    text(210,91,"L",gc_black);

    //風加速
    for(i=230; i<350; i++) {
        for(j=0; j<100; j++) {
            double circle_check=(double)(i-290)*(i-290)/(30*30)+(double)(j-50)*(j-50)/(30*30);
            if(0.9<=circle_check && circle_check<=1.1) {
                dot(i,j,gc_aqua);
            }
        }
    }
    g_line(290-25,50,290+15,50,gc_aqua);
    double theta;
    for(theta=-pi; theta<pi/2; theta+=pi/100) {
        dot(290+15+5*cos(theta),45+5*sin(theta),gc_aqua);
    }
    g_line(290-20,45,290,45,gc_aqua);
    for(theta=-pi; theta<pi/2; theta+=pi/100) {
        dot(290+5*cos(theta),40+5*sin(theta),gc_aqua);
    }
    g_line(290-20,55,290+10,55,gc_aqua);
    for(theta=-pi/2; theta<pi; theta+=pi/100) {
        dot(290+10+5*cos(theta),60+5*sin(theta),gc_aqua);
    }
    g_num(314,75,10,wind_remain,gc_black);
    text(290,91,"P",gc_black);
}

void draw_result(int m[10], int s[10], int ms[10], int count, int goal, double timer) {
    g_fill(0,0,width,height,gc_black);
    text(width/2,height/10-30,"Ranking",gc_yellow);
    int i;
    for(i=0; i<fmin(count,10); i++) {
        if(i!=9) {
            g_num(width/2-120, height/10+20+50*i, 10, i+1, gc_red);
        }else {
            g_num(width/2-120, height/10+20+50*i, 10, 0, gc_red);
            g_num(width/2-120-20, height/10+20+50*i, 10, 1, gc_red);
        }
        if(i==0) {
            text(width/2-110, height/10+20+50*i+10, "st", gc_red);
        }else if(i==1) {
            text(width/2-110, height/10+20+50*i+10, "nd", gc_red);
        }else if(i==2) {
            text(width/2-110, height/10+20+50*i+10, "rd", gc_red);
        }else {
            text(width/2-110, height/10+20+50*i+10, "th", gc_red);
        }

        if(m[i]<10) {
            g_num(width/2-25, height/10+20+50*i, 10, 0, gc_lightgreen);
            g_num(width/2-45, height/10+20+50*i, 10, m[i], gc_lightgreen);
        }else {
            g_num(width/2-45, height/10+20+50*i, 10, (m[i]/10)%10, gc_lightgreen);
            g_num(width/2-25, height/10+20+50*i, 10, m[i]%10, gc_lightgreen);
        }
        g_fill(width/2-13, height/10+20+50*i+5, 3, 3, gc_lightgreen);
        g_fill(width/2-13, height/10+20+50*i-5, 3, 3, gc_lightgreen);

        if(s[i]<10) {
            g_num(width/2, height/10+20+50*i, 10, 0, gc_lightgreen);
            g_num(width/2+20, height/10+20+50*i, 10, s[i], gc_lightgreen);
        }else {
            g_num(width/2, height/10+20+50*i, 10, (s[i]/10)%10, gc_lightgreen);
            g_num(width/2+20, height/10+20+50*i, 10, s[i]%10, gc_lightgreen);
        }
        g_fill(width/2+33, height/10+20+50*i+7, 3, 3, gc_lightgreen);

        if(ms[i]<10) {
            g_num(width/2+45, height/10+20+50*i, 10, 0, gc_lightgreen);
            g_num(width/2+65, height/10+20+50*i, 10, ms[i], gc_lightgreen);
        }else {
            g_num(width/2+45, height/10+20+50*i, 10, (ms[i]/10)%10, gc_lightgreen);
            g_num(width/2+65, height/10+20+50*i, 10, ms[i]%10, gc_lightgreen);
        }
    }

    if(goal==1) {
        int dy=50;
        text(width/2-120-30,572+dy,"Your Time", gc_red);
        if(timer/600<10) {
            g_num(width/2-25,572+dy,10,(int)(timer/60)%10,gc_lightgreen);
            g_num(width/2-45,572+dy,10,0,gc_lightgreen);
        }else {
            g_num(width/2-25,572+dy,10,(int)(timer/60)%10,gc_lightgreen);
            g_num(width/2-45,572+dy,10,(int)(timer/600)%10,gc_lightgreen);
        }
        g_fill(width/2-13, 577+dy, 3, 3, gc_lightgreen);
        g_fill(width/2-13, 567+dy, 3, 3, gc_lightgreen);

        double timer_sec=timer-(int)(timer/60)*60;
        if(timer_sec<10) {
            g_num(width/2+20,572+dy,10,(int)(timer_sec)%10,gc_lightgreen);
            g_num(width/2,572+dy,10,0,gc_lightgreen);
        }else {
            g_num(width/2+20,572+dy,10,(int)(timer_sec)%10,gc_lightgreen);
            g_num(width/2,572+dy,10,(int)(timer_sec/10)%10,gc_lightgreen);
        }
        g_fill(width/2+33, 579+dy, 3, 3, gc_lightgreen);
        

        if((int)(timer*100)%100<10) {
            g_num(width/2+65,572+dy,10,(int)(timer*100)%10,gc_lightgreen);
            g_num(width/2+45,572+dy,10,0,gc_lightgreen);
        }else {
            g_num(width/2+65,572+dy,10,(int)(timer*100)%10,gc_lightgreen);
            g_num(width/2+45,572+dy,10,(int)(timer*10)%10,gc_lightgreen);
        }

    }
}

void draw_achievement(int achieve[18]) {
    g_fill(0,0,width,height,gc_black);
    text(width/2-13,height/10-30,"Achievement",gc_yellow);

    int i;
    int x1=400,x2=700;
    for(i=0; i<20; i++) {
        GC gc_achieve;
        int got=0;
        if(achieve[i]==1) {
            gc_achieve=gc_lightgreen;
            got=1;
        }else {
            gc_achieve=gc_red;
        }
        if(i==0) {
            text(x1, height/10+20+30*i, "Graduated from paper driver", gc_achieve);
            text(x2, height/10+20+30*i, "Goal 1 time.", gc_achieve);
        }else if(i==1) {
            text(x1, height/10+20+30*i, "Let's remove the beginner's mark", gc_achieve);
            text(x2, height/10+20+30*i, "Goal 10 times.", gc_achieve);
        }else if(i==2) {
            text(x1, height/10+20+30*i, "Became a skilled driver", gc_achieve);
            text(x2, height/10+20+30*i, "Goal 50 times.", gc_achieve);
        }else if(i==3) {
            text(x1, height/10+20+30*i, "You are driving professional", gc_achieve);
            text(x2, height/10+20+30*i, "Goal 100 times.", gc_achieve);
        }else if(i==4) {
            text(x1, height/10+20+30*i, "Are you free?", gc_achieve);
            text(x2, height/10+20+30*i, "Goal 1000 times.", gc_achieve);
        }else if(i==5) {
            text(x1, height/10+20+30*i, "Apprentice magician", gc_achieve);
            text(x2, height/10+20+30*i, "Use magic 1 time.", gc_achieve);
        }else if(i==6) {
            text(x1, height/10+20+30*i, "Getting used to it?", gc_achieve);
            text(x2, height/10+20+30*i, "Use magic 10 times.", gc_achieve);
        }else if(i==7) {
            text(x1, height/10+20+30*i, "Magic in your life", gc_achieve);
            text(x2, height/10+20+30*i, "Use magic 50 times.", gc_achieve);
        }else if(i==8) {
            text(x1, height/10+20+30*i, "I've mastered the magic", gc_achieve);
            text(x2, height/10+20+30*i, "Use magic 500 times.", gc_achieve);
        }else if(i==9) {
            text(x1, height/10+20+30*i, "I can't live without magic", gc_achieve);
            text(x2, height/10+20+30*i, "Use magic 5000 times.", gc_achieve);
        }else if(i==10) {
            text(x1, height/10+20+30*i, "A waste of magic", gc_achieve);
            text(x2, height/10+20+30*i, "Use magic 10000 times.", gc_achieve);
        }else if(i==11) {
            text(x1, height/10+20+30*i, "Read the instructions!", gc_achieve);
            text(x2, height/10+20+30*i, "Goal without using magic", gc_achieve);
        }else if(i==12) {
            text(x1, height/10+20+30*i, "Time attack beginner", gc_achieve);
            text(x2, height/10+20+30*i, "Goal within 2 minutes.", gc_achieve);
        }else if(i==13) {
            text(x1, height/10+20+30*i, "Time attack intermediate", gc_achieve);
            text(x2, height/10+20+30*i, "Goal within 1 minute.", gc_achieve);
        }else if(i==14) {
            text(x1, height/10+20+30*i, "Time attack advanced", gc_achieve);
            text(x2, height/10+20+30*i, "Goal within 40 seconds.", gc_achieve);
        }else if(i==15) {
            text(x1, height/10+20+30*i, "Time attack master", gc_achieve);
            text(x2, height/10+20+30*i, "Goal within 30 seconds.", gc_achieve);
        }else if(i==16) {
            text(x1, height/10+20+30*i, "GodSpeed", gc_achieve);
            text(x2, height/10+20+30*i, "Goal within 25 seconds.", gc_achieve);
        }else if(i==17) {
            text(x1, height/10+20+30*i, "How?", gc_achieve);
            text(x2, height/10+20+30*i, "Goal within 20 seconds.", gc_achieve);
        }else if(i==18) {
            text(x1, height/10+20+30*i, "What did you do?", gc_achieve);
            text(x2, height/10+20+30*i, "Took over 10 minutes to reach the goal.", gc_achieve);
        }else if(i==19) {
            text(x1, height/10+20+30*i, "Safe driving", gc_achieve);
            text(x2, height/10+20+30*i, "Reach the goal without hitting the wall even once.", gc_achieve);
        }
    }
}

void draw_stats(int num[6]) {
    g_fill(0,0,width,height,gc_black);
    text(width/2-13,height/10-30,"Statistics",gc_yellow);

    int i,x1=500,parindent=30;
    GC gc_stats1=gc_lightgreen, gc_stats2=gc_red;
    int run_h=(int)(num[5]/3600) , run_m=(int)((num[5]-(int)(num[5]/3600)*3600)/60), run_s=(int)(num[5]-(int)(num[5]/60)*60);
    char play_str[10], run_h_str[10], run_m_str[10], run_s_str[10], warp_str[10], rotate_str[10], wind_str[10], shield_str[10], magic_str[10]; 
    sprintf(play_str,"%d",(int)num[0]);
    sprintf(run_s_str,"%d",run_s);
    sprintf(run_m_str,"%d",run_m);
    sprintf(run_h_str,"%d",run_h);
    sprintf(warp_str,"%d",(int)num[1]);
    sprintf(rotate_str,"%d",(int)num[2]);
    sprintf(wind_str,"%d",(int)num[3]);
    sprintf(shield_str,"%d",(int)num[4]);
    sprintf(magic_str,"%d",(int)num[1]+(int)num[2]+(int)num[3]+(int)num[4]);

    for(i=0; i<9; i++) {
        if(i==0) {
            text(x1, height/10+20+30*i, "Game play", gc_stats2);
        }else if(i==1) {
            text(x1+parindent, height/10+20+30*i, "Goals:", gc_stats1);
            text(x1+parindent+40, height/10+20+30*i, play_str, gc_stats1);
        }else if(i==2) {
            text(x1+parindent, height/10+20+30*i, "Driving time:", gc_stats1);
            text(x1+parindent+83, height/10+20+30*i, run_h_str, gc_stats1);
            text(x1+parindent+98, height/10+20+30*i, "h", gc_stats1);
            text(x1+parindent+108, height/10+20+30*i, run_m_str, gc_stats1);
            text(x1+parindent+123, height/10+20+30*i, "m", gc_stats1);
            text(x1+parindent+133, height/10+20+30*i, run_s_str, gc_stats1);
            text(x1+parindent+148, height/10+20+30*i, "s", gc_stats1);
        }else if(i==3) {
            text(x1, height/10+20+30*i, "Magic use", gc_stats2);
        }else if(i==4) {
            text(x1+parindent, height/10+20+30*i, "Warp:", gc_stats1);
            text(x1+parindent+32, height/10+20+30*i, warp_str, gc_stats1);
        }else if(i==5) {
            text(x1+parindent, height/10+20+30*i, "Rotate:", gc_stats1);
            text(x1+parindent+45, height/10+20+30*i, rotate_str, gc_stats1);
        }else if(i==6) {
            text(x1+parindent, height/10+20+30*i, "Wind rotate:", gc_stats1);
            text(x1+parindent+75, height/10+20+30*i, wind_str, gc_stats1);
        }else if(i==7) {
            text(x1+parindent, height/10+20+30*i, "Shield:", gc_stats1);
            text(x1+parindent+45, height/10+20+30*i, shield_str, gc_stats1);
        }else if(i==8) {
            text(x1+parindent, height/10+20+30*i, "Total:", gc_stats1);
            text(x1+parindent+40, height/10+20+30*i, magic_str, gc_stats1);
        }
    }
}

void draw_rain_mark(double timer_rain, int *rain_flag, int *shield_flag, double *player_v) {
    if(timer_rain<3) {
        int i,j;
        for(i=height/4; i<height*3/4; i++) {
            for(j=width*3/8; j<width*5/8; j++) {
                double circle_check=(double)(i-height/2)*(i-height/2)/(30*30)+(double)(j-width/2)*(j-width/2)/(30*30);
                if(0.9<=circle_check && circle_check<=1.1) {
                    dot(j,i,gc_lightgreen);
                }
            }
        }
        for(i=height/4; i<height*3/4; i++) {
            for(j=width*3/8; j<width*5/8; j++) {
                double circle_check=(double)(i-height/2)*(i-height/2)/(5*5)+(double)(j-width/2)*(j-width/2)/(5*5);
                if(0.95<=circle_check && circle_check<=1.05) {
                    dot(j,i,gc_lightgreen);
                }
            }
        }
        double theta;
        double pi=3.1415926535;
        for(theta=0; theta<=2*pi; theta+=0.01) {
            if((pi/3<=theta && theta<=pi*2/3) || (pi<=theta && theta<=pi*4/3) || (pi*5/3<=theta && theta<=2*pi)) {
                dot((int)(width/2+15*cos(theta)),(int)(height/2+15*sin(theta)),gc_lightgreen);
            }
        }
        g_line((int)(width/2+5*cos(pi/3)),(int)(height/2+5*sin(pi/3)),(int)(width/2+15*cos(pi/3)),(int)(height/2+15*sin(pi/3)),gc_lightgreen);
        g_line((int)(width/2+5*cos(pi*2/3)),(int)(height/2+5*sin(pi*2/3)),(int)(width/2+15*cos(pi*2/3)),(int)(height/2+15*sin(pi*2/3)),gc_lightgreen);
        g_line((int)(width/2+5*cos(pi)),(int)(height/2+5*sin(pi)),(int)(width/2+15*cos(pi)),(int)(height/2+15*sin(pi)),gc_lightgreen);
        g_line((int)(width/2+5*cos(pi*4/3)),(int)(height/2+5*sin(pi*4/3)),(int)(width/2+15*cos(pi*4/3)),(int)(height/2+15*sin(pi*4/3)),gc_lightgreen);
        g_line((int)(width/2+5*cos(pi*5/3)),(int)(height/2+5*sin(pi*5/3)),(int)(width/2+15*cos(pi*5/3)),(int)(height/2+15*sin(pi*5/3)),gc_lightgreen);
        g_line((int)(width/2+5*cos(0)),(int)(height/2+5*sin(0)),(int)(width/2+15*cos(0)),(int)(height/2+15*sin(0)),gc_lightgreen);
        text(width/2-15,height/2+41,"space",gc_black);
        g_num(width/2+24,height/2+25,10,3-(int)timer_rain,gc_black);
    }else if(timer_rain<7) {
        int i;
        double pi=3.1415926535;
        for(i=0; i<1000; i++) {
            if(rain[i].x1==-100) {
                rain[i].x1=rand()%width;
                rain[i].y1=0;
                double l=30+rand()%100;
                rain[i].x2=rain[i].x1-l*cos(-pi/3);
                rain[i].y2=rain[i].y1-l*sin(-pi/3);
                break;
            }
        }
        if(*shield_flag==0) {
            *player_v=0;
        }
    }else if(timer_rain>7.5) {
        *rain_flag=0;
        *shield_flag=0;
    }
}

void draw_rain() {
    int i=0;
    double pi=3.1415926535;
    for(i=0; i<1000; i++) {
        if(rain[i].x1!=-100) {
            rain[i].x1-=50*cos(pi/3);
            rain[i].x2-=50*cos(pi/3);
            rain[i].y1+=50*sin(pi/3);
            rain[i].y2+=50*sin(pi/3);

            g_line((int)rain[i].x1,(int)rain[i].y1,(int)rain[i].x2,(int)rain[i].y2,gc_lightgreen);
            if(rain[i].y2>height+100) {
                rain[i].x1=-100;
            }
        }
    }
}

void draw_wind(double player_v, double v_weight, int wind_flag) {
    int i;
    if(player_v>0 && v_weight>=3) {
        for(i=0; i<1000; i++) {
            if(wind[i].timer!=-1) {
                g_line((int)wind[i].x1,(int)wind[i].y1,(int)wind[i].x2,(int)wind[i].y2,gc_aqua);
            }
        }
    }
}

void updata_wind() {
    int i,j=0;
    double pi=3.1415926535;

    for(i=0; i<1000; i++) {
        if(wind[i].timer!=-1) {
            if(clock()/1000000.-wind[i].timer>=0.05) {
                wind[i].timer=-1;
            }
        }
    }

    for(i=0; i<1000; i++) {
        if(wind[i].timer==-1) {
            wind[i].timer=clock()/1000000.;
            if(j==0) {
                double y=rand()%720;
                double l1=rand()%50;
                double l2=50+rand()%150;
                double theta=pi/4-pi/(2*720)*y;
                wind[i].x1=l1*cos(theta);
                wind[i].y1=y+l1*sin(theta);
                wind[i].x2=l2*cos(theta);
                wind[i].y2=y+l2*sin(theta);
                j++;
            }else if(j==1) {
                double y=rand()%720;
                double l1=rand()%50;
                double l2=50+rand()%50;
                double theta=pi*3/4+pi/(2*720)*y;
                wind[i].x1=1280+l1*cos(theta);
                wind[i].y1=y+l1*sin(theta);
                wind[i].x2=1280+l2*cos(theta);
                wind[i].y2=y+l2*sin(theta);
                j++;
            }else if(j==2) {
                double x=rand()%1280;
                double l1=rand()%50;
                double l2=50+rand()%50;
                double theta=pi/4+pi/(2*1280)*x;
                wind[i].x1=x+l1*cos(theta);
                wind[i].y1=l1*sin(theta);
                wind[i].x2=x+l2*cos(theta);
                wind[i].y2=l2*sin(theta);
                j++;
            }else if(j==3) {
                double x=rand()%1280;
                double l1=rand()%50;
                double l2=50+rand()%50;
                double theta=-pi/4-pi/(2*1280)*x;
                wind[i].x1=x+l1*cos(theta);
                wind[i].y1=720+l1*sin(theta);
                wind[i].x2=x+l2*cos(theta);
                wind[i].y2=720+l2*sin(theta);
                j=0;
                break;
            }
        }
    }
}