#pragma once

#ifndef NTCOLOR_H
#define NTCOLOR_H



#include<algorithm>
#include<memory>
#include<assert.h>
#include<iostream>
#include<SDL.h>
template<typename T, size_t n> class NtMatrix;

/*
========================
|	   NtVector        |
========================
*/

//模板基类  只是为了实现子类的功能，抽取共同部分。不是is-a的关系
template<typename T>
class NtVectorBase
{
public:

	T operator [](const size_t i )const { assert(i < size_); return pData[i]; }
	T& operator [](const size_t i) { assert(i < size_); return pData[i]; }
	void Print()const
	{
		for (size_t i = 0; i < size_; i++)
		{
			std::cout << this->operator[](i) << " ";
		}
		std::cout << std::endl;
	}
	void zero()
	{
		for (int i = 0; i < size_; i++)
			pData[i] = 0;
	}
protected:
	NtVectorBase(size_t size, T *data)
	{
		size_ = size;
		pData = data;
	}
	void normalize()
	{
		T sum = 0;
		for (size_t i = 0; i < size_; i++)
		{
			T a = this->operator[](i);
			sum += a*a;
		}
		sum = sqrt(sum);

		for (size_t i = 0; i < size_; i++)
		{
			this->operator[](i) /= sum;
		}
	};


private:
	size_t size_;
	T* pData;
};

template<typename T, size_t n>
class  NtVector :public NtVectorBase<T> {};

template<typename T>
class  NtVector<T, 2> :public NtVectorBase<T>
{
public:
	NtVector(T X, T Y) :NtVectorBase<T>(2,raw) {
		raw[0] = X;  raw[1] = Y;
	}
	NtVector() :NtVectorBase<T>(2,raw) {}
	void normalize() { NtVectorBase<T>::normalize(); }

	NtVector(const NtVector &rhs) :NtVectorBase<T>(2) { raw[0] = rhs.x();  raw[1] = rhs.y(); }
	NtVector& operator = (const NtVector&  rhs)
	{
		x(rhs.x());
		y(rhs.y());
		
		return *this;
	}
	T x() const{ return raw[0]; }
	T y() const { return raw[1]; }
	void x(T x) { raw[0] = x; }
	void y(T y) { raw[1] = y; }
	T raw[2] = { 0 };
};

template<typename T>
class  NtVector<T, 3> :public NtVectorBase<T>
{
public:
	NtVector(T X, T Y, T Z) :NtVectorBase<T>(3,raw) {
		raw[0] = X;  raw[1] = Y;; raw[2] = Z;
	}
	NtVector() :NtVectorBase<T>(3,raw) {}
	void normalize() { NtVectorBase<T>::normalize(); }

	NtVector(const NtVector &rhs) :NtVectorBase<T>(3,raw) { raw[0] = rhs.x();  raw[1] = rhs.y(); raw[2] = rhs.z();}
	NtVector& operator = (const NtVector&  rhs)
	{
		x(rhs.x());
		y(rhs.y());
		z(rhs.z());
	
		return *this;
	}

	T raw[3] = { 0 };
	T x() const { return raw[0]; }
	T y() const { return raw[1]; }
	T z() const { return raw[2]; }
	void x(T x) { raw[0] = x; }
	void y(T y) { raw[1] = y; }
	void z(T z) { raw[2] = z; }
};

template<typename T>
class  NtVector<T, 4> :public NtVectorBase<T>
{
public:
	NtVector(T X, T Y, T Z, T W) :NtVectorBase<T>(4,raw) { raw[0] = X;  raw[1] = Y; raw[2] = Z; raw[3] = W; }
	NtVector() :NtVectorBase<T>(4,raw) {}
	NtVector(const NtVector &rhs) :NtVectorBase<T>(4,raw) { 
		raw[0] = rhs.x();  raw[1] = rhs.y(); raw[2] = rhs.z(); raw[3] = rhs.w();
	}
	void normalize() { NtVectorBase<T>::normalize(); }
	NtVector& operator = (const NtVector&  rhs)
	{
		x (rhs.x());
		y (rhs.y());
		z (rhs.z());
		w (rhs.w());
		return *this;
	}
	void Perspective()
	{
		x(raw[0]/raw[3]);
		y(raw[1]/raw[3]);
		z(raw[2]/raw[3]);
		w(1);
	}
	T raw[4] = {0};

	T x() const { return raw[0]; }
	T y()const { return raw[1];  }
	T z() const { return raw[2]; }
	T w() const { return raw[3]; }
	void x(T x) { raw[0] = x; }
	void y(T y) { raw[1] = y; }
	void z(T z) { raw[2] = z; }
	void w(T w) { raw[3] = w; }
};


template<typename T, std::size_t len>
NtVector<T, len>operator + (const NtVector<T, len>& lhs, const NtVector<T, len>&  rhs)
{
	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		ret[i] = lhs[i] + rhs[i];
	}
	return ret;
}

template<typename  T, std::size_t len>
void operator += (NtVector<T, len>& lhs, const NtVector<T, len>&  rhs)
{
	for (int i = 0; i < len; i++)
	{
		lhs[i] += rhs[i];
	}
}

template<typename  T, std::size_t len>
NtVector<T, len>  operator - (const NtVector<T, len>& lhs, const NtVector<T, len>&  rhs)
{
	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		ret[i] = lhs[i] - rhs[i];
	}
	return ret;
}

template<typename  T, std::size_t len>
void operator -= (NtVector<T, len>& lhs, const NtVector<T, len>&  rhs)
{
	for (int i = 0; i < len; i++)
	{
		lhs[i] -= rhs[i];
	}
}

template<typename  T, std::size_t len>
T  operator * (const NtVector<T, len>& lhs, const NtVector<T, len>&  rhs)
{
	T ret = 0;
	for (int i = 0; i < len; i++)
	{
		ret += lhs[i] * rhs[i];
	}
	return ret;
}

template<typename  T, std::size_t len>
NtVector<T, len>  operator * (const NtVector<T, len>& lhs, T rhs)
{

	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		ret[i] = lhs[i] * rhs;
	}
	return ret;
}

template<typename  T, std::size_t len>
void  operator *= (NtVector<T, len>& lhs, T rhs)
{

	for (int i = 0; i < len; i++)
	{
		lhs[i] *= rhs;
	}

}
template<typename  T>
NtVector<T, 3> Cross(const NtVector<T, 3>& lhs, const NtVector<T, 3>&  rhs)
{
	NtVector<T, 3> ret;

	return NtVector<T, 3>(lhs.y()*rhs.z() - lhs.z()*rhs.y(), lhs.z()*rhs.x() - lhs.x()*rhs.z(), lhs.x()*rhs.y() - rhs.x()*lhs.y());
}
template<typename  T, std::size_t len>
NtVector<T, len>  operator * (const NtVector<T, len>& lhs, const NtMatrix<T, len>&  rhs)
{
	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			ret[i] += lhs[j] * rhs[j][i];
		}
	}
	return ret;
}
template<typename  T>
NtVector<T, 3> NtVector4To3(const NtVector<T, 4>& rhs)
{
	return NtVector<T, 3>(rhs.x(), rhs.y(), rhs.z());
}

template<typename  T>
NtVector<T, 4> NtVector3To4(const NtVector<T, 3>& rhs,T w = 0)
{
	return NtVector<T, 4>(rhs.x(), rhs.y(), rhs.z(), w);
}
template<typename  T>
NtVector<T, 2> NtVector3To2(const NtVector<T, 3>& rhs)
{
	return NtVector<T, 2>(rhs.x(), rhs.y());
}

/*
========================
|	   NtMatrix        |
========================
*/


template<typename T, size_t n>
class NtMatrix
{
public:
	NtMatrix()
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (i == j)
					raw[i][j] = 1;
	};
	NtMatrix(std::initializer_list<T> v)
	{
		auto it = v.begin();
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				raw[i][j] = *(it + i * n + j);
			}
		}
	}

	void zero() {
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				raw[i][j] = 0;
	}
	NtVector<T, n> operator[](const size_t i) const { return raw[i]; }
	NtVector<T, n>& operator[](const size_t i) { return raw[i]; }

	NtMatrix&  operator = (const NtMatrix& rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			raw[i] = rhs[i];
		}
		return *this;
	}
	void Print()const
	{
		for (size_t i = 0; i < n; i++)
		{
			raw[i].Print();
		}
	}
private:
	NtVector<T, n> raw[n];
};


template<typename  T, std::size_t len>
NtMatrix<T, len>  operator * (const NtMatrix<T, len>& lhs, const NtMatrix<T, len>& rhs)
{
	NtMatrix<T, len>ret;
	ret.zero();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++)
				ret[i][j] += lhs[i][k] * rhs[k][j];
		}
	}
	return ret;
}

using  NtVector3 = NtVector<float, 3>;
using  NtVector2 = NtVector<float, 2>;
using  NtVector4 = NtVector<float, 4>;

using  NtVector3i = NtVector<int, 3>;
using  NtVector2i = NtVector<int, 2>;
using  NtVector4i = NtVector<int, 3>;

using  NtMatrix3x3 = NtMatrix<float, 3>;
using  NtMatrix4x4 = NtMatrix<float, 4>;


NtMatrix3x3 NtMatrixRotationAxis(const NtVector3&axis, float angle);

NtMatrix4x4 NtMatrixPerspective(float FovAngleY, float Aspect, float NearZ, float FarZ);




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
	NtColor operator +(const NtColor&rhs)const;
	void operator +=(const NtColor&rhs);
	Uint32 GetColor32()const { return  (R_ << 24) | (G_ << 16) | (B_ << 8) | A_; }
private:
	unsigned char R_, G_, B_, A_;
	//Uint32 ColorPack_;
};


#endif // !NTCOLOR_H