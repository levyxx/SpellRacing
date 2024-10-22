#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern Display *disp;
extern int scrn;
extern Window win;
extern Colormap cmap;
extern GC   gc, gc_red, gc_lightgreen, gc_blue[256], gc_white, gc_black, gc_brown, gc_aqua, gc_gray, gc_green, gc_lightgray, gc_orange[256],
            gc_lightyellow, gc_yellow, gc_darkyellow, gc_pink, gc_purple, gc_softpurple, gc_grayscale[255], gc_purplescale[255], gc_greenscale[255], gc_redscale[255];
extern XImage *image;
extern XColor color;
extern FILE *fp;
extern XEvent event;

extern int width;
extern int height;

extern Pixmap backbuffer;

void initwin();
void dot(int x, int y, GC gc);
void text(int x, int y,char *str, GC gc);

void g_line(int x0,int y0, int x1, int y1, GC gc);
void g_rgb(unsigned short r,unsigned short g, unsigned short b);
void g_fill(int x0,int y0, int wid, int hei, GC gc);
void g_box(int x0,int y0, int wid, int hei, GC gc);
void g_segment_vertical(int x0, int y0, int size, GC gc);
void g_segment_horizontal(int x0, int y0, int size, GC gc);
void g_num(int x0,int y0, int size, int n, GC gc);
void g_clear();

int g_width();
int g_height();

void flush_display();

void init_rgb(unsigned short r, unsigned short g, unsigned short b, GC gc);
void init_rgb_all();