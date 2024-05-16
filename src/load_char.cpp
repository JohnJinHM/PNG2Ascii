#include "load_char.h"
#include "../font8x8-master/font8x8.h"
#include <string>

// void render(unsigned char *bitmap) {
//     int x,y;
//     int set;
//     int mask;
//     for (x=0; x < 8; x++) {
//         for (y=0; y < 8; y++) {
//             set = bitmap[x] & 1 << y;
//             printf("%c", set ? 'X' : ' ');
//         }
//         printf("\n");
//     }
// }

CharMaps::CharMaps(){
    // Use font8x8_basic only (contains all ascii chars)
    map_len = sizeof(font8x8_basic)/sizeof(*font8x8_basic);

    lumin = new double[map_len];
    lumin_non_empty = new double[map_len];
    lumin_len = 0;
    
    chars = new char[map_len];
    char_bitmaps = new int**[map_len];

    int set = 0;

    for(int i = 0; i < map_len; i++){
        char_bitmaps[i] = new int*[8];
        lumin[i] = 0;
        for (int x = 0; x < 8; x++) {
            char_bitmaps[i][x] = new int[8];
            for (int y = 0; y < 8; y++) {
                set = font8x8_basic[i][x] & 1 << y;
                if(set){
                    // Beware of remapping x and y
                    char_bitmaps[i][x][y] = 1;
                    lumin[i]++;
                }
                else{
                    char_bitmaps[i][x][y] = 0;
                }
            }
        }
        lumin[i] = lumin[i]*255/64;
        if(lumin[i] == 0) chars[i] = ' ';
        else{
            chars[i] = (char) i;
            lumin_non_empty[lumin_len] = lumin[i];
            lumin_len++;
        }
    }
}