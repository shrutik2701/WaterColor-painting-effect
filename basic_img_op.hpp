#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include"commons.hpp"

using namespace std;

void print_image(vector<unsigned char> image, int channel=0)
{
	for(int i =0; i<ROWS; i++)
	{
		for(int j=0; j<COLS; j++)
			cout<<int(image[i * COLS * CHS + j * CHS + channel])<<" ";
		cout<<endl;
	}
}

vector<unsigned char> readRaw(char* fileName) {
    FILE *file;
    vector<unsigned char> ImageData(ROWS * COLS * CHS);  // Allocate 1D vector

    if (!(file = fopen(fileName, "rb"))) {
        cout << "Cannot open file: " << fileName << endl;
        exit(1);
    }

    size_t readCount = fread(ImageData.data(), sizeof(unsigned char), ROWS * COLS * CHS, file);
    if (readCount != ROWS * COLS * CHS) {
        cout << "Warning: Could not read the full image data!" << endl;
    }

    fclose(file);
    return ImageData;
}


void writeRaw(const char* fileName, const vector<unsigned char>& ImageData) {
    FILE *file;

    if (!(file = fopen(fileName, "wb"))) {
        cout << "Cannot open file for writing: " << fileName << endl;
        exit(1);
    }
    size_t writeCount = fwrite(ImageData.data(), sizeof(unsigned char), ROWS * COLS * CHS, file);
    if (writeCount != ROWS * COLS * CHS) {
        cout << "Warning: Could not write the full image data!" << endl;
    }

    fclose(file);
}

vector<unsigned char> constant_padding(vector<unsigned char> image, int pad, int ele=0)
{
	vector<unsigned char> new_image(CHS*(ROWS+pad*2)*(COLS+pad*2), static_cast<unsigned char>(ele));
	
	for(int i=0; i<ROWS; i++)
		for(int j=0; j<COLS;j++)
			for(int k=0; k<CHS; k++)
			{
				int org_index = i*COLS*CHS + j*CHS + k;
				int new_index = (i+pad)*(COLS+pad*2)*CHS + (j+pad)*CHS + k;
				new_image[new_index] = image[org_index];
			}
		
	ROWS = ROWS+pad*2; COLS = COLS+pad*2;
	return new_image;
}

vector<vector<vector < int> > > replicate_padding(vector<vector<vector < int> > > image, int pad)
{
	vector<vector<vector < int> > > padded_img(CHS,vector<vector< int> >(ROWS+pad*2, vector< int>(COLS+pad*2, 0)));

		for( int i=0; i<ROWS+pad*2; i++)
			for(int j=0; j<COLS+pad*2; j++)
				for(int k=0; k<CHS; k++)
				{
					if(i<pad && j<pad) // top left corner
						padded_img[k][i][j] = image[k][0][0];
					else if(i>ROWS+pad-1 && j>COLS+pad-1) // bottom right corner
						padded_img[k][i][j] = image[k][ROWS-1][COLS-1];
					else if (i>=pad && i<ROWS+pad && j>=pad & j<COLS+pad) // original image elements
						padded_img[k][i][j] = image[k][i-pad][j-pad];
					else if(i<pad && j>COLS+pad-1) // top right corner
						padded_img[k][i][j] = image[k][0][COLS-1];
					else if(i>ROWS+pad-1 && j<pad) // bottom left corner
						padded_img[k][i][j] = image[k][ROWS-1][0]; // "image" is dealing with ORIGINAL coordinates!!
					else{

						if(i<pad)
							padded_img[k][i][j] = image[k][0][j-pad];
						else if ( i> ROWS+pad-1)
							padded_img[k][i][j] = image[k][ROWS-1][j-pad];
						else if(j<pad)
							padded_img[k][i][j] = image[k][i-pad][0];
						else
							padded_img[k][i][j] = image[k][i-pad][COLS-1];

				}
					}
	return padded_img;
}

vector<vector<vector < int> > > reflective_padding(vector<vector<vector < int> > > image, int pad)
{
	vector<vector<vector < int> > > padded_img(CHS,vector<vector< int> >(ROWS+pad*2, vector< int>(COLS+pad*2, 0)));

	for( int i=0; i<ROWS+pad*2; i++)
			for(int j=0; j<COLS+pad*2; j++)
				for(int k=0; k<CHS; k++)
				{
					if(i<pad && j<pad) // top left corner
						padded_img[k][i][j] = image[k][pad-i][pad-j];
					else if(i<pad && j>COLS+pad-1) // top right corner
						padded_img[k][i][j] = image[k][pad-i][COLS - j + pad*2];
					else if(i>ROWS+pad-1 && j<pad) // left bottom corner padding
						padded_img[k][i][j] = image[k][ROWS-i+pad*2][pad-j];
					else if(i>ROWS+pad-1 && j>COLS+pad-1) // bottom right corner
						padded_img[k][i][j] = image[k][ROWS-i+pad*2][COLS - j + pad*2];
					else if (i>=pad && i<ROWS+pad && j>=pad & j<COLS+pad) // original image elements
						padded_img[k][i][j] = image[k][i-pad][j-pad];
					else if(i<pad)
						padded_img[k][i][j] = image[k][pad-i][j-pad];
					else if ( i> ROWS+pad-1)
							padded_img[k][i][j] = image[k][2*ROWS+pad-i-2][j-pad];//[ROWS-1-i+pad*2][j-pad];
					else if(j<pad)
							padded_img[k][i][j] = image[k][i-pad][pad-j];
					else
							padded_img[k][i][j] = image[k][i-pad][2*COLS+pad-j-2];
				}

	return padded_img;
}