#pragma once
#ifndef NTIMAGE_H

#define NTIMAGE_H

#define NTIMAGE_H
template<typename T>
class NtImage
{
public:
	NtImage(int width, int height);
	NtImage();
	~NtImage();
	NtImage<T> operator = (const NtImage<T>&rhs);

	/*Get 方法*/
	T Get(int x, int y)const;
	T Get(float x, float y)const;
	T Get(int i)const;
	int GetWidth()const;
	int GetHeight()const;
	//float GetZ(int x, int y)const;
	T* GetBuffer32() { return Buffer_; }
	const T* GetBuffer32() const { return Buffer_; }
	int GetBytesPerPixel() const { return BytesPerPixel_; }
	/*Set 方法*/
	void Set(int x, int y, T color);
	void SetWidth(int width) { Width_ = width; };
	void SetHeight(int height) { Height_ = height; };
	//void SetZ(int x, int y,float z);
	void SetBytesPerPixel(int bpp) { BytesPerPixel_ = bpp; }
	void SetBuffer(T*Buffer);
	void SetBuffer(unsigned char*Buffer, size_t size);
	void CleanBuffer() { if (Buffer_) FillImage(0); }
	void FillImage(T t) { std::fill(Buffer_, Buffer_+Width_*Height_, t); }
	/*other*/
	bool Scale(int w, int h);
	T* CreateBuffer_(int size) { CleanBuffer(); Buffer_ = new T[size](); return Buffer_; }
	bool flip_horizontally();
	bool flip_vertically();
private:
	int Width_;
	int Height_;
	int BytesPerPixel_;
	//	float* ZBuffer_ = nullptr;
	T *Buffer_;
	//unsigned char *data;

};


/*
========================
|	 NtImage           |
========================
*/
template<typename T>
NtImage<T>::NtImage() :Width_(0), Height_(0), Buffer_(nullptr) {}

template<typename T>
NtImage<T>::NtImage(int width, int height)
{
	Width_ = width;
	Height_ = height;
	Buffer_ = new T[width*height]();
	//std::fill(Buffer_, Buffer_ + width * height, 0);
	//ZBuffer_ = new float[width*height]();
}


template<typename T>
NtImage<T>::~NtImage()
{
	//delete[] ZBuffer_;
	//delete [] Buffer_;
}

template<typename T>
NtImage<T> NtImage<T>::operator = (const NtImage<T>&rhs)
{
	*(this->Buffer_) = rhs.GetBuffer32();
	this->Width_ = rhs.GetWidth();
	this->Height_ = rhs.GetHeight();
}

template<typename T>
void NtImage<T>::Set(int x, int y, T color)
{

	int idx = Width_ * y + x;

	if (0 <= idx && idx < Width_ * Height_)
		Buffer_[Width_*y + x] = color;
}
template<typename T>
T NtImage<T>::Get(int x, int y)const
{
	int idx = Width_ * y + x;
	assert(0 <= idx && idx < Width_ * Height_);
	return Buffer_[idx];
}
template<typename T>
T NtImage<T>::Get(float x, float y)const
{
	int w = x * Width_;
	int h = y * Height_;
	int idx = Width_ * h + w;
	assert(0 <= idx && idx < Width_ * Height_);
	return Buffer_[idx];
}

template<typename T>
T NtImage<T>::Get(int i)const
{
	assert(i < Width_ * Height_);
	return Buffer_[i];
}
template<typename T>
int NtImage<T>::GetWidth()const
{
	return Width_;
}
template<typename T>
int NtImage<T>::GetHeight()const
{
	return Height_;
}

template<typename T>
void NtImage<T>::SetBuffer(T*Buffer)
{
	delete[]Buffer_;
	Buffer_ = Buffer;
}
template<typename T>
void NtImage<T>::SetBuffer(unsigned char*Buffer, size_t size)
{
	//只限在数据的长宽，像素位都相等的情况下进行设置
	assert(size / BytesPerPixel_ == Width_ * Height_);
	delete[]Buffer_;

	if (BytesPerPixel_ == 4)
		memcpy(Buffer_, Buffer, size);
	else
	{
		Buffer_ = new T[size / BytesPerPixel_];
		int rightBit = 24;
		int pixelCount = 0;
		for (int i = 0; i < size; i += BytesPerPixel_)
		{
			for (int j = 0; j < BytesPerPixel_; j++) {
				Buffer_[pixelCount] |= Buffer[i] << rightBit;
				rightBit -= 8;
			}
		}
		++pixelCount;
	}
}

template<typename T>
bool NtImage<T>::Scale(int w, int h)
{
	if (w <= 0 || h <= 0 || !Buffer_) return false;
	T *tdata = new T[w*h];
	int nscanline = 0;
	int oscanline = 0;
	int erry = 0;
	unsigned long nlinebytes = w;
	unsigned long olinebytes = Width_;
	for (int j = 0; j < Height_; j++) {
		int errx = Width_ - w;
		int nx = -1;
		int ox = -1;
		for (int i = 0; i < Width_; i++) {
			ox += 1;
			errx += w;
			while (errx >= (int)Width_) {
				errx -= Width_;
				nx += 1;
				memcpy(tdata + nscanline + nx, Buffer_ + oscanline + ox, 4);
			}
		}
		erry += h;
		oscanline += olinebytes;
		while (erry >= (int)Height_) {
			if (erry >= (int)Height_ << 1) // it means we jump over a scanline
				memcpy(tdata + nscanline + nlinebytes, tdata + nscanline, nlinebytes);
			erry -= Height_;
			nscanline += nlinebytes;
		}
	}
	delete[] Buffer_;
	Buffer_ = tdata;
	Width_ = w;
	Height_ = h;
	return true;

}
template<typename T>
bool NtImage<T>::flip_horizontally()
{
	if (!Buffer_)return false;
	int halfW = Width_ / 2;
	for (int y = 0; y < Height_; y++)
	{
		for (int x = 0; x < halfW; x++)
		{
			T c0 = GetPixel(x, y);
			T c1 = GetPixel(Width_ - 1 - x, y);
			SetPixel(x, y, c1);
			SetPixel(Width_ - 1 - x, y, c0);
		}
	}
	return true;
}
template<typename T>
bool NtImage<T>::flip_vertically()
{
	if (!Buffer_)false;
	int lineSize = Width_ * 4;
	int halfH = Height_ / 2;
	T* tmp = new T[Width_];
	for (int y = 0; y < halfH; y++)
	{
		memmove(tmp, Buffer_ + y * Width_, lineSize);
		memmove(Buffer_ + y * Width_, Buffer_ + (Height_ - 1 - y) * Width_, lineSize);
		memmove(Buffer_ + (Height_ - 1 - y) * Width_, tmp, lineSize);
	}
	delete[]tmp;
	return true;
}
#endif // !NTIMAGE_H