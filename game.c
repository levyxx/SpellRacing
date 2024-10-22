#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "winlib.h"
#include "object.h"
#include "draw.h"
#include "move.h"
#include "save.h"
#include "turtle.h"

double pi=3.1415926535;

typedef struct player{
    double x;
    double y;
    double angle;
    double FOV;
    double v;
} player_t;

int main(){
    srand(time(NULL));
    initwin();
    init_rgb_all();

    width=1280;
    height=720;

    player_t player;
    player.x=45, player.y=645, player.angle=3*pi/2, player.FOV=pi/2, player.v=0;
    //player.x=1185, player.y=90, player.angle=-pi/2, player.FOV=pi/2, player.v=0; //ゴール手前(デバッグ用)
    
    int i,j,k,l,dx,dy;
    double di,dj;
    int is_race_start=0, ok_startdash=1, is_generating=0;
    double timer=0,timer_start=-1,timer_end=-1,timer_start_wind=-1,timer_start_rain=-1,timer_end_rain=-1,timer_rain=0;
    double g_time=1e9;
    double view_dist=width/4;
    double max_player_v=3, v_weight=1;
    double n=0.03,h,v,fv;
    double xl,yl,xr,yr,xm,ym;
    int menu=1,gen=0,make=0,game=0,goal=0,ranking=0,achievement=0,stats=0;
    int mode=0; //0:game 1:ranking 2:achievement 3:stats 4:exit
    int ground=0;
    double t=0;
    int w_flag=0, s_flag=0, d_flag=0, a_flag=0, D_flag=0, A_flag=0, shift_flag=0, hit_flag=0;
    int r_angle, r_length, max_length;
    int D_count=0,A_count=0;
    int converted_player_v;
    int can_warp=0, warp_remain=3, left_remain=3, right_remain=3, wind_remain=3, wind_flag=0, shield_flag=0, rain_flag=0, shield_times=0;
    int dist=0;
    double count=0;
    int num_record;

    //rmapを初期化
    init_rmap();
    while(rmap[21][39]==0 || dist==0) {
        init_rmap();
        //mapを生成
        dig(1,1,23,41);
        dist=bfs(1,1);
    }

    //mapとcarを初期化
    init_object(width,height);

    //酸性雨の初期化
    for(i=0; i<1000; i++) {
        rain[i].x1=-100;
    }

    //風の初期化
    for(i=0; i<1000; i++) {
        wind[i].timer=-1;
    }
    
    while(1) {
        g_fill(0,0,width,height,gc_black);
        if(menu==1) {
            double leng=900,sx=190,sy=600;
            GC gc_koch=gc_red;
            set_pos(sx,sy);
            set_angle(-60);
		    koch(leng,5,gc_koch);
            set_angle(60);
            koch(leng,5,gc_koch);
            set_angle(180);
            koch(leng,5,gc_koch);

            GC gc_title=gc_aqua;
            draw_title(gc_title);
            
            text(width/2-20+5,height*9/10-80,"start",gc_lightgreen);
            text(width/2-20,height*9/10-50,"ranking",gc_lightgreen);
            text(width/2-20-13,height*9/10-20,"achievement",gc_lightgreen);
            text(width/2-20-8,height*9/10+10,"statistics",gc_lightgreen);
            text(width/2-20+8,height*9/10+40,"exit",gc_lightgreen);
            if(mode==0) {
                text(width/2+40,height*9/10-80,"<=",gc_lightgreen);
            }else if(mode==1) {
                text(width/2+40,height*9/10-50,"<=",gc_lightgreen);
            }else if(mode==2) {
                text(width/2+40,height*9/10-20,"<=",gc_lightgreen);
            }else if(mode==3) {
                text(width/2+40,height*9/10+10,"<=",gc_lightgreen);
            }else if(mode==4) {
                text(width/2+40,height*9/10+40,"<=",gc_lightgreen);
            }


            while (XPending(disp)) {
                XNextEvent(disp, &event);
                if (event.type == KeyPress) {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    if(keysym == XK_Return) {
                        if(mode==0) {        //game
                            menu=0;
                            gen=1;
                            is_generating=1;
                            timer_start = clock()/1000000.;
                        }else if(mode==1) {  //ranking
                            menu=0;
                            ranking=1;
                        }else if(mode==2) {  //achievement
                            menu=0;
                            achievement=1;
                        }else if(mode==3) {  //stats
                            menu=0;
                            stats=1;
                        }else if(mode==4) {  //exit
                            menu=0;
                            return 0;
                        }
                    }else if(keysym == XK_Up) {
                        mode+=4;
                        mode%=5;
                    }else if(keysym == XK_Down) {
                        mode++;
                        mode%=5;
                    }else if(keysym == XK_Escape) {
                        menu=0;
                        return 0;
                    }
                }
            }

            flush_display();
            usleep(1000000/120);
            continue;
        }

        //ランキングの表示
        if(ranking) {
            num_record=load_times("times.txt", record);
            sort_times(record, num_record);
            to_vector(record, num_record, m, s, ms);
            draw_result(m, s, ms, num_record, goal, timer);
            text(40,50,"< Esc", gc_lightgreen);

            while (XPending(disp)) {
                XNextEvent(disp, &event);
                if (event.type == KeyPress) {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    if(keysym == XK_Escape) {
                        menu=1;
                        ranking=0;
                    }
                }
            }

            flush_display();
            usleep(1000000/120);
            continue;
        }

        //実績の表示
        if(achievement) {
            achieve_updata("achievement.txt",warp_remain,left_remain,right_remain,wind_remain,shield_times,300,hit_flag,goal);
            draw_achievement(achieve);
            text(40,50,"< Esc", gc_lightgreen);

            while (XPending(disp)) {
                XNextEvent(disp, &event);
                if (event.type == KeyPress) {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    if(keysym == XK_Escape) {
                        menu=1;
                        achievement=0;
                    }
                }
            }

            flush_display();
            usleep(1000000/120);
            continue;
        }

        //統計の表示
        if(stats) {
            load_stats("statistics.txt",num);
            draw_stats(num);
            text(40,50,"< Esc", gc_lightgreen);

            while (XPending(disp)) {
                XNextEvent(disp, &event);
                if (event.type == KeyPress) {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    if(keysym == XK_Escape) {
                        menu=1;
                        stats=0;
                    }
                }
            }

            flush_display();
            usleep(1000000/120);
            continue;
        }

        //タイマーの更新
        if(!goal) {
            timer_end = clock()/1000000.;
        }
        if(gen==1) {
            timer=(timer_end-timer_start);
        }else if(is_race_start==0) {
            timer=0;
        }else {
            timer=(timer_end-timer_start)*1.5;
        }

        if(gen==1) {
            //マップ生成の描画
            generating_map(timer, order_i, order_j, &is_generating, &gen, &g_time, dist);

            flush_display();
            usleep(1000000/120);
            continue;
        }
        
        //疑似3Dでマップを描画
        draw_3d(map, width, height, view_dist, n, player.angle, player.FOV, player.x, player.y, &can_warp);

        //空の描画
        draw_sky();

        //車体の影の描画(位置は固定)
        if(ground==1) {
            draw_car_shade();
        }

        if(shield_flag==1) {
            draw_box(&t);
        }

        //車体の描画
        draw_car(d_flag, D_flag, a_flag, A_flag, player.v, car);

        //スピードメータの描画
        draw_speed_meter(player.v, max_player_v);

        //タイマーの表示(順に小数,秒,時間)
        draw_timer(timer);

        //minimap表示
        draw_minimap(map, player.x, player.y, player.angle);

        //魔法のアイコンを表示
        draw_magic_icon(warp_remain, left_remain, right_remain, wind_remain);

        if(goal) {
            if(player.y>50) {
                player.y-=0.5;
            }else if(player.angle>-7*pi/6) {
                player.angle-=pi/50.;
            }else {
                if(n<2) {
                    ground=0;
                    n+=0.02;
                }else {
                    draw_result(m, s, ms, num_record, goal, timer);
                    text(40,50,"< Esc", gc_lightgreen);

                    while (XPending(disp)) {
                        XNextEvent(disp, &event);
                        if (event.type == KeyPress) {
                            KeySym keysym = XLookupKeysym(&event.xkey, 0);
                            if(keysym == XK_Escape) {
                                
                                //全ての変数を初期化してメニューに戻る

                                player.x=45, player.y=645, player.angle=3*pi/2, player.FOV=pi/2, player.v=0;
                                //player.x=1185, player.y=90, player.angle=-pi/2, player.FOV=pi/2, player.v=0; //ゴール手前(デバッグ用)
                                is_race_start=0, ok_startdash=1, is_generating=0;
                                timer=0,timer_start=-1,timer_end=-1,timer_start_wind=-1,timer_start_rain=-1,timer_end_rain=-1,timer_rain=0;
                                g_time=1e9;
                                view_dist=width/4;
                                max_player_v=3, v_weight=1;
                                n=0.03;
                                menu=1,gen=0,make=0,game=0,goal=0,ranking=0,achievement=0,stats=0;
                                mode=0; //0:game 1:ranking 2:exit
                                ground=0;
                                t=0;
                                w_flag=0, s_flag=0, d_flag=0, a_flag=0, D_flag=0, A_flag=0, shift_flag=0, hit_flag=0;
                                r_angle, r_length, max_length;
                                D_count=0,A_count=0;
                                converted_player_v;
                                can_warp=0, warp_remain=3, left_remain=3, right_remain=3, wind_remain=3, wind_flag=0, shield_flag=0, rain_flag=0, shield_times=0;
                                dist=0;
                                count=0;
                                num_record;

                                //rmapを初期化
                                init_rmap();
                                while(rmap[21][39]==0 || dist==0) {
                                    init_rmap();
                                    //mapを生成
                                    dig(1,1,23,41);
                                    dist=bfs(1,1);
                                }

                                //mapとcarを初期化
                                init_object(width,height);

                                //酸性雨の初期化
                                for(i=0; i<1000; i++) {
                                    rain[i].x1=-100;
                                }

                            }
                        }
                    }
                }
            }

            flush_display();
            usleep(1000000/120);
            continue;
        }

        //カウントダウン
        if(is_race_start==0) {
            draw_countdown(&is_race_start,&timer_start,&timer_end,&ok_startdash,&v_weight,&n,&player.angle,&ground);
            flush_display();
            usleep(1000000/120);
            continue;
        }


        //キーの検出
        while (XPending(disp)) {
            XNextEvent(disp, &event);
            if (event.type == KeyPress) {
                KeySym keysym = XLookupKeysym(&event.xkey, 0);
                if(keysym == XK_Shift_L) {
                    shift_flag=1;
                }

                if (keysym == XK_d) {
                    if(shift_flag==1) {
                        D_flag=1;
                    }else {
                        d_flag = 1;
                    }
                }else if (keysym == XK_a) {
                    if(shift_flag==1) {
                        A_flag=1;
                    }else {
                        a_flag = 1;
                    }
                }else if (keysym == XK_w) {
                    w_flag = 1;
                }else if (keysym == XK_s) {
                    s_flag = 1;
                }else if (keysym == XK_k) {
                    if(warp_remain>0 && can_warp) {
                        warp_remain--;
                        warp(map, player.angle, &player.x, &player.y);
                    }
                }else if (keysym == XK_j) {
                    if(left_remain>0) {
                        left_remain--;
                        rotate_angle(map, 1, &player.angle, &player.x, &player.y, &left_remain, &right_remain);
                    }
                }else if (keysym == XK_l) {
                    if(right_remain>0) {
                        right_remain--;
                        rotate_angle(map, 0, &player.angle, &player.x, &player.y, &left_remain, &right_remain);
                    }
                }else if (keysym == XK_p) {
                    if(wind_remain>0 && wind_flag==0) {
                        wind_remain--;
                        timer_start_wind = clock()/1000000.;
                        wind_flag=1;
                    }
                }else if (keysym == XK_space) {
                    if(0<timer_rain && timer_rain<3 && rain_flag==1 && shield_flag==0) {
                        shield_times++;
                        shield_flag=1;
                    }
                }
            } else if (event.type == KeyRelease) {
                KeySym keysym = XLookupKeysym(&event.xkey, 0);
                if(keysym == XK_Shift_L) {
                    shift_flag=0;
                }

                if (keysym == XK_d) {
                    D_flag=0, d_flag=0;
                } else if (keysym == XK_a) {
                    A_flag=0, a_flag=0;
                } else if (keysym == XK_w) {
                    w_flag = 0;
                } else if (keysym == XK_s) {
                    s_flag = 0;
                }
            }
        }
        
        //視点移動
        move_view(map, &player.angle, player.v, player.x, player.y, D_flag, d_flag, A_flag, a_flag);

        //速度の変更
        change_v(&player.v, &v_weight, w_flag, s_flag, timer);

        //プレイヤーの移動
        move_player(map, &player.x, &player.y, &player.v, player.angle, &hit_flag);

        //ドリフトの摩擦熱の描画
        draw_drift(&D_flag, &A_flag, player.v, &D_count, &A_count);

        //ワープ先の魔法陣を描画
        draw_magic(can_warp, warp_remain);

        //風加速
        wind_accelerate(timer_start_wind, &v_weight, &wind_flag);

        //風の表示
        draw_wind(player.v, v_weight, wind_flag);

        //風の更新
        updata_wind();

        //酸性雨発生の抽選
        if(rain_flag==0) {
            int r_rain=rand()%500;
            if(r_rain==0) {
                rain_flag=1;
                timer_start_rain=clock()/1000000.;
            }
        }

        //酸性雨のマークを表示
        if(rain_flag==1) {
            timer_end_rain=clock()/1000000.;
            timer_rain=timer_end_rain-timer_start_rain;
            draw_rain_mark(timer_rain, &rain_flag, &shield_flag, &player.v);

            //酸性雨の描画
            if(3<timer_rain && timer_rain<7.5) {
                draw_rain();
            }
        }


        if(1170<=player.x && player.x<=1200 && player.y<=75) {
            goal=1;
            player.v=0;
            player.angle=-pi/2;
            d_flag=0,D_flag=0,a_flag=0,A_flag=0;
            shield_flag=0;
            rain_flag=0;
            double timer_sec=timer-(int)(timer/60)*60;
            save_time("times.txt", (int)(timer/60)%100, (int)(timer_sec)%100, (int)(timer*100)%100);
            num_record=load_times("times.txt", record);
            sort_times(record, num_record);
            to_vector(record, num_record, m, s, ms);
            save_stats("statistics.txt", warp_remain, left_remain, right_remain, wind_remain, shield_times, (int)timer);
            load_stats("statistics.txt", num);
            achieve_updata("achievement.txt", warp_remain, left_remain, right_remain, wind_remain, shield_times, timer, hit_flag, goal);
        }

        
        flush_display();  //裏画面を表画面に描画
        usleep(1000000/120);
    }
    

    return 0;
}