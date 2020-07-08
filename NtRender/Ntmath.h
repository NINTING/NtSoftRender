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

//ģ�����  ֻ��Ϊ��ʵ������Ĺ��ܣ���ȡ��ͬ���֡�����is-a�Ĺ�ϵ
template<typename T>
class NtVectorBase
{
public:

	T operator [](const size_t i )const { 
		assert(i < size_); return pData[i]; 
	}
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
		if (sum == 0)return;
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
class  NtVector :public NtVectorBase<T> {
public:
	

};
template<typename T>
class  NtVector<T, 1> :public NtVectorBase<T>
{
public:
	NtVector operator()(T i) 
	{
		return NtVector(i);
	}

	T operator()(NtVector i) { return raw[0]; }

	NtVector(T X) :NtVectorBase<T>(1, raw) {
		raw[0] = X; 

	}
	NtVector() :NtVectorBase<T>(1, raw) {}
	void normalize() { NtVectorBase<T>::normalize(); }

	NtVector(const NtVector &rhs) :NtVectorBase<T>(2, raw) { raw[0] = rhs.x();  }
	NtVector& operator = (const NtVector&  rhs)
	{
		x(rhs.x());
		return *this;
	}
	T x() const { return raw[0]; }	
	void x(T x) { raw[0] = x; }
	T raw[1] = { 0 };
};

template<typename T>
class  NtVector<T, 2> :public NtVectorBase<T>
{
public:
	NtVector(T X, T Y = 0) :NtVectorBase<T>(2,raw) {
		raw[0] = X;  raw[1] = Y;
	}
	NtVector() :NtVectorBase<T>(2,raw) {}
	void normalize() { NtVectorBase<T>::normalize(); }

	NtVector(const NtVector &rhs) :NtVectorBase<T>(2,raw) { raw[0] = rhs.x();  raw[1] = rhs.y(); }
	NtVector& operator = (const NtVector&  rhs)
	{
		//x(rhs.x());
		//y(rhs.y());
		memcpy(raw, rhs.raw, 2 * sizeof(T));
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
	NtVector(T X, T Y = 0, T Z = 0) :NtVectorBase<T>(3,raw) {
		raw[0] = X;  raw[1] = Y;; raw[2] = Z;
	}
	NtVector() :NtVectorBase<T>(3,raw) {}
	void normalize() { NtVectorBase<T>::normalize(); }

	NtVector(const NtVector &rhs) :NtVectorBase<T>(3,raw) { raw[0] = rhs.x();  raw[1] = rhs.y(); raw[2] = rhs.z();}
	NtVector& operator = (const NtVector&  rhs)
	{
		//x(rhs.x());
		//y(rhs.y());
		//z(rhs.z());
		memcpy(raw, rhs.raw, 3* sizeof(T));
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

	NtVector(const NtVector<T, 3> &rhs, T w) : NtVectorBase<T>(4, raw) {
		raw[0] = rhs.x();  raw[1] = rhs.y(); raw[2] = rhs.z(); raw[3] = w;
	}
	NtVector(T X, T Y, T Z, T W) :NtVectorBase<T>(4, raw) { raw[0] = X;  raw[1] = Y; raw[2] = Z; raw[3] = W; }
	NtVector() :NtVectorBase<T>(4, raw) {}
	NtVector(const NtVector &rhs) :NtVectorBase<T>(4, raw) {
		raw[0] = rhs.x();  raw[1] = rhs.y(); raw[2] = rhs.z(); raw[3] = rhs.w();
	}

	void normalize() { NtVectorBase<T>::normalize(); }

	NtVector& operator = (const NtVector&  rhs)
	{
		//	x (rhs.x());
		//	y (rhs.y());
		//	z (rhs.z());
		//	w (rhs.w());
		memcpy(raw, rhs.raw, 4 * sizeof(T));
		return *this;
	}
	void Perspective()
	{
		x(raw[0] / raw[3]);
		y(raw[1] / raw[3]);
		z(raw[2] / raw[3]);
		w(1);
	}
	T raw[4] = { 0 };

	T x() const { return raw[0]; }
	T y()const { return raw[1]; }
	T z() const { return raw[2]; }
	T w() const { return raw[3]; }

	T r() const { return raw[0]; }
	T g()const { return raw[1]; }
	T b() const { return raw[2]; }
	T a() const { return raw[3]; }

	NtVector<T, 3>xyz()const { return NtVector<T, 3>(x(), y(), z()); }
	void xyz(NtVector<T, 3> xyz) { x(xyz.x()); y(xyz.y()); z(xyz.z()); }
	void x(T x) { raw[0] = x; }
	void y(T y) { raw[1] = y; }
	void z(T z) { raw[2] = z; }
	void w(T w) { raw[3] = w; }

	void r(T x) { raw[0] = x; }
	void g(T y) { raw[1] = y; }
	void b(T z) { raw[2] = z; }
	void a(T w) { raw[3] = w; }
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
	NtVector<T, m> ret;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			float a = lhs[j];
			float b = rhs[j][i];
			ret[i] += (lhs[j] * rhs[j][i]);

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
		auto it = vs.begin();
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
	NtVector<T, n>col(int c)const
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

	NtMatrix<T, n - 1, m - 1>get_minor(size_t r,size_t c)const //����ʽ  ����������ʽ
	{	

		NtMatrix<T, n - 1, m - 1> ret;

		for (int i = 0; i < n - 1; i++)
			for (int j = 0; j < m - 1; j++)
				ret[i][j] = raw[i < r ? i : i + 1][j < c ? j : j + 1];
		return ret;
	}
	float cofactor(size_t r, size_t c)const//���������ʽ������ʽ
	{

		return get_minor(r, c).det()*((r+c) % 2 ? -1 : 1);
	}

	//������������ʽת�ã�����������ת�þ������ת��
	NtMatrix adjugate()const
	{
		NtMatrix<T, n, m>ret;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				ret[i][j] = cofactor(i, j);
		return ret;
	}
	float det()	const	//������ʽ
	{
		

		float ans = 0;
		for (int i = 0; i < m; i++)
		{
			ans += raw[0][i] * cofactor(0, i);
		}
		return ans;
	}
	NtMatrix invert_transpose()const
	{
		NtMatrix ret = adjugate();
		//��������ÿһ��Ԫ�ض���ԭ����Ĵ�������ʽ������ʽ
		//��˾���ԭ���������ʽ
		float tmp = raw[0] * ret[0];

		return ret / tmp;
	}
	NtMatrix invert()const
	{
		return invert_transpose().transpose();
	}

	

	NtMatrix<T,m,n> transpose()const
	{
		NtMatrix<T, m, n> ret;
		for (int i = 0; i < m; i++)
		{
			ret[i] = col(i);
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


template<typename T>
class NtMatrix<T,1, 1>
{
public:
	NtMatrix()
	{
		raw[0][0] = 1;
	};
	NtVector<T, 1> operator[](const size_t i) const { return raw[i]; }
	NtVector<T, 1>& operator[](const size_t i) { return raw[i]; }
	float det()	const	//������ʽ
	{
		return raw[0][0];
	}
private:
	NtVector<T, 1> raw[1];
};

template<typename  T, std::size_t row,size_t col0,size_t col1>
NtMatrix<T, row,col1>  operator * (const NtMatrix<T, row,col0>& lhs, const NtMatrix<T, col0,col1>& rhs)
{
	NtMatrix<T, row, col1>ret;
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



using  NtVector1 = NtVector<float, 1>;
using  NtVector3 = NtVector<float, 3>;
using  NtVector2 = NtVector<float, 2>;
using  NtVector4 = NtVector<float, 4>;
using  NtColor4 = NtVector<unsigned char, 4>;

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
NtMatrix4x4 NtMatrixOrthogonal(float Width, float Height, float NearZ, float FarZ);

NtColor4 Uint32ToRGBA(Uint32 t);

NtMatrix4x4 Transiation(float x, float y, float z);
#endif // !NTCOLOR_H