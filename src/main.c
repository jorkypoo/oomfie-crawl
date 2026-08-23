#include "../inc/window.h"
#include "../inc/ui.h"
#include "../inc/filesystem.h"

#include "../inc/menu.h"

// basic sdl frame loop functions
void input(Application* game);
void update(Application* game);
void render(Application* game);

// menu is later handed off to the screen
Menu* menu = NULL;
Screen* screen = NULL;
Application app = { 0 };

// ME FUCKING AROUND
SDL_Texture* hanyuu = NULL;


int main(int argc, char* argv[]) {
  if (!init_game(&app)) 
    return 1;

  if (!init_global_font(&app, "assets/monogram.ttf", 32, 255, 255, 255))
    SDL_Log("font not initialised: %s", SDL_GetError());
  specify_alt_font_color(&app, 255, 128, 0, 255);
  
  menu = get_menu(app.renderer, 0);
  screen = init_screen(menu);

  // ME FUCKING AROUND
  hanyuu = create_tex(app.renderer, "assets/hanyuu_bg.png");

  /* ===== main game loop ===== */
  while (app.running) {
    input(&app);
    init_rendering(&app); // this function is engine code and must be called before user defined rendering
    render(&app);
    upscale_game(&app); // engine code to be called after rendering code
    SDL_Delay(30/1000); // encapsulate this into one function that the user doesn't have to care about
    // more engine code here required to update shit at the end of each frame - call deferred
  }

  // cleanup
  free_screen(screen);
  quit_game(&app);
  return 0;
}


void input(Application *app) {
  SDL_Event e;

  //char* current_res = get_current_game_resolution(app);
  get_scaled_mouse_coords(app);

  // handle possible screen changes - my current call_deferred
  handle_screen_updates(screen);

  Menu* tmp = NULL;
  while (SDL_PollEvent(&e)) {
    
    switch (e.type) {
      case SDL_EVENT_QUIT:
        app->running = 0;
        break;

      case SDL_EVENT_WINDOW_RESIZED:
        app->width = e.window.data1;
        app->height = e.window.data2;        
        break;

      case SDL_EVENT_KEY_DOWN:
        switch (e.key.key) {
          case SDLK_ESCAPE:
            app->running = 0;
            break;
          case SDLK_F1:
            if (app->borderless) break;
            //current_res = scroll_game_resolutions(app);
            scroll_game_resolutions(app);
            break;
          case SDLK_F2:
            toggle_borderless_mode(app);
            break;
          case SDLK_F3:
            // toggle_fullscreen_mode(game); // fullscreen not worky wah
            break;
          /*
            case SDLK_F8:
            tmp = get_menu(app->renderer, 0);
            if (!tmp)
              break;
            
            if (!update_screen_current_menu(screen, tmp))
              SDL_Log("error switching menus");

            break;
          case SDLK_F10:
            tmp = get_menu(app->renderer, 1);
            if (!tmp)
              break;
            
            if (!update_screen_current_menu(screen, tmp))
              SDL_Log("error switching menus");

            break;
          */
        }
    }
  
  // handle input of current menu
  handle_screen_input(app, screen, &e);
  // handle_screen_input(app, screen, &e); // looks nicer, doesn't it?
  }
}


void render(Application* app) {
  /*
  // rendering a cheeky sample map
  for (int y = 0; y < BASE_HEIGHT/16; y++) {
    for (int x = 0; x < BASE_HEIGHT/16; x++) {
      if (x % 2 == 0) SDL_SetRenderDrawColor(app->renderer, 0, 255, 0, 255);
      else            SDL_SetRenderDrawColor(app->renderer, 0, 0, 255, 255);
    
    SDL_FRect rect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderFillRect(app->renderer, &rect);
    }
  }
  */

  // render hanyuu bc i wanna see something cute when i'm working on this project
  // ME FUCKING AROUND
  SDL_FRect rect = {0, 0, 480, 480};
  SDL_RenderTexture(app->renderer, hanyuu, NULL, &rect);
  
  // render the current menu
  render_screen(app, screen);
}
