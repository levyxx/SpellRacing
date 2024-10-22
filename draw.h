typedef struct{
    double x1;
    double y1;
    double x2;
    double y2;
} rain_t;

typedef struct{
    double x1;
    double y1;
    double x2;
    double y2;
    double timer;
} wind_t;

extern int m[10],s[10],ms[10];
extern rain_t rain[1000];
extern wind_t wind[1000];

void draw_title(GC gc);
void koch(double leng, int n, GC gc);
void draw_box(double *t);

void draw_sky();
void draw_car_shade();
void draw_car(int d_flag, int D_flag, int a_flag, int A_flag, double player_v, int car[36][53]);
void draw_speed_meter(double player_v, double max_player_v);
void draw_timer(double timer);
void draw_minimap(int map[720][1280], double player_x, double player_y, double player_angle);
void draw_countdown(int *is_race_start, double *timer_start, double *timer_end, int *ok_startdash, double *v_weight, double *n, double *player_angle, int *ground);
void draw_drift(int *D_flag, int *A_flag, double player_v, int *D_count, int *A_count);
void draw_3d(int map[720][1280], int width, int height, double view_dist, double n, double player_angle, double FOV, double player_x, double player_y, int *can_warp);
void generating_map(double timer, int order_i[23*41], int order_j[23*41], int *is_generating, int *gen, double *g_time, int dist);
void draw_magic(int can_warp, int warp_remain);
void draw_magic_icon(int warp_remain, int left_remain, int right_remain, int wind_remain);
void draw_result(int m[10], int s[10], int ms[10], int count, int goal, double timer);
void draw_achievement(int achieve[18]);
void draw_stats(int num[6]);
void draw_rain_mark(double timer_rain, int *rain_flag, int *shield_flag, double *player_v);
void draw_rain();
void draw_wind(double player_vdouble, double v_weight, int wind_flag);
void updata_wind();