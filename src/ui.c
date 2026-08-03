#include "../inc/ui.h"

#include <SDL3/SDL_timer.h>


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


int on_button(button* b, float mx, float my) {
  return mx >= b->rect.x && mx <= b->rect.x + b->rect.w && my >= b->rect.y && my <= b->rect.y + b->rect.h;
}


void update_button_pos(button* b, float x, float y) {
  b->rect.x = x;
  b->rect.y = y;
}


void update_button_pos_relative(button* b, float x, float y) {
  b->rect.x += x;
  b->rect.y += y;
}


void init_screen(screen* s) {
  s->current = 0;
}


void add_menu_to_screen(screen* s, menu* m) {
  
}


void handle_screen_input(mouse_position* mpos, screen* s, SDL_Event* e) {
  
}


void render_screen(SDL_Renderer* r, screen* s) {
  render_menu(r, s->menus[s->current]);
}


void destroy_screen(screen* s) {
  
}


void init_menu(menu* menu) {
  menu->count = 0;
  menu->current_selection = 0;
}


void free_menu(menu* src) {
  if (!src) return;
  
  for (int i = 0; i < src->count; i++) {
    if (src->buttons[i]->text)        free(src->buttons[i]->text);
    if (src->buttons[i]->tex_default) SDL_DestroyTexture(src->buttons[i]->tex_default);
    if (src->buttons[i]->tex_hovered) SDL_DestroyTexture(src->buttons[i]->tex_hovered);
    if (src->buttons[i]->tex_clicked) SDL_DestroyTexture(src->buttons[i]->tex_clicked);
    if (src->buttons[i])              free(src->buttons[i]);
  }
  src = NULL; // necessary?
}


void add_button_to_menu(menu* menu, button* b) {
  if (menu->count > MAX_MENU_BUTTONS) return;
  menu->buttons[menu->count++] = b;
}


void handle_menu_event(mouse_position* mpos, menu* menu, SDL_Event* e) {
  for (int i = 0; i < menu->count; i++) {
    if (!menu->buttons[i]) { // just incase
      SDL_Log("warning! button no. %d in menu is...invalid?", i);
      continue;
    }
    handle_button_event(mpos, menu->buttons[i], e);
    // want to change this to
    // for (int i = 0; i < menu->buttons.count; i++)
    // menu->buttons[i].handle_event(mpos, e);
  }
}


void render_menu(SDL_Renderer* r, menu* menu) {
  for (int i = 0; i < menu->count; i++) {
    if (!menu->buttons[i]) { 
      SDL_Log("warning! button no. %d in menu is...invalid?", i);
      continue;
    }
    render_button(r, menu->buttons[i]);
  }}


void init_button(button* target, float x, float y, float w, float h, void (*callback)()) {
  target->hovered = 0;
  target->clicked = 0;
  SDL_FRect r = {x, y, w, h};
  target->rect = r;
  target->callback = callback;

  target->tex_default = NURUPO;
  target->tex_hovered = NURUPO;
  target->tex_clicked = NURUPO;
  target->text = NURUPO;
  target->text_len = 0;
}


void init_button_textures(SDL_Renderer* r, button* target, char* def_path, char* hov_path, char* clk_path) {
  SDL_Texture* t = NURUPO;
  if (def_path) {
    t = create_tex(r, def_path);

    if (!t) SDL_Log("error: could not create texture from file %s", def_path);
    else target->tex_default = t;
  }
  
  if (!hov_path) { // there is no hovered texture; use default tex
    if (t) target->tex_hovered = t;
  } else {
    t = create_tex(r, hov_path);

    if (!t) SDL_Log("error: could not create texture from file %s", hov_path);
    else target->tex_hovered = t;
  }

  if (!clk_path) { // there is no clicked texture; use default tex
    if (t) target->tex_clicked = t;
  } else {
    t = create_tex(r, clk_path);

    if (!t) SDL_Log("error: could not create texture from file %s", clk_path);
    else target->tex_clicked = t;
  }
}


void init_button_text(SDL_Renderer* r, button* target, char* text) {
  if (!r || !target || !text) return;

  SDL_Log("init_button_text function not implemented yet");
}


void handle_button_event(mouse_position* mouse, button* b, SDL_Event* event) {
  b->hovered = on_button(b, mouse->x, mouse->y);
  
  switch (event->type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      if (b->hovered && b->callback && event->button.button == SDL_BUTTON_LEFT) {
        b->clicked = 1;
        b->callback();
      }
      break;
    }
    
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      if (b->clicked) {
        b->clicked = 0;
      }
      break;
    }
  }
}


void render_simple_button(SDL_Renderer* renderer, button* target, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, &target->rect);
}


void render_button(SDL_Renderer* r, button* b) {
  if (b->clicked) {
    if (b->tex_clicked)
      SDL_RenderTexture(r, b->tex_clicked, NULL, &b->rect);
    else
      render_simple_button(r, b, 0, 255, 0, 255);
  }
  else if (b->hovered) {
    if (b->tex_hovered)
      SDL_RenderTexture(r, b->tex_hovered, NULL, &b->rect);
    else
      render_simple_button(r, b, 255, 255, 0, 255);
  }
  else {
    if (b->tex_default)
      SDL_RenderTexture(r, b->tex_default, NULL, &b->rect);
    else
      render_simple_button(r, b, 255, 0, 0, 255);
  }

  if (b->selected) {
    // draw a ring of yellow around the button if it is currently selected
    // via the mouse or otherwise
  }

  if (b->text) {
    // add text to the button to be drawn here... i forgor how rn
    // b->text added to a texture... add an applicable rect... render... ig...
  }
}


