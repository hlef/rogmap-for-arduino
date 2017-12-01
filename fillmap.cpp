#include "fillmap.h"
#include <stdlib.h>
#include <string.h>

#if defined(ARDUBOY_PLATFORM)
    #include <Arduboy2.h>
#elif defined(ARDUINO_PLATFORM)
    #include <Arduino.h>
#else
    #include <algorithm>

    using std::min;

    int random(int min, int max) {
        if(min >= max) {
            return min;
        }
        return random() % (max - min) + min;
    }
#endif

static const int chunk_size_in_bits = sizeof(chunk_t) * 8;

bool is_valid_coordinate(map_t* map, int x, int y) {
    if (x < 0 || x > map->width || y < 0 || y > map->height) {
        return false;
    }
    return true;
}

int get_chunk_offset(map_t* map, int x, int y) {
    return y * map->chunked_width + roundDown(x, chunk_size_in_bits) / chunk_size_in_bits;
}

int get_bit_offset(map_t* map, int x) {
    return x % chunk_size_in_bits;
}

int get_at_xy(map_t* map, int x, int y) {
    if (!is_valid_coordinate(map, x, y)) {
        return -1;
    }

    int chunk_offset = get_chunk_offset(map, x, y);
    int bit_offset   = get_bit_offset(map, x);

    int element;
    switch (bit_offset) {
        case 0:
            element = map->chunks[chunk_offset].e1;
            break;
        case 1:
            element = map->chunks[chunk_offset].e2;
            break;
        case 2:
            element = map->chunks[chunk_offset].e3;
            break;
        case 3:
            element = map->chunks[chunk_offset].e4;
            break;
        case 4:
            element = map->chunks[chunk_offset].e5;
            break;
        case 5:
            element = map->chunks[chunk_offset].e6;
            break;
        case 6:
            element = map->chunks[chunk_offset].e7;
            break;
        case 7:
            element = map->chunks[chunk_offset].e8;
            break;
        default:
            element = -1;
    }

    return element;
}

int set_at_xy(map_t* map, int x, int y, int to_set) {
    if (!is_valid_coordinate(map, x, y)) {
        return -1;
    }

    int chunk_offset = get_chunk_offset(map, x, y);
    int bit_offset   = get_bit_offset(map, x);

    switch (bit_offset) {
        case 0:
            map->chunks[chunk_offset].e1 = to_set;
            break;
        case 1:
            map->chunks[chunk_offset].e2 = to_set;
            break;
        case 2:
            map->chunks[chunk_offset].e3 = to_set;
            break;
        case 3:
            map->chunks[chunk_offset].e4 = to_set;
            break;
        case 4:
            map->chunks[chunk_offset].e5 = to_set;
            break;
        case 5:
            map->chunks[chunk_offset].e6 = to_set;
            break;
        case 6:
            map->chunks[chunk_offset].e7 = to_set;
            break;
        case 7:
            map->chunks[chunk_offset].e8 = to_set;
            break;
        default:
            return -1;
    }

    return 0;
}

unsigned int roundUp(unsigned int value, unsigned int roundTo)
{
    return (value + (roundTo - 1)) & ~(roundTo - 1);
}

unsigned int roundDown(unsigned int value, unsigned int roundTo)
{
    return (value / roundTo) * value;
}

int fill_map(map_t* map, float min_filling, float max_room_size) {
    if (min_filling >= 1 ||
            min_filling <= 0 ||
            max_room_size > 1 ||
            (int)(max_room_size * map->height) < 2 ||
            (int)(max_room_size * map->width) < 2) {
        return -1;
    }

    int map_size = map->width * map->height;

    coordinate point;

    // Choose initial point at random, make sure it fulfills is_suitable_initial_point
    do {
        point = (coordinate) { .x = (int) random(0, map->width),
                               .y = (int) random(0, map->height)};
    } while (!is_suitable_initial_point(map, point));

    // Generation loop
    int i = 0;
    do {
        i += generate_elliptic_room(map, &point, max_room_size);
    } while (i < map_size * min_filling);

    return 0;
}

int add_point(map_t* map, coordinate point) {
    if (get_at_xy(map, point.x, point.y) != CHAR_ROOM) {
        set_at_xy(map, point.x, point.y, CHAR_ROOM);
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
    int max_height = min(map->height - point->y, (int) point->y);
    max_height = min(max_height, (int) (map->height * max_room_size_factor * 0.5f));

    int max_width = min(map->width - point->x, (int) point->x);
    max_width = min(max_width, (int) (map->width * max_room_size_factor * 0.5f));

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
