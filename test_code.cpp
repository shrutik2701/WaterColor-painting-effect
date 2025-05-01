#include<iostream>
#include<stdio.h>
#include "basic_img_op.hpp"

using namespace std;

int ROWS = 3, COLS= 3, CHS=1;

int main()
{
	vector<unsigned char> arr = {1,2,3,4,5,6,7,8,9}, new_arr;
	cout<<"image\n";
	print_image(arr);
	new_arr= constant_padding(arr, 2);
	cout<<"new_image\n";
	print_image(new_arr);

	return 0;
}