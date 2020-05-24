#pragma once
#include"Nt.h"
#include<string>
namespace NtUtility
{
	bool Read_Tga_file(std::string filename, NtImage<Uint32>*img);
}