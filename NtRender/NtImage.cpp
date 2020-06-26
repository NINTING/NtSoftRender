#include"NtImage.h"


/*
========================
|	   NtColor         |
========================
*/
Tex2D_4F White2D = Tex2D_4F(1, 1, 1);
Tex2D_4F Bump2D = Tex2D_4F(1, 1, NtVector4(0.5f,0.5,1.f,1));
NtColor NtColor::White = NtColor(0xffffff00);
NtColor NtColor::Black = NtColor(0x00000000);
NtColor NtColor::Blue = NtColor(0x0000ff00);
NtColor NtColor::Green = NtColor(0x00ff0000);
NtColor NtColor::Red = NtColor(0xff000000);

NtColor NtColor::operator = (NtVector<unsigned char, 4>&rhs)
{
	return NtColor(rhs[0], rhs[0], rhs[0], rhs[0]);
}

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
	for (int i = 0; i < 2; i++)
	{
		ret[i] = this->operator[](i) + rhs[i];
	}
	ret[3] = A_;
	return ret;
}
void  NtColor::operator +=(const NtColor&rhs)
{
	*this = *(this) + rhs;
}

NtColor NtColor::operator *(const NtVector4& rhs)const
{
	return NtColor(R_*rhs[0], G_*rhs[1], B_*rhs[2], A_*rhs[3]);
}

NtColor float4ToColor(const NtVector4&rhs)
{
	return NtColor(rhs[0] * 255, rhs[1] * 255, rhs[2] * 255, rhs[3] * 255);
}
NtColor OneColor()
{
	return NtColor(1, 1, 1, 1);
}

/*
Tex2D_4F WhitePlain(int W, int H)
{
	Tex2D_4F ret(W, H);
	ret.FillImage(1);
	return ret;
}
*/

Uint32 RGBAToUint32(NtColor4 color)
{

	return color[0] << 24 | color[1] << 16 | color[2] << 8 | color[3];
}

Tex2D_4F RGBAImageToFloat4Image(const Tex2D_UC&rhs)
{
	size_t size = rhs.GetImageSize();
	Tex2D_4F ret(rhs.GetWidth(), rhs.GetHeight());
	for (int i = 0; i < size; i++)
	{
		
		ret.Set(i, RGBAToFloat4(rhs.GetPixel(i)));
	}
	return ret;
}
Tex2D_UC Float4ImageToRGBAImage(const Tex2D_4F&rhs)
{
	size_t size = rhs.GetImageSize();
	Tex2D_UC ret(rhs.GetWidth(), rhs.GetHeight());
	for (int i = 0; i < size; i++)
	{
		ret.Set(i, Float4ToRGBA(rhs.GetPixel(i)));
	}
	return ret;
}
NtVector4 RGBAToFloat4(const NtColor4&rgba)
{
	return NtVector<float, 4>(rgba.r()*over255, rgba.g()*over255, rgba.b()*over255, rgba.a()*over255);
}

NtColor4 Float4ToRGBA(const NtVector4&rhs)
{
	return NtVector<unsigned char, 4>(rhs.r() * 255, rhs.g() * 255, rhs.b() * 255, rhs.a() * 255);
}