#pragma once
#include <vector>
#include <fstream>
#include"Nt.h"

/*
typedef union PixelInfo
{
	std::uint32_t Colour;
	struct
	{
		std::uint8_t R, G, B, A;
	};
} *PPixelInfo;


class Tga
{
private:
	std::vector<std::uint8_t> Pixels;
	bool ImageCompressed;
	std::uint32_t width, height, size, BitsPerPixel,BytePerPixel;

public:
	Tga(const char* FilePath);
	std::vector<std::uint8_t> GetPixels() { return this->Pixels; }
	std::uint32_t GetWidth() const { return this->width; }
	std::uint32_t GetHeight() const { return this->height; }
	bool HasAlphaChannel() const{ return BitsPerPixel == 32; }
	uint32_t GetPixel(float x,float y) const{ 
				
		int idx = y*(height-1)*(width-1)*BytePerPixel + x*(width-1)* BytePerPixel;
		uint32_t color = (Pixels[idx] << 24) + (Pixels[idx + 1] << 16) + (Pixels[idx + 2] << 8);
		if (HasAlphaChannel())
			color += Pixels[idx + 3];
		return color; 
	}
	void PutToImage(NtImage* image);
};



#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>

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

struct TGAColor {
	unsigned char bgra[4];
	unsigned char bytespp;

	TGAColor() : bgra(), bytespp(1) {
		for (int i = 0; i < 4; i++) bgra[i] = 0;
	}

	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) : bgra(), bytespp(4) {
		bgra[0] = B;
		bgra[1] = G;
		bgra[2] = R;
		bgra[3] = A;
	}

	TGAColor(unsigned char v) : bgra(), bytespp(1) {
		for (int i = 0; i < 4; i++) bgra[i] = 0;
		bgra[0] = v;
	}

	TGAColor(const unsigned char *p, unsigned char bpp) : bgra(), bytespp(bpp) {
		for (int i = 0; i < (int)bpp; i++) {
			bgra[i] = p[i];
		}
		for (int i = bpp; i < 4; i++) {
			bgra[i] = 0;
		}
	}

	unsigned char& operator[](const int i) { return bgra[i]; }
	Uint32 bgra32() { return (bgra[2] << 24) + (bgra[1] << 16 )+ (bgra[0] << 8) + bgra[3]; }
	TGAColor operator *(float intensity) const {
		TGAColor res = *this;
		intensity = (intensity > 1.f ? 1.f : (intensity < 0.f ? 0.f : intensity));
		for (int i = 0; i < 4; i++) res.bgra[i] = bgra[i] * intensity;
		return res;
	}
};

class TGAImage {
protected:
	unsigned char* data;
	int width;
	int height;
	int bytespp;

	bool   load_rle_data(std::ifstream &in);
	bool unload_rle_data(std::ofstream &out);
public:
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};

	TGAImage();
	TGAImage(int w, int h, int bpp);
	TGAImage(const TGAImage &img);
	bool read_tga_file(const char *filename);
	bool write_tga_file(const char *filename, bool rle = true);
	bool flip_horizontally();
	bool flip_vertically();
	bool scale(int w, int h);
	TGAColor get(int x, int y);
	bool set(int x, int y, TGAColor &c);
	bool set(int x, int y, const TGAColor &c);
	~TGAImage();
	TGAImage & operator =(const TGAImage &img);
	int get_width();
	int get_height();
	int get_bytespp();
	unsigned char *buffer();
	void clear();
	void PutToImage(NtImage* image);
};

#endif //__IMAGE_H__
*/