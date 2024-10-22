void move_view(int map[720][1280], double *player_angle, double player_v, double player_x, double player_y, int D_flag, int d_flag, int A_flag, int a_flag);
void change_v(double *player_v, double *v_weight, int w_flag, int s_flag, double timer);
void move_player(int map[720][1280], double *player_x, double *player_y, double *player_v, double player_angle, int *hit_flag);
void warp(int map[720][1280], double player_angle, double *player_x, double *player_y);
void rotate_angle(int map[720][1280], int angle, double *player_angle, double *player_x, double *player_y, int *left_remain, int *right_remain);
void wind_accelerate(double timer_start_wind, double *v_weight, int *wind_flag);