#include "load_char.h"
#include "../font8x8-master/font8x8.h"
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
    int map_len = sizeof(font8x8_block)/sizeof(*font8x8_block);
    char_bitmaps = new int**[map_len];
    for(int i = 0; i < map_len; i++){
        char_bitmaps[i] = new int*[8];
        for (int x = 0; x < 8; x++) {
            char_bitmaps[i][x] = new int[8];
            for (int y = 0; y < 8; y++) {
                // 注意xy反转
            }
        }
    }
}