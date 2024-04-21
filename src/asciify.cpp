#include "asciify.h"

void visualize(Image* img){
    for(int y = 0; y < img->height; y+=3) {
        png_bytep row = img->row_pointers[y];
        for(int x = 0; x < img->width; x+=1) {
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

Token::Token(Image* img, int row_start, int col_start, int token_size, int bit_len):
    token_size{token_size}, bit_len{bit_len}, row_start{row_start}, col_start{col_start}
{
    bits_cnt = token_size/bit_len;
    int avg_greyscale = 0;

    bitmap = (int**)malloc(sizeof(int*)*bits_cnt);
    for(int y = 0; y < bits_cnt; y++){
        bitmap[y] = (int*)malloc(sizeof(int)*bits_cnt);
        for(int x = 0; x < bits_cnt; x++){
            
            for(int img_y = 0; img_y < bit_len; img_y++){
                png_bytep row = img->row_pointers[col_start+x*bit_len+img_y];
                for(int img_x = 0; img_x < bit_len; img_x++){
                    png_bytep px = &row[(row_start+y*bit_len+img_x)*4];
                    // Fill 0 for out-of-bound?
                    avg_greyscale += (px[0]*11+px[1]*16+px[2]*5)*px[3];
                }
            }

            bitmap[y][x] = avg_greyscale/8160/bit_len/bit_len;
            
            avg_greyscale = 0;
        }
    }
}

string Token::to_string(){
    string temp = "";
    for(int y = 0; y < bits_cnt; y++){
        for(int x = 0; x < bits_cnt; x++){
            temp += std::to_string(bitmap[y][x]);
            temp += ",";
        }
        temp += "\n";
    }
    return temp;
}

TokenizedImage::TokenizedImage(Image* img, int token_size, int bit_len):
    img{img}, token_size{token_size}, bit_len{bit_len}
{   
    tokens_width = img->width/token_size;
    tokens_height = img->height/token_size;

    tokens = new Token**[tokens_height];
    for(int y = 0; y < tokens_height; y++){
        tokens[y] = new Token*[tokens_width];
        for(int x = 0; x < tokens_width; x++){
            tokens[y][x] = new Token(img, x*token_size, y*token_size, token_size, bit_len); 
        }
    }
}

string TokenizedImage::to_string(){
    string temp = "";
    for(int y = 0; y < tokens_height; y++){
        for(int x = 0; x < tokens_width; x++){
            temp += tokens[y][x]->to_string();
        }
        temp += "\n\n";
    }
    return temp;
}