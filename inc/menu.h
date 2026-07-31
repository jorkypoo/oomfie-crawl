#ifndef __MENUS__
#define __MENUS__

/* this file helps define some menus that'll be used
 * throughout the game for use during the game
 * perhaps by some state/ui manager?? idk
 * each menu will be custom made in this c file for now
 */

#include "window.h"
#include "ui.h"


#define MENU_L_BOUND 480
#define MENU_R_BOUND 640
#define MENU_WIDTH   160
#define MENU_HEIGHT  480

menu get_main_menu(SDL_Renderer* renderer);
void free_menu(menu* src);
void get_options_menu(SDL_Renderer* renderer, menu* dest);

#endif
