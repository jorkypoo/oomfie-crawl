#ifndef __WINDOW__
#define __WINDOW__

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>


#define BASE_WIDTH  640
#define BASE_HEIGHT 480
#define BLOCK_SIZE  16

#define NURUPO ((void*)0)


typedef struct resolution resolution;
struct resolution {
  int  width;
  int  height;
  char *name;
};

// must modify this in window.c soz i might be retarded
// defines all the available resolutions the game can change to
extern resolution resolutions[];

// N.B. App struct was previously called Game lolsies


typedef struct mouse_position mouse_position;
struct mouse_position {
  float x;
  float y;
};

typedef struct Application Application;
struct Application {
  SDL_Window*   window;
  SDL_Renderer* renderer;
  SDL_Texture*  game_texture;

  // window dimensions - set to and kept at BASE_WIDTH & HEIGHT in init()
  // the game is upscaled later so game logic should be written in terms of those macros 
  float width;
  float height;

  // relative mouse coordinates 
  mouse_position mouse_pos;
  
  int resolution; // kind of a tracker for running through the resolutions array
  int borderless; // whether the game is running in borderless or windowed
  int fullscreen; // whether the game is running in fullscreen or not
  int running;  // for the SDL main loop
};


// grab the mouse coordinates and scale them for use in-game
void get_scaled_mouse_coords(Application* game);

// update the scale factor for every button... idk its for ui
void get_game_scale(Application* game, float* sx, float* sy);

// used indirectly inside scroll_game_resolutions()
void set_game_resolution(Application* game, int w, int h);

// returns games current resolution for displaying ig; something like "600x400"
char* get_current_game_resolution(Application* game);

// used in a menu or whatever; scrolls through available screen resolutions
// also returns games current resolution
char* scroll_game_resolutions(Application* game);

// toggles between windowed & fullscreen borderless mode
void toggle_borderless_mode(Application* game);

// currently not working, but should force display to be whatever the selected game resolution is
void toggle_fullscreen_mode(Application* game);

// initialises SDL and everything and returns a game object
int init_game(Application* game); 

// cleanup
void quit_game(Application* game);

// wrapper to set the rendering target to the game texture
void init_rendering(Application* game); // MUST BE CALLED BEFORE CUSTOM RENDERING CODE

// scales game window up based on currently selected resolution
// helps maintain game & ui logic at the base window dimensions
void upscale_game(Application* game); // MUST BE CALLED AFTER CUSTOM RENDERING CODE

#endif
