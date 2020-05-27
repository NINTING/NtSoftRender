#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Ntmath.h"
#include"Nt.h"
#include<algorithm>
#include<math.h>
#include<vector>

class NtSofterRender;

struct LightConstant
{
	NtVector3 Strength;
	NtVector3 Direction;
	//NtVector3 Position;
};
enum LightType
{
	DirectionalLight,
	PointLight,
	SpotLight
};

struct Light
{
	Light() { pConstant = new LightConstant(); }

	void SetStrength(const NtVector3& s) { pConstant->Strength = s; };
	void SetDirection(const NtVector3& dir) { pConstant->Direction = dir; };
	NtVector3 GetStrength() { return  pConstant->Strength; };
	NtVector3 GetDirection() { return pConstant->Direction; };
	LightConstant* GetLightConstant() { return pConstant; }
	~Light() { delete pConstant; }
	int index;
	LightType type = DirectionalLight;
private:
	LightConstant*pConstant;
	
	//NtVector3 Position;
};


std::shared_ptr<Light> CreateDirectionalLight(const NtVector3&Strength, const NtVector3&Dir);

NtVector3  SchlickFresnel(const NtVector3& r0, const NtVector3 &normal, const NtVector3 &halfVec);

NtVector3 BilnnPhone(const NtVector3& LightStrength, const NtVector3&normal, const NtVector3&lightVec, const NtVector3 & toEye, const MaterialConstant& mat);

NtVector3 ComputeDirectionLight(const LightConstant& l, const NtVector3&normal, const MaterialConstant& mat,const NtVector3& toEye);

NtVector4 ComputeLight(const std::vector<LightConstant>& DirLights, const NtVector3&pos, const NtVector3&normal, const NtVector3& toEye, const  MaterialConstant&mat);


#endif