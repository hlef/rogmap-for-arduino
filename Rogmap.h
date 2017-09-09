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
    void fill(float min_filling, float max_room_size);
    int get_width();
    int get_height();

  private:
    map_t map;
};

#endif
