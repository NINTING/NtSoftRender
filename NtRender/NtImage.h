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
//uint32 格式 AGBR
const Uint32 White32 = 0x00ffffff;
const Uint32 Black32 = 0x00000000;
const Uint32 Red32 = 0x000000ff;
const Uint32 Green32 = 0x00ff0000;
const Uint32 Blue32 = 0x0000ff00;
const Uint32 Trans32 = 0xff000000;

const NtVector4 WhiteF4 = NtVector4(1, 1, 1, 0);

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
	NtColor(NtVector<unsigned char, 4>&rhs);
	

	NtColor operator = (NtVector<unsigned char, 4U>&rhs);
	static NtColor White, Black, Red, Green, Blue;
	unsigned char& operator [](size_t i) { return i == 0 ? R_ : i == 1 ? G_ : i == 2 ? B_ : A_; }
	unsigned char operator [](size_t i) const { return i == 0 ? R_ : i == 1 ? G_ : i == 2 ? B_ : A_; }
	void SetColor(unsigned char R, unsigned char G, unsigned char B, int A = 0)
	{
		R_ = R; G_ = G; B_ = B; A_ = A;
	}
	NtColor operator *(float intensity)const;
	NtColor operator *(const NtVector4& rhs)const;
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
NtColor OneColor();


/*
========================
|	   NtImage         |
========================
*/



template<class T,size_t BytesPerPixel>
class NtImage
{

public:

	NtImage(int width, int height,T t = 0)
	{
		Width_ = width;
		Height_ = height;
		Buffer_ = new T[width*height* BytesPerPixel]();
		FillImage(t);
	}
	NtImage(int width, int height, NtVector<T,BytesPerPixel> t)
	{
		Width_ = width;
		Height_ = height;
		Buffer_ = new T[width*height* BytesPerPixel]();
		size_t size= width * height;
		for (int i = 0; i < size; i++)
			memcpy(Buffer_ + i * BytesPerPixel, t.raw, BytesPerPixel * sizeof(T));

	}
	NtImage(const NtImage&rhs)
	{
		this->Width_ = rhs.GetWidth();
		this->Height_ =rhs.GetHeight();
		this->Buffer_ = new T[this->Width_ *this->Height_ * BytesPerPixel]();
		int size = this->Width_*this->Height_* BytesPerPixel;
		memcpy(Buffer_,rhs.GetBuffer(), size*sizeof(T));
	}



	NtImage() { Buffer_ = nullptr, Width_ = 0; Height_ = 0; };
	~NtImage()
	{
		if(Buffer_)
			delete[] Buffer_;
	}

	void swap(NtImage&rhs)
	{
		T* tmp = Buffer_;
		
		this->Buffer_ = rhs.GetBuffer();

		rhs.SetBuffer(tmp);
		int Width = rhs.GetWidth();
		int Height = rhs.GetHeight();
	
		rhs.SetWidth(this->GetWidth());
		rhs.SetHeight(this->GetHeight());
		this->Width_ = Width;
		this->Height_ = Height;
	}

	NtImage& operator = (const NtImage&rhs)
	{
		NtImage tmp = NtImage(rhs);
		swap(tmp);
		return *this;
	} 
	NtImage& operator = (NtImage && rhs)
	{
		swap(rhs);
		return *this;
	}

	/*Get 方法*/
	NtVector<T, BytesPerPixel> GetPixel(int index)const
	{

 		NtVector<T, BytesPerPixel> ret;
		memcpy(ret.raw, Buffer_ + index* BytesPerPixel, BytesPerPixel * sizeof(T));
		//for (int i = 0; i < BytesPerPixel; i++)
		//	ret[i] = Buffer_[index + i];
		return ret;
	}

	NtVector<T, BytesPerPixel> GetPixel(int x, int y)const
	{
		if (x > Width_)x -= Width_;
		if (y > Height_)y -= Height_;
		if (x < 0)x = Width_ + x;
		if (y < 0)y = Height_ + y;
		NtVector<T, BytesPerPixel> ret;
		
		size_t index = x * BytesPerPixel + y * Width_*BytesPerPixel;
		memcpy(ret.raw, Buffer_+index, BytesPerPixel * sizeof(T));
		//for (int i = 0; i < BytesPerPixel; i++)
		//	ret[i] = Buffer_[index + i];
		return ret;	
	}
	NtVector<T, BytesPerPixel> GetPixel(float x, float y)const
	{
		x = x*(Width_-1);
		 y = y*(Height_-1);
		int x0 = (int)floor(x);
		int x1 = (int)ceilf(x);
		int y0 = (int)floorf(y);
		int y1 = (int)ceilf(y);
		NtVector<T, BytesPerPixel> p0 = this->GetPixel(x0, y0);
		NtVector<T, BytesPerPixel> p1 = this->GetPixel(x1, y0);
		NtVector<T, BytesPerPixel> p2 = this->GetPixel(x0, y1);
		NtVector<T, BytesPerPixel> p3 = this->GetPixel(x1, y1);

		NtVector<T, BytesPerPixel> hp0 = lerp(p0, p1, x - x0);
		NtVector<T, BytesPerPixel> hp1 = lerp(p2, p3, x - x0);
		NtVector<T, BytesPerPixel> p = lerp(hp0, hp1, y - y0);
		
		return p;
	}
	
	size_t GetImageSize()const { return Width_ * Height_; }
	

	int GetWidth()const
	{
		return Width_;
	}
	int GetHeight()const
	{
		return Height_;
	}
	//float GetZ(int x, int y)const;
	T * GetBuffer() { return this->Buffer_; }
	T * GetBuffer() const
	{ return this->Buffer_; }
	T * GetBuffer(int i) const {
		return this->Buffer_+i;
	}
	int GetBytesPerPixel() const 
	{ return BytesPerPixel; }
	
	/*Set 方法*/
	//void Set(int x, int y, Uint32 t)
	//{
		//int idx = x * BytesPerPixel + y * Width_*BytesPerPixel;
		//memcpy(Buffer_ + idx, &t, BytesPerPixel * sizeof(T));
	//}
	void Set(int x, int y, T t)
	{
		int idx = x * BytesPerPixel + y * Width_*BytesPerPixel;
		Buffer_[idx] = t;
	}
	void Set(int x, int y, const NtVector<T, BytesPerPixel>& color)
	{
		int idx = x * BytesPerPixel + y * Width_*BytesPerPixel;
		memcpy(Buffer_ + idx, color.raw, BytesPerPixel * sizeof(T));
		//for (int i = 0; i < BytesPerPixel; i++)
		//	Buffer_[idx + i] = color[i];
	}
	/*void Set(int x, int y, T rhs)
	{
		int idx = x * BytesPerPixel + y * Width_*BytesPerPixel;
		memcpy(Buffer_ + idx * BytesPerPixel, color.raw, BytesPerPixel * sizeof(T));
		//for (int i = 0; i < BytesPerPixel; i++)
		//	Buffer_[idx + i] = rhs;
	}*/
	void Set(int i, const NtVector<T, BytesPerPixel>& color)
	{
		memcpy(Buffer_ + i*BytesPerPixel, color.raw, BytesPerPixel*sizeof(T));
	//	for (int j = 0; j < BytesPerPixel; j++)
	//		Buffer_[j + i] = color[j];
	}
	void Set(int i, T& color)
	{
			Buffer_[i] = color;
	}
	void Set(int i, T* color)
	{
		Buffer_[i] = *color;
	}
	void SetWidth(int width) { Width_ = width; };
	void SetHeight(int height) { Height_ = height; };
	//void SetZ(int x, int y,float z);
	
	void CleanBuffer() { if (Buffer_) FillImage(0); }
	void FillImage(T t) { std::fill(Buffer_, Buffer_+Width_*Height_*BytesPerPixel, t); }
	/*other*/
	void Scale(int w, int h)
	{
		if (w == Width_ && h == Height_)return;
		float overW = 1.f / w;
		float overH = 1.f / h;
		T* tmp = new T[w*h*BytesPerPixel];
		int sw = Width_, sh = Height_;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				NtVector<T, BytesPerPixel> p;
				p = GetPixel(j*overW, i*overH);
				for (int k = 0; k < BytesPerPixel; k++)
					tmp[i*BytesPerPixel*w + j * BytesPerPixel+k] = p[k];
			}
		}
		Width_ = w;
		Height_ = h;
		delete[]Buffer_;
		Buffer_ = tmp;

	}
	T* CreateBuffer_(int size) { if (Buffer_)delete[]Buffer_; Buffer_ = new T[size](); return Buffer_; }
	void SetBuffer(T*Buffer)
	{
		
		Buffer_ = Buffer;
	}

	void ReSetBuffer(T*Buffer, size_t size)
	{
		//只限在数据的长宽，像素位都相等的情况下进行设置
		assert(size / BytesPerPixel == Width_ * Height_);
		delete[]Buffer_;
		memcpy(Buffer_, Buffer, size);
	}
	void Reset(int w,int h,T*buffer)
	{
		Width_ = w;
		Height_ = h;
		ReSetBuffer(buffer, w*h*BytesPerPixel);
	}
	void Reset(const NtImage&rhs)
	{
		Width_ = rhs.GetWidth();
		Height_ = rhs.GetHeight();
		ReSetBuffer(rhs.GetBuffer()	, Width_*Height_*BytesPerPixel);
	}
	bool flip_horizontally();
	bool flip_vertically();

private:

	int Width_;
	int Height_;



	//RGBA 存储形式为RGBA | BGR
	T *Buffer_;
	//unsigned char *data;

};

template<class T, size_t BytesPerPixel>
bool NtImage<T, BytesPerPixel>::flip_horizontally()
{
	if (!Buffer_)return false;
	int halfW = Width_ / 2;
	for (int y = 0; y < Height_; y++)
	{
		for (int x = 0; x < halfW; x++)
		{
			NtVector<T,BytesPerPixel> c0 = GetPixel(x, y);
			NtVector<T, BytesPerPixel> c1 = GetPixel(Width_ - 1 - x, y);
			Set(x, y, c1);
			Set(Width_ - 1 - x, y, c0);
		}
	}
	return true;
}
template<class T, size_t BytesPerPixel>
bool NtImage<T, BytesPerPixel>::flip_vertically()
{
	if (!Buffer_)false;
	int lineSize = Width_ * BytesPerPixel ;
	int halfH = Height_ >> 1;
	T * tmp = new T[lineSize];
	for (int y = 0; y < halfH; y++)
	{
		unsigned long l1 = y * lineSize;
		unsigned long l2 = (Height_ - 1 - y)*lineSize;

		memmove(tmp, Buffer_ + l1, lineSize * sizeof(T));
		memmove(Buffer_ + l1, Buffer_ + l2, lineSize * sizeof(T));
		memmove(Buffer_ + l2, tmp, lineSize * sizeof(T));
	}
	delete[]tmp;
	return true;
}


using Tex2D_1F = NtImage<float, 1>;
using Tex2D_4F = NtImage<float,4>; 
using Tex2D_3F = NtImage<float, 3>;
using Tex2D_UC = NtImage<unsigned char, 4>;

extern Tex2D_4F White2D;
extern Tex2D_4F Bump2D;
//Tex2D_4F WhitePlain(int W, int H);

Uint32 RGBAToUint32(NtColor4 color);

Tex2D_4F RGBAImageToFloat4Image(const Tex2D_UC&rhs);
Tex2D_UC Float4ImageToRGBAImage(const Tex2D_4F&rhs);

NtVector4 RGBAToFloat4(const NtColor4&rgba);

NtColor4 Float4ToRGBA(const NtVector4&rhs);


#endif