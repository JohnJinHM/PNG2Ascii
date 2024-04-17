#include "read_img.h"
#include <iostream>
#include <string>
using namespace std;

// 计划：
// Tokenize, 给定字符长度，将图片等分
// 亮度匹配 (block) / 形状匹配 (XOR sum)
// string asciify(Image* img);
void visualize(Image* img);

// 亮度：RGB*A/3
// 先取average亮度
class Token{
    public:
    const int token_size{}, bit_len{};
    int bit_cnt, lumin;
    int** bitmap;

    Token(png_bytep* bytes, int token_size, int bit_len);
};

class TokenizedImage{
    public:
    Image* img{};
    const int token_size{}, bit_len{};
    int tokens_width, tokens_height;
    Token **tokens;

    TokenizedImage(Image* img, int token_size, int bit_len);
};

// Process:
// 传入：img, token尺寸（最小8*8-形状 1*1亮度）
// 如果不是8*8倍数不接受？