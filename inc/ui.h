#ifndef __USER_INTERFACE__
#define __USER_INTERFACE__

#include <SDL3/SDL.h>

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>

#include "window.h"
#include "filesystem.h"


typedef struct Element Element;
typedef struct Button Button;
typedef struct Label Label;
typedef struct Texture Texture;

typedef struct Menu Menu;
typedef struct Screen Screen;

/* the following declarations are global/utility stuff */

// am i using this rn i forgor
#define MENU_ELEMENT_COUNT (12)

SDL_Texture* create_tex(SDL_Renderer* renderer, char* path);


/* =================================================
 * element struct acts as a base for all ui elements
 * and their source is located in element_base.c
 */

/* ====================
 * ===== elements =====
 * ==================== */

// all ui elements inherit from element
struct Element {
  SDL_FRect rect;

  // each element can be looked up with this id
  char id[8];

  int hovered;
  int clicked;

  void (*handle_event)(Application* app, Element* e, SDL_Event* ev);
  void (*draw)(Application* app, Element* e);
  void (*destroy)(Element* e);
};

// basically the menu calls these functions, which calls the elements given
// function pointer to the relevant function - button calls draw_button and so on
void handle_element_event(Application* app, Element* e, SDL_Event* ev);
void draw_element(Application* app, Element* e);
void destroy_element(Element* e);

void add_element_id(Element* e, char* id);

// called within handle handle_element_event to detect hovering
// as not everything requires to be clicked, that is handled per child
bool on_element(Element* e, float mx, float my);


/* ==========================================
 * the following structs inherit from element
 * and their source is located in element_inherit.c 
 */

/* ===================
 * ===== buttons =====
 * =================== */

// a button with definable callbacks that can operate on user defined data
struct Button {
  Element base;

  SDL_Texture* tex_default;
  SDL_Texture* tex_hovered;
  SDL_Texture* tex_clicked;

  char* text;
  size_t text_len;
  
  void (*callback)(void* userdata); 
  void* userdata;
};

Button* init_button(float x, float y, float w, float h, void(*callback)(void* data), void* data);
int init_button_text(Button* b, char* text);
int init_button_textures(SDL_Renderer* r, Button* b, char* def_path, char* hov_path, char* clk_path);

// these are to be called by the user, but they can also use the  ones above
Button* init_button_offset(SDL_Renderer* r, char* path, int offset, void (*callback)(void* callback_data), void* userdata);
Button* init_button_match(SDL_Renderer* r, char* path, char* match, void (*callback)(void* callback_data), void* userdata);

// these must take an element instead of button, which is then cast into a button
void handle_button_event(Application* app, Element* e, SDL_Event* event);
void render_button(Application* app, Element* e);
void free_button(Element* e);

void render_simple_button(SDL_Renderer* renderer, Button* target, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/* ===================
 * ===== labels ======
 * =================== */

// a label for drawing a *single* line of text with a background tex/color
// centres text within it's rect automatically
struct Label {
  Element base;

  SDL_Texture* bg; // background texture
  SDL_Color color; // if bg NULL, render background as this color (if set)

  char* text;
  size_t text_len;
};

// create a label with dimensions, text to draw and a background texture
Label* init_label(SDL_Renderer* r, float x, float y, float w, float h, char* text, char* texture_path);
void add_label_bg_color(Label* t, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// updates label text with id id with text in menu m
// maybe should be deferred
void update_label_text(Menu* m, char* id, char* text);

// create label from csv: x,y,w,h,text,texpath,r,g,b,a
// texpath,r,g,b,a can be 0 to be not set
Label* init_label_offset(SDL_Renderer* r, char* path, int offset);
Label* init_label_match(SDL_Renderer* r, char* path, char* offset);

void handle_label_event(Application* app, Element* e, SDL_Event* event);
void render_label(Application* app, Element* e);
void free_label(Element* e);


/* =====================
 * ===== textures ======
 * ===================== */

// probably the simplest element: a texture... that's it!
struct Texture {
  Element base;

  SDL_Texture* tex;
};

// initialise texture with the provided image's native w*h or a defined w*h
// specifying dimensions will not render the texture nicely, however
// please use only one per element
Texture* init_texture(SDL_Renderer* r, char* path, float x, float y);
Texture* init_texture_dimensions(SDL_Renderer* r, char* path, float x, float y, float w, float h);

Texture* init_texture_offset(SDL_Renderer* r, char* path, int offset);
Texture* init_texture_match(SDL_Renderer* r, char* path, char* match);

void handle_texture_event(Application* app, Element* e, SDL_Event* event);
void render_texture(Application* app, Element* e);
void destroy_texture(Element* e);


/* =============================================================================
 * the following structs are abstractions from elements that the user works with
 * their source is located in screen.c
 */

/* ==================
 * ===== menus ======
 * ================== */

// menus are made up of elements
// menu elements are drawn in the order you add them - first first, last last - so watch for overlaps!
struct Menu {
  Element** elements;
  size_t count;
  size_t capacity;
};

Menu* init_menu();
int add_element_to_menu(Menu* menu, Element* e);

void handle_menu_event(Application* app, Menu* menu, SDL_Event* e);
void render_menu(Application* app, Menu* menu);
void free_menu(Menu* src); 


/* ====================
 * ===== screens ======
 * ==================== */

// and screens are made up of a menu and a game screen
// this is primarily what the user will be operating with
struct Screen {
  //Game* current_game;
  Menu* current_menu;

  // bit ugly but these are helpers for changing menus/games at the end of each frame
  // these can be removed as long as my new call deferred system works
  int cur_menu;
  int prv_menu;
  int cur_game;
  int prv_game;
};

Screen* init_screen(Menu* m);
int update_screen_current_menu(Screen* s, Menu* m);
// int update_screen_current_game(Screen* s. Game* g);

void handle_screen_input(Application* app, Screen* s, SDL_Event* e);
void render_screen(Application* app, Screen* s);
void free_screen(Screen* s);

// internal use
int should_change_screen_menu(Screen* s);
int should_change_screen_game(Screen* s);


#endif
