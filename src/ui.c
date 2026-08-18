#include "../inc/ui.h"

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

void handle_element_event(mouse_position* mpos, Element* e, SDL_Event* ev) {
  if (!e)
    return;

  e->hovered = on_element(e, mpos->x, mpos->y);

  if (!e->handle_event)
    return;

  e->handle_event(mpos, e, ev);
}

void destroy_element(Element* e) {
  if (!e)
    return;

  if (e->destroy)
    e->destroy(e);
}


bool on_element(Element* e, float mx, float my) {
  return mx >= e->rect.x && mx <= e->rect.x + e->rect.w && my >= e->rect.y && my <= e->rect.y + e->rect.h;
}

/* button shit */

Button* init_button(float x,float y,float w,float h,void(*callback)(void* data),void* data) {
  Button* dest = malloc(sizeof(Button));
  if (!dest)
    return NULL;

  dest->base.hovered = 0;
  dest->base.clicked = 0;
  
  SDL_FRect r = {x, y, w, h};
  dest->base.rect = r;

  dest->base.handle_event = handle_button_event;
  dest->base.draw = render_button;
  dest->base.destroy = free_button;

  dest->callback = callback;
  dest->userdata = data;

  dest->tex_default = NULL;
  dest->tex_hovered = NULL;
  dest->tex_clicked = NULL;

  return dest;
}

int init_button_textures(SDL_Renderer* r,Button* b,char* def_path,char* hov_path,char* clk_path) {
  if (!r || !b) 
    return 0;
  
  SDL_Texture* t;
  if (def_path) {
    t = create_tex(r, def_path);

    if (!t) { // create the default texture, that other textures may share
      SDL_Log("error: could not create default button texture from file %s", def_path);
      return 0;
    } else {
      b->tex_default = t;
    }
    
  } else {
    SDL_Log("error: define at least def_path when creating textured button");
    return 0;
  }
  
  if (!hov_path) { // there is no hovered texture; use default tex
    if (t) {
      b->tex_hovered = t;
    }
  } else { // create texture from hov_path
    t = create_tex(r, hov_path);

    if (!t) { 
      SDL_Log("error: could not create texture from file %s", hov_path);
      return 0;
    } else {
      b->tex_hovered = t; 
    }
  }

  if (!clk_path) { // there is no clicked texture; use default tex
    if (t) { 
      b->tex_clicked = t;
    }
  } else { // create texture from clk_path
    t = create_tex(r, clk_path);

    if (!t) {
      SDL_Log("error: could not create texture from file %s", clk_path);
      return 0;
    } else {
      b->tex_clicked = t;
    }
  }
  return 1;
}

// these are to be called by the user, but they can also use the  ones above
Button* init_button_offset(SDL_Renderer* r, char* path, int offset, void (*callback)(void* callback_data), void* userdata) {
  if (!path || !r) return NULL;
  
  // returns x,y,w,h,def_path,hov_path,clk_path
  // where hov_path and clk_path can be omitted, or 0 - representing null
  char* bs = get_line_offset(path, offset);
  if (!bs) return NULL;
  
  float x = atof(get_delimited_value(bs, DELIMITER, 0));
  float y = atof(get_delimited_value(bs, DELIMITER, 1));
  float w = atof(get_delimited_value(bs, DELIMITER, 2));
  float h = atof(get_delimited_value(bs, DELIMITER, 3));
  Button* dest = init_button(x,y,w,h,callback,userdata);

  // dpath must be defined; others can be empty or defined
  char* dpath = get_delimited_value(bs, DELIMITER, 4);
  if (!dpath) { // button path must not be shit
      SDL_Log("default button path must be defined in %s", path);
      return NULL;
  }

  // allow for test buttons to be created
  if (!strcmp(dpath, "test") || !strcmp(dpath, "t") || !strcmp(dpath, "poop")) { 
    free(dpath);
    dpath = NULL;
  }
   
  // following entries can either be omitted, or 0
  // so make sure you don't neglect a filepath that starts with 0
  // fucking maniac
  char* hpath = get_delimited_value(bs, DELIMITER, 5);
  if (hpath) if (strlen(hpath) <= 1 && *hpath == '0') hpath = NULL;

  char* cpath = get_delimited_value(bs, DELIMITER, 6);
  if (cpath) if (strlen(cpath) <= 1 && *cpath == '0') cpath = NULL;

  // if dpath was defined as a testing texture, no need to pass it to this function
  // also, it would call an error of dpath wasn't defined lolsies
  if (dpath)
    if (!init_button_textures(r, dest, dpath, hpath, cpath))
      return NULL;

  // gotta free result of many of these functions
  free(bs);
  if (dpath) free(dpath);
  if (hpath) free(hpath);
  if (cpath) free(cpath);
  return dest;
}

Button* init_button_match(SDL_Renderer* r, char* path, char* match, void (*callback)(void* callback_data), void* userdata) {
  if (!path || !r) return NULL;

  char* bs = get_line_match(path, match);
  if (!bs) return NULL;

  float x = atof(get_delimited_value(bs, DELIMITER, 0));
  float y = atof(get_delimited_value(bs, DELIMITER, 1));
  float w = atof(get_delimited_value(bs, DELIMITER, 2));
  float h = atof(get_delimited_value(bs, DELIMITER, 3));
  Button* dest = init_button(x,y,w,h,callback,userdata);

  char* dpath = get_delimited_value(bs, DELIMITER, 4);
  if (!dpath) { 
      SDL_Log("default button path must be defined in %s", path);
      return NULL;
  }

  if (!strcmp(dpath, "test") || !strcmp(dpath, "t") || !strcmp(dpath, "poop")) { 
    SDL_Log("hi");
    free(dpath);
    dpath = NULL;
  }

  char* hpath = get_delimited_value(bs, DELIMITER, 5);
  if (hpath) if (strlen(hpath) <= 1 && *hpath == '0') hpath = NULL;

  char* cpath = get_delimited_value(bs, DELIMITER, 6);
  if (cpath) if (strlen(cpath) <= 1 && *cpath == '0') cpath = NULL;

  if (dpath) 
    if (!init_button_textures(r, dest, dpath, hpath, cpath))
      return NULL;

  free(bs);
  if (dpath) free(dpath);
  if (hpath) free(hpath);
  if (cpath) free(cpath);
  return dest;
}

void handle_button_event(mouse_position* mpos, Element* e, SDL_Event* event) {
  if (!mpos || !e || !event) return;

  Button* b = (Button*)e;

  switch (event->type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      if (b->base.hovered && b->callback && event->button.button == SDL_BUTTON_LEFT) {
        b->base.clicked = 1;
        b->callback(b->userdata);
      }
      break;
    }
    
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      if (b->base.clicked) {
        b->base.clicked = 0;
      }
      break;
    }
  }
}

void render_button(Application* app, Element* e) {
  if (!e) return;

  Button* b = (Button*)e;

  if (b->base.clicked) {
    if (b->tex_clicked)
      SDL_RenderTexture(app->renderer, b->tex_clicked, NULL, &b->base.rect);
    else
      render_simple_button(app->renderer, b, 0, 255, 0, 255);
  }
  else if (b->base.hovered) {
    if (b->tex_hovered)
      SDL_RenderTexture(app->renderer, b->tex_hovered, NULL, &b->base.rect);
    else
      render_simple_button(app->renderer, b, 255, 255, 0, 255);
  }
  else {
    if (b->tex_default)
      SDL_RenderTexture(app->renderer, b->tex_default, NULL, &b->base.rect);
    else
      render_simple_button(app->renderer, b, 255, 0, 0, 255);
  }
}

void free_button(Element* e) {
  if (!e) return;

  Button* b = (Button*)e;

  if (b->tex_default) SDL_DestroyTexture(b->tex_default);
  if (b->tex_hovered) SDL_DestroyTexture(b->tex_hovered);
  if (b->tex_clicked) SDL_DestroyTexture(b->tex_clicked);
    
  free(b);
}

void render_simple_button(SDL_Renderer* renderer, Button* target, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  if (!renderer || !target)
    return;
  
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, &target->base.rect);
}

/* label shit */

Label* init_label(SDL_Renderer* r, float x, float y, float w, float h, char* text, char* texture_path) {
  Label* dest = malloc(sizeof(Label));
  if (!dest) return NULL;

  SDL_FRect re = {x,y,w,h};
  dest->base.rect = re;

  dest->base.hovered = 0;
  dest->base.clicked = 0;

  dest->base.handle_event = handle_label_event;
  dest->base.draw = render_label;
  dest->base.destroy = free_label;

  // dest color's alpha is set to 0 until initialised
  dest->color.r = 0;
  dest->color.g = 0;
  dest->color.b = 0;
  dest->color.a = 0;

  if (texture_path) { // texture path can be NULL
    dest->bg = create_tex(r, texture_path);
    if (!dest->bg) {
      SDL_Log("could not create label bg texture from %s; will use a default color", texture_path);
      dest->bg = NULL;
    }
  } else { // texture path can be NULL
    dest->bg = NULL;
  }
  
  dest->text = strdup(text);
  if (!dest->text)
    return NULL;

  dest->text_len = strlen(dest->text) + 1;

  return dest;
}

void add_label_bg_color(Label* t, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  t->color.r = r;
  t->color.g = g;
  t->color.b = b;

  if (a == 0)
    t->color.a = 1;
  else
    t->color.a = a;
}

void handle_label_event(mouse_position* mpos, Element* e, SDL_Event* event) {
  if (!e) return;

  Label* t = (Label*)e;

  // don't have to do anything rlly
}

void render_label(Application* app, Element* e) {
  // render bg THEN text so text is drawn on top
  if (!e) return;

  Label* t = (Label*)e;

  // render bg
  if (t->bg) { // render background, or a default 
    SDL_RenderTexture(app->renderer, t->bg, NULL, &t->base.rect);
  } else if (t->color.a != 0) { // used color, if it was added
    SDL_SetRenderDrawColor(app->renderer, t->color.r, t->color.g, t->color.b, t->color.a);
    SDL_RenderFillRect(app->renderer, &t->base.rect);
  } else {
    SDL_SetRenderDrawColor(app->renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(app->renderer, &t->base.rect);
  }

  // render text
  if (!t->text) return;
  SDL_Color white = {255, 255, 255, 255};
  SDL_FRect drect;

  SDL_Surface* s = TTF_RenderText_Solid(app->font, t->text, 0, white);
  SDL_Texture* d = SDL_CreateTextureFromSurface(app->renderer, s);
  SDL_DestroySurface(s);

  // grab dimensions for positioning the text
  SDL_GetTextureSize(d, &drect.w, &drect.h);

  // centre text y value
  drect.y = t->base.rect.y + ((t->base.rect.h - drect.h) / 2);

  // centre text x value... gleep
  drect.x = t->base.rect.x + ((t->base.rect.w - drect.w) / 2); 

  
  //SDL_Log("rect size - h:%f, w:%f", drect.w, drect.h);
  SDL_RenderTexture(app->renderer, d, NULL, &drect);
  SDL_DestroyTexture(d);
}

void free_label(Element* e) {
  if (!e) return;

  Label* dest = (Label*)e;

  if (dest->text) free(dest->text);
  if (dest->bg)   SDL_DestroyTexture(dest->bg);
  
  free(dest);
}

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

void handle_menu_event(mouse_position* mpos, Menu* menu, SDL_Event* e) {
  if (!menu) return;
  
  for (int i = 0; i < menu->count; i++) {
    if (!menu->elements[i]) { // just incase
      SDL_Log("warning! button no. %d in menu is...invalid?", i);
      continue;
    }
    handle_element_event(mpos, menu->elements[i], e);
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

  if (s->current_menu != m) {
    free_menu(s->current_menu);
  }

  s->current_menu = m;
  return 1;
}


void handle_screen_input(mouse_position* mpos, Screen* s, SDL_Event* e) {
  handle_menu_event(mpos, s->current_menu, e);
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
