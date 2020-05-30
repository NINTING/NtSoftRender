#pragma once
#ifndef NTIMAGE_H

#define NTIMAGE_H

#include<algorithm>
#include<SDL.h>
#include"Ntmath.h"

/*
========================
|	   NtColor         |
========================
*/

const Uint32 White32 = 0xffffff00;
const Uint32 Black32 = 0x00000000;
const Uint32 Red32 = 0xff000000;
const Uint32 Green32 = 0x00ff0000;
const Uint32 Blue32 = 0x0000ff00;
const Uint32 Trans32 = 0x000000ff;



const float over255 = 1.f/255.f;
class NtColor
{
	
public:

	NtColor() { R_ = G_ = B_ = A_ = 0; }
	NtColor(unsigned char R, unsigned char G, unsigned char B, int A = 0) :R_(R), G_(G), B_(B), A_(A)
	{
		//ColorPack_ = (R << 24) + (G << 16) +( B << 8) + A;
	}
	NtColor(Uint32 color) {
		R_ = color >> 24, G_ = (color & Green32) >> 16, B_ = (color & Blue32) >> 8, A_ = color & Trans32;
	}
	static NtColor White, Black, Red, Green, Blue;
	unsigned char& operator [](size_t i) { return i == 0 ? R_ : i == 1 ? G_ : i == 2 ? B_ : A_; }
	unsigned char operator [](size_t i) const { return i == 0 ? R_ : i == 1 ? G_ : i == 2 ? B_ : A_; }
	void SetColor(unsigned char R, unsigned char G, unsigned char B, int A = 0)
	{
		R_ = R; G_ = G; B_ = B; A_ = A;
	}
	NtColor operator *(float intensity)const;
	NtColor operator *=(float intensity);
	NtColor operator *(const NtColor& rhs)const;

	NtColor operator +(const NtColor&rhs)const;
	void operator +=(const NtColor&rhs);
	Uint32 GetColor32()const { return  (R_ << 24) | (G_ << 16) | (B_ << 8) | A_; }
	NtVector4 ToVector4() {
		return NtVector<float, 4>(R_ * over255, G_ * over255, B_ * over255, A_ * over255);
	}
	NtVector3 ToVector3() {
		return NtVector<float, 3>(R_ * over255*2.f, G_ * over255*2.f, B_ * over255*2.f);
	}
	unsigned char R_, G_, B_, A_;
	//Uint32 ColorPack_;
};
NtColor float4ToColor(const NtVector4&rhs);

class NtImage
{

public:

	NtImage(int width, int height,int BytesPerPixel);
	NtImage();
	~NtImage();
	NtImage& operator = (const NtImage&rhs);

	/*Get 方法*/
	Uint32 GetRGBA32(int x, int y)const;
	Uint32 GetRGBA32(float x, float y)const;
	Uint32 GetRGBA32(int i)const;

	NtColor GetPixel(int x, int y)const;
	NtColor GetPixel(float x,float y)const;
	NtVector4 GetPixelfloat(float x, float y)const;
	size_t GetImageSize()const { return Width_ * Height_; }
	
	//如果该纹理存储的是法向量
	NtVector3 normal(float x, float y);
	NtVector4 diffuse(float x, float y);

	float spec(float x, float y);


	int GetWidth()const;
	int GetHeight()const;

	//float GetZ(int x, int y)const;
	Uint32* GetBuffer32() const {  return reinterpret_cast<Uint32*>(Buffer_); }
	unsigned char * GetBuffer() const { return Buffer_; }
	int GetBytesPerPixel() const { return BytesPerPixel_; }
	/*Set 方法*/
	void Set(int x, int y, const NtColor& color);
	void Set(int i, const NtColor& color);
	void SetWidth(int width) { Width_ = width; };
	void SetHeight(int height) { Height_ = height; };
	//void SetZ(int x, int y,float z);
	void SetBytesPerPixel(int bpp) { BytesPerPixel_ = bpp; }
	
	void SetBuffer(unsigned char*Buffer, size_t size);
	void CleanBuffer() { if (Buffer_) FillImage(0); }
	void FillImage(unsigned char t) { std::fill(Buffer_, Buffer_+Width_*Height_*BytesPerPixel_, t); }
	/*other*/
	bool Scale(int w, int h);
	unsigned char* CreateBuffer_(int size) { if (Buffer_)delete[]Buffer_; Buffer_ = new unsigned char[size](); return Buffer_; }
	bool flip_horizontally();
	bool flip_vertically();
private:

	int Width_;
	int Height_;
	int BytesPerPixel_;


	//RGBA 存储形式为RGBA | BGR
	unsigned char *Buffer_;
	//unsigned char *data;

};


#endif