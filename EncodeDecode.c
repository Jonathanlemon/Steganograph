/*************************
 *Jonathan Lemon
 *CPSC 2310 F21 Section 001
 *jtlemon@clemson.edu
 *************************/

#include "EncodeDecode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Parameters: c - the char to convert
	    b - the pointer to the binary buffer
Return: void
This function converts c to its binary representation and stores it in b
*/
void charToBinary(char c, int* b){
	//Set all values of every int equal to zero, I thought c did this by default, but I had issues with the memory locations having some non-zero value
	b[0] = 0;
	b[1] = 0;
	b[2] = 0;
	b[3] = 0;
	b[4] = 0;
	b[5] = 0;
	b[6] = 0;
	b[7] = 0;
	b[8] = 0;
	//Number to start dividing the char by
	int div = 128;
	//index in the binary buffer
	int counter = 1;
	//Convert decimal value to binary by subtracting by powers of 2, and putting a 1 at each corresponding bit
	while (c > 0) {
		//If the char can be divided by this power of 2, add a bit and update the value
		if (c >= div) {
			b[counter] = 1;
			c -= div;
		}
		//Scale the divisor down by a factor of 2 every iteration
		div /= 2;
		//Increment the index
		counter++;
	}
}

/*
Parameters: c - the int to convert
	    b - the pointer to the binary buffer
Return: void
This function converts c to its binary representation and stores it in b
*/
void dec2bin(int* b, int c) {
	//Use charToBinary since we know b is 8 bits and cannot excede therefore c can be represented as a char
	char ch = c;
	charToBinary(ch, b);
}

/*
Parameters: b - the pointer to a binary buffer to be converted to a character
Return: c - the character representation of the bit stream in b
This function converts the bitstream b to a character and returns the character
*/
unsigned char binToCharacter(int * b) {
	int index = 8;
	int val = 0;
	int mult = 1;
	//Convert binary to decimal by using addition with powers of 2
	while (index >= 0) {
		if (b[index] == 1) {
			val += mult;
		}
		index--;
		mult *= 2;
	}
	unsigned char c = val;
	return c;
}

/*
Parameters: p - 2d array of pixels to be modified
	    h - ppm header of corresponding pixels
Return: void
This function removes the last digit of every pixels r g and b values, and sets them to 0
*/
void removeDigit(pixel_t** p, header_t h) {
	int rows = h.height;
	int columns = h.width;
	//Loop through every pixel triplet and divide by 10 and multiply by 10 to set last digit to 0
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			p[i][j].r -= (p[i][j].r % 10);
			p[i][j].g -= (p[i][j].g % 10);
			p[i][j].b -= (p[i][j].b % 10);
		}
	}
}

/*
Parameters: headIn - header of a ppm file
	    row - pointer to integer to update the row value
	    column - pointer to integer to update the column
Return: This function returns 0 if it is successful and produces a valid result and 1 if the row value grows too large for the header
This function increments column and/Or row by 1 to increment the pixel being indexed in encodeMsg()
*/
int incrementPix(header_t headIn, int* row, int* column){
	*column+=1;
	//If column gets too large, set it to 0 and increment row by 1
	if (*column >= headIn.width) {
		*row+=1;
		*column = 0;
	}
	//If the loop has exceeded past the img header dimensions, exit
	if (*row >= headIn.height) {
		return 1;
	}
	return 0;
}

/*
Parameters: in - input ppm file
	    out - output ppm file
	    msg - message to be encoded in the output file
Return: void
This function encodes msg in the output file by converting every char to binary and placing each bit in the lsb of the r g and b values for the pixels
*/
void encodeMsg(FILE* in, FILE* out, char* msg) {
	//Process input file
	header_t headIn = readHeader(in);
	pixel_t** pixels = readPixel(in, headIn);
	removeDigit(pixels, headIn);
	writeHeader(out, headIn);
	int column = 0;
	int row = 0;
	//Loop through every character in msg	
	for (int i = 0; i < (strlen(msg)); i++) {
		if (row < headIn.height) {
			int* binaryBuf = (int*)malloc(9 * sizeof(int));
			//Convert msg to binary
			charToBinary(msg[i], binaryBuf);
			//Add first 3 bits
			pixels[row][column].r += (binaryBuf[0]);
			pixels[row][column].g += (binaryBuf[1]);
			pixels[row][column].b += (binaryBuf[2]);
			//Increment pixel
			if(incrementPix(headIn, &row, &column)==1){
				free(binaryBuf);
				break;
			}
			//Add second 3 bits
			pixels[row][column].r += (binaryBuf[3]);
			pixels[row][column].g += (binaryBuf[4]);
			pixels[row][column].b += (binaryBuf[5]);
			//Increment pixel
			if(incrementPix(headIn, &row, &column)==1){
				free(binaryBuf);
				break;
			}
			//Add last 3 bits
			pixels[row][column].r += (binaryBuf[6]);
			pixels[row][column].g += (binaryBuf[7]);
			pixels[row][column].b += (binaryBuf[8]);
			//Increment pixel
			if(incrementPix(headIn, &row, &column)==1){
				free(binaryBuf);
				break;
			}
			//Free the memory for the binary buffer
			free(binaryBuf);
		}
	}
	//Write the encoded pixels to the file
	writePixels(out, pixels, headIn);
}

/*
Parameters: f - the ppm file to be decoded
Return: void
This function decodes the characters hidden in f by encodeMsg and prints the message to the terminal
*/
void decodeMsg(FILE* f) {
	int index = 0;
	//Read in data
	header_t headIn = readHeader(f);
	pixel_t** pixels = readPixel(f, headIn);
	//Get total number of pixels
	int totalNumOfPix = headIn.width * headIn.height;
	int row = 0;
	int column = 0;
	int buffer[9];
	//For each pixel, call queue with the lsb
	for (int i = 0; i < totalNumOfPix; i++) {
	//%2 ensures that only a 0 or 1 is sent to queue, basically just the last bit
		queue(pixels[row][column].r % 2, buffer, &index);
		queue(pixels[row][column].g % 2, buffer, &index);
		queue(pixels[row][column].b % 2, buffer, &index);
		//Increment pixel and if column gets too large, increment row by 1
		column++;
		if (column >= headIn.width) {
			column = 0;
			row++;
		}
	}
}

/*
Parameters: n - the bit value
	    buf - buffer to store bits in
	    index - index of buffer
Return: void
This function places n in buf[*index} and prints out the character that buffer represents when it completes a character
*/
void queue(int n, int* buf, int* index) {
	//If the index is 9, it means a char has finished being built and can now be printed
	if (*index == 9) {
		*index = 0;
		//Only print the char if it actually has a value
		if ((buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7] + buf[8]) > 0){
			printf("%c", binToCharacter(buf));
		}
	}
	//Queue the value of n
	buf[*index] = n;
	//Increment index
	*index = *index + 1;
}
