#include "../inc/core.h"

/* font shit */

int init_global_font(Application* app, char* path, float px, Uint8 r, Uint8 g, Uint8 b) {
  if (!app) return 0;

  app->font_size = px;
  app->font_color.r = r;
  app->font_color.g = g;
  app->font_color.b = b;
  app->font_color.a = 255;

  // defaults for alt font color
  app->alt_font_color.r = 255;
  app->alt_font_color.g = 165;
  app->alt_font_color.b = 0;
  app->alt_font_color.a = 255;

  app->font = TTF_OpenFont(path, px);
  if (!app->font) {
    app->font = NULL;
    return 0;
  }

  return 1;
}


void specify_alt_font_color(Application* app, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  app->alt_font_color.r = r;
  app->alt_font_color.g = g;
  app->alt_font_color.b = b;
  app->alt_font_color.a = a;
}


