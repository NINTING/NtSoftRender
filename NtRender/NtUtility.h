#pragma once
#include"Nt.h"
#include<string>

#include<fstream>
#include<iostream>
#include"SDL_image.h"
namespace NtUtility
{
	NtVector4 RandomVec();
	float RandomF(float l, float r);
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};
	bool load_rle_data(std::ifstream &in, Tex2D_UC *img);

	bool Read_Tga_file(std::string filename, Tex2D_UC *img);
	bool Read_Tga_file(std::string filename, Tex2D_4F* ret);
	void UtilityInit();
	Tex2D_UC Read_file(const std::string& filename);

	void RandomOffsetVec(NtVector4*offset);
	void RandomNormalImage(int w, int h, Tex2D_3F**out);
	

}



