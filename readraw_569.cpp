#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include"commons.hpp"
#include"basic_img_op.hpp"

using namespace std;

int ROWS = 256;
int COLS = 256;
int CHS = 1;

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Rows = 256] [Cols = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		CHS = 1; // default is grey image
	} 
	else {
		CHS = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 6){
			ROWS = atoi(argv[4]);
			COLS = atoi(argv[5]);
		}
	}
	
	vector<unsigned char> Image = readRaw(argv[1]);
	

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	//fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	//fclose(file);

	return 0;
}
