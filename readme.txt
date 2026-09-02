simple window management/ui library for sdl3 written in C

setup: a makefile exists; just run make
for examples, see menu.c and the main function
still a work-in-progress

Features:
  - a screen is made up of a menu and a menu is made up of buttons
  - while (running) loop look must be set up in a specific way; see main function
  - various menu elements can be initialised: buttons, text labels and basic textures
  - csv init support for all elements will be done soon, im just lazy

TODO:
  - if no texture is specified for a button, do not render it, instead of rendering some bullshit
  - i'd like fullscreen mode to work...
  - add a textbox that allows for wrapped text - for item descriptions and such
  - allow for user to change keyboard inputs/select between various input maps
  - keyboard navigation for buttons, and the user can use tab maybe to swap ...
  ... between focus on the menu and the game
