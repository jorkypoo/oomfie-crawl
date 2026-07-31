#include "../inc/menu.h"

#include <stdlib.h>

// filepaths for textures
#define BTN_PATH "assets/button_default.png"
#define BTN_PATH2 "assets/button_pressed.png"


void test_btn_func() { SDL_Log("hi from button"); }
void test_btn_text() { SDL_Log("hi fromt textured button"); }


menu get_main_menu(SDL_Renderer* renderer) {
  // if (!renderer) return {0};  

  button* test_button = malloc(sizeof(button));
  button* test_button2 = malloc(sizeof(button));

  init_button(test_button, 480, 0, 80, 48, test_btn_func);
  init_button_textures(renderer, test_button, BTN_PATH, BTN_PATH2, NULL);

  init_button(test_button2, 560, 0, 80, 48, test_btn_func);
  init_button_textures(renderer, test_button2, BTN_PATH, BTN_PATH2, NULL);

  menu dest;
  init_menu(&dest);
  add_button_to_menu(&dest, test_button);
  add_button_to_menu(&dest, test_button2);

  //if (!dest) return {0};
  return dest;
}


void free_menu(menu* src) {
  for (int i = 0; i < src->count; i++) {
    if (src->buttons[i]->text) free(src->buttons[i]->text);
    free(src->buttons[i]);
  }
}


void get_options_menu(SDL_Renderer* renderer, menu* dest) {
  if (!renderer) return;

  button test_button = { 0 };
  button test_button2 = { 0 };
  
  init_button(&test_button, MENU_L_BOUND, 0, MENU_WIDTH, 32, test_btn_func);
  init_button(&test_button2, MENU_L_BOUND, 32, MENU_WIDTH, 32, test_btn_func);

  init_menu(dest);
  add_button_to_menu(dest, &test_button);
  add_button_to_menu(dest, &test_button2);
}
