#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include"NtShader.h"

class NtGroudVertexFormat;

/*
========================
|	sample Ground      |
========================
*/

#define mks_ptrGroud std::make_shared<NtGroudVertexFormat>() 


class NtGroudVertexFormat :public NtVertexOutBaseFormat
{
	
public:
	using sVFptr = std::shared_ptr<NtGroudVertexFormat>;
	using VFptr = NtGroudVertexFormat * ;
	using cVFptr =const NtGroudVertexFormat * ;
	NtGroudVertexFormat() = default;
	NtGroudVertexFormat(const NtVector4& posH, const NtVector3&posW, const NtColor&color) : NtVertexOutBaseFormat(posH), PosW(posW), Color(color) {}
	NtVector3 PosW;//世界坐标
	NtColor Color;
	
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtGroudVertexFormat() = default;
};

class GroudShader :public NtShader
{
	
public:
	using sVFptr = NtGroudVertexFormat::sVFptr;
	using cVFptr = NtGroudVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtColor PixelShader(const NtVertexOutBaseFormat* pin);
};



/*
========================
|	  Cartoon   Ground |
========================
*/

#define mks_ptrCGroud std::make_shared<NtCGroudVertexFormat>() 

class NtCGroudVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<NtCGroudVertexFormat>;
	using VFptr = NtCGroudVertexFormat * ;
	using cVFptr = const NtCGroudVertexFormat *;
public:
	NtCGroudVertexFormat() = default;
	NtCGroudVertexFormat(const NtVector4& posH, const NtVector3&posW, const float  i) : NtVertexOutBaseFormat(posH), PosW(posW), intensity(i){}
	NtVector3 PosW;//世界坐标
	float intensity;
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtCGroudVertexFormat() = default;
};

class CGroudShader :public NtShader
{
public:
	using sVFptr = NtCGroudVertexFormat::sVFptr;
	using cVFptr = NtCGroudVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtColor PixelShader(const NtVertexOutBaseFormat* pin);
};





/*
========================
|	TextureGround      |
========================
*/



#define mks_ptrTGroud std::make_shared<NtTGroudVertexFormat>() 

class NtTGroudVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<NtTGroudVertexFormat>;
	using VFptr = NtTGroudVertexFormat * ;
	using cVFptr = const NtTGroudVertexFormat *;
public:
	NtTGroudVertexFormat() = default;
	NtTGroudVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH), PosW(posW) {}
	NtVector3 PosW;//世界坐标
	NtVector2 texCoor;
	float intensity;
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtTGroudVertexFormat() = default;
};

class TGroudShader :public NtShader
{
public:
	using sVFptr = NtTGroudVertexFormat::sVFptr;
	using cVFptr = NtTGroudVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtColor PixelShader(const NtVertexOutBaseFormat* pin);
};










#endif // !EFFECT_H
