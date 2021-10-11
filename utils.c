/*************************
 *Jonathan Lemon
 *CPSC 2310 F21 Section 001
 *jtlemon@clemson.edu
 *************************/

#include "utils.h"

/*
Parameters: x - the incoming number of arguments from the command line
	    y - the correct number of arguments
Return: void
This function checks to see if the proper number of command line arguments were received
*/
void checkArgs(int x, int y) {
	//If the value of argc is not 3 (2 files plus 1 extra argument) error is thrown
	if(x!=(y+1)){
		fprintf(stderr, "%s", "Invalid number of arguments!\n");
		exit(1);
	}
}

/*
Parameters: fp - the file pointer to check
Return: void
This function checks to see if the file was opened properly
*/
void checkFile(FILE* fp) {
	//If the file pointer is null, it did not open properly
	if(fp==NULL){
		fprintf(stderr, "%s", "Bad File Pointer! Couldn't open the specified file!\n");
		exit(1);
	}
}
