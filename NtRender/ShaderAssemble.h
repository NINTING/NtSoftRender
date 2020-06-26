#pragma once
#include"NtShader.h"
#include"Effect.h"
#include<vector>
#include"Ntmath.h"
class Model;
class NtCamera;
class NtSofterRender;
struct Light;
class ShadowMap;
void PhoneAssemble(const Model& model, const NtCamera& camera, const NtSofterRender* render, 
	const Light& dirctionalLight,const NtVector4&Ambient);



void TPhoneAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render, 
	const Light& dirctionalLight, const NtVector4&Ambient);




void STPhoneAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient, const ShadowMap& shadowMap);


void BlendAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient, float blendScale);

void SSAOAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient);