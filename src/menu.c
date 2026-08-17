#include "../inc/menu.h"


// filepaths for textures
#define BTN_PATH "assets/button_default.png"
#define BTN_PATH2 "assets/button_pressed.png"

#define BTN_INIT_FILE "btns.csv"


void change_menu_to_options(void* userdata) { 
  screen->cur_menu = 1;
}


Menu* get_main_menu(SDL_Renderer* r) {
  if (!r) return NULL;  

  Button* tb1 = init_button_offset(r, BTN_INIT_FILE, 0, change_menu_to_options, NULL);
  if (!tb1) return NULL;
  
  Menu* dest = init_menu();
  if (!dest) return NULL;

  if (!add_element_to_menu(dest, &tb1->base)) return NULL;

  return dest;
}


void* callback2 = 0;
void change_menu_to_mainmenu(void* userdata) { 
  screen->cur_menu = 0;
}


Menu* get_options_menu(SDL_Renderer* renderer) {
  if (!renderer) return NULL;  

  Button* tb1 = init_button(MENU_L_BOUND, 0, MENU_WIDTH, 32, change_menu_to_mainmenu, NULL);
  if (!tb1) return NULL;

  Button* tb2 = init_button(MENU_L_BOUND, 32, MENU_WIDTH, 32, change_menu_to_mainmenu, NULL);
  if (!tb2) return NULL;
  
  Menu* dest = init_menu();
  if (!dest) return NULL;

  if (!add_element_to_menu(dest, &tb1->base)) return NULL;
  
  if (!add_element_to_menu(dest, &tb2->base)) return NULL;

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


void handle_screen_updates(Screen* screen) {
  Menu* tmp_menu = NULL;
  //Game* tmp_game = NULL;
  if (should_change_screen_menu(screen)) {
    tmp_menu = get_menu(app.renderer, screen->cur_menu);
    if (!tmp_menu) SDL_Log("error switching menus");

    if (!update_screen_current_menu(screen, tmp_menu)) {
      SDL_Log("error updating screen menu");
    }
  }

  if (should_change_screen_game(screen)) {
    ;
  }
  
}
