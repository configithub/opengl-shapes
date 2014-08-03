#include "render_tools.h"

SDL_Event events;
bool running;

int mouse_x, mouse_y;

void manage_inputs() {
  while(SDL_PollEvent(&events)) {
    if(events.type == SDL_QUIT)
      running = false;
  }
  SDL_GetMouseState(&mouse_x, &mouse_y);
}


void render() { 
  clear_screen();
  //draw_line(-40, -40, 40, 40);
  //draw_triangle(15,0, 0,15, -15,0);
  //draw_square(-40,-40, 40,-40, 40,40, -40,40);
  draw_square_ray_opaque(WWIDTH/2,-40+WHEIGHT/2, 40+WWIDTH/2,WHEIGHT/2, 
                         WWIDTH/2,40+WHEIGHT/2, -40+WWIDTH/2,WHEIGHT/2);
  draw_all_rays(&mouse_x, &mouse_y);
  SDL_GL_SwapBuffers();
}


void loop() {
  running = true;
  while(running) { 
    manage_inputs();
    render();
  }
}


int main(int argc, char **argv) {
  init_sdl();
  loop();
}
