simple window management/ui library for sdl3 written in C

setup: decent makefile exists; just run make
for examples, see menu.c and the main function
still a work-in-progress

notes:
  - f1 to cycle resolutions and f2 to shift to borderless
  - f8 & f10 to switch between menus (just for testing)
  - ui.c & window.c act as engine code, so please don't change them without reason
  - while (running) loop look must be set up in a specific way; again, see main function

TODO:
  - allow for button callbacks to update Screen 
  - create a base "element" struct which different ui elements inherit from
  - allows for menus to be made up of textboxes, bars, textures and buttons
