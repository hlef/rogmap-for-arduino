/*
  rogmap.h - Library for random dungeon generation.
  Created by Hugo Lefeuvre, Sept. 09 2017.
  Licensing: See LICENSE file.
*/
#ifndef Rogmap_h
#define Rogmap_h

#include "fillmap.h"

class Rogmap
{
    public:
        Rogmap(int width, int height);
        char* as_char_array();
        char get_char_at(int x, int y);
        void fill(float min_filling, float max_room_size);
        int get_width();
        int get_height();

    private:
        map_t *map;
};

#endif
