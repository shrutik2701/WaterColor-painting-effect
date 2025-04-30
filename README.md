# Watercolor Painting Effect
This project transforms raw grayscale images to simulate a watercolor painting effect using basic image processing techniques in C++.

## Compilation
To compile this project:
g++ -std=c++11 Basic_ip_operations.cpp

## Execution
To run this project:
./program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Rows = 256] [Cols = 256]
(an example input and output is stored in "Flower.raw" and "Flower_painting.raw" respectively.

## Branches
main: Stable version — produces the desired watercolor effect.

mem_mgmt (in progress): Refactoring for improved memory efficiency by replacing the 3D int vector with a 1D char vector.
