#pragma once
#include"Nt.h"
#include<string>

#include<fstream>
#include<iostream>

namespace NtUtility
{
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};
	bool load_rle_data(std::ifstream &in, NtImage*img); 

	bool Read_Tga_file(std::string filename, NtImage*img);

	
}

