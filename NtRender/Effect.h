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
	NtVector3 PosW;//��������
	NtColor Color;
	
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtGroudVertexFormat() = default;
};

class GroudShader :public NtShader
{
	
public:
	LightConstant DirctionalLight;
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
	NtVector3 PosW;//��������
	float intensity;
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtCGroudVertexFormat() = default;
};

class CGroudShader :public NtShader
{
public:
	LightConstant DirctionalLight;
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
	NtVector3 PosW;//��������
	NtVector2 texCoor;
	float intensity;
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtTGroudVertexFormat() = default;
};

class TGroudShader :public NtShader
{
public:
	LightConstant DirctionalLight;
	std::shared_ptr<NtImage>diffuse;
	using sVFptr = NtTGroudVertexFormat::sVFptr;
	using cVFptr = NtTGroudVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtColor PixelShader(const NtVertexOutBaseFormat* pin);
};





/*
========================
|	Phone Shanding      |
========================
*/



#define mks_ptrPhone std::make_shared<NtPhoneVertexFormat>() 

class NtPhoneVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<NtPhoneVertexFormat>;
	using VFptr = NtPhoneVertexFormat * ;
	using cVFptr = const NtPhoneVertexFormat *;
public:
	NtPhoneVertexFormat() = default;
	NtPhoneVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH), PosW(posW) {}
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);
	void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtPhoneVertexFormat() = default;
public:
	NtVector3 PosW;//��������
	NtVector2 texCoor;
	NtVector3 Normal;
	
};

class PhoneShader :public NtShader
{
public:
	//������
	std::vector<LightConstant> DirectionalLights;
	std::vector<LightConstant> PointLights;
	std::vector<LightConstant> SpotLights;
	NtVector4 Ambient;
	std::shared_ptr<NtImage>diffuseTex;
	std::shared_ptr<NtImage>normalTex;
	std::shared_ptr<NtImage>specularTex;
	NtVector3 EyePosW;
	MaterialConstant mat;
	
	//������
	using sVFptr = NtPhoneVertexFormat::sVFptr;
	using cVFptr = NtPhoneVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtColor PixelShader(const NtVertexOutBaseFormat* pin);
};



/*
========================
|	TangentPhone Shanding      |
========================
*/



#define mks_ptrTPhone std::make_shared<NtTPhoneVertexFormat>() 

class NtTPhoneVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<NtTPhoneVertexFormat>;
	using VFptr = NtTPhoneVertexFormat * ;
	using cVFptr = const NtTPhoneVertexFormat *;
public:
	NtTPhoneVertexFormat() = default;
	NtTPhoneVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH), PosW(posW) {}
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);
	void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtTPhoneVertexFormat() = default;
public:
	NtVector3 PosW;//��������
	NtVector2 texCoor;
	NtVector3 Normal;
	NtVector3 TangentW;

};

class TPhoneShader :public NtShader
{
public:
	//������
	std::vector<LightConstant> DirectionalLights;
	std::vector<LightConstant> PointLights;
	std::vector<LightConstant> SpotLights;
	NtVector4 Ambient;
	std::shared_ptr<NtImage>diffuseTex;
	std::shared_ptr<NtImage>tangentTex;
	std::shared_ptr<NtImage>specularTex;
	NtVector3 EyePosW;
	MaterialConstant mat;

	//������
	using sVFptr = NtTPhoneVertexFormat::sVFptr;
	using cVFptr = NtTPhoneVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtColor PixelShader(const NtVertexOutBaseFormat* pin);
};






#endif // !EFFECT_H
