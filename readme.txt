simple window management/ui library for sdl3 written in C

setup: decent makefile exists; just run make
for examples, see menu.c and the main function
still a work-in-progress

notes:
  - f1 to cycle resolutions and f2 to shift to borderless
  - ui.c, filesystem.c & window.c act as engine code, so please don't change them without reason
  - while (running) loop look must be set up in a specific way; again, see main function
  - buttons can be initialised from a csv file; follows pattern: x,y,w,h,p1,p2,p3

TODO:
  - create a base "element" struct which different ui elements inherit from
  - allows for menus to be made up of textboxes, bars, textures and buttons
