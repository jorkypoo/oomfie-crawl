#include "../inc/ui.h"

/* menu shit */

Menu* init_menu() {
  Menu* dest = malloc(sizeof(Menu));
  if (!dest) return NULL;
  
  dest->count = 0;
  dest->capacity = MENU_ELEMENT_COUNT;

  dest->elements = malloc(sizeof(Element*) * MENU_ELEMENT_COUNT);
  if (!dest->elements) {
    printf("poop in init_menu\n");
    return NULL;
  }

  return dest;
}

int add_element_to_menu(Menu* menu, Element* e) {
  if (!menu || !e)
    return 0;

  if (menu->count >= menu->capacity) {
    size_t new_capacity = menu->capacity == 0 ? MENU_ELEMENT_COUNT : menu->capacity * 2;
    Element** new_elements = realloc(menu->elements, new_capacity * sizeof(*new_elements));

    if (!new_elements)
      return 0;

    menu->elements = new_elements;
    menu->capacity = new_capacity;
  }

  menu->elements[menu->count++] = e;

  return 1;
}

void handle_menu_event(Application* app, Menu* menu, SDL_Event* e) {
  if (!menu) return;
  
  for (int i = 0; i < menu->count; i++) {
    if (!menu->elements[i]) { // just incase
      SDL_Log("warning! button no. %d in menu is...invalid?", i);
      continue;
    }
    handle_element_event(app, menu->elements[i], e);
  }
}

void render_menu(Application* app, Menu* menu) {
  if (!app->renderer || !menu) return;

  for (int i = 0; i < menu->count; i++) {
    if (!menu->elements[i]) { 
      SDL_Log("warning! button no. %d in menu is...invalid?", i);
      continue;
    }
    draw_element(app, menu->elements[i]);
  }
}

void free_menu(Menu* src) {
  if (!src) return;

  for (int i = 0; i < src->count; i++) 
    src->elements[i]->destroy(src->elements[i]);
  
  free(src->elements);
  free(src);
}


Screen* init_screen(Menu* m) {
  Screen* dest = malloc(sizeof(Screen));
  if (!dest) {
    free_menu(m);
    return NULL;
  }

  dest->current_menu = m;
  //dest->current_game = g;

  dest->cur_menu = 0;
  dest->prv_menu = 0;

  dest->cur_game= 0;
  dest->prv_game = 0;
  return dest;
}


void free_screen(Screen* s) {
  if (!s) return;

  free_menu(s->current_menu);
  free(s);
}


int update_screen_current_menu(Screen* s, Menu* m) {
  if (!s || !m) return 0;

  if (s->current_menu != m) 
    free_menu(s->current_menu);
  
  s->current_menu = m;
  return 1;
}


void handle_screen_input(Application* app, Screen* s, SDL_Event* e) {
  handle_menu_event(app, s->current_menu, e);
}


void render_screen(Application* app, Screen* s) {
  render_menu(app, s->current_menu);
}


int should_change_screen_menu(Screen* s) {
  if (s->cur_menu != s->prv_menu) {
    s->prv_menu = s->cur_menu;
    return 1;
  }
  return 0;
}

int should_change_screen_game(Screen* s) {
  if (s->cur_game != s->prv_menu) {
    s->prv_game = s->cur_game;
    return 1;
  }
  return 0;
}
