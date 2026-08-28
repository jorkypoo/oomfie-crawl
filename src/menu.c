#include "../inc/menu.h"


// filepaths for textures
#define BTN_PATH "assets/button_default.png"
#define BTN_PATH2 "assets/button_pressed.png"

#define BTN_INIT_FILE "btns.csv"
#define LBL_INIT_FILE "labels.csv"
#define TEX_INIT_FILE "tex.csv"


void change_menu_to_options(void* userdata) { 
  screen->cur_menu = 1;
}

void change_menu_opt(void* userdata) {
  Menu* tmp = get_menu(app.renderer, 1);
  update_screen_args* args = malloc(sizeof(update_screen_args));
  args->screen = screen; args->menu = tmp;
  void* cast_args = (void*) args;
  add_deferred_call(deferred_update_screen, cast_args);
}

void button_change_label_text(void* userdata) {
  update_label_text(screen->current_menu, "top", "poop");
}

Menu* get_main_menu(SDL_Renderer* r) {
  if (!r) return NULL;  

  Button* tb1 = init_button_offset(r, BTN_INIT_FILE, 0, change_menu_opt, NULL);
  Button* tb2 = init_button_offset(r, BTN_INIT_FILE, 1, button_change_label_text, NULL);

  Label* tl1 = init_label_offset(r, LBL_INIT_FILE, 0);
  Label* tl2 = init_label_offset(r, LBL_INIT_FILE, 1);

  Texture* tt1 = init_texture_offset(r, TEX_INIT_FILE, 0);
  Texture* tt2 = init_texture_offset(r, TEX_INIT_FILE, 1);
  
  Menu* dest = init_menu();
  if (!dest) return NULL;

  add_element_to_menu(dest, &tb1->base);
  add_element_to_menu(dest, &tb2->base);
  add_element_to_menu(dest, &tl1->base);
  add_element_to_menu(dest, &tl2->base);
  add_element_to_menu(dest, &tt1->base);
  add_element_to_menu(dest, &tt2->base);

  return dest;
}


void* callback2 = 0;
void change_menu_to_mainmenu(void* userdata) { 
  Menu* tmp = get_menu(app.renderer, 0);
  update_screen_args* args = malloc(sizeof(update_screen_args));
  args->screen = screen; args->menu = tmp;
  void* cast_args = (void*) args;
  add_deferred_call(deferred_update_screen, cast_args);
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
