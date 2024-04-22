#include <iostream>

#include "asciify.h"

using std::cin, std::cout, std::endl, std::string;

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
    visualize(img);
    TokenizedImage* tk = new TokenizedImage(img, 4, 1);
    cout << tk->to_string() << endl;
    cin.clear();

    cin.ignore(INT_MAX,'\n');
    cout << "Press any key to continue... " << endl;
    cin.ignore();
    return 0;
}

