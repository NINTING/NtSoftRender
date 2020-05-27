#include"NtImage.h"
/*
========================
|	 NtImage           |
========================
*/

NtImage::NtImage() :Width_(0), Height_(0), Buffer_(nullptr) {}


NtImage::NtImage(int width, int height,int bspp)
{
	Width_ = width;
	Height_ = height;
	BytesPerPixel_ = bspp;
	Buffer_ = new unsigned char[width*height*bspp]();
	//std::fill(Buffer_, Buffer_ + width * height, 0);
	//ZBuffer_ = new float[width*height]();
}



NtImage::~NtImage()
{
	//delete[] ZBuffer_;
	//delete [] Buffer_;
}


NtImage& NtImage::operator = (const NtImage&rhs)
{
	*(this->Buffer_) = *rhs.GetBuffer();
	this->Width_ = rhs.GetWidth();
	this->Height_ = rhs.GetHeight();
	return *this;
}


void NtImage::Set(int x, int y,const NtColor& color)
{
	int idx = x * BytesPerPixel_ + y * Width_*BytesPerPixel_;
	for(int i = 0;i<BytesPerPixel_;i++)
		Buffer_[idx+i] = color[i];
}

Uint32 NtImage::GetRGBA32(int x, int y)const
{
	int i = Width_ * y + x;
	assert(0 <= i && i< Width_ * Height_);
	i *= BytesPerPixel_;
	unsigned char p3 = 0;
	if (BytesPerPixel_ == 1)
		return Buffer_[i] << 24;
	if (BytesPerPixel_ == 4)
		p3 = Buffer_[i + 3];
	return Buffer_[i] << 24 | Buffer_[i + 1] << 16 | Buffer_[i + 2] << 8 | p3;
}

Uint32 NtImage::GetRGBA32(float x, float y)const
{
	int w = x * Width_;
	int h = y * Height_;
	int i= Width_ * h + w;
	assert(0 <= i && i < Width_ * Height_);
	i *= BytesPerPixel_;
	unsigned char p3 = 0;
	if (BytesPerPixel_ == 1)
		return Buffer_[i] << 24;
	if (BytesPerPixel_ == 4)
		p3 = Buffer_[i + 3];
	return Buffer_[i] << 24 | Buffer_[i + 1] << 16 | Buffer_[i + 2] << 8 | p3;
}


Uint32 NtImage::GetRGBA32(int i)const
{
	assert(i < Width_ * Height_);
	i *= BytesPerPixel_;
	if (BytesPerPixel_ == 1)
		return Buffer_[i]<<24;

	unsigned char p3 = 0;
	if (BytesPerPixel_ == 4)
		p3 = Buffer_[i + 3];
	return Buffer_[i]<<24 | Buffer_[i+1] << 16 | Buffer_[i+2] << 8 | p3 ;
}
NtColor NtImage::GetPixel(int x, int y)const
{
	NtColor ret;
	size_t index = x*BytesPerPixel_ + y * Width_*BytesPerPixel_;
	
	for (int i = 0; i < BytesPerPixel_; i++)
		ret[i] = Buffer_[index + i];
	return ret;
}

NtColor NtImage::GetPixel(float x, float y)const
{
	NtColor ret;
	int wx = x * Width_;
	int hy = y * Height_;
	size_t index = wx * BytesPerPixel_ + hy * Width_*BytesPerPixel_;

	for (int i = 0; i < BytesPerPixel_; i++)
		ret[i] = Buffer_[index + i];
	return ret;
}

NtVector3 NtImage::normal(float x, float y)
{
	//法线向量存储格式不同 ，为了兼容RGBA格式，因此法线存储变为了(z，y，x)
	//以0.5为分界线，小于0.5的为负，大于0.5为正
	 int wx = x*Width_;
	 int hy = y * Height_;
	size_t index = wx * BytesPerPixel_ + hy * Width_*BytesPerPixel_;
	return NtVector3(Buffer_[index+2]/255.f*2.f-1.f,Buffer_[index + 1] / 255.f*2.f - 1.f,Buffer_[index] / 255.f*2.f - 1.f);
}

NtVector4 NtImage::diffuse(float x, float y)
{
	int wx = x * Width_;
	int hy = y * Height_;
	size_t index = wx * BytesPerPixel_ + hy * Width_*BytesPerPixel_;
	unsigned char alpha = 0;
	if (BytesPerPixel_ == 4)
		alpha = Buffer_[3];
	return NtVector4(Buffer_[index] * over255, Buffer_[index+1] * over255, Buffer_[index+2] * over255, alpha*over255);

}
float NtImage::spec(float x, float y)
{
	int wx = x * Width_;
	int hy = y * Height_;
	size_t index = wx * BytesPerPixel_ + hy * Width_*BytesPerPixel_;
	return Buffer_[index]*over255;
}

int NtImage::GetWidth()const
{
	return Width_;
}

int NtImage::GetHeight()const
{
	return Height_;
}



void NtImage::SetBuffer(unsigned char*Buffer, size_t size)
{
	//只限在数据的长宽，像素位都相等的情况下进行设置
	assert(size / BytesPerPixel_ == Width_ * Height_);
	delete[]Buffer_;

	if (BytesPerPixel_ == 4)
		memcpy(Buffer_, Buffer, size);
	else
	{
		Buffer_ = new unsigned char[size];
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


bool NtImage::Scale(int w, int h)
{
	if (w <= 0 || h <= 0 || !Buffer_) return false;
	unsigned char *tdata = new unsigned char[w*h*BytesPerPixel_];
	int nscanline = 0;
	int oscanline = 0;
	int erry = 0;
	unsigned long nlinebytes = w * BytesPerPixel_;
	unsigned long olinebytes = Width_* BytesPerPixel_;
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
				memcpy(tdata + nscanline + nx, Buffer_ + oscanline + ox, BytesPerPixel_);
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

bool NtImage::flip_horizontally()
{
	if (!Buffer_)return false;
	int halfW = Width_ / 2;
	for (int y = 0; y < Height_; y++)
	{
		for (int x = 0; x < halfW; x++)
		{
			NtColor c0 = GetPixel(x, y);
			NtColor c1 = GetPixel(Width_ - 1 - x, y);
			Set(x, y, c1);
			Set(Width_ - 1 - x, y, c0);
		}
	}
	return true;
}

bool NtImage::flip_vertically()
{
	if (!Buffer_)false;
	int lineSize = Width_ * BytesPerPixel_;
	int halfH = Height_ >>1;
	unsigned char * tmp = new unsigned char[lineSize];
	for (int y = 0; y < halfH; y++)
	{
		unsigned long l1 = y * lineSize;
		unsigned long l2 = (Height_ - 1 - y)*lineSize;

		memmove(tmp, Buffer_ + l1, lineSize);
		memmove(Buffer_ + l1, Buffer_ + l2, lineSize);
		memmove(Buffer_ +l2, tmp, lineSize);
	}
	delete[]tmp;
	return true;

	return true;
}


/*
========================
|	   NtColor         |
========================
*/

NtColor NtColor::White = NtColor(0xffffff00);
NtColor NtColor::Black = NtColor(0x00000000);
NtColor NtColor::Blue = NtColor(0x0000ff00);
NtColor NtColor::Green = NtColor(0x00ff0000);
NtColor NtColor::Red = NtColor(0xff000000);
NtColor NtColor::operator *(float intensity)const
{
	return NtColor(R_ * intensity, G_ * intensity, B_ * intensity, A_);
};
NtColor NtColor::operator *=(float intensity)
{
	R_ *= intensity, G_ *= intensity, B_ *= intensity;
	return *this;
}

NtColor NtColor::operator *(const NtColor& rhs)const
{
	return NtColor(this->R_*rhs.R_, this->G_*rhs.G_, this->B_*rhs.B_, this->A_*rhs.A_);
}
/*
void NtColor::operator *= (const NtColor& rhs)
{
	this->R_*=rhs.R_, this->G_*rhs.G_, this->B_*rhs.B_, this->A_*rhs.A_;
}
*/

NtColor  NtColor::operator +(const NtColor&rhs)const
{
	NtColor ret = NtColor();
	for (int i = 0; i < 3; i++)
	{
		ret[i] = this->operator[](i) + rhs[i];
	}
	return ret;
}
void  NtColor::operator +=(const NtColor&rhs)
{
	*this = *(this) + rhs;
}



NtColor float4ToColor(const NtVector4&rhs)
{
	return NtColor(rhs[0] * 255, rhs[1] * 255, rhs[2] * 255, rhs[3] * 255);
}
