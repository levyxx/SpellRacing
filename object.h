extern int map[720][1280];
extern int rmap[23][41];
extern int dist[23][41];
extern int order_i[23*41];
extern int order_j[23*41];
extern int car[36][53];

void init_object(int width, int height);
void init_rmap();
void dig(int i, int j, int rmap_height, int rmap_width);
int bfs(int sx, int sy);