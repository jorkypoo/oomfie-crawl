simple window management/ui library for sdl3 written in C

setup: decent makefile exists; just run make
for examples, see menu.c and the main function
still a work-in-progress

Features:
  - f1 to cycle resolutions and f2 to shift to borderless
  - ui.c, filesystem.c & window.c act as engine code, so please don't change them without reason
  - while (running) loop look must be set up in a specific way; again, see main function
  - various menu elements can be initialised: buttons, text labels and basic textures
  - csv init support for all elements will be done soon, im just lazy

TODO:
  - add the fucking fps code dude
  - i'd like fullscreen mode to work....
  - pass the app into event handling instead of just the mouse position
  - add a textbox that allows for wrapped text - for item descriptions and such
  - maybe a deferred call system: allows functions to be called after all the...
  ...frame's code has been executed. would help with changing menus and future shit...
  ...i imagine this would mostly be hard coded stuff yk
  - allow for user to change keyboard inputs/select between various input maps

My Notes:
  - call deferred could simply be a struct attached to the app which contains some...
  ...booleans that you can flip. then, at the end of the frame, check to see what...
  ...functions need to be run
  - sagu also has an idea that's more complicated but would work better
