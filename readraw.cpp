#include <iostream>
#include <fstream>   // for file handling
#include <cstdlib>   // for exit()

using namespace std;

int main(int argc, char *argv[])
{
// Variables
    int BytesPerPixel = 1; // default
    int Size = 256;        // default

    // Check syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "Usage: " << argv[0] << " input_image.raw output_image.raw [BytesPerPixel = 1] [Rows = 256] [Cols = 256]" << endl;
        return 1;
    }

    // Optional arguments
    if (argc >= 4) BytesPerPixel = atoi(argv[3]);
    if (argc >= 5) {ROWS = atoi(argv[4]); COLS = atoi(argv[4]);}

    // Open input file
    ifstream inputFile(argv[1], ios::binary);
    if (!inputFile) {
        cerr << "Error: Cannot open input file: " << argv[1] << endl;
        return 1;
    }

    // Open output file
    ofstream outputFile(argv[2], ios::binary);
    if (!outputFile) {
        cerr << "Error: Cannot open output file: " << argv[2] << endl;
        return 1;
    }

    // [You can add processing code here if you want]

    cout << "Files opened successfully!" << endl;

    // Close files
    inputFile.close();
    outputFile.close();

    return 0;
}
