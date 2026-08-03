#include "../inc/window.h"
#include "../inc/ui.h"
#include "../inc/menu.h"

// basic sdl frame loop functions
void input(game* game);
void update(game* game);
void render(game* game);


menu current;
screen cur;


int main(int argc, char* argv[]) {

  game game = { 0 }; 
    if (!init_game(&game)) { 
    return 1;
  }

  current = get_menu(game.renderer, SCREEN_MAIN_MENU);

  /*SDL_Log("%f,%f\n%f,%f", current.buttons[0]->rect.x, current.buttons[0]->rect.y,\
                          current.buttons[1]->rect.x, current.buttons[1]->rect.y); */

  while (game.running) {
    input(&game);

    init_rendering(&game);
    render(&game);

    //SDL_Log("%f, %f", current.buttons[0]->rect.x, current.buttons[1]->rect.x);
    upscale_game(&game);

    // fps shit
    SDL_Delay(15/1000);
  }
  free_menu(&current);
  quit_game(&game);
  return 0;
}


void input(game *game) {
  SDL_Event e;
  
  char* current_res = get_current_game_resolution(game);
  get_scaled_mouse_coords(game);

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
            free_menu(&current);
            current = get_menu(game->renderer, SCREEN_MAIN_MENU);
            break;
          case SDLK_F10:
            free_menu(&current);
            current = get_menu(game->renderer, SCREEN_OPTIONS);
            break;
      }
    }
  
  // handle input RE current menu
  handle_menu_event(&game->mouse_pos, &current, &e);
  //SDL_Log("%f,%f", current.buttons[0]->rect.x, current.buttons[1]->rect.x);
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
  render_menu(game->renderer, &current);
}
