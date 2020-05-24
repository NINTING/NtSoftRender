
#include"NtRender.h"
#include"Effect.h"
#include<cstdbool>
extern std::shared_ptr<NtSofterRender> render;


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

	this->Color = tmpA->Color *a + tmpB->Color * b + tmpC->Color * c;

}

 

 ptrBase GroudShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrGroud;
	NtMatrix4x4& w = render->GetWorldMatrix();
	NtMatrix4x4& v = render->GetViewMatrix();
	NtMatrix4x4& p = render->GetProjMatrix();
	NtVector3 Light_dir =render->GetLightDir();
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

	this->intensity = tmpA->intensity *a + tmpB->intensity * b + tmpC->intensity * c;

}



ptrBase CGroudShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrCGroud;
	NtMatrix4x4& w = render->GetWorldMatrix();
	NtMatrix4x4& v = render->GetViewMatrix();
	NtMatrix4x4& p = render->GetProjMatrix();
	NtVector3 Light_dir = render->GetLightDir();
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
	ret->PosH = tmpR->PosH* t + this->PosH * vt;
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
	float intensity = tmpA->intensity *a + tmpB->intensity * b + tmpC->intensity * c;
	this->texCoor[0] = tmpA->texCoor[0] *a + tmpB->texCoor[0] * b + tmpC->texCoor[0] * c;
	this->texCoor[1] = tmpA->texCoor[1] * a + tmpB->texCoor[1] * b + tmpC->texCoor[1] * c;

}



ptrBase TGroudShader::VertexShader(const NtVertex& vertex)
{
	sVFptr ret = mks_ptrTGroud;
	NtMatrix4x4& w = render->GetWorldMatrix();
	NtMatrix4x4& v = render->GetViewMatrix();
	NtMatrix4x4& p = render->GetProjMatrix();
	NtVector3 Light_dir = render->GetLightDir();
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
	NtColor color = render->GetTextureFregment32(0, tmp->texCoor[0], tmp->texCoor[1]);
	color *= i;
	return color;
}
