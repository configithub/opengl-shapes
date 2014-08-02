#include "render_tools.h"

SDL_Event events;
bool running;


void manage_inputs() {
  while(SDL_PollEvent(&events)) {
    if(events.type == SDL_QUIT)
      running = false;
  }
}


void render() { 
  clear_screen();
  //draw_triangle(15,0, 0,15, -15,0);
  //draw_square(-40,-40, 40,-40, 40,40, -40,40);
  draw_square(0,-40, 40,0, 0,40, -40,0);
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
