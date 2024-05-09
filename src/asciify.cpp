#include "asciify.h"
#include "load_char.h"
#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>

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
    int x_pos, y_pos;
    int x_max = img->width;
    int y_max = img->height;
    int avg_gs = 0;;

    xbits_cnt = x_len/xbit_len;
    ybits_cnt = y_len/ybit_len;    
    gs = 0;

    bitmap = new int*[ybits_cnt];
    for(int y = 0; y < ybits_cnt; y++){
        bitmap[y] = new int[xbits_cnt];
        for(int x = 0; x < xbits_cnt; x++){
            
            for(int img_y = 0; img_y < ybit_len; img_y++){
                y_pos = y_start+y*ybit_len+img_y;
                png_bytep row = img->row_pointers[y_pos];
                for(int img_x = 0; img_x < xbit_len; img_x++){
                    x_pos = x_start+x*xbit_len+img_x;
                    png_bytep px = &row[x_pos*4];

                    // Fill 0 for out-of-bound
                    if(x_pos >= x_max || y_pos >= y_max){
                        // Padding pixel
                    }
                    else{
                        // (R * 11 + G * 16 + B * 5)/32
                        // Currently only represent bright color
                        avg_gs += (px[0]*11+px[1]*16+px[2]*5)*px[3];
                        // printf("%d,%d,%d,%d,", px[0],px[1],px[2],px[3]);
                    }

                    
                }
            }
            
            avg_gs = avg_gs/8160/xbit_len/ybit_len; //255*32
            gs += avg_gs;
            bitmap[y][x] = avg_gs; 
            
            avg_gs = 0;
        }
    }
    gs = gs/xbits_cnt/ybits_cnt; // throws error when token size too small
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

TokenizedImage::TokenizedImage(Image* img, int xtoken_cnt, int ytoken_cnt, int mode):
    img{img}, xtoken_cnt{xtoken_cnt}, ytoken_cnt{ytoken_cnt}, mode{mode}
{   
    switch (mode){
        case 0:
            xbit_len = 1;
            ybit_len = 1;
            //Prevent cropping
            xtoken_len = img->width/xtoken_cnt+1;
            ytoken_len = img->height/ytoken_cnt+1;
            
            gs = new int[xtoken_cnt*ytoken_cnt];
            gs_cnt = 0;

            tokens = new Token**[ytoken_cnt];
            for(int y = 0; y < ytoken_cnt; y++){
                tokens[y] = new Token*[xtoken_cnt];
                for(int x = 0; x < xtoken_cnt; x++){
                    tokens[y][x] = new Token(img, xbit_len, ybit_len, xtoken_len, ytoken_len, x*xtoken_len, y*ytoken_len); 
                    if(tokens[y][x]->gs!=0){
                        gs[gs_cnt] = tokens[y][x]->gs;
                        gs_cnt++;
                    }
                }
            }
            break;

        case 1:
            xtoken_len = 8;
            ytoken_len = 8;
            xbit_len = img->width/xtoken_cnt/xtoken_len;
            ybit_len = img->height/ytoken_cnt/ytoken_len;
            // Prevent cropping
            xtoken_cnt = img->width/xbit_len/xtoken_len;
            ytoken_cnt = img->height/ybit_len/ytoken_len;
            break;
    }

}

std::string TokenizedImage::to_string(){
    std::string temp = "";
    CharMaps* char_maps = new CharMaps();
    switch (this->mode){
        case 0:
            map_brightness(char_maps->lumin_non_empty, char_maps->lumin_len);
            
            for(int y = 0; y < ytoken_cnt-1; y++){
                for(int x = 0; x < xtoken_cnt; x++){
                    int min_diff = 255;
                    int best_match = 0;
                    int curr_diff = 0;
                    Token curr = *tokens[y][x];
                    if(curr.gs==0) temp+=" ";
                    else{
                        int curr_lumin = lumin_mean+(curr.gs-gs_mean)*lumin_sd/gs_sd;
                        // printf("lm:%d\n", curr_lumin);
                        for(int i = 0; i < char_maps->map_len-1; i++){
                            curr_diff = std::abs(char_maps->lumin[i]-curr_lumin);
                            if(curr_diff < min_diff){
                                min_diff = curr_diff;
                                best_match = i;
                            }
                        }
                        temp += char_maps->chars[best_match];
                    }
                }
                temp += "\n";
            }
            break;
        case 1:
            break;
    }
    return temp;
}

void TokenizedImage::map_brightness(int* lumin_array, int lumin_len){
    lumin_mean = std::accumulate(lumin_array,&lumin_array[0]+lumin_len,0.0)/lumin_len;
    double lumin_sq_sum = std::inner_product(lumin_array,&lumin_array[0]+lumin_len,lumin_array,0.0);
    lumin_sd = std::sqrt(lumin_sq_sum / lumin_len - lumin_mean * lumin_mean);

    gs_mean = std::accumulate(gs,&gs[0]+gs_cnt,0.0)/gs_cnt;
    double gs_sq_sum = std::inner_product(gs,&gs[0]+gs_cnt,gs,0.0);
    gs_sd = std::sqrt(gs_sq_sum / gs_cnt - gs_mean * gs_mean);
}