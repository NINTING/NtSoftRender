#pragma once
#include"NtShader.h"
#include"Effect.h"
#include<vector>
#include"Ntmath.h"
class Model;
class NtCamera;
class NtSofterRender;
class Light;
class ShadowMap;
void PhoneAssemble(const Model& model, const NtCamera& camera, const NtSofterRender* render, 
	const Light& dirctionalLight,const NtVector4&Ambient);



void TPhoneAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render, 
	const Light& dirctionalLight, const NtVector4&Ambient);




void STPhoneAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient, const ShadowMap& shadowMap);