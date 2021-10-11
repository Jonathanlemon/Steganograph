

#ifndef PPM_H
#define PPM_H
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}pixel_t;

typedef struct
{
    char magicNum[3];
    int width;
    int height;
    int maxVal;
}header_t;

header_t readHeader(FILE*);
pixel_t** readPixel(FILE*, header_t);
void writeHeader(FILE*, header_t);
void writePixels(FILE*, pixel_t**, header_t);
void ckws_comments (FILE *);
void freeMemory(pixel_t**, header_t);

void checkForComments(FILE*);

#endif
