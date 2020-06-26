#pragma once
#ifndef NTCUBETEX
#define NTCUBETEX



#include"NtImage.h"
template<class T,size_t bbp>
class NtCube
{
	
public:
	enum Cubeface
	{
		POSITIVE_X = 3,
		NEGATIVE_X = 1,
		POSITIVE_Z = 0,
		NEGATIVE_Z = 2,
		POSITIVE_Y = 5,
		NEGATIVE_Y = 4,
	};
	NtCube() = default;
	NtCube(std::shared_ptr<NtImage<T, bbp>> Imgs[6])
	{
		for (int i = 0; i < 6; i++)
			imgs[i] = Imgs[i];
	}
	NtCube(std::initializer_list<std::shared_ptr<NtImage<T, bbp>>> Imgs)
	{
	
		for (int i = 0; i < Imgs.size(); i++)
			imgs[i] = Imgs[i];
	}
	void Setface(std::shared_ptr<NtImage>&img, int face)
	{
		imgs[face] = img;
	}
private:
	static void xMainAixs(float x,float y,float z,float& s, float &t, float&m)
	{
		m = x;
		s = x > 0 ? -z : z;
		t = -y;
	}
	static void zMainAixs(float x, float y, float z, float& s, float &t, float&m)
	{
		m = z;
		s = z > 0 ? x : -x;
		t = -y;
	}
	static void yMainAixs(float x, float y, float z, float& s, float &t, float&m)
	{
		m = y;
		s = x;
		t = y>0?z:-z;
	}
	NtVector<T,bbp> sample(float x,float y,float z)
	{
		float absx = abs(x);
		float absy = abs(y);
		float absz = abs(z);
		int index;
		float u, v;
		float m, s, t;
		if (absx >= absy&&absx>=absz)
		{
			xMainAixs(x, y, z, s, t, m);
			index = x > 0 ? POSITIVE_X : NEGATIVE_X;
		}
		if (absy >= absx && absy >= absz)
		{
			yMainAixs(x, y, z, s, t, m);
			index = y > 0 ? POSITIVE_Y : NEGATIVE_Y;
		}
		else 
		{
			zMainAixs(x, y, z, s, t, m);
			index = z > 0 ? POSITIVE_Z : NEGATIVE_Z;
		}
		float absm = abs(m);
		u = 0.5*(s / abs(m) + 1);
		v = 0.5*(t / abs(m) + 1);
		return imgs[index]->GetPixel(u, v);
	}

private:
	//0:-z,1:-x,2:z,3:x,4:-y:5:y
	std::shared_ptr<NtImage<T, bbp>> imgs[6];
};


#endif // NTCUBETEX