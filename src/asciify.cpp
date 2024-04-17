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


// Token::Token(int* bits){
//     for(int i = 0; i < len; i++){
//         for(int j = 0; j < len; j++){
//             int bit = bits[i*len+j];
//             bitmap[i][j] = bit;
//             lumin += bit;
//         }
//     }
//     lumin = lumin >> 6; // divide by 64
// }

Token::Token(png_bytep* bytes, int token_size, int bit_len):
    token_size{token_size}, bit_len{bit_len}
{
    bit_cnt = token_size / bit_len; // Length of bitmap row/col, %8=0 for bitmap comparison
    for(int i = 0; i < token_size; i+=bit_len){
        for(int j = 0; j < token_size; j+=bit_len){
            // TODO
            // 怎么将pixels压缩为bits?
        }
    }

}

TokenizedImage::TokenizedImage(Image* img, int token_size, int bit_len):
    img{img}, token_size{token_size}, bit_len{bit_len}
{
    tokens_width = img->width/token_size+1;
    tokens_height = img->height/token_size+1;
    tokens = (Token**)malloc(sizeof(Token*)*tokens_height);
    for(int y = 0; y < tokens_height; y++){
        tokens[y] = (Token*)malloc(sizeof(Token)*tokens_width);
        // TODO
        // 分割img，新建token
    }

}