#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "winlib.h"

#define DISPLAY_DEPTH    8

Display *disp;
int scrn;
Window win;
Colormap cmap;
GC    gc, gc_red, gc_lightgreen, gc_blue[256], gc_white, gc_black, gc_brown, gc_aqua, gc_gray, gc_green, gc_lightgray, gc_orange[256],
      gc_lightyellow, gc_yellow, gc_darkyellow, gc_pink, gc_purple, gc_softpurple, gc_grayscale[255], gc_purplescale[255], gc_greenscale[255], gc_redscale[255];
XImage *image;
XColor color;
FILE *fp;
XEvent event;

int width;
int height;

Pixmap backbuffer;

FatalError(char *str)
{
  if (fp != NULL) {
    fclose(fp);
  }
  fprintf(stderr,"Error:%s\n",str);
  
  fflush(stderr);
  exit(1);
}



void initwin(){
  XSetWindowAttributes wa;
  XVisualInfo vi;
  XSizeHints hints;
  XEvent event;
  if((disp =XOpenDisplay(NULL))==NULL){
    FatalError("Can't open display.");
  }
  scrn = DefaultScreen(disp);

  //width = DisplayWidth(disp, scrn);
  //height = DisplayHeight(disp, scrn);
  width=1280;
  height=720;

  /*
    if(! XMatchVisualInfo(disp,scrn,DISPLAY_DEPTH,DirectColor, &vi)){
    FatalError("Can't open color display.");
    }
  wa.background_pixel = 0;
  wa.border_pixel =0;
  wa.colormap =XCreateColormap(disp,RootWindow(disp,scrn),
			      vi.visual,AllocNone);
  wa.event_mask = KeyPressMask |StructureNotifyMask |ExposureMask
    |ButtonPressMask;
  */
  
  win = XCreateSimpleWindow(disp, RootWindow(disp,scrn),0,0,width,height,2,2, WhitePixel(disp,scrn) );
  backbuffer = XCreatePixmap(disp, win, width, height, DefaultDepth(disp, 0));
  XSelectInput(disp,win,KeyPressMask | KeyReleaseMask);


  /*  image =XCreateImage(disp, qvi.visual, vi.depth,
		      ZPixmap, 0, 0, width, height, 32, 0);
  if ((image->data = (char *)malloc(image->bytes_per_line*height)) ==NULL){
    FatalError("No memory..");
  }
  memset(image->data,0, image->bytes_per_line*height);
  */
  XMapWindow(disp,win);
  gc = XCreateGC(disp, win, 0,0);
  gc_red = XCreateGC(disp, win, 0,0);
  gc_lightgreen = XCreateGC(disp, win, 0,0);
  gc_white = XCreateGC(disp, win, 0,0);
  gc_black = XCreateGC(disp, win, 0,0);
  gc_brown = XCreateGC(disp, win, 0,0);
  gc_aqua = XCreateGC(disp, win, 0,0);
  gc_gray = XCreateGC(disp, win, 0,0);
  gc_green = XCreateGC(disp, win, 0,0);
  gc_lightgray = XCreateGC(disp, win, 0,0);
  gc_lightyellow = XCreateGC(disp, win, 0,0);
  gc_yellow = XCreateGC(disp, win, 0,0);
  gc_darkyellow = XCreateGC(disp, win, 0,0);
  gc_pink = XCreateGC(disp, win, 0,0);
  gc_softpurple = XCreateGC(disp, win, 0,0);
  gc_purple = XCreateGC(disp, win, 0,0);
  int i;
  for(i=0; i<256; i++) {
    gc_blue[i] = XCreateGC(disp, win, 0,0);
    gc_orange[i] = XCreateGC(disp, win, 0,0);
    gc_grayscale[i] = XCreateGC(disp, win, 0,0);
    gc_purplescale[i] = XCreateGC(disp, win, 0,0);
    gc_greenscale[i] = XCreateGC(disp, win, 0,0);
    gc_redscale[i] = XCreateGC(disp, win, 0,0);
  }
  XSetForeground(disp, gc, BlackPixel(disp,scrn));
  XSetBackground(disp, gc, WhitePixel(disp,scrn));
  XSetFunction(disp, gc, GXcopy);
  XMapRaised(disp,win);
  g_clear();
  XFlush(disp);
  
}

void dot(int x, int y, GC gc){
  XDrawLine(disp,backbuffer,gc,x,y,x,y);
}

void text(int x, int y, char *str, GC gc){
  XDrawString(disp,backbuffer,gc,x,y,str,strlen(str));
}

void g_line(int x0, int y0, int x1, int y1, GC gc){
  XDrawLine(disp,backbuffer,gc,x0,y0,x1,y1);
}

void g_rgb(unsigned short r, unsigned short g, unsigned short b){
  XColor xc;
  xc.red = r; xc.green = g; xc.blue = b;
  XAllocColor(disp,DefaultColormap(disp,scrn),&xc);
  XSetForeground(disp,gc,xc.pixel);
}

void g_fill(int x0,int y0, int wid, int hei, GC gc){
  XFillRectangle(disp,backbuffer,gc,x0,y0,wid, hei);
}

void g_box(int x0, int y0, int wid, int hei, GC gc){
  XDrawRectangle(disp,backbuffer,gc,x0,y0,wid, hei);
}

void g_clear(){
  g_rgb(0,0,0);
  //g_rgb(65535,65535,65535);
  XFillRectangle(disp,win,gc,0,0,width, height);    
  //g_rgb(0,0,0);
  g_rgb(65535,65535,65535);
}

int g_width() {
    return DisplayWidth(disp, scrn);
}

int g_height() {
    return DisplayHeight(disp, scrn);
}

void flush_display() {
    XCopyArea(disp, backbuffer, win, gc, 0, 0, width, height, 0, 0);
    XFlush(disp);
}

void init_rgb(unsigned short r, unsigned short g, unsigned short b, GC gc) {
    XColor xc;
    xc.red = r, xc.green = g, xc.blue = b;
    XAllocColor(disp, DefaultColormap(disp, scrn), &xc);
    XSetForeground(disp, gc, xc.pixel);
}

void init_rgb_all() {
    init_rgb(65535, 0, 0, gc_red);
    init_rgb(0, 65535, 0, gc_lightgreen);
    init_rgb(0, 0, 0, gc_black);
    init_rgb(65535,65535,65535, gc_white);
    init_rgb(65535*116/255,65535*80/255,65535*48/255, gc_brown);
    init_rgb(0,65535,65535, gc_aqua);
    init_rgb(65535*110/255,65535*110/255,65535*110/255, gc_gray);
    init_rgb(65535*150/255,65535*150/255,65535*150/255, gc_lightgray);
    init_rgb(0,65535*155/255,0, gc_green);
    init_rgb(65535,65535,0, gc_lightyellow);
    init_rgb(65535,65535*210/255,0, gc_yellow);
    init_rgb(65535*153/255,65535*102/255,0, gc_darkyellow);
    init_rgb(65535,65535*192/255,65535*203/255, gc_pink);
    init_rgb(65535,0,65535, gc_purple);
    init_rgb(65535*184/255,65535*162/255,65535*226/255, gc_softpurple);
    int i;
    for(i=0; i<256; i++) {
      init_rgb(0,0,65535*i/255, gc_blue[i]);
      init_rgb(65535,65535*i/255,0, gc_orange[i]);
      init_rgb(65535*i/255,65535*i/255,65535*i/255, gc_grayscale[i]);
      init_rgb(65535*i/255,0,65535*i/255, gc_purplescale[i]);
      init_rgb(0,65535*i/255,0, gc_greenscale[i]);
      init_rgb(65535*i/255,0,0, gc_redscale[i]);
    }
}

void g_segment_vertical(int x0, int y0, int size, GC gc) {
  int i;
  for(i=-size/6; i<=size/6; i++) {
    g_line(x0+i,y0-size/2+1+abs(i),x0+i,y0+size/2-1-abs(i),gc);
  }
}

void g_segment_horizontal(int x0, int y0, int size, GC gc) {
  int i;
  for(i=-size/6; i<=size/6; i++) {
    g_line(x0-size/2+1+abs(i),y0+i,x0+size/2-1-abs(i),y0+i,gc);
  }
}

void g_num(int x0, int y0, int size, int n, GC gc) {
  if(n==1) {
    g_segment_vertical(x0+size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
  }else if(n==2) {
    g_segment_vertical(x0+size/2,y0-size/2,size,gc);
    g_segment_vertical(x0-size/2,y0+size/2,size,gc);
    g_segment_horizontal(x0,y0-size,size,gc);
    g_segment_horizontal(x0,y0,size,gc);
    g_segment_horizontal(x0,y0+size,size,gc);
  }else if(n==3) {
    g_segment_vertical(x0+size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
    g_segment_horizontal(x0,y0-size,size,gc);
    g_segment_horizontal(x0,y0,size,gc);
    g_segment_horizontal(x0,y0+size,size,gc);
  }else if(n==4) {
    g_segment_vertical(x0+size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
    g_segment_vertical(x0-size/2,y0-size/2,size,gc);
    g_segment_horizontal(x0,y0,size,gc);
  }else if(n==5) {
    g_segment_vertical(x0-size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
    g_segment_horizontal(x0,y0-size,size,gc);
    g_segment_horizontal(x0,y0,size,gc);
    g_segment_horizontal(x0,y0+size,size,gc);
  }else if(n==6) {
    g_segment_vertical(x0-size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
    g_segment_vertical(x0-size/2,y0+size/2,size,gc);
    g_segment_horizontal(x0,y0-size,size,gc);
    g_segment_horizontal(x0,y0,size,gc);
    g_segment_horizontal(x0,y0+size,size,gc);
  }else if(n==7) {
    g_segment_vertical(x0+size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
    g_segment_horizontal(x0,y0-size,size,gc);
  }else if(n==8) {
    g_segment_vertical(x0+size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
    g_segment_vertical(x0-size/2,y0-size/2,size,gc);
    g_segment_vertical(x0-size/2,y0+size/2,size,gc);
    g_segment_horizontal(x0,y0-size,size,gc);
    g_segment_horizontal(x0,y0,size,gc);
    g_segment_horizontal(x0,y0+size,size,gc);
  }else if(n==9) {
    g_segment_vertical(x0+size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
    g_segment_vertical(x0-size/2,y0-size/2,size,gc);
    g_segment_horizontal(x0,y0-size,size,gc);
    g_segment_horizontal(x0,y0,size,gc);
    g_segment_horizontal(x0,y0+size,size,gc);
  }else if(n==0) {
    g_segment_vertical(x0+size/2,y0-size/2,size,gc);
    g_segment_vertical(x0+size/2,y0+size/2,size,gc);
    g_segment_vertical(x0-size/2,y0-size/2,size,gc);
    g_segment_vertical(x0-size/2,y0+size/2,size,gc);
    g_segment_horizontal(x0,y0-size,size,gc);
    g_segment_horizontal(x0,y0+size,size,gc);
  }
}



