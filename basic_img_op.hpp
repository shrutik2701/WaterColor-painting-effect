#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm> 
#include<cmath>
#include"commons.hpp"


using namespace std;

void print_image(vector<vector<vector < int> > > image, int channel)
{
	for(int i =0; i<image[0].size(); i++)
	{
		for(int j=0; j<image[0][0].size(); j++)
			cout<<image[channel][i][j]<<" ";
		cout<<endl;
	}
	//cout<<"SUCCCESSS";
}

vector<vector<vector<int>>> readRaw(const char* fileName) {
    FILE *file;

    // Temporary buffer to read raw data
    vector<unsigned char> buffer(ROWS * COLS * CHS);

    if (!(file = fopen(fileName, "rb"))) {
        cout << "Cannot open file: " << fileName << endl;
        exit(1);
    }

    size_t readCount = fread(buffer.data(), sizeof(unsigned char), ROWS * COLS * CHS, file);
    if (readCount != ROWS * COLS * CHS) {
        cout << "Warning: Could not read the full image data!" << endl;
    }
    fclose(file);

    // Convert 1D buffer to 3D vector<int>
    vector<vector<vector<int>>> ImageData(CHS, vector<vector<int>>(ROWS, vector<int>(COLS)));

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            for (int k = 0; k < CHS; k++) {
                int index = i * COLS * CHS + j * CHS + k;
                ImageData[k][i][j] = static_cast<int>(buffer[index]);
            }
        }
    }

    return ImageData;
}

void writeRaw(const char* fileName, const vector<vector<vector<int>>>& ImageData) {
    FILE *file;

    if (!(file = fopen(fileName, "wb"))) {
        cout << "Cannot open file for writing: " << fileName << endl;
        exit(1);
    }
	int rows = ImageData[0].size(), cols = ImageData[0][0].size(), chs=ImageData.size();
	cout<<"\n"<<rows<<" "<<cols;
    // Flatten 3D vector into 1D buffer
    vector<unsigned char> buffer(rows*cols*chs);
	
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < chs; k++) {
                int index = i * cols * chs + j * chs + k;
                buffer[index] = static_cast<unsigned char>(ImageData[k][i][j]);
            }
        }
    }

    size_t writeCount = fwrite(buffer.data(), sizeof(unsigned char), rows*cols*chs, file);
    if (writeCount != rows*cols*chs) {
        cout << "Warning: Could not write the full image data!" << endl;
    }

    fclose(file);
}


vector<vector<vector < int> > > constant_padding(vector<vector<vector < int> > > image, int pad, int ele=0)
{
	vector<vector<vector < int> > > new_image(CHS,vector<vector< int> >(ROWS+pad*2, vector< int>(COLS+pad*2, ele)));

	for(int i=0; i<ROWS; i++)
		for(int j=0; j<COLS;j++)
			for(int k=0; k<CHS; k++)
				new_image[k][i+pad][j+pad]= image[k][i][j];

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


vector<vector<vector <int> > > conv_3D(vector<vector<vector <int> > > image, vector<vector <float> > filter, int pad)
{
	vector<vector<vector <int> > > res(CHS,vector<vector< int> >(ROWS, vector< int>(COLS, 0)));
	float sum;

	for(int i= pad; i<ROWS+pad; i++)
		for(int j=pad; j<COLS+pad; j++)
			for(int k=0; k<CHS; k++)
			{
				sum = 0;
				//cout<<"\n i-pad "<<i<<" j "<<j<<"\n";
				for(int m=0; m<filter.size(); m++)
					for(int n = 0; n<filter[0].size(); n++)
					{
						//cout<<"\n m "<<m<<" n "<<n;
						sum += (image[k][i-pad+m][j-pad+n] * filter[m][n]);
						//cout<<res[k][i-pad][j-pad];
					}
				res[k][i-pad][j-pad] = round(sum);
			}

	return res;
}


int conv_block(vector<vector < int> > im1, vector<vector <float> > im2)
{
	int out = 0;

	for(int i=0; i<im1.size(); i++)
		for(int j=0; j<im1[0].size(); j++)
			out += im1[i][j] * im2[i][j];
	
	return out; 
}




vector<vector<vector <int> > > mean_filter(vector<vector<vector <int> > > image, int filter_size)
{
	//cout<<"OR HEREEEE";
	vector<vector <float> > filter(filter_size, vector<float> (filter_size,1.0/(filter_size*filter_size)));
	vector<vector<vector <int> > > res(CHS,vector<vector< int> >(ROWS, vector< int>(COLS, 0))), padded_img, a ;
	int pad = floor(filter_size/2);
	float sum;
	padded_img = constant_padding(image, pad);
	cout<<"pad : "<<pad<<"\n";
	cout<<filter.size()<<"    "<<filter[0].size();
	//cout<<CHS<<" "<<padded_img.size()<<"\n"<<ROWS<<" "<<padded_img[0].size()<<"\n"<<COLS<<" "<<padded_img[0][0].size();
	
	//a = 


	return conv_3D(padded_img, filter, pad);;//
}


vector<vector<vector <int> > > gauss_filter(vector<vector<vector <int> > > image, int filter_size, float sigma)
{
	vector<vector <float> > filter(filter_size, vector<float> (filter_size,0));
	vector<vector<vector <int> > > res(CHS,vector<vector< int> >(ROWS, vector< int>(COLS, 0))), padded_img;
	int pad = floor(filter_size/2);
	float w_tot, y_num, w;
	padded_img = reflective_padding(image, pad);

	for(int i= pad; i<ROWS+pad; i++)
		for(int j=pad; j<COLS+pad; j++)
			for(int k=0; k<CHS; k++)
			{
				w_tot = 0;
				y_num=0;
				for(int m = i-pad; m<= i+pad; m++)
					for(int n = j-pad; n<=j+pad; n++)
					{
						w = 1/(sqrt(2*3.14)*sigma) *exp(-(pow((i-m),2)+pow((j-n),2))/(2*sigma*sigma));
						//cout<<"\n w: "<<w;
						w_tot = w_tot+w;
                		y_num = y_num+ padded_img[k][m][n]*w ;
					}
					//cout<<"\n y_num: "<<y_num<<" w_tot: "<<w_tot;
				res[k][i-pad][j-pad] = floor(y_num/w_tot);
			}

	return res;
}

vector<vector<vector <int> > > median_filter(vector<vector<vector <int> > > image, int filter_size)
{
	vector<vector<vector <int> > > output(CHS,vector<vector< int> >(ROWS, vector< int>(COLS, 0))), padded_image;
	int pad = floor(filter_size/2);
	padded_image = reflective_padding(image,pad);
    int padded_rows = padded_image[0].size();
    int padded_cols = padded_image[0][0].size();

	for (int ch = 0; ch < CHS; ch++) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                vector<int> neighbors;

                // Collect filter_size x filter_size neighbors
                for (int fi = 0; fi < filter_size; fi++) {
                    for (int fj = 0; fj < filter_size; fj++) {
                        int ni = i + fi;  
                        int nj = j + fj;
                        neighbors.push_back(padded_image[ch][ni][nj]);
                    }
                }

                // Find the median
                sort(neighbors.begin(), neighbors.end());
                output[ch][i][j] = neighbors[neighbors.size() / 2];
            }
        }
    }

	return output;

}


// Gaussian function
double gaussian(double x, double sigma) {
    return exp(-(x * x) / (2 * sigma * sigma));
}

vector<vector<vector<int>>> bilateral_filter(vector<vector<vector<int>>> image, int filter_size = 5, double sigma_spatial = 1.5, double sigma_intensity = 30.0)
{
	vector<vector<vector <int> > > output(CHS,vector<vector< int> >(ROWS, vector< int>(COLS, 0))), padded_image;
	int pad = floor(filter_size/2);
	padded_image = reflective_padding(image,pad);
    int padded_rows = padded_image[0].size();
    int padded_cols = padded_image[0][0].size();

	for (int ch = 0; ch < CHS; ch++) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                double sum_weights = 0.0;
                double sum_values = 0.0;

                int center_value = padded_image[ch][i + pad][j + pad];

                for (int fi = -pad; fi <= pad; fi++) {
                    for (int fj = -pad; fj <= pad; fj++) {
                        int ni = i + pad + fi;
                        int nj = j + pad + fj;

                        int neighbor_value = padded_image[ch][ni][nj];

                        // Spatial weight (based on distance)
                        double spatial_weight = gaussian(sqrt(fi * fi + fj * fj), sigma_spatial);

                        // Intensity weight (based on pixel value difference)
                        double intensity_weight = gaussian(neighbor_value - center_value, sigma_intensity);

                        double weight = spatial_weight * intensity_weight;

                        sum_weights += weight;
                        sum_values += neighbor_value * weight;
                    }
                }

                // Normalize
                output[ch][i][j] = static_cast<int>(round(sum_values / sum_weights));
            }
        }
    }

    return output;
}