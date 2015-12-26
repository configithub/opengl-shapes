#ifndef _RENDER_TOOLS_H_
#define _RENDER_TOOLS_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <stdbool.h>

#define WWIDTH 400
#define WHEIGHT 300
#define BPP_DEPTH 16

typedef struct {
  int x; int y;
} fpoint;

bool init_sdl();

void clear_screen();

void load_png(char* name, int* texture_id);

void draw_line(int x1, int y1, int x2, int y2);

void draw_triangle(int x1, int x2, int y1, int y2, 
                                        int x3, int y3);

void draw_square(int x1, int y1, int x2, int y2, 
                 int x3, int y3, int x4, int y4);

void draw_fsquare(int x, int y, int r);
void draw_point(int x, int y, int r);

void draw_all_rays(int *x, int *y);

void draw_square_ray_opaque(int x1, int y1, int x2, int y2, 
                 int x3, int y3, int x4, int y4);

void draw_rectangle_texture(GLuint texture, int x, int y, 
                  int xtex, int ytex, int wtex, int htex, 
                GLfloat r, GLfloat g, GLfloat b, GLfloat a);


void draw_ngon(fpoint* center, int r, int n);
void draw_ngon2(fpoint* center, int r, int n);
void draw_ngon3(fpoint* center, int r, int n);
void draw_ngon4(fpoint* center, int r, int n);
void draw_stargon(fpoint* center, int r, int n);

#endif
