#include"NtUtility.h"

#pragma pack(push,1)
struct TGA_Header {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)

bool NtUtility::load_rle_data(std::ifstream &in, NtImage*img) {

	unsigned long pixelcount = img->GetWidth()*img->GetHeight();
	

	unsigned long currentpixel = 0; 
	unsigned long currentbyte = 0;
	int bytespp = img->GetBytesPerPixel();
	unsigned char *data = img->GetBuffer();  
	NtColor colorbuffer;
	unsigned char bgra[4] = { 0 };
	do {
		unsigned char chunkheader = 0;
		chunkheader = in.get();
		if (!in.good()) {
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		if (chunkheader < 128) {
			chunkheader++;
			for (int i = 0; i < chunkheader; i++) {
				in.read((char *)bgra, bytespp);
				if (!in.good()) {
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				for (int i = 0; i < bytespp; i++) {
					data[currentbyte++] = bgra[i];
				}
				
				currentpixel++;
				/*
				if (bytespp == RGBA){

					data[currentpixel++] = (bgra[0] << 24) | (bgra[1] << 16) | (bgra[2] << 8) | bgra[3];
					}
				else if (bytespp == RGB)
					data[currentpixel++] = (bgra[2] << 24) | (bgra[1] << 16) | (bgra[0] << 8) | bgra[3];
				*/


				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
		else {
			chunkheader -= 127;
			in.read((char *)bgra, bytespp);
			if (!in.good()) {
				std::cerr << "an error occured while reading the header\n";
				return false;
			}
			for (int i = 0; i < chunkheader; i++) {
				for (int i = 0; i < bytespp; i++) {
					/*
					if (i == 3) {
						data[currentbyte++] = bgra[3];
						break;
					}
					*/
					data[currentbyte++] = bgra[i];
				}
				currentpixel++;

				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);
	return true;
}

bool NtUtility::Read_Tga_file(std::string filename, NtImage*img)
{
	{
		img->CleanBuffer();
		//Uint32* data = *(img->GetPointToBuffer32());


		std::ifstream in;
		in.open(filename.c_str(), std::ios::binary);
		if (!in.is_open()) {
			std::cerr << "can't open file " << filename << "\n";
			in.close();
			return false;
		}
		TGA_Header header;
		in.read((char *)&header, sizeof(header));
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the header\n";
			return false;
		}
		img->SetWidth(header.width);
		img->SetHeight(header.height);
		img->SetBytesPerPixel(header.bitsperpixel >> 3);
		int bytespp = img->GetBytesPerPixel();
		int width = img->GetWidth();
		int height = img->GetHeight();
		unsigned long nbytes = bytespp * width*height;
		unsigned char * data = img->CreateBuffer_(nbytes);

		if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA)) {
			in.close();
			std::cerr << "bad bpp (or width/height) value\n";
			return false;
		}
	


		if (3 == header.datatypecode || 2 == header.datatypecode) {
			in.read((char *)data, nbytes);
			if (!in.good()) {
				in.close();
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
		}
		else if (10 == header.datatypecode || 11 == header.datatypecode) {
			if (!load_rle_data(in, img)) {
				in.close();
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
		}
		else {
			in.close();
			std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
			return false;
		}
		if (!(header.imagedescriptor & 0x20)) {
			//
			//img->flip_vertically();
		}
		if (header.imagedescriptor & 0x10) {
		//	img->flip_horizontally();
		}
		std::cerr << width << "x" << height << "/" << bytespp * 8 << "\n";
		in.close();
		return true;

	}
}
