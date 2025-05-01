#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "basic_img_op.hpp"

using namespace std;

int g_m = [0,0.25,0; 0.25,0,0.25; 0,0.25,0];
int vertical_mat = [0,0.5,0; 0,0,0; 0,0.5,0];
int horiz_mat = [0,0,0; 0.5,0,0.5; 0,0,0];

int main(int argc, char *argv[])
{	
	FILE *file;
	int channels, rows = 256, cols = 256;

	if(argc<3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Height = 256] [Width = 256]" << endl;
		return 0;
	}

	if (argc < 4){
		channels = 1; // default is grey image
	} 
	else {
		channels = atoi(argv[3]);
		if (argc == 5)
			rows = atoi(argv[4]);
		else
			rows = atoi(argv[4]);
			cols = atoi(argv[5]);
	}

	unsigned char Image[rows][cols][channels], Zero_padded[rows+2][cols+2][channels];

	readRaw(Image, argv[1], rows, cols, channels);

	for(int i=0; i<rows+2; i++)
		for(int j=0; j<cols+2; j++)
			for(k=0; k< channels; k++)
		{
			if(i==0 || j==0 || i ==rows+1 || j==cols+1)
				Zero_padded[i][j][k] = 0;
				continue;
			Zero_padded[i][j][k] = Image[i-1][j-1][k];
		}

	
    





}