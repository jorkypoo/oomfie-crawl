#include "../inc/window.h"
#include "../inc/ui.h"
#include "../inc/menu.h"

// basic sdl frame loop functions
void input(game* game);
void update(game* game);
void render(game* game);


Menu* menu = NULL;
Screen* screen = NULL;


int main(int argc, char* argv[]) {

  game game = { 0 }; 
    if (!init_game(&game)) { 
    return 1;
  }

  menu = get_menu(game.renderer, 0);
  screen = init_screen(menu);

  while (game.running) {
    input(&game);

    init_rendering(&game); // this function is engine code and must be called before user defined rendering
    
    render(&game);

    upscale_game(&game); // engine code to be called after rendering code

    // fps shit
    SDL_Delay(15/1000);
  }
  free_screen(screen);
  quit_game(&game);
  return 0;
}


void input(game *game) {
  SDL_Event e;
  
  char* current_res = get_current_game_resolution(game);
  get_scaled_mouse_coords(game);

  Menu* tmp = NULL;
  while (SDL_PollEvent(&e)) {
    
    switch (e.type) {
      case SDL_EVENT_QUIT:
        game->running = 0;
        break;

      case SDL_EVENT_WINDOW_RESIZED:
        game->width = e.window.data1;
        game->height = e.window.data2;        
        break;

      case SDL_EVENT_KEY_DOWN:
        switch (e.key.key) {
          case SDLK_ESCAPE:
            game->running = 0;
            break;
          case SDLK_F1:
            if (game->borderless) break;
            current_res = scroll_game_resolutions(game);
            //SDL_Log("%s", current_res);
            break;
          case SDLK_F2:
            toggle_borderless_mode(game);
            break;
          case SDLK_F3:
            // toggle_fullscreen_mode(game); // fullscreen not worky wah
            break;
          case SDLK_F8:
            tmp = get_menu(game->renderer, 0);
            if (!tmp)
              break;
            
            if (!update_screen_current_menu(screen, tmp))
              SDL_Log("error switching menus");

            break;
          case SDLK_F10:
            tmp = get_menu(game->renderer, 1);
            if (!tmp)
              break;
            
            if (!update_screen_current_menu(screen, tmp))
              SDL_Log("error switching menus");

            break;
        }
    }
  
  // handle input RE current menu
  handle_screen_input(&game->mouse_pos, screen, &e);
  }
}


void render(game* game) {
  // rendering a cheeky sample map
  for (int y = 0; y < BASE_HEIGHT/16; y++) {
    for (int x = 0; x < BASE_HEIGHT/16; x++) {
      if (x % 2 == 0) SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
      else            SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
    
    SDL_FRect rect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderFillRect(game->renderer, &rect);
    }
  }
  
  // render the current menu
  render_screen(game->renderer, screen);
}
