#ifndef __USER_INTERFACE__
#define __USER_INTERFACE__

#include <SDL3/SDL.h>

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>

#include "window.h"
#include "filesystem.h"

/* this file represents engine code that the user should interface with - not modify */


#define MAX_MENU_BUTTONS 24


// kinda changing this halfway through so buttons and text boxes inherit from element
// and menus are collections of elements
// will be implemented soon
typedef struct Element Element;
struct Element {
  SDL_FRect rect;
  int hovered;
  int clicked;
  int selected;
};


typedef struct Button Button;
struct Button {
  // Element base;
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
  // not implemented yet
  char* text;
  size_t text_len;
  
  // callback function... duh
  void (*callback)(void* userdata); 
  
  // data used by the callback function for whatever 
  // assigned individually for each function
  void* userdata;
}; // ===== button struct


typedef struct Textbox Textbox;
struct Textbox {
  Element base;

  char* text;
  size_t text_len;
};


typedef struct Input_Field Input_Field;
struct Input_Field {
  Element base;
};


typedef struct Progress_Bar Progress_Bar;
struct Progress_Bar {
  Element base;
};


// a menu is made of buttons
// soon to be elements
typedef struct Menu Menu;
struct Menu {
  Button* buttons[MAX_MENU_BUTTONS];
  size_t count;
  size_t current_selection;
};


// screen is the object that the user operates with
// but they will need to make their own menus inside a file or wtv
// however, once passed to the screen, the menu will be taken care of
// remember to free that initial menu
typedef struct Screen Screen;
struct Screen {
  //Game* current_game;
  Menu* current_menu;

  // cur_menu and cur_game to be used by user; others for internal use
  // cur_menu & cur_game can be set by button callbacks; just see menu.c, soz
  int cur_menu;
  int prv_menu;
  int cur_game;
  int prv_game;
};


/* ===== helper functions ===== */

// create texture from filepath; return NULL on failure
SDL_Texture* create_texture(SDL_Renderer* r, char* path);


// these should update on Elements instead of buttons

// returns 1 if mouse cursor is on a button; else 0
int on_button(Button* b, float mx, float my);

// helper functions to update button global and relative position, respectively
void update_button_pos(Button* b, float x, float y);
void update_button_pos_relative(Button* b, float x, float y);


/* ===== screen functions ===== */

// init and return a menu
Screen* init_screen(Menu* m);

// free a screen and it's attached menu and game objects
void free_screen(Screen* s);

// updates a screen to use a new menu
int update_screen_current_menu(Screen* s, Menu* m);

// same as above
// int update_screen_current_game(Screen* s. Game* g);

// wrapper for handle_menu_input basically
void handle_screen_input(mouse_position* mpos, Screen* s, SDL_Event* e);

// wrapper for render_menu
void render_screen(SDL_Renderer* r, Screen* s);


// kinda lazy but needed for button callbacks being able to change the screen
int should_change_screen_menu(Screen* s);
int should_change_screen_game(Screen* s);


/* ===== menu functions ===== */

// create a menu to store buttons
Menu* init_menu();

// frees menu and all buttons present in it; leaves callers pointer dangling
// mostly used internally but u may have to free ur initial menu after assigning
// it to the Screen object 
void free_menu(Menu* src); 

// append a button to the given menu
// order in which buttons are arranged in list does not matter
int add_button_to_menu(Menu* menu, Button* b);

// basically a wrapper got handle_button_event() that loops through all buttons
// in given menu 
void handle_menu_event(mouse_position* mpos, Menu* menu, SDL_Event* e);

// wrapper for render_button() but loops through all buttons in a given menu
void render_menu(SDL_Renderer* r, Menu* menu);


/* ===== button functions ===== */

// creates a basic button based on given parameters
// add a callback function with user provided data can access
Button* init_button(float x, float y, float w, float h,\
                 void (*callback)(void* callback_data), void* userdata);

// adds textures to an existing button already initialised with init_button()
// def(ault)_path must not be NULL; other paths that are NULL will reuse def(ault)_path
int init_button_textures(SDL_Renderer* r, Button* target,\
                          char* def_path, char* hov_path, char* clk_path);

/* button files will be standardized; see btns in root for an example */

// initialises a button from a file; searches button file with an offset: 0 being first entry, etc
Button* init_button_offset(SDL_Renderer* r, char* path, int offset, void (*callback)(void* callback_data), void* userdata);

// same as above, but searches for a string match. function above ignores any strings at the beginning of lines
Button* init_button_match(SDL_Renderer* r, char* path, char* match, void (*callback)(void* callback_data), void* userdata);

// add text to the button; NULL is ok
int init_button_text(SDL_Renderer* r, Button* target, char* text);

// frees existing textures and text 
void free_button(Button* b);

// handles button's callback function if button was clicked
void handle_button_event(mouse_position* mpos, Button* b, SDL_Event* event);

// render a button with a texture, or a default colour, if no textures were added
void render_button(SDL_Renderer* r, Button* b);

// for internal use
void render_simple_button(SDL_Renderer* renderer, Button* target, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


#endif
