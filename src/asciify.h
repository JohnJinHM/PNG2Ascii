#ifndef ASCIIFY_H
#define ASCIIFY_H

#include <string>
#include "read_img.h"

using std::string;
// 计划：
// Tokenize, 给定字符长度，将图片等分
// 亮度匹配 (block) / 形状匹配 (XOR sum)
// string asciify(Image* img);
void visualize(Image* img);

// 亮度：RGB*A/3
// 先取average亮度
class Token{
    public:
    // x/ybit_len: Number of pixels for each bit
    // x/y_len: Number of pixels for each token
    // x/ybits_cnt: Count of bits in each token
    const int xbit_len{}, ybit_len{}, x_len{}, y_len{}, x_start{}, y_start{};
    int xbits_cnt, ybits_cnt, gs;
    double** bitmap;

    Token(Image* img, int xbit_len, int ybit_len, int x_len, int y_len, int x_start, int y_start);
    // std::string to_string();
};

class TokenizedImage{
    public:
    Image* img{};
    Token*** tokens;

    int xtoken_cnt{}, ytoken_cnt{}, mode{};
    int xtoken_len, ytoken_len;
    int xbit_len, ybit_len;

    double* gs;
    int gs_cnt;
    double gs_mean, gs_sd, lumin_mean, lumin_sd;
    
    TokenizedImage(Image* img, int xtoken_cnt, int ytoken_cnt, int mode);
    std::string to_string();
    void map_brightness(double* lumin_array, int lumin_len);
    void normalize_bitmap(Token* token);
};

#endif