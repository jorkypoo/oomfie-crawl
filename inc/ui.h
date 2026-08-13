#ifndef __USER_INTERFACE__
#define __USER_INTERFACE__

#include <SDL3/SDL.h>

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>

#include "window.h"

/* this file represents engine code that the user should interface with - not modify */


#define MAX_MENU_BUTTONS 24


// kinda changing this halfway through so buttons and text boxes inherit from element
// and menus are collections of elements
// will be implemented soon
typedef struct Element Element;
struct Element {
  SDL_FRect rect;
};


typedef struct Button Button;
struct Button {
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
}; 


// a menu is made of buttons
typedef struct Menu Menu;
struct Menu {
  Button* buttons[MAX_MENU_BUTTONS];
  size_t count;
  size_t current_selection;
};


// screen is the object that the user operates with
// but they will need to make and work with their own menus
// however, once passed to the screen, the menu will be taken care of
typedef struct Screen Screen;
struct Screen {
  //Game* current_game;
  Menu* current_menu;

  // kinda internal use
  // cur_menu & cur_game can be set by button callbacks
  // if the cur & prv don't match, update the menu
  // kinda to be used by the user too
  int cur_menu;
  int prv_menu;
  int cur_game;
  int prv_game;
};


/* ===== helper functions ===== */

// create texture from filepath; return NULL on failure
SDL_Texture* create_texture(SDL_Renderer* r, char* path);

// returns 1 if mouse cursor is on a button; else 0
int on_button(Button* b, float mx, float my);

// helper functions to update button global and relative position, respectively
void update_button_pos(Button* b, float x, float y);
void update_button_pos_relative(Button* b, float x, float y);


/* ===== screen functions ===== */

Screen* init_screen(Menu* m);

void free_screen(Screen* s);

int update_screen_current_menu(Screen* s, Menu* m);

void handle_screen_input(mouse_position* mpos, Screen* s, SDL_Event* e);

void render_screen(SDL_Renderer* r, Screen* s);


// kinda lazy but needed for button callbacks being able to change the screen
int should_change_screen_menu(Screen* s);
int should_change_screen_game(Screen* s);


/* ===== menu functions ===== */

// create a menu to store buttons
Menu* init_menu();

// frees all buttons present in a menu
void free_menu(Menu* src); 

// append a button to the given menu
int add_button_to_menu(Menu* menu, Button* b);

// basically a wrapper got handle_button_event() that loops through all buttons
// in given menu 
void handle_menu_event(mouse_position* mpos, Menu* menu, SDL_Event* e);

// wrapper for render_button() but loops through all buttons in a given menu
void render_menu(SDL_Renderer* r, Menu* menu);


/* ===== button functions ===== */

// PLSSSSSSSSSSSSSSsssssSSSSSSSSssSSSSSSSSSS MAKE THESE RETURN A BOOOOOOOOOOOooooOOOOooOooOOoOOOOOLLLlll

// creates a basic button with callback based on given parameters
// add a callback function with data it will access
// just look at the examples, sorry (if i can get this working)
Button* init_button(float x, float y, float w, float h,\
                 void (*callback)(void* callback_data), void* userdata);

// adds textures to an existing button already initialised with init_button()
// def(ault)_path must not be NULL; other paths that are NULL reuse def(ault)_path
int init_button_textures(SDL_Renderer* r, Button* target,\
                          char* def_path, char* hov_path, char* clk_path);

// add text to the button; NULL is ok
int init_button_text(SDL_Renderer* r, Button* target, char* text);

void free_button(Button* b);

// called in input loop and activates buttons callback function if button clicked
void handle_button_event(mouse_position* mpos, Button* b, SDL_Event* event);

// render a button; if default texture is NULL, button will be rendered noticably red...
void render_button(SDL_Renderer* r, Button* b);

void render_simple_button(SDL_Renderer* renderer, Button* target, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


#endif
