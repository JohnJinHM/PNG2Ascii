#ifndef LOAD_CHAR_H
#define LOAD_CHAR_H

class CharMaps{
    public:
    int map_len;
    char* chars;
    int* lumin;
    int* lumin_non_empty;
    int lumin_len;
    int*** char_bitmaps;

    CharMaps();
};

#endif