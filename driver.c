

#include "ppm.h"
#include "EncodeDecode.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/*
Parameters: fp - the file pointer to the ppm file of which you want to print pixels as numeric values
Return: void
This function prints out the r g and b components for every pixel in a ppm file as a number between 0-255
*/
void printPixel(FILE* fp) {
    header_t h=readHeader(fp);
    pixel_t** p=readPixel(fp, h);
    for (int i = 0; i < h.height; i++) {
        for (int j = 0; j < h.width; j++) {
            printf("%d %d %d, ", p[i][j].r, p[i][j].g, p[i][j].b);
        }
        printf("\n");
    }
}

/*
Parameters: h - the header of the ppm file to print
Return: void
This function prints out the values of the members of h
*/
void printHeader(header_t h) {
    printf("%s Width: %d\nHeight: %d\nMAX_VAL: %d\n",h.magicNum, h.width, h.height, h.maxVal);
}

int main(int argc, char* argv[])
{
    checkArgs(argc, 2);
    FILE* in = fopen(argv[1], "r");
    checkFile(in);
    FILE* out = fopen(argv[2], "w");
    checkFile(out);
    //This is my test message.  It can be something different
    char* msg = "I really hope you are reading this in the terminal and no errors occured! Hopefully I did well.";
    encodeMsg(in, out, msg);

    FILE* dec = fopen(argv[2], "r");
    checkFile(dec);
    decodeMsg(dec);

    fclose(in);
    fclose(out);
    fclose(dec);

    return EXIT_SUCCESS;
}


