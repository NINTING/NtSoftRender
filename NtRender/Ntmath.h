#pragma once

#ifndef NTCOLOR_H
#define NTCOLOR_H



#include<algorithm>
#include<memory>
#include<assert.h>
#include<iostream>
#include<SDL.h>
template<typename T, size_t n, size_t m> class NtMatrix;
class NtColor;
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
NtVector<T, len> operator - (const NtVector<T, len>&rhs)
{
	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		ret[i] = -rhs[i];
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
NtVector<T, len> ComponentMultiply(const NtVector<T, len>& lhs, const NtVector<T, len> rhs)
{

	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		ret[i] = lhs[i] * rhs[i];
	}
	return ret;
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
NtVector<T, len>  operator - (T lhs, const NtVector<T, len>&  rhs)
{
	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		ret[i] = lhs - rhs[i];
	}
	return ret;
}
template<typename  T, std::size_t len>
NtVector<T, len>  operator + (T lhs, const NtVector<T, len>&  rhs)
{
	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		ret[i] = lhs + rhs[i];
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
const NtVector<T, len> operator / (const NtVector<T, len>& lhs, const NtVector<T, len>&  rhs)
{
	NtVector<T, len> ret;
	for (int i = 0; i < len; i++)
	{
		ret[i] = lhs[i] / rhs[i];
	}
	return ret;
}



template<typename  T, std::size_t len>
NtVector<T, len> operator * (const NtVector<T, len>& lhs, const T&  rhs)
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
template<typename  T, std::size_t n, std::size_t m>
NtVector<T, m>  operator * (const NtVector<T, n>& lhs, const NtMatrix<T, n,m>&  rhs)
{
	NtVector<T, len> ret;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
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
template<typename  T,size_t t,size_t n>
void NtVectorTruncation(const NtVector<T, t>& rhs, NtVector<T, n>& out)
{
	assert(t > n);
	
	for (int i = 0; i < n; i++)
		out[i] = rhs[i];
	
}
/*
========================
|	   NtMatrix        |
========================
*/


template<typename T, size_t n,size_t m>
class NtMatrix
{
public:
	NtMatrix(std::initializer_list<NtVector<T, m>>vs)
	{
		auto it = v.begin();
		for (int i = 0; i < n; i++)
			raw[i] = *(it + i);
		
	}
	NtMatrix()
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				if (i == j)
					raw[i][j] = 1;
	};
	NtMatrix(std::initializer_list<T> v)
	{
		auto it = v.begin();
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				raw[i][j] = *(it + i * m + j);
			}
		}
	}
	NtVector<T, n>col(int c)
	{
		NtVector<T, n> ret;
		for (int i = 0; i < n; i++)
		{
			ret[i] = raw[i][c];
		}
		return ret;
	}
	void zero() 
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				raw[i][j] = 0;
	}
	NtVector<T, m> operator[](const size_t i) const { return raw[i]; }
	NtVector<T, m>& operator[](const size_t i) { return raw[i]; }

	NtMatrix&  operator = (const NtMatrix& rhs)
	{
		for (size_t i = 0; i < n; i++)
		{
			raw[i] = rhs[i];
		}
		return *this;
	}

	NtMatrix<T, n - 1, m - 1>get_minor(size_t r,size_t c)const //余子式  用于求行列式
	{
		NtMatrix<T, n - 1, m - 1> ret;
		for (int i = 0; i < m - 1; i++)
			for (int j = 0; j < n - 1; j++)
				ret[i][j] = raw[i < r ? i : i + 1][j < c ? j : j + 1];
		return ret;
	}
	float cofactor(size_t r, size_t c)const//求代数余子式的行列式
	{
		return get_minor(0, i).det()*(i % 2 ? -1 : 1);
	}

	//不讲代数余子式转置，而是留到逆转置矩阵进行转置
	NtMatrix adjugate()const
	{
		NtMatrix<T, n, m>ret;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				ret[i][j] = cofactor(i, j);
		return ret;
	}
	NtMatrix invert_transpose()const
	{
		NtMatrix ret = adjugate();
		//伴随矩阵的每一个元素都是原矩阵的代数余子式的行列式
		//相乘就是原矩阵的行列式
		float tmp = raw[0] * ret[0];

		return ret / tmp;
	}
	NtMatrix invert()const
	{
		return invert_transpose().transpose();
	}

	float det()	const	//求行列式
	{
		float ans = 0;
		for (int i = 0; i < n; i++)
		{
			ans += raw[0][i] * cofactor(0, i);
		}
	}

	NtMatrix<T,m,n> transpose()const
	{
		NtMatrix<T, m, n> ret;
		for (int i = 0; i < m; i++)
		{
			ret[i] = col[i];
		}
		return ret;
	}

	void Print()const
	{
		for (size_t i = 0; i < n; i++)
		{
			raw[i].Print();
		}
	}

private:
	NtVector<T, m> raw[n];
};


template<typename  T, std::size_t row,size_t col0,size_t col1>
NtMatrix<T, row,col1>  operator * (const NtMatrix<T, row,col0>& lhs, const NtMatrix<T, col0,col1>& rhs)
{
	NtMatrix<T, len>ret;
	ret.zero();

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col1; j++) {
			for (int k = 0; k < col0; k++)
				ret[i][j] += lhs[i][k] * rhs[k][j];
		}
	}
	return ret;
}

template<typename  T, std::size_t row, size_t col>
NtMatrix<T, row, col> operator /(const NtMatrix<T, row, col>& lhs, T rhs)
{
	NtMatrix<T, row, col> ret;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			ret[i][j] = lhs[i][j] / rhs;
	}
	return ret;
}


using  NtVector3 = NtVector<float, 3>;
using  NtVector2 = NtVector<float, 2>;
using  NtVector4 = NtVector<float, 4>;

using  NtVector3i = NtVector<int, 3>;
using  NtVector2i = NtVector<int, 2>;
using  NtVector4i = NtVector<int, 3>;

using  NtMatrix2x2 = NtMatrix<float, 2,2>;
using  NtMatrix3x3 = NtMatrix<float, 3,3>;
using  NtMatrix4x4 = NtMatrix<float, 4,4>;
using  NtMatrix2x3 = NtMatrix<float, 2,3>;	//2row ,3col


NtMatrix3x3 NtMatrixRotationAxis(const NtVector3&axis, float angle);

NtMatrix4x4 NtMatrixPerspective(float FovAngleY, float Aspect, float NearZ, float FarZ);

NtMatrix2x3 ComputerTangent(const NtMatrix2x2& TexCoordM, const NtMatrix2x3& SideM);




#endif // !NTCOLOR_H