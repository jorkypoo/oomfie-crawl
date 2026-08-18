#ifndef __USER_INTERFACE__
#define __USER_INTERFACE__

#include <SDL3/SDL.h>

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>

#include "window.h"
#include "filesystem.h"


#define MENU_ELEMENT_COUNT (4)

// all ui elements inherit from element
typedef struct Element Element;
struct Element {
  SDL_FRect rect;

  int hovered;
  int clicked;

  void (*handle_event)(mouse_position* mpos, Element* e, SDL_Event* ev);
  void (*draw)(Application* app, Element* e);
  void (*destroy)(Element* e);
};


typedef struct Button Button;
struct Button {
  Element base;

  SDL_Texture* tex_default;
  SDL_Texture* tex_hovered;
  SDL_Texture* tex_clicked;
  
  void (*callback)(void* userdata); 
  void* userdata;
};


typedef struct Textbox Textbox;
struct Textbox {
  Element base;

  SDL_Texture* bg; // background texture
  SDL_Color color; // if bg NULL, render background as this color (if set)

  char* text;
  size_t text_len;
};

// menus are made up of elements
typedef struct Menu Menu;
struct Menu {
  Element** elements;
  size_t count;
  size_t capacity;
};

// and screens are made up of a menu and a game screen
typedef struct Screen Screen;
struct Screen {
  //Game* current_game;
  Menu* current_menu;

  int cur_menu;
  int prv_menu;
  int cur_game;
  int prv_game;
};


/* ===== element functions ===== */

// basically the menu calls these functions, which calls the elements given
// function pointer to the relevant function - button calls draw_button and so on
void handle_element_event(mouse_position* mpos, Element* e, SDL_Event* ev);
void draw_element(Application* app, Element* e);
void destroy_element(Element* e);

// called within handle handle_element_event to detect hovering
// as not everything requires to be clicked, that is handled per child
bool on_element(Element* e, float mx, float my);

/* ===== button functions ===== */

// basic initialising stuff
Button* init_button(float x, float y, float w, float h, void(*callback)(void* data), void* data);
int init_button_textures(SDL_Renderer* r, Button* b, char* def_path, char* hov_path, char* clk_path);

// these are to be called by the user, but they can also use the  ones above
Button* init_button_offset(SDL_Renderer* r, char* path, int offset, void (*callback)(void* callback_data), void* userdata);
Button* init_button_match(SDL_Renderer* r, char* path, char* match, void (*callback)(void* callback_data), void* userdata);

// these must take an element instead of button, which is then cast into a button
void handle_button_event(mouse_position* mpos, Element* e, SDL_Event* event);
void render_button(Application* app, Element* e);
void free_button(Element* e);

// internal use
void render_simple_button(SDL_Renderer* renderer, Button* target, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* ===== textbox functions ===== */

// create a textbox with dimensions, text to draw and a background texture
Textbox* init_textbox(SDL_Renderer* r, float x, float y, float w, float h, char* text, char* texture_path);
void add_textbox_bg_color(Textbox* t, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// textbox function that automatically determine the width...
// fuck around with these functions more
// these functions wouldn't be given a width or height, that's what's calculated

void handle_textbox_event(mouse_position* mpos, Element* e, SDL_Event* event);
void render_textbox(Application* app, Element* e);
void free_textbox(Element* e);


/* ===== menu functions ===== */ 

Menu* init_menu();
int add_element_to_menu(Menu* menu, Element* e);

void handle_menu_event(mouse_position* mpos, Menu* menu, SDL_Event* e);
void render_menu(Application* app, Menu* menu);
void free_menu(Menu* src); 

/* ===== screen functions ===== */

Screen* init_screen(Menu* m);
int update_screen_current_menu(Screen* s, Menu* m);
// int update_screen_current_game(Screen* s. Game* g);

void handle_screen_input(mouse_position* mpos, Screen* s, SDL_Event* e);
void render_screen(Application* app, Screen* s);
void free_screen(Screen* s);

// internal use
int should_change_screen_menu(Screen* s);
int should_change_screen_game(Screen* s);

#endif
