#include "fillmap.h"
#include <stdlib.h>
#include <string.h>
#include <Arduboy2.h>

int fill_map(map_t* map, float min_filling, float max_room_size) {
    if (min_filling >= 1 ||
            min_filling <= 0 ||
            max_room_size > 1 ||
            (int)(max_room_size * map->height) < 2 ||
            (int)(max_room_size * map->width) < 2) {
        return -1;
    }

    int map_size = map->width * map->height;

    // Initialize map
    memset(map->elements, CHAR_EMPTY, map_size * sizeof(char));

    coordinate point;

    // Choose initial point at random, make sure it fulfills is_suitable_initial_point
    do {
        point = (coordinate) { .x = (unsigned short) random(0, map->width),
                               .y = (unsigned short) random(0, map->height)};
    } while (!is_suitable_initial_point(map, point));

    // Generation loop
    int i = 0;
    do {
        i += generate_elliptic_room(map, &point, max_room_size);
    } while (i < map_size * min_filling);

    return 0;
}

int add_point(map_t* map, coordinate point) {
    if (ACCESS_XY_IN_ARRAY(map, point.x, point.y) != CHAR_ROOM) {
        ACCESS_XY_IN_ARRAY(map, point.x, point.y) = CHAR_ROOM;
        return 1;
    }
    return 0;
}

int is_suitable_initial_point(map_t* map, coordinate initial_point) {
    if((initial_point.y < 1) || ((map->height - initial_point.y) <= 1) ||
       ((map->width - initial_point.x) <= 1) || (initial_point.x < 1)) {
        return 0;
    }
    return 1;
}

void compute_room_dimensions(map_t* map, coordinate* point, int *height, int *width, float max_room_size_factor) {
    int max_height = min(map->height - point->y, point->y);
    max_height = min(max_height, map->height * max_room_size_factor * 0.5f);

    int max_width = min(map->width - point->x, point->x);
    max_width = min(max_width, map->width * max_room_size_factor * 0.5f);

    *height = random(1, max_height);
    *width = random(1, max_width);
}

/* Generate an elliptic room in passed map around passed point. Return the
   number of points newly marked in the map. */
int generate_elliptic_room(map_t* map, coordinate* point, float max_room_size_factor) {
    int height, width;
    compute_room_dimensions(map, point, &height, &width, max_room_size_factor);

    int squarew = width*width;
    int squareh = height*height;

    // Update map & select next starting point using reservoir sampling
    coordinate current_replacement;
    int i = 0, len = 1;
    for ( int y = -height; y <= height; y++ ) {
        for ( int x = -width; x <= width; x++ ) {
            if ( (x * x * squareh + (y * y) * squarew) <= (squareh * squarew) ) {
                coordinate newpnt = (coordinate) { .x = point->x + x, .y = point->y + y };
                i += add_point(map, newpnt);
                if(is_suitable_initial_point(map, newpnt)){
                    if((random() % len) == 0) {
                        current_replacement = newpnt;
                    }
                    len++;
                }
            }
        }
    }

    *point = current_replacement;
    return i;
}
