#ifndef __WINDOW__
#define __WINDOW__

#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>


#define BASE_WIDTH  640
#define BASE_HEIGHT 480
#define MENU_WIDTH  160
#define MENU_HEIGHT 480
#define MENU_X      480
#define MENU_Y      0
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
  SDL_Texture*  game_texture; // acts as the game's rendering target - keeps logic at a single resolution
  TTF_Font*     font;         // global font

  SDL_Color font_color;
  SDL_Color alt_font_color; // for selected text and such, so it is not built into the engine code
  float font_size;

  // output tracks for sfx and bg music
  // most ui sounds can use MIX_PlayAudio wrappers for the most part
  MIX_Mixer* mixer;
  MIX_Track* music_track;
  Sint64 music_track_pos;
  MIX_Track* misc1_track;
  MIX_Track* misc2_track;

  // window dimensions - set to and kept at BASE_WIDTH & HEIGHT in init()
  // the game is upscaled later so game logic should be written in terms of those macros 
  float width;
  float height;

  // relative mouse coordinates 
  mouse_position mouse_pos;
  
  int resolution; // kind of a tracker for running through the resolutions array
  int borderless; // whether the game is running in borderless or windowed
  int fullscreen; // whether the game is running in fullscreen or not
  int running;    // for the SDL main loop
};


// grab the mouse coordinates and scale them for use in-game
void get_scaled_mouse_coords(Application* app);

// update the scale factor for every button... idk its for ui
void get_game_scale(Application* app, float* sx, float* sy);

// used indirectly inside scroll_game_resolutions()
void set_game_resolution(Application* app, int w, int h);

// returns games current resolution for displaying ig; something like "600x400"
char* get_current_game_resolution(Application* app);

// used in a menu or whatever; scrolls through available screen resolutions
// also returns games current resolution
char* scroll_game_resolutions(Application* app);

// toggles between windowed & fullscreen borderless mode
void toggle_borderless_mode(Application* app);

// currently not working, but should force display to be whatever the selected game resolution is
void toggle_fullscreen_mode(Application* app);

// wrapper to set the rendering target to the game texture
void init_rendering(Application* app); // MUST BE CALLED BEFORE CUSTOM RENDERING CODE

// scales game window up based on currently selected resolution
// helps maintain game & ui logic at the base window dimensions
void upscale_game(Application* app); // MUST BE CALLED AFTER CUSTOM RENDERING CODE

// initialises SDL and everything and returns a game object
int init_game(Application* app); 

// cleanup
void quit_game(Application* app);

/* ===== font stuff ===== */

// add a .ttf file & initialise it as the game's global font
int init_global_font(Application* app, char* path, float px, Uint8 r, Uint8 g, Uint8 b);

// specifies an alternative font color - optional, as the default is oragne
void specify_alt_font_color(Application* app, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* ===== mixer stuff ===== */

int init_app_mixers(Application* app);
void free_app_mixers(Application* app);

// stops and clears all sound playing on mixers - for changing levels and stuff
void clear_app_mixers(Application* app);

// fires off a temporary audio - for short sfx
void play_temp_audio(Application* app, char* path);

// plays and stores an audio track - for larger sfx files that are used
// often & would need to be cached
// not implemented yet & honestly not really necessary
void play_main_audio(Application* app, char* path);

// plays a looping track
void play_main_music(Application* app, char* path);

// pause/resume music track with fade in/out
// remembers position in track on pause so resume picks back from last pos
void pause_main_music(Application* app, Sint64 ms);
void resume_main_music(Application* app, Sint64 ms);

#endif
