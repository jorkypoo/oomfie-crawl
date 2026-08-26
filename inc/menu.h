#ifndef __MENUS__
#define __MENUS__

/* this file helps define some menus that'll be used
 * throughout the game for use during the game
 * perhaps by some state/ui manager?? idk
 * each menu will be custom made in this c file for now
 *
 * this file also doesn't represent engine code - it uses the engine to add menus
 */

#include "window.h"
#include "ui.h"
#include "deferred.h"


#define MENU_L_BOUND 480
#define MENU_R_BOUND 640
#define MENU_WIDTH   160
#define MENU_HEIGHT  480


typedef enum {
  SCREEN_MAIN_MENU,
  SCREEN_OPTIONS,
  SCREEN_CREDITS,
  SCREEN_GAME,
  SCREEN_NUM_OF,
} screen_id;

extern Screen* screen;
extern Application app;



// returns a menu object
Menu* get_main_menu(SDL_Renderer* renderer);
Menu* get_options_menu(SDL_Renderer* renderer);

// general menu grabbing thingy
Menu* get_menu(SDL_Renderer* r, screen_id choice);

// ugh kinda lazy to be here
void handle_screen_updates(Screen* screen);

#endif
