#include"ShaderAssemble.h"
#include"NtRender.h"
#include"Nt.h"
#include"ShadowMap.h"
#include"NtImage.h"
#include"Model.h"

void PhoneAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render, 
	const Light& dirctionalLight, const NtVector4&Ambient)
{
	NtMatrix4x4 world = NtMatrix4x4();

	PhoneShader*pgs = new PhoneShader();
	pgs->diffuseTex = model.GetDiffuseTex();
	pgs->w = world;
	pgs->v = camera.GetViewMatrix();
	pgs->p = camera.GetProjMatrix();
	pgs->EyePosW = camera.GetPos();
	pgs->DirectionalLights.push_back(*dirctionalLight.GetLightConstant());
	pgs->mat = model.GetMaterial();
	pgs->normalTex = model.GetNormalTex();
	pgs->specularTex = model.GetSpecularTex();
	pgs->Ambient = Ambient;


	render->SetVertexBuffer(model.GetVertexsBuffer());
	render->SetIndexBuffer(model.GetIndicesBuffer());
	render->SetShader(pgs);

}


void TPhoneAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient)
{
	NtMatrix4x4 world = NtMatrix4x4();
	render->SetRenderTarget(BackBufferWrite);
	TPhoneShader*gs = new TPhoneShader();
	gs->diffuseTex = model.GetDiffuseTex();
	gs->w = world;
	gs->v = camera.GetViewMatrix();
	gs->p = camera.GetProjMatrix();
	gs->EyePosW = camera.GetPos();
	gs->DirectionalLights.push_back(*dirctionalLight.GetLightConstant());
	gs->mat = model.GetMaterial();
	gs->tangentTex = model.GetTangentTex();
	gs->specularTex = model.GetSpecularTex();
	gs->Ambient = Ambient;


	render->SetVertexBuffer(model.GetVertexsBuffer());
	render->SetIndexBuffer(model.GetIndicesBuffer());
	render->SetShader(gs);

}



void STPhoneAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient, const ShadowMap& shadowMap)
{
	NtMatrix4x4 world = NtMatrix4x4();
	render->SetRenderTarget(BackBufferWrite);

	STPhoneShader*gs = new STPhoneShader();
	gs->diffuseTex = model.GetDiffuseTex();
	gs->w = world;
	gs->v = camera.GetViewMatrix();
	gs->p = camera.GetProjMatrix();
	gs->EyePosW = camera.GetPos();
	gs->DirectionalLights.push_back(*dirctionalLight.GetLightConstant());
	gs->mat = model.GetMaterial();
	gs->tangentTex = model.GetTangentTex();
	gs->specularTex = model.GetSpecularTex();
	gs->Ambient = Ambient;
	gs->ShadowTex = shadowMap.GetShadowMap();
	NtMatrix4x4 T({ 0.5,0,0,0,
	0,-0.5,0,0,
	0,0,1,0,
	0.5,0.5,0,1 });
	gs->vpt = shadowMap.GetViewMatrix() * shadowMap.GetProjMatrix()*T;

	render->SetVertexBuffer(model.GetVertexsBuffer());
	render->SetIndexBuffer(model.GetIndicesBuffer());
	render->SetShader(gs);
}