#include <png.h>

class Image{
    public:
    int width, height;
    png_bytep *row_pointers;
    char* filename{};
    
    Image(char* filename);
    void read_image();
};
