
#include"NtRender.h"
#include"Effect.h"
#include<cstdbool>
#include"LightHelper.h"
#include"NtImage.h"
extern std::shared_ptr<NtSofterRender> render;


float lerp(float a,float b,float t)
{
	return a + (b - a)*t;
}

/*
========================
|	  Sample   Ground   |
========================
*/

ptrBase NtGroudVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrGroud;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	ret->Color = this->Color;
	return ret;
}
ptrBase NtGroudVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrGroud;
	cVFptr  tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH* t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	ret->Color = tmpR->Color* t + this->Color* vt;
	return ret;
}

void NtGroudVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b,float c)
{
	
	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);
	
	
	this->PosW = tmpA->PosW* a + tmpB->PosW*b + tmpC->PosW*c;
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;

	this->Color = tmpA->Color *t0 + tmpB->Color * t1 + tmpC->Color * t2;

}

 

 ptrBase GroudShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrGroud;
	
	NtVector3 Light_dir = DirctionalLight.Direction;
	ret->PosW = vertex.GetPostion();
	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);

	ret->PosH = ret->PosH *  w * v;
	//ret->PosW = ret->PosH;
	ret->PosH = ret->PosH * p;
	NtVector4 nor = NtVector3To4(vertex.GetNormal());
	nor = nor * w;
	NtVector3 tn(nor.x(), nor.y(), nor.z());

	float intensity = tn * Light_dir;
	intensity = intensity < 0 ? 0 : intensity;

	ret->Color = NtColor(255, 255, 255)* intensity;
	return ret;
}
NtColor GroudShader::PixelShader(const NtVertexOutBaseFormat* pin)
{	
	return dynamic_cast<cVFptr> (pin)->Color;
}




/*
========================
|	  Cartoon   Ground   |
========================
*/

ptrBase NtCGroudVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrCGroud;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	ret->intensity = this->intensity;
	return ret;
}
ptrBase NtCGroudVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrCGroud;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH* t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	ret->intensity = tmpR->intensity * t + this->intensity * vt;
	return ret;
}

void NtCGroudVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	this->PosW = tmpA->PosW* a + tmpB->PosW*b + tmpC->PosW*c;
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;

	this->intensity = tmpA->intensity *t0 + tmpB->intensity * t1 + tmpC->intensity * t2;

}



ptrBase CGroudShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrCGroud;

	NtVector3 Light_dir = DirctionalLight.Direction;
	ret->PosW = vertex.GetPostion();
	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);

	ret->PosH = ret->PosH *  w * v;
	//ret->PosW = ret->PosH;
	ret->PosH = ret->PosH * p;
	NtVector4 nor = NtVector3To4(vertex.GetNormal());
	nor = nor * w;
	NtVector3 tn(nor.x(), nor.y(), nor.z());

	float intensity = tn * Light_dir;
	intensity = intensity < 0 ? 0 : intensity;

	ret->intensity = intensity;
	return ret;
}
NtColor CGroudShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	NtColor color(255, 255, 0);
	float i = dynamic_cast<cVFptr> (pin)->intensity;
	if (i > 0.8) 
		i = 1;
	else if (i > 0.6) 
		i = 0.8;
	else if (i > 0.4) 
		i = 0.6;
	else if (i > 0.2) 
		i = 0.4;
	

	color *= i;
	return color;
}




/*
========================
|	 Texture   Ground   |
========================
*/

ptrBase NtTGroudVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrTGroud;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	ret->texCoor[0] = this->texCoor[0];
	ret->texCoor[1] = this->texCoor[1];
	ret->intensity = this->intensity;
	return ret;
}
ptrBase NtTGroudVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrTGroud;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH*  t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	ret->texCoor[0] = tmpR->texCoor[0] * t + this->texCoor[0] * vt;
	ret->texCoor[1] = tmpR->texCoor[1] * t + this->texCoor[1] * vt;
	ret->intensity = tmpR->intensity * t + this->intensity * vt;
	return ret;
}

void NtTGroudVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	this->PosW = tmpA->PosW* a + tmpB->PosW*b + tmpC->PosW*c;
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->intensity = tmpA->intensity *t0 + tmpB->intensity * t1 + tmpC->intensity * t2;
	this->texCoor[0] = tmpA->texCoor[0] *t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;

}



ptrBase TGroudShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrTGroud;

	NtVector3 Light_dir = DirctionalLight.Direction;
	ret->PosW = vertex.GetPostion();
	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);

	ret->PosH = ret->PosH *  w * v;
	//ret->PosW = ret->PosH;
	ret->PosH = ret->PosH * p;
	NtVector4 nor = NtVector3To4(vertex.GetNormal());
	nor = nor * w;
	NtVector3 tn(nor.x(), nor.y(), nor.z());

	float intensity = tn * Light_dir;
	intensity = intensity < 0 ? 0 : intensity;
	ret->texCoor = NtVector3To2(vertex.GetDiffuse());
	ret->intensity = intensity;
	return ret;
}
NtColor TGroudShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	
	
	cVFptr tmp = dynamic_cast<cVFptr> (pin);
	float i = tmp->intensity;
	NtColor color = diffuse->GetPixel(tmp->texCoor[0], tmp->texCoor[1]);
	color *= i;
	return color;
}




/*
========================
|	 Phone Shanding   |
========================
*/

ptrBase NtPhoneVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrPhone;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	ret->texCoor[0] = this->texCoor[0];
	ret->texCoor[1] = this->texCoor[1];
	ret->Normal = this->Normal;
	return ret;
}
ptrBase NtPhoneVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrPhone;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH* t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	ret->texCoor[0] = tmpR->texCoor[0] * t + this->texCoor[0] * vt;
	ret->texCoor[1] = tmpR->texCoor[1] * t + this->texCoor[1] * vt;
	ret->Normal = tmpR->Normal * t + this->Normal * vt;
	return ret;
}

void NtPhoneVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	this->PosW = tmpA->PosW* a + tmpB->PosW*b + tmpC->PosW * c;
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->Normal = tmpA->Normal *t0 + tmpB->Normal * t1 + tmpC->Normal * t2;
	this->texCoor[0] = tmpA->texCoor[0] * t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;

}



ptrBase PhoneShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrPhone;
	
	ret->PosW = vertex.GetPostion();
	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);

	ret->PosH = ret->PosH *  w * v;
	//ret->PosW = ret->PosH;
	ret->PosH = ret->PosH * p;
	NtVector4 nor = NtVector3To4(vertex.GetNormal());
	nor = nor * w;
	
	ret->texCoor = NtVector3To2(vertex.GetDiffuse());
	NtVectorTruncation(nor, ret->Normal);
	return ret;
}
NtColor PhoneShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	cVFptr in = dynamic_cast<cVFptr> (pin);
	NtVector3 ToEye = EyePosW - in->PosW;
	ToEye.normalize();
	//获得材质
	
	//计算漫反射
	NtVector4& amb = Ambient;

	//漫反射纹理归一化
	NtVector4 diffuseAlpdeo = diffuseTex->diffuse(in->texCoor[0], in->texCoor[1]);

	NtVector4 AmbientLit = ComponentMultiply(amb , diffuseAlpdeo);
	
	float Roughness = specularTex->spec(in->texCoor[0], in->texCoor[1]);
	MaterialConstant Pmat(diffuseAlpdeo, mat.FresnelR0, 1-Roughness);

	NtVector3 normal = normalTex->normal(in->texCoor[0], in->texCoor[1]);
	normal.normalize();
	//计算光照
	NtVector4 Light = ComputeLight(DirectionalLights, in->PosW, normal, ToEye, Pmat);
	NtVector4 Colorf = Light + AmbientLit;
	NtColor color32 = float4ToColor(Colorf);

	return color32;
}




/*
==============================
|	Tangent Phone Shanding   |
==============================
*/

NtVector3 TangentToNormalW(const NtVector3& tangent, const NtVector3& TangentW, const NtVector3& N)
{
	NtVector3 T = TangentW - N*(TangentW*N);
	T.normalize();
	NtVector3 B = Cross(N, T);
	NtMatrix3x3 TBN({ T,B,N });
	return tangent * TBN;
}

ptrBase NtTPhoneVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrTPhone;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	ret->texCoor[0] = this->texCoor[0];
	ret->texCoor[1] = this->texCoor[1];
	ret->Normal = this->Normal;
	ret->TangentW = this->TangentW;
	return ret;
}
ptrBase NtTPhoneVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrTPhone;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH* t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	ret->texCoor[0] = tmpR->texCoor[0] * t + this->texCoor[0] * vt;
	ret->texCoor[1] = tmpR->texCoor[1] * t + this->texCoor[1] * vt;
	ret->Normal = tmpR->Normal * t + this->Normal * vt;
	ret->TangentW = tmpR->TangentW * t + this->TangentW * vt;
	return ret;
}

void NtTPhoneVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	this->PosW = tmpA->PosW* a + tmpB->PosW*b + tmpC->PosW * c;
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->Normal = tmpA->Normal *t0 + tmpB->Normal * t1 + tmpC->Normal * t2;
	this->texCoor[0] = tmpA->texCoor[0] * t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;
	this->TangentW = tmpA->TangentW *t0 + tmpB->TangentW * t1 + tmpC->TangentW * t2;
}



ptrBase TPhoneShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrTPhone;

	ret->PosW = vertex.GetPostion();
	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);

	ret->PosH = ret->PosH *  w * v;
	//ret->PosW = ret->PosH;
	ret->PosH = ret->PosH * p;
	
	NtVector4 nor = NtVector3To4(vertex.GetNormal());
	nor = nor * w;
	NtVectorTruncation(nor, ret->Normal);

	NtVector4 tangent = NtVector3To4(vertex.TangentU);
	tangent = tangent * w;
	NtVectorTruncation(tangent, ret->TangentW);

	ret->texCoor = NtVector3To2(vertex.GetDiffuse());
	
	return ret;
}
NtColor TPhoneShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	cVFptr in = dynamic_cast<cVFptr> (pin);
	NtVector3 ToEye = EyePosW - in->PosW;
	ToEye.normalize();
	//获得材质

	//计算漫反射
	NtVector4& amb = Ambient;

	//漫反射纹理归一化
	NtVector4 diffuseAlpdeo = diffuseTex->diffuse(in->texCoor[0], in->texCoor[1]);

	NtVector4 AmbientLit = ComponentMultiply(amb, diffuseAlpdeo);

	float Roughness = specularTex->spec(in->texCoor[0], in->texCoor[1]);
	MaterialConstant Pmat(diffuseAlpdeo, mat.FresnelR0, Roughness);

	NtVector3 tangentN = tangentTex->normal(in->texCoor[0], in->texCoor[1]);
	
	NtVector3 normal = TangentToNormalW(tangentN, in->TangentW, in->Normal);
	//计算光照
	NtVector4 Light = ComputeLight(DirectionalLights, in->PosW, normal, ToEye, Pmat);
	NtVector4 Colorf = Light + AmbientLit;
	NtColor color32 = float4ToColor(Colorf);

	return color32;
}



/*
==============================
|	Shadow Shanding   |
==============================
*/



ptrBase NtShadowVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrShadow;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	return ret;
}
ptrBase NtShadowVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrShadow;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH* t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	return ret;
}

void NtShadowVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);
	this->PosH.z( tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()*c);
	this->PosH.w(1.f/this->PosH.z());
}



ptrBase ShadowShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrShadow;

	ret->PosW = vertex.GetPostion();

	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);
	
	ret->PosH = ret->PosH *  w;
	NtVectorTruncation(ret->PosH, ret->PosW);
	ret->PosH = ret->PosH * v * p;
	return ret;
}
NtColor ShadowShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	return NtColor(pin->PosH[2]*255,0,0,0);
}





/*
==============================
|	STPhone Shanding         |
==============================
*/




ptrBase NtSTPhoneVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrSTPhone;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	ret->texCoor[0] = this->texCoor[0];
	ret->texCoor[1] = this->texCoor[1];
	ret->Normal = this->Normal;
	ret->TangentW = this->TangentW;
	ret->ShadowTexH = this->ShadowTexH;
	return ret;
}
ptrBase NtSTPhoneVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrSTPhone;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH* t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	ret->texCoor[0] = tmpR->texCoor[0] * t + this->texCoor[0] * vt;
	ret->texCoor[1] = tmpR->texCoor[1] * t + this->texCoor[1] * vt;
	ret->Normal = tmpR->Normal * t + this->Normal * vt;
	ret->TangentW = tmpR->TangentW * t + this->TangentW * vt;
	ret->ShadowTexH = tmpR->ShadowTexH * t + this->ShadowTexH * vt;
	return ret;
}

void NtSTPhoneVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	this->PosW = tmpA->PosW* a + tmpB->PosW*b + tmpC->PosW * c;
	this->PosH.z(tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()*c);
	this->Normal = tmpA->Normal *a + tmpB->Normal * b + tmpC->Normal * c;
	this->texCoor[0] = tmpA->texCoor[0] * a + tmpB->texCoor[0] * b + tmpC->texCoor[0] * c;
	this->texCoor[1] = tmpA->texCoor[1] * a + tmpB->texCoor[1] * b + tmpC->texCoor[1] * c;
	this->TangentW = tmpA->TangentW *a + tmpB->TangentW * b + tmpC->TangentW * c;
	this->ShadowTexH = tmpA->ShadowTexH *a + tmpB->ShadowTexH * b + tmpC->ShadowTexH * c;
}



float over = 1.f / 255;
float ComputerShadowFactor(NtVector4 TexPosH, std::shared_ptr<NtImage> ShadowTex)
{
	float imageSize = ShadowTex->GetImageSize();
	TexPosH.Perspective();
	float dp = TexPosH.z();
	float dx = 1.f / ShadowTex->GetWidth();
	
	//PCF
	const float offset[4][2] = { {0,0},{dx,0},{0,dx},{dx,dx} };
	float s0 = ShadowTex->GetPixelfloat(TexPosH.x() + offset[0][0], TexPosH.y() + offset[0][1])[0];
	float s1 = ShadowTex->GetPixelfloat(TexPosH.x() + offset[1][0], TexPosH.y() + offset[1][1])[0];
	float s2 = ShadowTex->GetPixelfloat(TexPosH.x() + offset[2][0], TexPosH.y() + offset[2][1])[0];
	float s3 = ShadowTex->GetPixelfloat(TexPosH.x() + offset[3][0], TexPosH.y() + offset[3][1])[0];
	//判断是否在阴影中
	float r0 = int(s0 >= dp);
	float r1 = int(s1 >= dp);
	float r2 = int(s2 >= dp);
	float r3 = int(s3 >= dp);
	TexPosH = TexPosH*imageSize;
	float t[2] = { TexPosH.x() - int(TexPosH.x()),TexPosH.y() - int(TexPosH.y()) };
	//双线性插值深度
	return  lerp(lerp(r0, r1, t[0]), lerp(r2, r3, t[0]), t[1]);
}


ptrBase STPhoneShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrSTPhone;

	ret->PosW = vertex.GetPostion();
	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);
	
	ret->PosH = ret->PosH *  w;
	
	ret->ShadowTexH = ret->PosH * vpt;

	ret->PosH = ret->PosH  * v * p;

	NtVector4 nor = NtVector3To4(vertex.GetNormal());
	nor = nor * w;
	NtVectorTruncation(nor, ret->Normal);

	NtVector4 tangent = NtVector3To4(vertex.TangentU);
	tangent = tangent * w;
	NtVectorTruncation(tangent, ret->TangentW);

	ret->texCoor = NtVector3To2(vertex.GetDiffuse());
	
	return ret;
}
NtColor STPhoneShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	cVFptr in = dynamic_cast<cVFptr> (pin);
	NtVector3 ToEye = EyePosW - in->PosW;
	ToEye.normalize();
	//获得材质

	//计算漫反射
	NtVector4& amb = Ambient;
	NtVector4 diffuseAlpdeo;
	//漫反射纹理归一化
	if (diffuseTex) {
		diffuseAlpdeo = diffuseTex->diffuse(in->texCoor[0], in->texCoor[1]);
	}
	else
		diffuseAlpdeo = mat.diffuseAlbedo;
	
	NtVector4 AmbientLit = ComponentMultiply(amb, diffuseAlpdeo);

	float Roughness;
	if (specularTex)
		Roughness = specularTex->spec(in->texCoor[0], in->texCoor[1]);
	else
		Roughness = mat.Roughness;

	MaterialConstant Pmat(diffuseAlpdeo, mat.FresnelR0, Roughness);
	NtVector3 normal;
	if (tangentTex) {
		NtVector3 tangentN = tangentTex->normal(in->texCoor[0], in->texCoor[1]);

		normal = TangentToNormalW(tangentN, in->TangentW, in->Normal);
	}
	else
		normal = in->Normal;
	//计算阴影//
	float shadowfactor = ComputerShadowFactor(in->ShadowTexH, ShadowTex);

	//计算光照
	NtVector4 Light = ComputeLight(DirectionalLights, in->PosW, normal, ToEye, Pmat, shadowfactor);
	NtVector4 Colorf = Light + AmbientLit;
	NtColor color32 = float4ToColor(Colorf);

	return color32;
}