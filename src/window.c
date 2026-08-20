#include "../inc/window.h"


resolution resolutions[] = {
  {640, 480, "640x480"},
  {960, 720, "960x720"},
  {1280, 960, "1280x960"},
};


void get_scaled_mouse_coords(Application* game) {
  float scale;
  if (game->width / BASE_WIDTH < game->height / BASE_HEIGHT) 
    scale = game->width / BASE_WIDTH;
  else 
    scale = game->height / BASE_HEIGHT;
  
  float offsetx = (game->width - (BASE_WIDTH * scale)) / 2;
  float offsety = (game->height - (BASE_HEIGHT * scale)) / 2;

  float mx, my;
  SDL_GetMouseState(&mx, &my);
  game->mouse_pos.x = (mx - offsetx) / scale;
  game->mouse_pos.y = (my - offsety) / scale;
}


void get_game_scale(Application* game, float* sx, float* sy) {
  *sx = BASE_WIDTH / game->width;
  *sy = BASE_HEIGHT / game->height;
}


void set_game_resolution(Application* game, int w, int h) {
  game->width = w;
  game->height = h;

  SDL_SetWindowSize(game->window, w, h);
}


char* get_current_game_resolution(Application* game) {
  return resolutions[game->resolution].name;
}


char* scroll_game_resolutions(Application* game) {
  game->resolution++;
  if (game->resolution >= SDL_arraysize(resolutions)) { game->resolution = 0; } 
  
  set_game_resolution(game, resolutions[game->resolution].width, resolutions[game->resolution].height);
  // SDL_Log("Resolution: %s", resolutions[game->resolution].name);
  return resolutions[game->resolution].name;
}


void toggle_borderless_mode(Application* game) {
  SDL_SetWindowFullscreen(game->window, !game->borderless);
  game->borderless = !game->borderless;
}


void toggle_fullscreen_mode(Application* game) { // this shit does not work
  // grab users display settings
  SDL_DisplayID disp = SDL_GetDisplayForWindow(game->window);
  const SDL_DisplayMode *cur = SDL_GetCurrentDisplayMode(disp);

  // grab all available display modes possible
  int count;
  SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(disp, &count);

  // see if an available display mode matches users display
  SDL_DisplayMode* chosen = NULL;
  for (int i = 0; i < count; i++) {
    if (modes[i]->w == cur->w && modes[i]->h == cur->h) {
      chosen = modes[i];
      break;
    }
  } 

  if (!chosen) { 
    SDL_Log("error: could not set game to fullscreen mode");
    return;
  }
  // set to fullscreen mode (idk how this wroks in terms of scaling rn)
  if (!SDL_SetWindowFullscreenMode(game->window, chosen)) SDL_Log("failed setting game to fullscreen");
}


int init_game(Application* game) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return 0;
  }

  if (!TTF_Init()) {
    SDL_Log("TTF_Init failed: %s", SDL_GetError());
    return 0;
  }

  game->width = BASE_WIDTH;
  game->height = BASE_HEIGHT;
  game->resolution = 0;

  game->mouse_pos.x = 0;
  game->mouse_pos.y = 0;

  game->borderless = 0;
  game->fullscreen = 0;

  game->font = NULL;

  game->window = SDL_CreateWindow("roguelike test", game->width, game->height, 0);
  if (!game->window) { 
    SDL_Log("%s", SDL_GetError()); 
    return 0; 
  }
  
  game->renderer = SDL_CreateRenderer(game->window, NULL);
  if (!game->renderer) { 
    SDL_Log("%s", SDL_GetError()); 
    return 0; 
  }

  game->game_texture = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, BASE_WIDTH, BASE_HEIGHT);
  if (!game->game_texture) {
    SDL_Log("%s", SDL_GetError());
    return 0;
  }

  SDL_SetTextureScaleMode(game->game_texture, SDL_SCALEMODE_NEAREST);
  
  game->running = 1;
  return 1;
}


void quit_game(Application* game) {
  if (game->font) TTF_CloseFont(game->font);

  SDL_DestroyTexture(game->game_texture);
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);

  TTF_Quit();
  SDL_Quit();
}


int init_global_font(Application* app, char* path, float px, Uint8 r, Uint8 g, Uint8 b) {
  if (!app) return 0;

  app->font_size = px;
  app->font_color.r = r;
  app->font_color.g = g;
  app->font_color.b = b;
  app->font_color.a = 255;

  // defaults for alt font color
  app->alt_font_color.r = 255;
  app->alt_font_color.g = 165;
  app->alt_font_color.b = 0;
  app->alt_font_color.a = 255;

  app->font = TTF_OpenFont(path, px);
  if (!app->font) {
    app->font = NULL;
    return 0;
  }

  return 1;
}


void specify_alt_font_color(Application* app, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  app->alt_font_color.r = r;
  app->alt_font_color.g = g;
  app->alt_font_color.b = b;
  app->alt_font_color.a = a;
}


void init_rendering(Application* game) {
  SDL_SetRenderTarget(game->renderer, game->game_texture);
  SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
  SDL_RenderClear(game->renderer);
}


void upscale_game(Application* game) {
  SDL_SetRenderTarget(game->renderer, NULL);
  
  float sx = game->width / BASE_WIDTH;
  float sy = game->height / BASE_HEIGHT;

  float scale = sx < sy ? sx : sy;

  int draww = (int)(BASE_WIDTH * scale);
  int drawh = (int)(BASE_HEIGHT * scale);

  SDL_FRect dst = { (game->width - draww) / 2.0, (game->height - drawh) / 2.0f, draww, drawh };

  SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
  SDL_RenderClear(game->renderer);
  
  SDL_RenderTexture(game->renderer, game->game_texture, NULL, &dst);

  SDL_RenderPresent(game->renderer);
}
