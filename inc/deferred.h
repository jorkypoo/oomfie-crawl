#ifndef __DEFERRED_CALL__
#define __DEFERRED_CALL__

#include "ui.h"

#define MAX_DEFERRED_CALLS 16

extern int poop;


// a function to be called by call_deferred()
typedef void (*Deferred_Function)(void* args);

// strucure of a function and it's arguments
typedef struct Deferred_Call Deferred_Call;
struct Deferred_Call {
  Deferred_Function func;
  void* args;
};

typedef struct Deferred_Stack Deferred_Stack;
struct Deferred_Stack {
  Deferred_Call array[MAX_DEFERRED_CALLS];
  int top;
};

/* stack functions */

// function to be called from main game loop 
void call_deferred();
void init_deferred_stack();

// called by the user
void add_deferred_call(Deferred_Function func, void* args);

/* deferred functions and their arguments */

void deferred_update_screen(void* args);
typedef struct {
  Screen* screen;
  Menu* menu;
} update_screen_args;

#endif
