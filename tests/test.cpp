#include "../Rogmap.h"
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define WIDTH  10
#define HEIGHT 10

int main() {
    srand(time(NULL));
    Rogmap map = Rogmap(10, 10);

    // Test 1 -- size is correct

    int width = map.get_width();
    assert(width == WIDTH);
    int height = map.get_height();
    assert(height == HEIGHT);

    std::cout << "[v] TEST 1 (size is correct)" << std::endl;

    // Test 2 -- before fill, map is empty

    char* array = map.as_char_array();
    for ( int i = 0; i < height*width; i++ ) {
        assert(array[i] == map.char_empty);
    }

    std::cout << "[v] TEST 2 (map is empty before fill)" << std::endl;

    // Test 3 -- map doesn't contain invalid characters

    array = map.as_char_array();

    map.fill(0.4f, 0.5f);
    array = map.as_char_array();

    std::cout << "After fill:" << std::endl;
    for ( int i = 0; i < height*width; i++ ) {
        assert(array[i] == map.char_empty || array[i] == map.char_room);
        std::cout << array[i];
        if ( (i+1) % width == 0 ) {
            std::cout << std::endl;
        }
    }

    std::cout << "[v] TEST 3 (map doesn't contain invalid characters)" << std::endl;

    // Test 4 -- re-fill works

    char* array2 = map.as_char_array();
    bool diff = false;

    map.fill(0.2f, 0.7f);
    array2 = map.as_char_array();

    std::cout << "After fill:" << std::endl;
    for ( int i = 0; i < height*width; i++ ) {
        assert(array2[i] == map.char_empty || array2[i] == map.char_room);
        if (array[i] != array2[i]) {
            diff = true;
        }
        std::cout << array2[i];
        if ( (i+1) % width == 0 ) {
            std::cout << std::endl;
        }
    }

    assert(diff);

    std::cout << "[v] TEST 4 (re-fill works)" << std::endl;
    return 0;
}
