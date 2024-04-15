#include <iostream>

#include "../font8x8-master/font8x8_basic.h"
#include "read_img.h"

using namespace std;

void render(unsigned char *bitmap) {
    int x,y;
    int set;
    int mask;
    for (x=0; x < 8; x++) {
        for (y=0; y < 8; y++) {
            set = bitmap[x] & 1 << y;
            printf("%c", set ? 'X' : ' ');
        }
        printf("\n");
    }
}

int main(){
    // unsigned char letter;
    // cout << "Enter a letter: ";
    // cin >> letter;

    // unsigned char *bitmap = font8x8_basic[letter];
    
    // render(bitmap);

    string filename;
    cout << "Enter the path to image: " << endl;
    cin >> filename;
    Image* img = new Image(&filename[0]);
    img->process_png_file();
    cin.clear();

    cin.ignore(INT_MAX,'\n');
    cin.ignore();
    return 0;
}

