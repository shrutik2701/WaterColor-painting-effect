#include "basic_img_op.hpp"
#include "commons.hpp"

using namespace std;

int ROWS=512, COLS=768, CHS=3;

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
	
	vector<vector<vector<int>>> image = readRaw(argv[1]), filt_img1, filt_img2, result(CHS, vector<vector<int>>(ROWS, vector<int>(COLS)));
	int filter_size = 5;

	// LEFT SIDE OF FLOWCHART
	filt_img1 = median_filter(image, filter_size);
	for(int i=0; i<7; i++)
		filt_img1 = bilateral_filter(filt_img1, filter_size, 20, 10);
	//writeRaw("Left_paint.raw", filt_img1);

	// RIGHT SIDE OF FLOWCHART
	filter_size = 7;
	filt_img2 = gauss_filter(image,filter_size, 2 );
	//writeRaw("Right_paint.raw", filt_img1);

	// COMBINED FINAL
	for(int i=0; i<ROWS; i++)
		for(int j=0; j<COLS; j++)
			for(int k=0; k<CHS; k++)
			{
				result[k][i][j] = floor(1.4*filt_img1[k][i][j] - 0.4*filt_img2[k][i][j]);
			}

	writeRaw(argv[2], result);

}