#include "../inc/menu.h"


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

  return dest;
}


menu get_options_menu(SDL_Renderer* renderer) {
  //if (!renderer) return {0};

  button* b1 = malloc(sizeof(button));
  button* b2 = malloc(sizeof(button));
  
  init_button(b1, MENU_L_BOUND, 0, MENU_WIDTH, 32, test_btn_func);
  init_button(b2, MENU_L_BOUND, 32, MENU_WIDTH, 32, test_btn_func);

  menu dest;
  init_menu(&dest);
  add_button_to_menu(&dest, b1);
  add_button_to_menu(&dest, b2);

  return dest;
}


menu get_menu(SDL_Renderer* r, screen_id choice) {
  menu ret;
  
  switch(choice) {
    case SCREEN_MAIN_MENU:
      ret = get_main_menu(r);
      break;
    case SCREEN_OPTIONS:
      ret = get_options_menu(r);
      break;
    case SCREEN_CREDITS:
      //get_credits_menu(r);
      SDL_Log("not implemented yet");
      break;
  }
  return ret;
}

