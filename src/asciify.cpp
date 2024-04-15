#include "asciify.h"

void visualize(Image* img){
    for(int y = 0; y < img->height; y+=30) {
        png_bytep row = img->row_pointers[y];
        for(int x = 0; x < img->width; x+=10) {
            png_bytep px = &(row[x * 4]);
            // Do something awesome for each pixel here...
            // printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
            if(px[3]==0) printf(" ");
            else printf("-");
        }
        printf("\n");
    }
}