#include "asciify.h"
#include "load_char.h"


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

Token::Token(Image* img, int xbit_len, int ybit_len, int x_len, int y_len, int x_start, int y_start):
    xbit_len{xbit_len}, ybit_len{ybit_len}, x_len{x_len}, y_len{y_len}, x_start{x_start}, y_start{y_start}
{
    xbits_cnt = x_len/xbit_len;
    ybits_cnt = y_len/ybit_len;
    int avg_greyscale = 0;
    lumin = 0;

    bitmap = new int*[ybits_cnt];
    for(int y = 0; y < ybits_cnt; y++){
        bitmap[y] = new int[xbits_cnt];
        for(int x = 0; x < xbits_cnt; x++){
            
            for(int img_y = 0; img_y < ybit_len; img_y++){
                png_bytep row = img->row_pointers[y_start+y*ybit_len+img_y];
                for(int img_x = 0; img_x < xbit_len; img_x++){
                    png_bytep px = &row[(x_start+x*xbit_len+img_x)*4];
                    // Fill 0 for out-of-bound?
                    // (R * 11 + G * 16 + B * 5)/32
                    avg_greyscale += (px[0]*11+px[1]*16+px[2]*5)*px[3];
                }
            }
            
            avg_greyscale = avg_greyscale/8160/xbit_len/ybit_len; //255*32
            lumin += avg_greyscale;
            bitmap[y][x] = avg_greyscale; 
            
            avg_greyscale = 0;
        }
    }
    lumin = lumin/xbits_cnt/ybits_cnt; // throws error when token size too small
}

// std::string Token::to_string(){
    // std::string temp = "";
    

    // for(int y = 0; y < bits_cnt; y++){
    //     for(int x = 0; x < bits_cnt; x++){
    //         temp += std::to_string(bitmap[y][x]);
    //         temp += ",";
    //     }
    //     temp += "\n";
    // }
    // return temp;
// }

TokenizedImage::TokenizedImage(Image* img, int xtoken_cnt, int ytoken_cnt, int xbit_len, int ybit_len):
    img{img}, xtoken_cnt{xtoken_cnt}, ytoken_cnt{ytoken_cnt}, xbit_len{xbit_len}, ybit_len{ybit_len}
{   
    xtoken_len = img->width/xtoken_cnt;
    ytoken_len = img->height/ytoken_cnt;

    tokens = new Token**[ytoken_cnt];
    for(int y = 0; y < ytoken_cnt; y++){
        tokens[y] = new Token*[xtoken_cnt];
        for(int x = 0; x < xtoken_cnt; x++){
            tokens[y][x] = new Token(img, xbit_len, ybit_len, xtoken_len, ytoken_len, x*xtoken_len, y*ytoken_len); 
        }
    }
}

std::string TokenizedImage::to_string(){
    CharMaps* char_maps = new CharMaps();
    std::string temp = "";
    for(int y = 0; y < ytoken_cnt; y++){
        for(int x = 0; x < xtoken_cnt; x++){
            int min_diff = 255;
            int best_match = 0;
            int curr_diff = 0;
            Token curr = *tokens[y][x];
            for(int i = 0; i < char_maps->map_len; i++){
                curr_diff = std::abs(char_maps->lumin[i]-curr.lumin);
                if(curr_diff < min_diff){
                    min_diff = curr_diff;
                    best_match = i;
                }
            }
            temp += char_maps->chars[best_match];
        }
        temp += "\n";
    }
    return temp;
}
