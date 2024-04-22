#ifndef ASCIIFY_H
#define ASCIIFY_H

#include <string>
#include "read_img.h"
#include "load_char.h"

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
    const int token_size{}, bit_len{}, row_start{}, col_start{};
    int bits_cnt, lumin;
    int** bitmap;

    Token(Image* img, int row_end, int col_end, int token_size, int bit_num);
    std::string to_string();
};

class TokenizedImage{
    public:
    Image* img{};
    const int token_size{}, bit_len{};
    int tokens_width, tokens_height;
    Token*** tokens;

    TokenizedImage(Image* img, int token_size, int bit_len);
    std::string to_string();
};

// 如果不是8*8倍数不接受？

#endif