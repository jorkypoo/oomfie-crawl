#include "../inc/menu.h"


// filepaths for textures
#define BTN_PATH "assets/button_default.png"
#define BTN_PATH2 "assets/button_pressed.png"


void* callback1 = 0;
void test_btn_text(void* userdata) { 
  SDL_Log("hi from textured button"); 
}


Menu* get_main_menu(SDL_Renderer* renderer) {
  if (!renderer) return NULL;  

  Button* tb1 = init_button(480, 0, 80, 48, test_btn_text, callback1);
  if (!tb1)
    return NULL;

  if (!init_button_textures(renderer, tb1, BTN_PATH, BTN_PATH2, NULL))
    return NULL;

  Button* tb2 = init_button(560, 0, 80, 48, test_btn_text, callback1);
  if (!tb2)
    return NULL;
  
  if (!init_button_textures(renderer, tb2, BTN_PATH, BTN_PATH2, NULL))
    return NULL;

  Menu* dest = init_menu();
  if (!dest)
    return NULL;

  if (!add_button_to_menu(dest, tb1))
    return NULL;
  
  if (!add_button_to_menu(dest, tb2))
    return NULL;

  return dest;
}


void* callback2 = 0;
void test_btn_func(void* userdata) { 
  SDL_Log("hi from button"); 
}


Menu* get_options_menu(SDL_Renderer* renderer) {
  if (!renderer) return NULL;  

  Button* tb1 = init_button(MENU_L_BOUND, 0, MENU_WIDTH, 32, test_btn_func, callback2);
  if (!tb1) return NULL;

  Button* tb2 = init_button(MENU_L_BOUND, 32, MENU_WIDTH, 32, test_btn_func, callback2);
  if (!tb2) return NULL;
  
  Menu* dest = init_menu();
  if (!dest) return NULL;

  if (!add_button_to_menu(dest, tb1)) return NULL;
  
  if (!add_button_to_menu(dest, tb2)) return NULL;

  return dest;
}


Menu* get_menu(SDL_Renderer* r, screen_id choice) {
  Menu* ret = NULL;
  
  switch(choice) {
    case SCREEN_MAIN_MENU:
      ret = get_main_menu(r);
      break;
    case SCREEN_OPTIONS:
      ret = get_options_menu(r);
      break;
  }
  return ret;
}

