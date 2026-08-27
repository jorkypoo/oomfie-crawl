#include "../inc/deferred.h"

Deferred_Stack _stack = { 0 };

void call_deferred() {
  if (_stack.top) {
    for (int i = 0; i < _stack.top; i++) {
      SDL_Log("function %d\tindex %d", _stack.top, i);
      _stack.array[i].func(_stack.array[i].args);
      _stack.array[i].func = NULL; free(_stack.array[i].args);
    }
  }
  _stack.top = 0;
}

void init_deferred_stack() {
  for (int i = 0; i < MAX_DEFERRED_CALLS; i++) {
    _stack.array[i].func = NULL;
    _stack.array->args = NULL;
  }
  _stack.top = 0;
}

void add_deferred_call(Deferred_Function func, void* args) {
  Deferred_Call tmp = {func, args};
  _stack.array[_stack.top++] = tmp;
}

void deferred_update_screen(void* arg) {
  update_screen_args* args = (update_screen_args*) arg;
  if (!update_screen_current_menu(args->screen, args->menu))
    SDL_Log("error calling deferred_update_screen");
}
