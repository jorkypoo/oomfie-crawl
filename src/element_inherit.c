#include "../inc/ui.h"

/* button shit */

Button* init_button(float x,float y,float w,float h,void(*callback)(void* data),void* data) {
  Button* dest = malloc(sizeof(Button));
  if (!dest)
    return NULL;

  dest->base.hovered = 0;
  dest->base.clicked = 0;
  
  SDL_FRect r = {x, y, w, h};
  dest->base.rect = r;

  char tmp[8] = {0};
  strcpy(dest->base.id, tmp);

  dest->base.handle_event = handle_button_event;
  dest->base.draw = render_button;
  dest->base.destroy = free_button;

  dest->callback = callback;
  dest->userdata = data;

  dest->tex_default = NULL;
  dest->tex_hovered = NULL;
  dest->tex_clicked = NULL;

  dest->text = NULL;
  dest->text_len = 0;

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

int init_button_text(Button* b, char* text) {
  if (!b || !text) return 0;

  b->text = strdup(text);
  if (!b->text) {
    b->text = NULL;
    return 0;
  }

  b->text_len = strlen(b->text) + 1;

  return 1;
}

// these are to be called by the user, but they can also use the  ones above
Button* init_button_offset(SDL_Renderer* r, char* path, int offset, void (*callback)(void* callback_data), void* userdata) {
  if (!path || !r) return NULL;
  
  // returns x,y,w,h,def_path,hov_path,clk_path
  // where hov_path and clk_path can be omitted, or 0 - representing null
  char* bs = get_line_offset(path, offset);
  if (!bs) return NULL;

  float x, y, w, h;
  char* tx = get_delimited_value(bs, DELIMITER, 1);
  char* ty = get_delimited_value(bs, DELIMITER, 2);
  char* tw = get_delimited_value(bs, DELIMITER, 3);
  char* th = get_delimited_value(bs, DELIMITER, 4);

  x = atof(tx); free(tx);
  y = atof(ty); free(ty);
  w = atof(tw); free(tw);
  h = atof(th); free(th);

  Button* dest = init_button(x,y,w,h,callback,userdata);

  // get the id and assign it to the button
  // must be 8 characters or less, otherwise it is discarded
  char* ttex = get_delimited_value(bs, DELIMITER, 0);
  if (ttex) {
    add_element_id(&dest->base, ttex);
    free(ttex);
  }

  // get the button text at offset 4 & assign keywords to not draw text on button
  char* txt = get_delimited_value(bs, DELIMITER, 5);
  if (!strcmp(txt, "test") || !strcmp(txt, "notext"))
    dest->text = NULL;
  else
    init_button_text(dest, txt);

  // dpath must be defined; others can be empty or defined
  char* dpath = get_delimited_value(bs, DELIMITER, 6);
  if (!dpath) { // button path must not be shit
      SDL_Log("default button path must be defined in %s", path);
      return NULL;
  }

  // allow for textureless text buttons to be created
  if (!strcmp(dpath, "test") || !strcmp(dpath, "notex") || !strcmp(dpath, "poop")) { 
    free(dpath);
    dpath = NULL;
  }
   
  // following entries can either be omitted, or 0
  // so make sure you don't neglect a filepath that starts with 0
  // fucking maniac
  char* hpath = get_delimited_value(bs, DELIMITER, 7);
  if (hpath) if (strlen(hpath) <= 1 && *hpath == '0') {
    free(hpath);
    hpath = NULL;
  }

  char* cpath = get_delimited_value(bs, DELIMITER, 8);
  if (cpath) if (strlen(cpath) <= 1 && *cpath == '0') {
    free(cpath);
    cpath = NULL;
  }

  // if dpath was defined as a testing texture, no need to pass it to this function
  // also, it would call an error of dpath wasn't defined lolsies
  if (dpath)
    if (!init_button_textures(r, dest, dpath, hpath, cpath))
      return NULL;

  // gotta free result of many of these functions
  free(bs);
  if (txt)   free(txt);
  if (dpath) free(dpath);
  if (hpath) free(hpath);
  if (cpath) free(cpath);
  return dest;
}

Button* init_button_match(SDL_Renderer* r, char* path, char* match, void (*callback)(void* callback_data), void* userdata) {
  if (!path || !r) return NULL;

  char* bs = get_line_match(path, match);
  if (!bs) return NULL;

  float x, y, w, h;
  char* tx = get_delimited_value(bs, DELIMITER, 1);
  char* ty = get_delimited_value(bs, DELIMITER, 2);
  char* tw = get_delimited_value(bs, DELIMITER, 3);
  char* th = get_delimited_value(bs, DELIMITER, 4);

  x = atof(tx); free(tx);
  y = atof(ty); free(ty);
  w = atof(tw); free(tw);
  h = atof(th); free(th);

  Button* dest = init_button(x,y,w,h,callback,userdata);

  char* ttex = get_delimited_value(bs, DELIMITER, 0);
  if (ttex) {
    add_element_id(&dest->base, ttex);
    free(ttex);
  }

  char* txt = get_delimited_value(bs, DELIMITER, 5);
  if (!strcmp(txt, "test") || !strcmp(txt, "notext"))
    dest->text = NULL;
  else
    init_button_text(dest, txt);

  char* dpath = get_delimited_value(bs, DELIMITER, 6);
  if (!dpath) { 
      SDL_Log("default button path must be defined in %s", path);
      return NULL;
  }

  if (!strcmp(dpath, "test") || !strcmp(dpath, "t") || !strcmp(dpath, "poop")) { 
    SDL_Log("hi");
    free(dpath);
    dpath = NULL;
  }

  char* hpath = get_delimited_value(bs, DELIMITER, 7);
  if (hpath) if (strlen(hpath) <= 1 && *hpath == '0') hpath = NULL;

  char* cpath = get_delimited_value(bs, DELIMITER, 8);
  if (cpath) if (strlen(cpath) <= 1 && *cpath == '0') cpath = NULL;

  if (dpath) 
    if (!init_button_textures(r, dest, dpath, hpath, cpath))
      return NULL;

  free(bs);
  if (txt)   free(txt);
  if (dpath) free(dpath);
  if (hpath) free(hpath);
  if (cpath) free(cpath);
  return dest;
}

void handle_button_event(Application* app, Element* e, SDL_Event* event) {
  if (!app || !e || !event) return;

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

  // rendering the button
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

  // rendering the text on top
  if (!b->text) return;

  // use the game's alt font color for when text is hovered
  SDL_Color font_color = app->font_color;
  if (b->base.hovered)
    font_color = app->alt_font_color;

  SDL_FRect drect;
  SDL_Surface* s = TTF_RenderText_Solid(app->font, b->text, 0, font_color);
  SDL_Texture* d = SDL_CreateTextureFromSurface(app->renderer, s);
  SDL_DestroySurface(s);

  // grab dimensions for positioning the text
  SDL_GetTextureSize(d, &drect.w, &drect.h);

  // centre text x&y values
  drect.y = b->base.rect.y + ((b->base.rect.h - drect.h) / 2);
  drect.x = b->base.rect.x + ((b->base.rect.w - drect.w) / 2); 
  
  SDL_RenderTexture(app->renderer, d, NULL, &drect);
  SDL_DestroyTexture(d);
}

void free_button(Element* e) {
  if (!e) return;

  Button* b = (Button*)e;

  if (b->tex_default) SDL_DestroyTexture(b->tex_default);
  if (b->tex_hovered) SDL_DestroyTexture(b->tex_hovered);
  if (b->tex_clicked) SDL_DestroyTexture(b->tex_clicked);

  if (b->text) free(b->text);
    
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

// x,y,w,h,text,txpath,r,g,b,a
Label* init_label_offset(SDL_Renderer* r, char* path, int offset) {
  if (!r || !path) return NULL;
  
  char* bs = get_line_offset(path, offset);
  if (!bs) return NULL;

  // to avoid a memory leak this becomes a lot more annoying
  // grab xywh values
  float x, y, w, h;
  char* tx = get_delimited_value(bs, DELIMITER, 1);
  char* ty = get_delimited_value(bs, DELIMITER, 2);
  char* tw = get_delimited_value(bs, DELIMITER, 3);
  char* th = get_delimited_value(bs, DELIMITER, 4);

  x = atof(tx); free(tx);
  y = atof(ty); free(ty);
  w = atof(tw); free(tw);
  h = atof(th); free(th);

  // get text and texpath
  char* text = get_delimited_value(bs, DELIMITER, 5);
  if (!text) return NULL;

  char* texpath = get_delimited_value(bs, DELIMITER, 6);
  if (!texpath)
    return NULL;

  // texture path can be not set with these keywords
  if (!strcmp(texpath, "notex") || !strcmp(texpath, "0")) {
    free(texpath);
    texpath = NULL;
  }

  Label* dest = init_label(r, x, y, w, h, text, texpath);
  if (!dest) return NULL;

  char* id = get_delimited_value(bs, DELIMITER, 0);
  if (id) {
    add_element_id(&dest->base, id);
    free(id);
  }
  
  int c, g, b, a;
  char* tc = get_delimited_value(bs, DELIMITER, 7); 
  char* tg = get_delimited_value(bs, DELIMITER, 8);
  char* tb = get_delimited_value(bs, DELIMITER, 9);
  char* ta = get_delimited_value(bs, DELIMITER, 10);

  c = atoi(tc); free(tc);
  g = atoi(tg); free(tg);
  b = atoi(tb); free(tb);
  a = atoi(ta); free(ta);

  if (a != 1)
    add_label_bg_color(dest, c, g, b, a);

  free(bs);
  if (text) free(text);
  if (texpath) free(texpath);
  return dest;
}

Label* init_label_match(SDL_Renderer* r, char* path, char* match) {
  if (!r || !path) return NULL;
  
  char* bs = get_line_match(path, match);
  if (!bs) return NULL;

  float x, y, w, h;
  char* tx = get_delimited_value(bs, DELIMITER, 0);
  char* ty = get_delimited_value(bs, DELIMITER, 1);
  char* tw = get_delimited_value(bs, DELIMITER, 2);
  char* th = get_delimited_value(bs, DELIMITER, 3);

  x = atof(tx); free(tx);
  y = atof(ty); free(ty);
  w = atof(tw); free(tw);
  h = atof(th); free(th);

  // get text and texpath
  char* text = get_delimited_value(bs, DELIMITER, 4);
  if (!text) return NULL;

  char* texpath = get_delimited_value(bs, DELIMITER, 5);
  if (!texpath)
    return NULL;

  // texture path can be not set with these keywords
  if (!strcmp(texpath, "notex") || !strcmp(texpath, "0")) {
    free(texpath);
    texpath = NULL;
  }

  Label* dest = init_label(r, x, y, w, h, text, texpath);
  if (!dest) return NULL;

  char* id = get_delimited_value(bs, DELIMITER, 0);
  if (id) {
    add_element_id(&dest->base, id);
    free(id);
  }

  int c, g, b, a;
  char* tc = get_delimited_value(bs, DELIMITER, 6); 
  char* tg = get_delimited_value(bs, DELIMITER, 7);
  char* tb = get_delimited_value(bs, DELIMITER, 8);
  char* ta = get_delimited_value(bs, DELIMITER, 9);

  c = atoi(tc); free(tc);
  g = atoi(tg); free(tg);
  b = atoi(tb); free(tb);
  a = atoi(ta); free(ta);

  if (a != 1)
    add_label_bg_color(dest, c, g, b, a);

  free(bs);
  if (text) free(text);
  if (texpath) free(texpath);
  return dest;
}

// search a menu for an ELEMENT with id and updates text to text
void update_label_text(Menu* m, char* id, char* text) {
  if (m->count < 1) return;
  Label* dest = NULL;
  for (size_t i = 0; i < m->count; i++) {
    if (!strcmp(m->elements[i]->id, id)) {
      dest = (Label*) m->elements[i];
      strcpy(dest->text, text);
      dest->text_len = strlen(dest->text) + 1;
    }
  }
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

void handle_label_event(Application* app, Element* e, SDL_Event* event) {
  if (!e) return;

  Label* t = (Label*)e;

  // don't have to do anything rlly
}

void render_label(Application* app, Element* e) {
  // render bg THEN text so text is drawn on top
  if (!e) return;

  Label* t = (Label*)e;

  // render bg
  if (t->bg) { // render background texture 
    SDL_RenderTexture(app->renderer, t->bg, NULL, &t->base.rect);
  } else if (t->color.a < 2) { // if texture not set, use color if it was added
    SDL_SetRenderDrawColor(app->renderer, t->color.r, t->color.g, t->color.b, t->color.a);
    SDL_RenderFillRect(app->renderer, &t->base.rect);
  } // else, render nothing
  
  // render text
  if (!t->text) return;

  SDL_Color font_color = app->font_color;
  SDL_FRect drect;
  SDL_Surface* s = TTF_RenderText_Solid(app->font, t->text, 0, font_color);
  SDL_Texture* d = SDL_CreateTextureFromSurface(app->renderer, s);
  SDL_DestroySurface(s);

  // grab dimensions for positioning the text
  SDL_GetTextureSize(d, &drect.w, &drect.h);
  drect.y = t->base.rect.y + ((t->base.rect.h - drect.h) / 2);
  drect.x = t->base.rect.x + ((t->base.rect.w - drect.w) / 2); 

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

/* texture shit */

Texture* init_texture(SDL_Renderer* r, char* path, float x, float y) {
  if (!r || !path) return NULL;

  Texture* dest = malloc(sizeof(Texture));
  if (!dest) return NULL;

  dest->tex = create_tex(r, path);
  if (!dest->tex) return NULL;

  SDL_GetTextureSize(dest->tex, &dest->base.rect.w, &dest->base.rect.h);
  dest->base.rect.x = x; dest->base.rect.y = y;
  dest->base.hovered = 0; dest->base.clicked = 0;

  dest->base.handle_event = handle_texture_event;
  dest->base.draw = render_texture;
  dest->base.destroy = destroy_texture;

  return dest;
}

Texture* init_texture_dimensions(SDL_Renderer* r, char* path, float x, float y, float w, float h) {
  if (!r || !path) return NULL;

  Texture* dest = malloc(sizeof(Texture));
  if (!dest) return NULL;

  dest->tex = create_tex(r, path);
  if (!dest->tex) return NULL;

  dest->base.rect.w = w; dest->base.rect.h = h;
  dest->base.rect.x = x; dest->base.rect.y = y;
  dest->base.hovered = 0; dest->base.clicked = 0;

  dest->base.handle_event = handle_texture_event;
  dest->base.draw = render_texture;
  dest->base.destroy = destroy_texture;

  return dest;
}

Texture* init_texture_offset(SDL_Renderer* r, char* path, int offset) {
  if (!r) return NULL;

  char* bs = get_line_offset(path, offset);
  if (!bs) return NULL;
  
  float x, y, w, h;
  char* tx = get_delimited_value(bs, DELIMITER, 1);
  char* ty = get_delimited_value(bs, DELIMITER, 2);
  char* tw = get_delimited_value(bs, DELIMITER, 3);
  char* th = get_delimited_value(bs, DELIMITER, 4);

  x = atof(tx); free(tx);
  y = atof(ty); free(ty);
  w = atof(tw); free(tw);
  h = atof(th); free(th);

  char* texpath = get_delimited_value(bs, DELIMITER, 5);
  if (!texpath) return NULL;

  Texture* dest;
  if (w == 0 && h == 0) { // use native texture dimensions
    dest = init_texture(r, texpath, x, y);
  } else { // use the specified dimensions
    dest = init_texture_dimensions(r, texpath, x, y, w, h);
  }
  if (!dest) return NULL;

  char* id = get_delimited_value(bs, DELIMITER, 0);
  if (id) {
    add_element_id(&dest->base, id);
    free(id);
  }

  free(bs);
  if (texpath) free(texpath);
  return dest;
}

Texture* init_texture_match(SDL_Renderer* r, char* path, char* match) {
  if (!r) return NULL;

  char* bs = get_line_match(path, match);
  if (!bs) return NULL;
  
  float x, y, w, h;
  char* tx = get_delimited_value(bs, DELIMITER, 1);
  char* ty = get_delimited_value(bs, DELIMITER, 2);
  char* tw = get_delimited_value(bs, DELIMITER, 3);
  char* th = get_delimited_value(bs, DELIMITER, 4);

  x = atof(tx); free(tx);
  y = atof(ty); free(ty);
  w = atof(tw); free(tw);
  h = atof(th); free(th);

  char* texpath = get_delimited_value(bs, DELIMITER, 5);
  if (!texpath) return NULL;

  Texture* dest;
  if (w == 0 && h == 0) { // use native texture dimensions
    dest = init_texture(r, texpath, x, y);
  } else { // use the specified dimensions
    dest = init_texture_dimensions(r, texpath, x, y, w, h);
  }
  if (!dest) return NULL;

  char* id = get_delimited_value(bs, DELIMITER, 0);
  if (id) {
    add_element_id(&dest->base, id);
    free(id);
  }

  free(bs);
  if (texpath) free(texpath);
  return dest;
}

void handle_texture_event(Application* app, Element* e, SDL_Event* event) {
  if (!e || !event) return;

  Texture* tex = (Texture*)e;
}

void render_texture(Application* app, Element* e) {
  if (!e || !app) return;

  Texture* t = (Texture*)e;

  SDL_RenderTexture(app->renderer, t->tex, NULL, &t->base.rect);
}

void destroy_texture(Element* e) {
  if (!e) return;

  Texture* t = (Texture*)e;
  if (t->tex) SDL_DestroyTexture(t->tex);
  free(t);
}


