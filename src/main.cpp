#include <iostream>

#include "asciify.h"

using std::cin, std::cout, std::endl, std::string;

int app_exit(int arg){
    cin.ignore(INT_MAX,'\n');
    cout << "Press any key to continue... " << endl;
    cin.ignore();
    exit(arg);
}

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
    // visualize(img);
    
    int img_x = img->width;
    int img_y = img->height;
    printf("Input size: %d * %d pixels\n", img_x, img_y);
    
    int mode = 0;
    cout << "Enter asciify mode (0 for GreyScale, 1 for BitMap(WIP)): " << endl;
    cin >> mode;
    
    int xtoken_cnt, ytoken_cnt;
    cout << "Enter expected output width: " << endl;
    cin >> xtoken_cnt;
    cout << "Enter expected output height (Enter 0 to use default): " << endl;
    cin >> ytoken_cnt;

    if(ytoken_cnt == 0){
        ytoken_cnt = xtoken_cnt/2.5; // Magic number, depend on font y vs. x 
    }

    // Image check
    switch (mode){
        case 0:
            if(xtoken_cnt>img_x||ytoken_cnt>img_y){
                cout << "Token length out of bound" << endl;
                app_exit(1);
            }
            break;
        case 1:
            if(xtoken_cnt*8>img_x||ytoken_cnt*8>img_y){
                cout << "Token length out of bound" << endl;
                app_exit(1);
            }
            break;
    }


    TokenizedImage* tk = new TokenizedImage(img,xtoken_cnt,ytoken_cnt,mode);
    
    cout << tk->to_string() << endl;
    cin.clear();

    app_exit(0);
}