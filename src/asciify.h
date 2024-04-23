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
    int xbits_cnt, ybits_cnt, lumin;
    int** bitmap;

    Token(Image* img, int xbit_len, int ybit_len, int x_len, int y_len, int x_start, int y_start);
    std::string to_string();
};

class TokenizedImage{
    public:
    Image* img{};
    const int xtoken_cnt{}, ytoken_cnt{}, xbit_len{}, ybit_len{};
    int xtoken_len, ytoken_len;
    Token*** tokens;

    TokenizedImage(Image* img, int xtoken_cnt, int ytoken_cnt, int xbit_len, int ybit_len);
    std::string to_string();
};

// 如果不是8*8倍数不接受？

#endif