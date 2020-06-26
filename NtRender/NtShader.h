#pragma once
#include"Ntmath.h"

class NtVertexOutBaseFormat;

#define WhiteTex std::make_shared<Tex2D_4F>(White2D);
#define BumpTex std::make_shared<Tex2D_4F>(Bump2D);

#define mks_ptrBase std::make_shared<NtVertexOutBaseFormat>()
#define ptrBase std::shared_ptr<NtVertexOutBaseFormat> 

class  NtVertex;
class NtVertexOutBaseFormat
{
public:
	NtVertexOutBaseFormat() = default;
		NtVertexOutBaseFormat(const NtVector4& posH) :PosH(posH) {}
	NtVector4 PosH;

	virtual ptrBase Copy()const = 0;
	virtual ptrBase  LineInterpolation(const NtVertexOutBaseFormat* rhs, float t) = 0;
	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c) = 0;
	virtual ~NtVertexOutBaseFormat() = default;
	
};

class NtShader
{
public:

	NtMatrix4x4 w;
	NtMatrix4x4 v;
	NtMatrix4x4 p;

	virtual ptrBase VertexShader(const NtVertex& vertex) = 0;
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat*) = 0;
	virtual ~NtShader() = default;
};




