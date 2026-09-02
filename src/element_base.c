#include "../inc/ui.h"

/* element and general shit */

SDL_Texture* create_tex(SDL_Renderer* r, char* path) {
  SDL_Surface* s = NULL;
  SDL_Texture* t = NULL;

  if (!path) return NULL;

  s = IMG_Load(path);
  if (!s) return NULL;
  
  t = SDL_CreateTextureFromSurface(r, s);
  if (!t) return NULL;

  return t;
}

/* element shit */

void draw_element(Application* app, Element* e) {
  if (!e || !e->draw)
    return;

  if (!app || !app->renderer)
    return;

  e->draw(app, e);
}

void handle_element_event(Application* app, Element* e, SDL_Event* ev) {
  if (!e)
    return;

  e->hovered = on_element(e, app->mouse_pos.x, app->mouse_pos.y);

  if (!e->handle_event)
    return;

  e->handle_event(app, e, ev);
}

void destroy_element(Element* e) {
  if (!e)
    return;

  if (e->destroy)
    e->destroy(e);
}


void add_element_id(Element* e, char* id) {
  // id should be 8 chars long + 1 \0
  // e->id is always set to all zeros
  if (strlen(id) > 8) return;

  strcpy(e->id, id);
}


bool on_element(Element* e, float mx, float my) {
  return mx >= e->rect.x && mx <= e->rect.x + e->rect.w && my >= e->rect.y && my <= e->rect.y + e->rect.h;
}


