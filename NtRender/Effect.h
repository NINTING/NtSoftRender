#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include"NtShader.h"
#include<vector>
#include"LightHelper.h"
#include"NtImage.h"
class NtGroudVertexFormat;

/*
========================
|	通用函数      |
========================
*/

float lerp(float a, float b, float t);
template<class T, size_t n>
NtVector<T, n> lerp(NtVector<T, n>& a, NtVector<T, n>& b,float t)
{
	NtVector<T, n> ret;
	for (int i = 0; i < n; i++)
	{
		ret[i] = a[i]*(1 - t) + t * b[i];
	}			
	return ret;
}

template<class T,size_t n>
NtVector<T, n> linearTex2D(const NtImage<T, n>*img,const NtVector2&uv)
{
	
	return img->GetPixel(uv.x(), uv.y());;
}
template<class T, size_t n>
NtVector<T, n> pointTex2D(const NtImage<T, n>*img, const NtVector2&uv)
{
	int x = uv.x()*img->GetWidth()+0.5;
	int y = uv.y()*img->GetHeight()+0.5;
	return img->GetPixel(x, y);
}


NtVector3 Normal(NtVector3&rhs);
NtVector3 Reflect(const NtVector3&in,const NtVector3&normal);
int sign(float);
float saturate(float x);

template<size_t n>
NtVector<float, n> saturate(NtVector<float, n> t)
{
	NtVector<float, n> ret;
	for (int i = 0; i < n; i++)
	{
		ret[i] = saturate(t[i]);
	}
	return ret;
}
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
	NtGroudVertexFormat(const NtVector4& posH, const NtVector3&posW, const NtVector4&color) : NtVertexOutBaseFormat(posH), PosW(posW), Color(color) {}
	NtVector3 PosW;//世界坐标
	NtVector4 Color;
	
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
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
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
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
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


	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtTGroudVertexFormat() = default;
};

class TGroudShader :public NtShader
{
public:
	LightConstant DirctionalLight;
	std::shared_ptr<Tex2D_4F>diffuse  = WhiteTex;
	using sVFptr = NtTGroudVertexFormat::sVFptr;
	using cVFptr = NtTGroudVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
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
	NtVector3 PosW;//世界坐标
	NtVector2 texCoor;
	NtVector3 Normal;
	
};

class PhoneShader :public NtShader
{
public:
	//数据区
	std::vector<LightConstant> DirectionalLights;
	std::vector<LightConstant> PointLights;
	std::vector<LightConstant> SpotLights;
	NtVector4 Ambient;
	std::shared_ptr<Tex2D_4F>diffuseTex = WhiteTex;
	std::shared_ptr<Tex2D_4F>normalTex = BumpTex;
	std::shared_ptr<Tex2D_4F>specularTex = WhiteTex;
	NtVector3 EyePosW;
	MaterialConstant mat;
	
	//方法区
	using sVFptr = NtPhoneVertexFormat::sVFptr;
	using cVFptr = NtPhoneVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
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
	
	NtTPhoneVertexFormat() {};
	NtTPhoneVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH), PosW(posW) {};
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);
	void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtTPhoneVertexFormat() = default;
public:
	NtVector3 PosW;//世界坐标
	NtVector2 texCoor;
	NtVector3 Normal;
	NtVector3 TangentW;

};

class TPhoneShader :public NtShader
{
public:
	//数据区
	std::vector<LightConstant> DirectionalLights;
	std::vector<LightConstant> PointLights;
	std::vector<LightConstant> SpotLights;
	NtVector4 Ambient;
	std::shared_ptr<Tex2D_4F>diffuseTex = WhiteTex;
	std::shared_ptr<Tex2D_4F>tangentTex = BumpTex;
	std::shared_ptr<Tex2D_4F>specularTex = WhiteTex;
	NtVector3 EyePosW;
	MaterialConstant mat;

	//方法区
	using sVFptr = NtTPhoneVertexFormat::sVFptr;
	using cVFptr = NtTPhoneVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
};



/*
========================
|	Shadow Shanding     |
========================
*/
//生成深度图


#define mks_ptrShadow std::make_shared<NtShadowVertexFormat>() 

class NtShadowVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<NtShadowVertexFormat>;
	using VFptr = NtShadowVertexFormat * ;
	using cVFptr = const NtShadowVertexFormat *;
public:
	NtShadowVertexFormat() = default;
	NtShadowVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH), PosW(posW) {}
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);
	void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtShadowVertexFormat() = default;
public:
	NtVector3 PosW;//世界坐标
};

class ShadowShader :public NtShader
{
public:
	//方法区
	using sVFptr = NtShadowVertexFormat::sVFptr;
	using cVFptr = NtShadowVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
};

/*
========================
| STPhone Shanding |
========================
*/
//阴影 法向渲染


#define mks_ptrSTPhone std::make_shared<NtSTPhoneVertexFormat>() 

class NtSTPhoneVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<NtSTPhoneVertexFormat>;
	using VFptr = NtSTPhoneVertexFormat * ;
	using cVFptr = const NtSTPhoneVertexFormat *;
public:
	NtSTPhoneVertexFormat()  {};
	NtSTPhoneVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH), PosW(posW) {}
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);
	void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~NtSTPhoneVertexFormat() = default;
public:
	NtVector3 PosW;//世界坐标
	NtVector2 texCoor;
	NtVector3 Normal;
	NtVector3 TangentW; 
	NtVector3 BitTangentW;
	NtVector4 ShadowTexH;	//保留w，先不进行透视除法，直到采样深度

};

class STPhoneShader :public NtShader
{
public:
	//数据区
	std::vector<LightConstant> DirectionalLights;
	std::vector<LightConstant> PointLights;
	std::vector<LightConstant> SpotLights;
	NtVector4 Ambient;
	std::shared_ptr<Tex2D_4F>diffuseTex =  WhiteTex;
	std::shared_ptr<Tex2D_4F>tangentTex =  BumpTex;
	std::shared_ptr<Tex2D_4F>specularTex =  WhiteTex;
	std::shared_ptr<Tex2D_1F>ShadowTex;
	std::shared_ptr<Tex2D_4F>SSAO_Tex;
	NtVector3 EyePosW;
	MaterialConstant mat;
	NtMatrix4x4 vpt;
	//方法区
	using sVFptr = NtSTPhoneVertexFormat::sVFptr;
	using cVFptr = NtSTPhoneVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
};



/*
========================
|	BlendGround      |
========================
*/



#define mks_ptrBlend std::make_shared<BlendVertexFormat>() 

class BlendVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<BlendVertexFormat>;
	using VFptr = BlendVertexFormat * ;
	using cVFptr = const BlendVertexFormat *;
public:
	BlendVertexFormat() = default;
	BlendVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH), PosW(posW) {}
	NtVector3 PosW;//世界坐标
	NtVector3 NormalW;
	NtVector2 texCoor;

	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~BlendVertexFormat() = default;
};

class BlendShader :public NtShader
{
public:
	LightConstant DirectionalLight;
	NtVector4 Ambient;
	std::shared_ptr<Tex2D_4F>diffuse = WhiteTex;
	float BlendScale = 1 ;
	using sVFptr = BlendVertexFormat::sVFptr;
	using cVFptr = BlendVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
};

/*
========================
|	     WriteNormal    |
========================
*/

#define mks_ptrWriteNormal std::make_shared<WriteNormalVertexFormat>() 

class WriteNormalVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<WriteNormalVertexFormat>;
	using VFptr = WriteNormalVertexFormat * ;
	using cVFptr = const WriteNormalVertexFormat *;
public:
	WriteNormalVertexFormat() = default;
	WriteNormalVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH), PosW(posW) {}
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);
	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~WriteNormalVertexFormat() = default;

public:
	NtVector3 PosW;//世界坐标
	NtVector3 NormalW;
};

class WriteNormalShader :public NtShader
{
public:


	using sVFptr = WriteNormalVertexFormat::sVFptr;
	using cVFptr = WriteNormalVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
};

/*
========================
|	     SSAO          |
========================
*/



#define mks_ptrSSAO std::make_shared<SSAOVertexFormat>() 

class SSAOVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<SSAOVertexFormat>;
	using VFptr = SSAOVertexFormat * ;
	using cVFptr = const SSAOVertexFormat *;
public:
	SSAOVertexFormat() = default;
	SSAOVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH) {}
	
	NtVector3 posV;
	NtVector2 texCoor;

	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~SSAOVertexFormat() = default;
};

class SSAOShader :public NtShader
{
public:

	int sampleCount;
	std::shared_ptr<Tex2D_4F>normalTex;
	std::shared_ptr<Tex2D_1F>depthTex;
	Tex2D_3F* randomNormalTex;
	std::shared_ptr<NtVector4>offsetVec;
	NtMatrix4x4 ptM;	//投影纹理坐标
	float surfaceEpsilon;
	float OcclusionFadeStart, OcclusionFadeEnd;
	float OcclusionRadius;

public:
	using sVFptr = SSAOVertexFormat::sVFptr;
	using cVFptr = SSAOVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);
	float OcclusionFunction(float distZ);
	virtual ~SSAOShader();

};


/*
========================
|	     AmibentSSAO   |
========================
*/



#define mks_ptrAmibentSSAO std::make_shared<AmibentSSAOVertexFormat>() 

class AmibentSSAOVertexFormat :public NtVertexOutBaseFormat
{
public:
	using sVFptr = std::shared_ptr<AmibentSSAOVertexFormat>;
	using VFptr = AmibentSSAOVertexFormat * ;
	using cVFptr = const AmibentSSAOVertexFormat *;
public:
	AmibentSSAOVertexFormat() = default;
	AmibentSSAOVertexFormat(const NtVector4& posH, const NtVector3&posW) : NtVertexOutBaseFormat(posH) {}


	NtVector3 posW;
	
	virtual ptrBase Copy()const;
	virtual ptrBase LineInterpolation(const NtVertexOutBaseFormat* rhs, float t);


	virtual void PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
		float a, float b, float c);
	virtual ~AmibentSSAOVertexFormat() = default;
};

class AmibentSSAOShader :public NtShader
{
public:

	int sampleCount;
	std::shared_ptr<Tex2D_4F>SSAOTex = WhiteTex;
	NtVector4 ambient;
	NtMatrix4x4 vpt;
public:
	using sVFptr = AmibentSSAOVertexFormat::sVFptr;
	using cVFptr = AmibentSSAOVertexFormat::cVFptr;
	virtual ptrBase VertexShader(const NtVertex& vertex);
	virtual NtVector4 PixelShader(const NtVertexOutBaseFormat* pin);


};

#endif // !EFFECT_H
