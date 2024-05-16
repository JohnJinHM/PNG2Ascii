#include "asciify.h"
#include "load_char.h"
#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>

// void visualize(Image* img){
//     for(int y = 0; y < img->height; y+=3) {
//         png_bytep row = img->row_pointers[y];
//         for(int x = 0; x < img->width; x+=1) {
//             png_bytep px = &(row[x * 4]);
//             // Do something awesome for each pixel here...
//             // printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
//             if(px[3]==0) printf(" ");
//             else printf("-");
//         }
//         printf("\n");
//     }
// }


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

    xbits_cnt = x_len/xbit_len;
    ybits_cnt = y_len/ybit_len;
    gs = 0;

    bitmap = new double*[ybits_cnt];
    for(int y = 0; y < ybits_cnt; y++){
        bitmap[y] = new double[xbits_cnt];
        for(int x = 0; x < xbits_cnt; x++){
            // Fill 0 for out-of-bound
            bitmap[y][x] = 0;
            for(int img_y = 0; img_y < ybit_len; img_y++){
                y_pos = y_start+y*ybit_len+img_y;
                png_bytep row = img->row_pointers[y_pos];
                for(int img_x = 0; img_x < xbit_len; img_x++){
                    x_pos = x_start+x*xbit_len+img_x;
                    if(x_pos >= x_max || y_pos >= y_max){
                        // Padding pixel
                    }
                    else{
                        png_bytep px = &row[x_pos*4];
                        // (R * 11 + G * 16 + B * 5)/32 formula for Grey Scale
                        // TODO: Change bitmap algorithm, current too low
                        bitmap[y][x] += (px[0]*11+px[1]*16+px[2]*5)*px[3];
                    }
                }
            }
            bitmap[y][x] = bitmap[y][x]/xbit_len/ybit_len;
            gs += bitmap[y][x];
        }
    }
    gs = gs/xbits_cnt/ybits_cnt/32/255;

    normalize_bitmap();
}

void Token::normalize_bitmap(){
    // Softmax doesn't work, try sth else
    double m = -INFINITY;
    for (size_t y = 0; y < 8; y++) {
        for(size_t x = 0; x < 8; x++){
            if (bitmap[y][x] > m) {
                m = bitmap[y][x];
            }
        }
    }

    double sum = 0.0;
    for (size_t y = 0; y < 8; y++) {
        for(size_t x = 0; x < 8; x++){
            sum += expf(bitmap[y][x] - m);
        }
    }

    double offset = m + logf(sum);
    for (size_t y = 0; y < 8; y++) {
        for(size_t x = 0; x < 8; x++){
            bitmap[y][x] = expf(bitmap[y][x] - offset);
            printf("%f\n",bitmap[y][x]);
        }
    }
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

TokenizedImage::TokenizedImage(Image* _img, int _xtoken_cnt, int _ytoken_cnt, int _mode):
    img{_img}, xtoken_cnt{_xtoken_cnt}, ytoken_cnt{_ytoken_cnt}, mode{_mode}
{   
    switch (mode){
        case 0:
            xbit_len = 1;
            ybit_len = 1;
            //Prevent cropping
            xtoken_len = img->width/xtoken_cnt+1;
            ytoken_len = img->height/ytoken_cnt+1;
            
            gs = new double[xtoken_cnt*ytoken_cnt];
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
            xbit_len = img->width/xtoken_cnt/8;
            ybit_len = img->height/ytoken_cnt/8;
            // 8*8 bitmap per token
            xtoken_len = xbit_len*8;
            ytoken_len = ybit_len*8;
            // Prevent cropping
            xtoken_cnt = img->width/xtoken_len;
            ytoken_cnt = img->height/ytoken_len;

            tokens = new Token**[ytoken_cnt];
            for(int y = 0; y < ytoken_cnt; y++){
                tokens[y] = new Token*[xtoken_cnt];
                for(int x = 0; x < xtoken_cnt; x++){
                    tokens[y][x] = new Token(img, xbit_len, ybit_len, xtoken_len, ytoken_len, x*xtoken_len, y*ytoken_len); 
                }
            }
            break;
    }

}

std::string TokenizedImage::to_string(){
    std::string temp = "";
    CharMaps* char_maps = new CharMaps();
    
    switch (mode){
        case 0:
            map_brightness(char_maps->lumin_non_empty, char_maps->lumin_len);
            for(int y = 0; y < ytoken_cnt; y++){
                for(int x = 0; x < xtoken_cnt; x++){
                    double min_diff = 255.0;
                    int best_match = 0;
                    double curr_diff = 0.0;
                    Token curr = *tokens[y][x];
                    if(curr.gs==0) temp+=" ";
                    else{
                        double curr_lumin = lumin_mean+(curr.gs-gs_mean)*lumin_sd/gs_sd;
                        // printf("lm:%d\n", curr_lumin);
                        for(int i = 0; i < char_maps->map_len; i++){
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
            for(int y = 0; y < ytoken_cnt; y++){
                for(int x = 0; x < xtoken_cnt; x++){
                    double min_diff = 255.0;
                    int best_match = 0;
                    double curr_diff = 0.0;
                    Token curr = *tokens[y][x];
                    for(int i = 32; i < char_maps->map_len; i++){
                        for(int _y = 0; _y < 8; _y++){
                            for(int _x = 0; _x < 8; _x++){
                                curr_diff += std::abs(tokens[y][x]->bitmap[_y][_x]-char_maps->char_bitmaps[i][_x][_y]);
                            }
                        }
                        curr_diff /= 64;
                        // printf("%f\n", curr_diff);
                        if(curr_diff < min_diff){
                            min_diff = curr_diff;
                            best_match = i;
                        }
                    }
                    temp += char_maps->chars[best_match];
                }
                temp += "\n";
            }
            break;
    }
    return temp;
}

void TokenizedImage::map_brightness(double* lumin_array, int lumin_len){
    lumin_mean = std::accumulate(lumin_array,&lumin_array[0]+lumin_len,0.0)/lumin_len;
    double lumin_sq_sum = std::inner_product(lumin_array,&lumin_array[0]+lumin_len,lumin_array,0.0);
    lumin_sd = std::sqrt(lumin_sq_sum / lumin_len - lumin_mean * lumin_mean);

    gs_mean = std::accumulate(gs,&gs[0]+gs_cnt,0.0)/gs_cnt;
    double gs_sq_sum = std::inner_product(gs,&gs[0]+gs_cnt,gs,0.0);
    gs_sd = std::sqrt(gs_sq_sum / gs_cnt - gs_mean * gs_mean);
}