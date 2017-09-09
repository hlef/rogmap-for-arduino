/*
  rogmap.h - Library for random dungeon generation.
  Created by Hugo Lefeuvre, Sept. 09 2017.
  Licensing: See LICENSE file.
*/
#ifndef Rogmap_h
#define Rogmap_h

#include "src/fillmap.h"

class Rogmap
{
  public:
    Rogmap(int width, int height);
    char* as_char_array();
    int get_width();
    int get_height();

  private:
    map_t map;
};

#endif
