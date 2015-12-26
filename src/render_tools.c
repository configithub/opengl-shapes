#include "render_tools.h"
#include <tgmath.h>
#include <stdlib.h>

typedef struct {
  int w;
  int h;
} wh;

#define max_texture_sheets 50
wh wh_array[max_texture_sheets]; 

typedef struct {
  int x;
  int y;
} point;

#define max_ray_corners 200
point ray_corners[max_ray_corners]; 
int current_ray_corner = 0;

void register_ray_corner(int x, int y) {
  // we will potentially check every point registered here, bad performances
  for (int i = 0; i < current_ray_corner; ++i) {
    if(ray_corners[i].x == x && ray_corners[i].y == y) { return; }
  }
  ray_corners[current_ray_corner].x = x;
  ray_corners[current_ray_corner].y = y;
  ++current_ray_corner;
}

bool init_sdl() {
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) == -1 ) {
    return false;
  }
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  if(! SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_OPENGL|SDL_DOUBLEBUF)  ) {
    return false;
  }
  glEnable(GL_BLEND);
  glViewport(0, 0, WWIDTH, WHEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WWIDTH, WHEIGHT, 0, -1, 1);
  glScalef(1, 1, 1);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);
  return true;
}


void clear_screen() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void load_png(char* name, int* texture_id) {
  SDL_Surface *pic = IMG_Load(name);
  GLenum glFormat = (pic->format->BitsPerPixel == 32 ? GL_RGBA : GL_RGB);
  GLuint currentTexture = 0;
  glGenTextures(1, &currentTexture);

  glBindTexture(GL_TEXTURE_2D, currentTexture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, glFormat, pic->w, pic->h, 0, glFormat, GL_UNSIGNED_BYTE, NULL);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pic->w, pic->h, glFormat, GL_UNSIGNED_BYTE, pic->pixels);

  wh_array[currentTexture].w = pic->w;
  wh_array[currentTexture].h = pic->h;
  SDL_FreeSurface(pic);
  *texture_id = currentTexture;
}

void draw_line(int x1, int y1, int x2, int y2) {
  GLfloat vertices[] = {x1,y1, x2,y2};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_LINES, 0, 2);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_triangle(int x1, int y1, int x2, int y2, 
                                        int x3, int y3) {
  GLfloat vertices[] = {x1,y1, x2,y2, x3,y3};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableClientState(GL_VERTEX_ARRAY);
}


void draw_square(int x1, int y1, int x2, int y2, 
                 int x3, int y3, int x4, int y4) {
  GLfloat vertices[] = {x1,y1, x2,y2, x3,y3, x4,y4};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_all_rays(int *x, int *y) {
  for (int i = 0; i < current_ray_corner; ++i) {
    draw_line(*x, *y, ray_corners[i].x, ray_corners[i].y);
  }
}

void draw_square_ray_opaque(int x1, int y1, int x2, int y2, 
                 int x3, int y3, int x4, int y4) {
  GLfloat vertices[] = {x1,y1, x2,y2, x3,y3, x4,y4};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableClientState(GL_VERTEX_ARRAY);
  // add rectangle corners to the ray corners array
  register_ray_corner(x1, y1);
  register_ray_corner(x2, y2);
  register_ray_corner(x3, y3);
  register_ray_corner(x4, y4);
}


void draw_rectangle_texture(GLuint texture, int x, int y, 
                    int xtex, int ytex, int wtex, int htex, 
                GLfloat r, GLfloat g, GLfloat b, GLfloat a) {

  int totalTexW = wh_array[texture].w;
  int totalTexH = wh_array[texture].h;

  float x1 = xtex / (float)totalTexW;  
  float y1 = ytex / (float)totalTexH;  

  float x2 = (xtex + wtex) / (float)totalTexW;  
  float y2 = (ytex + htex) / (float)totalTexH;  

  GLfloat textureCoordinates[] = { x1, y1, // top left
    x2, y1,  // top right
    x2, y2,  // bottom right
    x1, y2}; // bottom left
  GLfloat vertices[] = { (float)x, (float)y, // top left
    (float)x + wtex, (float)y, // top right
    (float)x + wtex, (float)y + htex, // bottom right
    (float)x, (float)y + htex }; // bottom left
  glColor4f(r, g, b, a);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void draw_ngon(fpoint* center, int r, int n) {
  int i; fpoint a; fpoint b;
  float pi = 3.1416;
  for(i=0; i<n; ++i) { 
    float theta = 2*i*pi / n; 
    a.x = center->x + r * cos(theta); 
    a.y = center->y + r * sin(theta);
    theta = 2*(i+1)*pi / n; 
    b.x = center->x + r * cos(theta);
    b.x = center->x + r * cos(theta) * cos(pi/n) / cos(fmod(theta, 2*pi/n)-pi/n);
    b.y = center->y + r * sin(theta);
    draw_line((int)a.x, (int)a.y, (int)b.x, (int)b.y);
  }
}



