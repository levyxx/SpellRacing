typedef struct{
    int m;
    int s;
    int ms;
} record_t;

extern record_t record[10000];
extern int num[6];
extern int achieve[20];

void save_time(const char *filename, int m, int s, int ms);
int load_times(const char *filename, record_t record[10000]);
void sort_times(record_t record[10000], int num_record);
void to_vector(record_t record[10000], int num_record, int m[10], int s[10], int ms[10]);
void save_stats(const char *filename, int warp_remain, int left_remain, int right_remain, int wind_remain, int shieled_times, int timer);
void load_stats(const char *filename, int num[6]);
void achieve_updata(const char *filename, int warp_remain, int left_remain, int right_remain, int wind_remain, int shield_times, double timer, double hit_flag, int goal);