/*************************
 *Jonathan Lemon
 *CPSC 2310 F21 Section 001
 *jtlemon@clemson.edu
 *************************/

#include "ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
Parameters: fp - the file pointer of a ppm file to be read
Return: h - the header struct with values set to those read in by fp
This function reads in and stores the header values from fp and returns the values in a struct
*/
header_t readHeader(FILE* fp) {
	header_t h;
	int c;
	fgets(h.magicNum, sizeof(h.magicNum), fp);
	//Ensure P6 file is provided
	if (h.magicNum[0] != 'P' || h.magicNum[1] != '6') {
		fprintf(stderr, "Invalid image format (must be 'P6')\n");
		exit(1);
	}
	//Check for comments
	ckws_comments(fp);
	//Ensure valid dimensions
	if (fscanf(fp, "%d %d", &h.width, &h.height) != 2) {
		fprintf(stderr, "Image dimensions off: %c %c\n", h.width, h.height);
		exit(1);
	}
	//Check for comments
	ckws_comments(fp);
	//Ensure valid rgb component
	if (fscanf(fp, "%d", &h.maxVal) != 1) {
		fprintf(stderr, "Bad rgb max_val value\n");
		exit(1);
	}
	//Check for comments
	ckws_comments(fp);
	return h;
}

/*
Parameters: fp - the file pointer of a ppm file to be read
	    h - the header of the ppm file
Return: grid - A 2d array of the pixels in the ppm file with their r, g, and b components separated as a struct
This function reads in and stores the pixel values from fp and returns the values in a struct
*/
pixel_t** readPixel(FILE* fp, header_t h) {
	//Allocate memory for entire grid
	pixel_t** grid = (pixel_t**)(malloc(h.height * sizeof(pixel_t*)));
	for (int i = 0; i < h.height; i++) {
		//Loop through and allocate memory for each row based on number of columns
		grid[i] = (pixel_t*)(malloc(h.width * sizeof(pixel_t)));
	}
	for (int i = 0; i < h.height; i++) {
		for (int j = 0; j < h.width; j++) {
			//Loop through and record every pixel
			fscanf(fp, "%c%c%c", &(grid[i][j].r), &(grid[i][j].g), &(grid[i][j].b));
		}
	}
	return grid;
}

/*
Parameters: fp - the file pointer of a ppm file to be read
	    h - the header of the ppm file
Return: void
This function writes the header h to the file fp
*/
void writeHeader(FILE* fp, header_t h) {
	//Write out header based on h parameter
	fprintf(fp, "%s\n%d %d\n%d\n", h.magicNum, h.width, h.height, h.maxVal);
}

/*
Parameters: fp - the file pointer of a ppm file to be read
	    h - the header of the ppm file
Return: void
This function writes the pixel values in p to the file fp, such that it fills the rows and columns with the correct r g and b values
*/
void writePixels(FILE* fp, pixel_t** p, header_t h) {
	int rows = h.height;
	int columns = h.width;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			//Loop through each pixel and write the char
			fprintf(fp, "%c%c%c", p[i][j].r, p[i][j].g, p[i][j].b);
		}
	}
	//Deallocate the 2d array of pixels after writing, it has no use after this function so it must be freed
	freeMemory(p, h);
}

/*
Parameters: fp - the file pointer of a ppm file to be read
Return: void
This function reads past any comments in the ppm file
*/
void ckws_comments(FILE* fp) {
	char c = getc(fp);
	if(c=='\n'){
		c = getc(fp);
	}
	//Loop through if character is #, eating each following character until the newline is reached
	while (c == '#') {
		while (c != '\n'){
			c = getc(fp);
		}
	}
	//Reset file pointer at newline
	ungetc(c, fp);
}

/*
Parameters: p - the 2d pointer of pixels
	    h - the header to organize the pixels in p
Return: void
This function frees the memory allocated for p
*/

void freeMemory(pixel_t** p, header_t h) {
	for (int i = 0; i < h.height; i++) {
		//Loop through and deallocate memory for each row based on number of columns
		free(p[i]);
	}
	free(p);
}
