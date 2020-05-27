#include"LightHelper.h"
#include "Nt.h"
#include"NtRender.h"

NtVector3  SchlickFresnel(const NtVector3& r0, const NtVector3 &normal, const NtVector3 &halfVec)
{
	float cosTheta = std::max((normal*halfVec), 0.f);
	float f0 = 1 - cosTheta;
	return r0 + (1.f - r0)*(f0*f0*f0*f0*f0);
}

NtVector3 BilnnPhone(const NtVector3& LightStrength, const NtVector3&normal, const NtVector3&lightVec, const NtVector3&toEye, const MaterialConstant& mat)
{
	NtVector3 halfVec = lightVec + toEye;
	halfVec.normalize();
	float m =  mat.Roughness * 256;
	float roughness = (m + 8)  * powf(std::max(normal*halfVec, 0.f), m) / m;

	NtVector3 fresnelFactor = SchlickFresnel(mat.FresnelR0, normal, halfVec);

	NtVector3 SpecAlbedo = fresnelFactor * roughness;
	SpecAlbedo = SpecAlbedo / (1.f + SpecAlbedo);
	NtVector3 tmpDiff;
	NtVectorTruncation(mat.diffuseAlbedo, tmpDiff);
	return ComponentMultiply(tmpDiff + SpecAlbedo, LightStrength);
}


NtVector3 ComputeDirectionLight(const LightConstant& l, const NtVector3&normal, const MaterialConstant& mat,const NtVector3& toEye)
{
	NtVector3 lightVec = -l.Direction;			//光向量与光传播方向
	NtVector3 LightStrength = l.Strength * std::max(l.Direction * normal, 0.f);
	return BilnnPhone(LightStrength, normal, lightVec, toEye, mat);
}

NtVector4 ComputeLight(const std::vector<LightConstant>& DirLights, const NtVector3&pos, const NtVector3&normal,
	const NtVector3& toEye, const  MaterialConstant&mat)
{
	NtVector3 ret;
	
	for (auto i : DirLights)
	{
		ret+=ComputeDirectionLight(i,normal,mat,toEye);
	}
	return NtVector3To4(ret, 1.f);
}
 


std::shared_ptr<Light> CreateDirectionalLight(const NtVector3&Strength, const NtVector3&Dir)
{
	std::shared_ptr<Light> ret = std::make_shared<Light>();
	ret->SetDirection(Dir);
	ret->SetStrength(Strength);
	return ret;
}
