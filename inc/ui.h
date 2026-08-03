#ifndef __USER_INTERFACE__
#define __USER_INTERFACE__

#include <SDL3/SDL.h>

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>

#include "window.h"


#define MAX_MENU_BUTTONS 24


typedef struct button button;
struct button {
  SDL_FRect rect;

  // booleans for states of teh button
  int hovered;
  int clicked;
  int selected;

  // textures for the button states
  SDL_Texture* tex_default;
  SDL_Texture* tex_hovered;
  SDL_Texture* tex_clicked;

  // and the text to be drawn on button
  char* text;
  size_t text_len;
  
  // callback function... duh
  void (*callback)(void* userdata); 
  
  // data sent to the callback function for whatever use
  // assigned individually for each function
  void* userdata;
  
  // function pointer for button handling
  void (*handle_event)(mouse_position* mpos, button* b, SDL_Event* event);
  
  // function pointer for button rendering
  void (*render)(SDL_Renderer* r, button* b);
}; // end of button struct definition


// a menu is made of buttons
typedef struct menu menu;
struct menu {
  button* buttons[MAX_MENU_BUTTONS];
  size_t count;
  size_t current_selection;
};

typedef struct screen screen;
struct screen {
  menu current_menu;
};

/* on screens:
 * tab should change your focus between game to menu
 * screen should keep track of whether to track menu
 * or game inputs and stuff... idk
 */


/* helper functions */

// create texture from filepath; return NULL on failure
SDL_Texture* create_texture(SDL_Renderer* r, char* path);

// handles a single buttons event with regards to the mouse
int on_button(button* b, float mx, float my);

// helper functions to update button global and relative position, respectively
void update_button_pos(button* b, float x, float y);
void update_button_pos_relative(button* b, float x, float y);


/* screen functions */

void init_screen(screen* s);

void update_screen_current_menu(screen* s, menu* m);

void handle_screen_input(mouse_position* mpos, screen* s, SDL_Event* e);

void render_screen(SDL_Renderer* r, screen* s);

void destroy_screen(screen* s);


/* menu functions */

// create a menu to store buttons
void init_menu(menu* menu);

// frees all buttons present in a menu
void free_menu(menu* src); 

// append a button to the given menu
void add_button_to_menu(menu* menu, button* b);

// basically a wrapper got handle_button_event() that loops through all buttons
// in given menu 
void handle_menu_event(mouse_position* mpos, menu* menu, SDL_Event* e);

// wrapper for render_button() but loops through all buttons in a given menu
void render_menu(SDL_Renderer* r, menu* menu);


/* button functions */

// creates a basic button with callback based on given parameters
// add a callback function with data it will access
// just look at the examples, sorry (if i can get this working)
void init_button(button* target, float x, float y, float w, float h,\
                 void (*callback)(void* callback_data), void* userdata);

// adds textures to an existing button already initialised with init_button()
// def(ault)_path must not be NULL; other paths that are NULL reuse def(ault)_path
void init_button_textures(SDL_Renderer* r, button* target,\
                          char* def_path, char* hov_path, char* clk_path);

// add text to the button; NULL is ok
void init_button_text(SDL_Renderer* r, button* target, char* text);

// called in input loop and activates buttons callback function if button clicked
void handle_button_event(mouse_position* mpos, button* b, SDL_Event* event);

// call to render a button
void render_button(SDL_Renderer* r, button* b);

// render this simple button with a provided RGBA value
void render_simple_button(SDL_Renderer* renderer, button* target, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


#endif
