
#include"NtRender.h"
#include"Effect.h"
#include<cstdbool>
#include"LightHelper.h"
#include"NtImage.h"
#include<algorithm>
extern std::shared_ptr<NtSofterRender> render;


float lerp(float a,float b,float t)
{
	return a + (b - a)*t;
}
NtVector3 Normal(const NtVector3&rhs)
{
	return NtVector3(rhs[0] * 2 - 1, rhs[1] * 2 - 1, rhs[2] * 2 - 1);
}
NtVector3 Reflect(const NtVector3&in,const NtVector3&normal)
{

	return in + normal * (-in * normal)*2.f;
}
float saturate(float x)
{
	return x > 1 ? 1 : (x < 0 ? 0 : x);
}
int sign(float t)
{
	if (t < 0)return -1;
	return t == 0 ? 0 : 1;
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
	
	
	
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;

	this->Color = tmpA->Color *t0 + tmpB->Color * t1 + tmpC->Color * t2;
	this->PosW = tmpA->PosW* t0 + tmpB->PosW*t1 + tmpC->PosW*t2;
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

	ret->Color = NtVector4(1,1,1,1)* intensity;
	return ret;
}
NtVector4 GroudShader::PixelShader(const NtVertexOutBaseFormat* pin)
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


	
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;

	this->intensity = tmpA->intensity *t0 + tmpB->intensity * t1 + tmpC->intensity * t2;
	this->PosH.z((tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()* c));
	this->PosW = tmpA->PosW* t0 + tmpB->PosW*t1 + tmpC->PosW*t2;
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
NtVector4 CGroudShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	NtVector4 color(1, 1, 1,0);
	float i = dynamic_cast<cVFptr> (pin)->intensity;
	if (i > 0.8f) 
		i = 1.f;
	else if (i > 0.6f) 
		i = 0.8f;
	else if (i > 0.4f) 
		i = 0.6f;
	else if (i > 0.2f) 
		i = 0.4f;
	

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



	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->intensity = tmpA->intensity *t0 + tmpB->intensity * t1 + tmpC->intensity * t2;
	this->texCoor[0] = tmpA->texCoor[0] *t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;
	this->PosH.z((tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()* c));
	this->PosW = tmpA->PosW* t0 + tmpB->PosW*t1 + tmpC->PosW*t2;
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
NtVector4 TGroudShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	
	
	cVFptr tmp = dynamic_cast<cVFptr> (pin);
	float i = tmp->intensity;
	NtVector4 color = diffuse->GetPixel(tmp->texCoor[0], tmp->texCoor[1]);
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



	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->Normal = tmpA->Normal *t0 + tmpB->Normal * t1 + tmpC->Normal * t2;
	this->texCoor[0] = tmpA->texCoor[0] * t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;
	this->PosH.z((tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()* c));
	this->PosW = tmpA->PosW* t0 + tmpB->PosW*t1 + tmpC->PosW*t2;
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
NtVector4 PhoneShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	cVFptr in = dynamic_cast<cVFptr> (pin);
	NtVector3 ToEye = EyePosW - in->PosW;
	ToEye.normalize();
	//获得材质
	
	//计算漫反射
	NtVector4& amb = Ambient;

	//漫反射纹理归一化
	NtVector4 diffuseAlpdeo = pointTex2D(diffuseTex.get(),in->texCoor);

	NtVector4 AmbientLit = ComponentMultiply(amb , diffuseAlpdeo);
	
	float Roughness = pointTex2D(specularTex.get(), in->texCoor)[0];
	MaterialConstant Pmat(diffuseAlpdeo, mat.FresnelR0, 1-Roughness);

	NtVector3 normal = Normal(NtVector4To3(pointTex2D(normalTex.get(), in->texCoor)));
	normal.normalize();
	//计算光照
	NtVector4 Light = ComputeLight(DirectionalLights, in->PosW, normal, ToEye, Pmat);
	NtVector4 emission = pointTex2D(emissionTex.get(), in->texCoor);
	NtVector4 Colorf = emission + Light + AmbientLit;
	

	return Colorf;
}




/*
==============================
|	Tangent Phone Shanding   |
==============================
*/

NtVector3 TangentToNormalW(const NtVector3& tangent, const NtVector3& T, const NtVector3& N)
{
	NtVector3 Tangent = T - N*(N * T);
	NtVector3 B = Cross(N, Tangent);
	NtMatrix3x3 TBN({ Tangent,B,N });
 	return tangent * TBN;
}
NtVector3 TangentToNormalW(const NtVector3& tangent, const NtVector3& T, const NtVector3& N, const NtVector3& B)
{

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


	
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->PosW = tmpA->PosW* t0 + tmpB->PosW* t1 + tmpC->PosW * t2;
	this->Normal = tmpA->Normal *t0 + tmpB->Normal * t1 + tmpC->Normal * t2;
	this->texCoor[0] = tmpA->texCoor[0] * t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;
	this->TangentW = tmpA->TangentW *t0 + tmpB->TangentW * t1 + tmpC->TangentW * t2;
	this->PosH.z((tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()* c));

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
NtVector4 TPhoneShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	cVFptr in = dynamic_cast<cVFptr> (pin);
	NtVector3 ToEye = EyePosW - in->PosW;
	ToEye.normalize();
	//获得材质

	//计算漫反射
	NtVector4& amb = Ambient;
	
	//漫反射纹理归一化
	NtVector4 diffuseAlpdeo = pointTex2D(diffuseTex.get(), in->texCoor);

	NtVector4 AmbientLit = ComponentMultiply(amb, diffuseAlpdeo);
	
	float Roughness =  pointTex2D(specularTex.get(), in->texCoor)[0];
	MaterialConstant Pmat(diffuseAlpdeo, mat.FresnelR0, Roughness);

	NtVector3 tangentN = Normal(NtVector4To3(pointTex2D(tangentTex.get(), in->texCoor)));
	tangentN.normalize();
	NtVector3 normal = TangentToNormalW(tangentN, in->TangentW, in->Normal);
	//计算光照
	NtVector4 Light = ComputeLight(DirectionalLights, in->PosW, normal, ToEye, Pmat);
	NtVector4 Colorf = Light + AmbientLit;
	Colorf =saturate(Colorf);

	return Colorf;
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
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->PosH.z((tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()* c));
	this->PosW = tmpA->PosW* a + tmpB->PosW*b + tmpC->PosW*c;
	//this->PosH.z( tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()*c);
	//this->PosH.w(1.f/this->PosH.z());
}



ptrBase ShadowShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrShadow;

	ret->PosW = vertex.GetPostion();

	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);
	
	ret->PosH = ret->PosH *  w;
	//NtVectorTruncation(ret->PosH, ret->PosW);
	ret->PosH = ret->PosH * v * p;
	return ret;
}
NtVector4 ShadowShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	return NtVector4(pin->PosH[2]*255,0,0,0);
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
	ret->BitTangentW = this->TangentW;
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
	ret->BitTangentW = tmpR->BitTangentW * t + this->BitTangentW * vt;
	ret->ShadowTexH = tmpR->ShadowTexH * t + this->ShadowTexH * vt;
	return ret;
}

void NtSTPhoneVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);



	
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->PosW = tmpA->PosW* t0 + tmpB->PosW*t1 + tmpC->PosW*t2;
	this->Normal = tmpA->Normal *t0 + tmpB->Normal * t1 + tmpC->Normal * t2;
	this->texCoor[0] = tmpA->texCoor[0] * t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;
	this->TangentW = tmpA->TangentW *t0 + tmpB->TangentW * t1 + tmpC->TangentW *t2;
	this->BitTangentW = tmpA->BitTangentW *t0 + tmpB->BitTangentW * t1 + tmpC->BitTangentW *t2;
	this->ShadowTexH = tmpA->ShadowTexH *t0 + tmpB->ShadowTexH * t1 + tmpC->ShadowTexH *t2;
	this->PosH.z((tmpA->PosH.z() * a + tmpB->PosH.z()*b + tmpC->PosH.z()* c));
}



float over = 1.f / 255;
float ComputerShadowFactor(NtVector4 TexPosH, std::shared_ptr<Tex2D_1F> ShadowTex)
{
	float imageSize = ShadowTex->GetImageSize();
	TexPosH.Perspective();
	float dp = TexPosH.z();
	float dx = 1.f / ShadowTex->GetWidth();

	//PCF
	const float offset[4][2] = { {0,0},{dx,0},{0,dx},{dx,dx} };
	float s0 = pointTex2D(ShadowTex.get(),NtVector2(TexPosH.x() + offset[0][0], TexPosH.y() + offset[0][1]))[0]+0.003;
	float s1 = pointTex2D(ShadowTex.get(), NtVector2(TexPosH.x() + offset[1][0], TexPosH.y() + offset[1][1]))[0]+0.003;
	float s2 = pointTex2D(ShadowTex.get(), NtVector2(TexPosH.x() + offset[2][0], TexPosH.y() + offset[2][1]))[0]+0.003;
	float s3 = pointTex2D(ShadowTex.get(), NtVector2(TexPosH.x() + offset[3][0], TexPosH.y() + offset[3][1]))[0]+0.003;
	
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

	ret->BitTangentW = Cross(ret->Normal, ret->TangentW);
	ret->texCoor = NtVector3To2(vertex.GetDiffuse());
	
	return ret;
}
NtVector4 STPhoneShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	cVFptr in = dynamic_cast<cVFptr> (pin);
	NtVector3 ToEye = EyePosW - in->PosW;
	ToEye.normalize();
	//获得材质

	//计算漫反射
	NtVector4 amb = Ambient;
	if (SSAO_Tex)
	{
		NtVector4 access = linearTex2D(SSAO_Tex.get(), in->texCoor);
		amb = ComponentMultiply(amb, access);
	}
	
	NtVector4 diffuseAlpdeo;
	//漫反射纹理归一化
	if (diffuseTex) {
		diffuseAlpdeo =pointTex2D( diffuseTex.get(),in->texCoor);
	}
	else
		diffuseAlpdeo = mat.diffuseAlbedo;
	
	NtVector4 AmbientLit = ComponentMultiply(amb, diffuseAlpdeo);

	float Roughness;
	if (specularTex)
		Roughness = pointTex2D(specularTex.get(), in->texCoor)[0];
	else
		Roughness = mat.Roughness;

	MaterialConstant Pmat(diffuseAlpdeo, mat.FresnelR0, Roughness);
	NtVector3 normal;
	if (tangentTex) {
		NtVector3 tangentN =Normal(NtVector4To3(pointTex2D( tangentTex.get(),in->texCoor)));

		normal = TangentToNormalW(tangentN, in->TangentW, in->Normal);
	}
	else
		normal = in->Normal;


	//计算阴影//
	float shadowfactor = ComputerShadowFactor(in->ShadowTexH, ShadowTex);
	if (shadowfactor > 1)
		printf("dasd");
	//计算光照
	NtVector4 Light = ComputeLight(DirectionalLights, in->PosW, normal, ToEye, Pmat, shadowfactor);
	NtVector4 Colorf = Light + AmbientLit;

	
	
	return saturate(Colorf);
}





/*
========================
|	 Blend   Ground   |
========================
*/

ptrBase BlendVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrBlend;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	ret->texCoor[0] = this->texCoor[0];
	ret->texCoor[1] = this->texCoor[1];
	ret->NormalW = this->NormalW;
	return ret;
}
ptrBase BlendVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrBlend;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH*  t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	ret->texCoor[0] = tmpR->texCoor[0] * t + this->texCoor[0] * vt;
	ret->texCoor[1] = tmpR->texCoor[1] * t + this->texCoor[1] * vt;
	ret->NormalW = tmpR->NormalW * t + this->NormalW * vt;
	return ret;
}

void BlendVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->PosW = tmpA->PosW* t0 + tmpB->PosW*t1 + tmpC->PosW*t2;
	this->texCoor[0] = tmpA->texCoor[0] * t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;
	this->NormalW = tmpA->NormalW * t0 + tmpB->NormalW * t1 + tmpC->NormalW * t2;
	this->PosH =tmpA->PosH * a + tmpB->PosH *b + tmpC->PosH* c;
}



ptrBase BlendShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrBlend;
	ret->PosW = vertex.GetPostion();
	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);
	ret->PosH = ret->PosH *  w * v;
	//ret->PosW = ret->PosH;
	ret->PosH = ret->PosH * p;
	NtVector4 nor = NtVector3To4(vertex.GetNormal());
	nor = nor * w;
	NtVector3 tn(nor.x(), nor.y(), nor.z());

	ret->NormalW = tn;
	ret->texCoor = NtVector3To2(vertex.GetDiffuse());
	
	return ret;
}
NtVector4 BlendShader::PixelShader(const NtVertexOutBaseFormat* pin)
{

	NtVector3 Light_dir = -DirectionalLight.Direction;
	cVFptr tmp = dynamic_cast<cVFptr> (pin);
	
	NtVector4 color;
	color = diffuse->GetPixel(tmp->texCoor[0], tmp->texCoor[1]);
	
	color.xyz(color.xyz() * saturate((tmp->NormalW * Light_dir)));
	
	color += Ambient;
	color = saturate(color);
	if (color.x() < 0)
		printf("ds");
	color.a(color.a() * BlendScale);
	
	return color;
}




/*
========================
|	 WriteNormal      |
========================
*/

ptrBase WriteNormalVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrWriteNormal;
	ret->PosH = this->PosH;
	ret->PosW = this->PosW;
	ret->NormalW = this->NormalW;

	
	return ret;
}
ptrBase WriteNormalVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrWriteNormal;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH*  t + this->PosH * vt;
	ret->PosW = tmpR->PosW* t + this->PosW * vt;
	ret->NormalW = tmpR->NormalW * t + this->NormalW * vt;

	return ret;
}

void WriteNormalVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;

	this->PosW = tmpA->PosW* t0 + tmpB->PosW*t1 + tmpC->PosW*t2;
	this->NormalW = tmpA->NormalW * t0 + tmpB->NormalW * t1 + tmpC->NormalW * t2;
	this->PosH = tmpA->PosH*a + tmpB->PosH*b + tmpC->PosH*c;
}



ptrBase WriteNormalShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrWriteNormal;
	ret->PosW = vertex.GetPostion();
	ret->PosH = NtVector3To4(ret->PosW, (float)1.0);
	ret->PosH = ret->PosH *  w * v;
	//ret->PosW = ret->PosH;
	ret->PosH = ret->PosH * p;
	NtVector4 nor = NtVector3To4(vertex.GetNormal());
	//将其变换到观察空间
	NtMatrix4x4 wvInvTrans = (w * v).invert_transpose();
	nor = nor * wvInvTrans;
	NtVector3 tn(nor.x(), nor.y(), nor.z());
	ret->NormalW = tn;
	
	
	return ret;
}
NtVector4 WriteNormalShader::PixelShader(const NtVertexOutBaseFormat* pin)
{


	cVFptr tmp = dynamic_cast<cVFptr> (pin);
	NtVector3 normal = tmp->NormalW;

	return NtVector4(normal, 0.f);
}





/*
========================
|		SSAO      |
========================
*/

ptrBase SSAOVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrSSAO;
	ret-> posV = this->posV;
	ret->texCoor = this->texCoor;
	ret->PosH = this->PosH;
	return ret;
}
ptrBase SSAOVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrSSAO;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH*  t + this->PosH * vt;
	ret->posV = tmpR->posV*t + this->posV*vt;
	ret->texCoor[0] = tmpR->texCoor[0] * t + this->texCoor[0] * vt;
	ret->texCoor[1] = tmpR->texCoor[1] * t + this->texCoor[1] * vt;

	return ret;
}

void SSAOVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->texCoor[0] = tmpA->texCoor[0] * t0 + tmpB->texCoor[0] * t1 + tmpC->texCoor[0] * t2;
	this->texCoor[1] = tmpA->texCoor[1] * t0 + tmpB->texCoor[1] * t1 + tmpC->texCoor[1] * t2;
	this->posV = tmpA->posV* t0 + tmpB->posV*t1 + tmpC->posV*t2;
	this->PosH = tmpA->PosH*a + tmpB->PosH*b+tmpC->PosH*c;
}
//工具函数
float NdcDepthToViewDepth(float z_ndc,const NtMatrix4x4&proj)
{
	//z_ndc = proj[2][2]+proj[3][2]/z
	return  proj[3][2]/(z_ndc - proj[2][2]);
}



ptrBase SSAOShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrSSAO;
	
	NtVector3 pos =  vertex.GetPostion();
	//屏幕空间转换到ndc空间	
	ret->PosH = NtVector4(pos.x()*2-1,1-pos.y()*2,0.f,1.f);
	ret->texCoor = NtVector2(pos.x(), pos.y());
	NtVector4 tmp = ret->PosH*p.invert();
	tmp.Perspective();
	ret->posV =NtVector4To3(tmp);
	return ret;
}
NtVector4 SSAOShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	cVFptr in = dynamic_cast<cVFptr> (pin);
	//重新构建观察空间坐标
	float depth = pointTex2D(depthTex.get(), in->texCoor)[0];
	NtVector3 normal =NtVector4To3(pointTex2D(normalTex.get(), in->texCoor));
	if (normal.x() == 0 && normal.y() == 0 && normal.z() == 0)return NtVector4(1, 1, 1, 0);
	depth = NdcDepthToViewDepth(depth,p);
	NtVector3 viewP = in->posV * (depth / in->posV.z());
	NtVector3 randVec = normal;
	float occlusionSum = 0.f;
	for (int i = 0; i < sampleCount; i++)
	{
		NtVector3 offset = Reflect(-NtVector4To3(offsetVec.get()[i]), randVec);
		float flip = sign(offset*normal);
		NtVector3 sampleP =  viewP + offset * flip * OcclusionRadius;;
		NtVector4 projTP = NtVector3To4(sampleP, 1.f)* ptM;
		projTP.Perspective();
		if (projTP.x() > 1 || projTP.x() < 0 || projTP.y() < 0 || projTP.y() > 1)
			continue;
		//if (projTP.x() < 0|| projTP.y()>=1 || projTP.y() <0 || projTP.x() >= 1)
		//	printf("warning");
		float rz = pointTex2D(depthTex.get(),NtVector2(projTP.x(),projTP.y()))[0];
		rz = NdcDepthToViewDepth(rz,p);
		NtVector3 r =sampleP * (rz / sampleP.z());
		
		float distZ = viewP.z() - rz;
		NtVector3 rp = (r - viewP);
		rp.normalize();
		float dp = std::max(rp*normal,0.f);
		occlusionSum += dp * OcclusionFunction(distZ);
	}
	occlusionSum /= sampleCount;
	float access = 1 - occlusionSum;
	access = saturate(pow(access,2));
	
	return NtVector4(access, access, access,0);
}
float SSAOShader::OcclusionFunction(float distZ)
{
	float occlusion = 0.0f;
	if (distZ > surfaceEpsilon)
	{
		float fadeLength = OcclusionFadeEnd - OcclusionFadeStart;
		occlusion = saturate((OcclusionFadeEnd-distZ)/fadeLength);
	}
	return occlusion;
}

SSAOShader::~SSAOShader()
{
	delete randomNormalTex;
}


/*
========================
|	 Ambient SSAO      |
========================
*/

ptrBase AmibentSSAOVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrAmibentSSAO;
	ret->PosH = this->PosH;
	ret->posW = this->posW;


	return ret;
}
ptrBase AmibentSSAOVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrAmibentSSAO;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH*  t + this->PosH * vt;
	
	ret->posW = tmpR->posW * t + this->posW * vt;
	

	return ret;
}

void AmibentSSAOVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;
	this->posW= tmpA->posW * t0 + tmpB->posW * t1 + tmpC->posW * t2;


	
	this->PosH = tmpA->PosH*a + tmpB->PosH*b + tmpC->PosH*c;
}



ptrBase AmibentSSAOShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrAmibentSSAO;
	NtVector3 Pos = vertex.GetPostion();
	ret->PosH = NtVector3To4(Pos, (float)1.0);
	ret->PosH = ret->PosH *  w ;
	ret->posW = NtVector4To3(ret->PosH);
	ret->PosH = ret->PosH * v * p;
	return ret;
}
NtVector4 AmibentSSAOShader::PixelShader(const NtVertexOutBaseFormat* pin)
{

	NtVector4 amb = ambient;
	cVFptr tmp = dynamic_cast<cVFptr> (pin);

	NtVector4 texCoor =  NtVector3To4(tmp->posW,1.f) * vpt;
	texCoor.Perspective();
	NtVector4 ao = pointTex2D(SSAOTex.get(), NtVector2(texCoor.x(), texCoor.y()));

	amb =ComponentMultiply( amb ,ao);
	return  amb;
}

/*
========================
|	 Skybox      |
========================
*/

ptrBase SkyboxVertexFormat::Copy()const
{
	sVFptr ret = mks_ptrSkybox;
	ret->PosH = this->PosH;
	ret->Posl = this->Posl;



	return ret;
}
ptrBase SkyboxVertexFormat::LineInterpolation(const   NtVertexOutBaseFormat* rhs, float t)
{
	sVFptr ret = mks_ptrSkybox;
	cVFptr tmpR = dynamic_cast<cVFptr>(rhs);
	float vt = (1 - t);
	ret->PosH = tmpR->PosH*  t + this->PosH * vt;
	ret->Posl = tmpR->Posl* t + this->Posl * vt;


	return ret;
}

void SkyboxVertexFormat::PerspectiveInterpolation(const   NtVertexOutBaseFormat* A, const  NtVertexOutBaseFormat* B, const  NtVertexOutBaseFormat* C,
	float a, float b, float c)
{

	cVFptr tmpA = dynamic_cast<cVFptr>(A);
	cVFptr tmpB = dynamic_cast<cVFptr>(B);
	cVFptr tmpC = dynamic_cast<cVFptr>(C);


	
	float z3 = tmpA->PosH.w() * a + tmpB->PosH.w()*b + tmpC->PosH.w()*c;
	this->PosH.z(1 / z3);
	this->PosH.w(z3);
	float t0 = this->PosH.z()*tmpA->PosH.w() * a;
	float t1 = this->PosH.z()*tmpB->PosH.w() * b;
	float t2 = this->PosH.z()*tmpC->PosH.w() * c;

	this->Posl = tmpA->Posl* t0 + tmpB->Posl*t1+ tmpC->Posl*t2;
	this->PosH = tmpA->PosH*a + tmpB->PosH*b + tmpC->PosH*c;
	this->PosH.z(1.f);
}



ptrBase SkyboxShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrSkybox;
	ret->Posl = vertex.GetPostion();
	NtVector4 posW = NtVector3To4(ret->Posl, (float)1.0) * w;
	posW = posW + NtVector3To4(eyePosW);
	
	ret->PosH = posW * v * p;
	//ret->PosW = ret->PosH;
	ret->PosH.z(ret->PosH.w());

	return ret;
}
NtVector4 SkyboxShader::PixelShader(const NtVertexOutBaseFormat* pin)
{
	cVFptr tmp = dynamic_cast<cVFptr> (pin);
	return skybox->sample(tmp->Posl);
}



