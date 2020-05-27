#pragma once
#include"Ntmath.h"

class NtVertexOutBaseFormat;

#define mks_ptrBase std::make_shared<NtVertexOutBaseFormat>()
#define ptrBase std::shared_ptr<NtVertexOutBaseFormat> 

class  NtVertex;
class NtVertexOutBaseFormat
{
public:
	NtVertexOutBaseFormat() = default;
	NtVertexOutBaseFormat(const NtVector4& posH) :PosH(posH) {}
	NtVector4 PosH;
	virtual ptrBase  LineInterpolation(const NtVertexOutBaseFormat* rhs, float t) = 0;
	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c) = 0;
	virtual ~NtVertexOutBaseFormat() = default;
	virtual ptrBase Copy()const = 0;
};

class NtShader
{
public:

	NtMatrix4x4 w;
	NtMatrix4x4 v;
	NtMatrix4x4 p;

	virtual ptrBase VertexShader(const NtVertex& vertex) = 0;
	virtual NtColor PixelShader(const NtVertexOutBaseFormat*) = 0;
};

