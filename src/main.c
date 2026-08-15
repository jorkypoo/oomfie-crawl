#include "../inc/window.h"
#include "../inc/ui.h"
#include "../inc/menu.h"
#include "../inc/filesystem.h"

// basic sdl frame loop functions
void input(Application* game);
void update(Application* game);
void render(Application* game);

// menu is later handed off to the screen
Menu* menu = NULL;
Screen* screen = NULL;
Application app = { 0 };


int main(int argc, char* argv[]) {

  /*

  // testing out some stuff for filesystem management 
  //list_dir_alpha_sorted(".", SORT_DESCENDING);

  // testing out creating buttons from files
  char* tmp = get_line_offset("btns.csv", 1);
  //if (tmp) printf("%s\n", tmp);
  if (tmp) free(tmp); // must free results from these functions

  char* tmp2 = get_line_match("btns.csv", "poop");
  //if (tmp2) printf("%s\n", tmp2);

  char* tmp3 = get_delimited_value(tmp2, ',', 5);
  //if (tmp3) printf("%s\n", tmp3);
  if (tmp3) free(tmp3);
  if (tmp2) free(tmp2); // again, returned by get_line_match and must free

  */

  if (!init_game(&app)) { 
    return 1;
  }

  menu = get_menu(app.renderer, 0);
  screen = init_screen(menu);

  while (app.running) {
    input(&app);

    init_rendering(&app); // this function is engine code and must be called before user defined rendering
    
    render(&app);

    upscale_game(&app); // engine code to be called after rendering code

    // fps shit
    SDL_Delay(15/1000);
  }
  free_screen(screen);
  quit_game(&app);
  return 0;
}


void input(Application *app) {
  SDL_Event e;

  char* current_res = get_current_game_resolution(app);
  get_scaled_mouse_coords(app);

  // handle possible screen changes
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
            current_res = scroll_game_resolutions(app);
            //SDL_Log("%s", current_res);
            break;
          case SDLK_F2:
            toggle_borderless_mode(app);
            break;
          case SDLK_F3:
            // toggle_fullscreen_mode(game); // fullscreen not worky wah
            break;
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
        }
    }
  
  // handle input RE current menu
  handle_screen_input(&app->mouse_pos, screen, &e);
  }
}


void render(Application* app) {
  // rendering a cheeky sample map
  for (int y = 0; y < BASE_HEIGHT/16; y++) {
    for (int x = 0; x < BASE_HEIGHT/16; x++) {
      if (x % 2 == 0) SDL_SetRenderDrawColor(app->renderer, 0, 255, 0, 255);
      else            SDL_SetRenderDrawColor(app->renderer, 0, 0, 255, 255);
    
    SDL_FRect rect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderFillRect(app->renderer, &rect);
    }
  }
  
  // render the current menu
  render_screen(app->renderer, screen);
}
