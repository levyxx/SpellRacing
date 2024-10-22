/*
  turtle.c
  winlibを用いたタートルグラフィック用ライブラリ  
*/

#include <math.h>
#include "winlib.h"

double r = 0.0, x = 100.0, y = 100.0; /* 現在の角度、x座標、y座標 */
int pen = 0;   /* ペンを下ろしているとき０、上げているとき１  */

void set_angle(double r0) {
  r = r0;
}
double get_angle(){
  return r;
}

double get_posx() {
  return x;
}

double get_posy() {
  return y;
}


void set_pos(int x0, int y0) {
  x = (double)x0;
  y = (double)y0;
}

void set_dpos(double x0, double y0) {
  x = x0;
  y = y0;
}

void dforward(double len, GC gc) {
  double new_x, new_y;
  //ペンを下ろしているときは現在地から移動先に直線をひく
  if (pen == 0) {
    new_x = x + len * cos(2.0 * M_PI * r / 360.0);
    new_y = y + len * sin(2.0 * M_PI * r / 360.0);
    g_line((int)(x+0.5), (int)(y+0.5), (int)(new_x+0.5), (int)(new_y+0.5), gc);
    x = new_x;
    y = new_y;
  //ペンが上げているときは座標の移動のみ
  } else {
    x = x + len * cos(2.0 * M_PI * r / 360.0);
    y = y + len * sin(2.0 * M_PI * r / 360.0);
  }
}

void dturn(double r0) {
  r = r + r0;
}

void turn(int r0) {
  dturn((double)r0);
}

void up() {
  pen = 1;
}

void down() {
  pen = 0;
}

