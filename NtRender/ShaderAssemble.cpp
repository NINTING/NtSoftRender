#include"ShaderAssemble.h"
#include"NtRender.h"
#include"Nt.h"
#include"ShadowMap.h"
#include"NtImage.h"
#include"Model.h"
#include"NtUtility.h"
#include"Geometry.h"


extern std::shared_ptr<NtWindow> window;
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
	render->SetRenderTarget(BackBufferOn);
	TPhoneShader*gs = new TPhoneShader();
	if (model.GetDiffuseTex())
		gs->diffuseTex = model.GetDiffuseTex();
	if(model.GetTangentTex())
		gs->tangentTex = model.GetTangentTex();
	if (model.GetSpecularTex())
		gs->specularTex = model.GetSpecularTex();
	if (model.GetEmissionTex())
		gs->emissionTex = model.GetEmissionTex();
	gs->w = model.GetWorld();
	gs->v = camera.GetViewMatrix();
	gs->p = camera.GetProjMatrix();
	gs->EyePosW = camera.GetPos();
	gs->DirectionalLights.push_back(*dirctionalLight.GetLightConstant());
	gs->mat = model.GetMaterial();
	
	gs->Ambient = Ambient;


	render->SetVertexBuffer(model.GetVertexsBuffer());
	render->SetIndexBuffer(model.GetIndicesBuffer());
	render->SetShader(gs);

}



void STPhoneAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient, const ShadowMap& shadowMap)
{
	NtMatrix4x4 world = NtMatrix4x4();
	render->SetRenderTarget(BackBufferOn);

	STPhoneShader*gs = new STPhoneShader();
	gs->diffuseTex = model.GetDiffuseTex();
	gs->w = model.GetWorld();
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



void BlendAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient,float blendScale)
{
	BlendShader*gs = new BlendShader();
	NtMatrix4x4 world = NtMatrix4x4();
	
	gs->w = world;
	gs->v = camera.GetViewMatrix();
	gs->p = camera.GetProjMatrix();
	gs->DirectionalLight = *dirctionalLight.GetLightConstant();
	gs->Ambient = Ambient;
	gs->BlendScale = blendScale;
	if(model.GetDiffuseTex())
	gs->diffuse = model.GetDiffuseTex();
	//准备深度图
	render->SetRenderTarget(BlendOn);
	render->SetRenderTarget(BackBufferOn);
	render->SetRenderTarget(ZwriteOff);
	render->SetCullState(CullFront);
	render->SetBlendFactor(AlphaSrc, OneMinusAlphaDest);
	render->SetVertexBuffer(model.GetVertexsBuffer());
	render->SetIndexBuffer(model.GetIndicesBuffer());
	render->SetShader(gs);
	render->Draw();

	//渲染
	render->SetRenderTarget(BlendOn);
	render->SetRenderTarget(ZwriteOn);
	render->SetCullState(Cullback);
	render->SetBlendFactor(AlphaSrc, OneMinusAlphaDest);
	render->SetRenderTarget(BackBufferOn);


	render->SetVertexBuffer(model.GetVertexsBuffer());
	render->SetIndexBuffer(model.GetIndicesBuffer());

}

void SSAOAssemble(const Model& model, const NtCamera& camera, NtSofterRender* render,
	const Light& dirctionalLight, const NtVector4&Ambient)
{
	NtMatrix4x4 world = NtMatrix4x4();
	static NtMatrix4x4 T({
	0.5,0,0,0,
	0,-0.5,0,0,
	0,0,1,0,
	0.5,0.5,0,1 });
	int ssaow = render->GetBackBuffer()->GetWidth()/2;
	int ssaoh = render->GetBackBuffer()->GetHeight()/2;
	static std::shared_ptr<Tex2D_4F> ssaoTex  = std::shared_ptr<Tex2D_4F>(new Tex2D_4F(ssaow, ssaoh));;
	NtMatrix4x4 vpt = camera.GetViewMatrix()*camera.GetProjMatrix()*T;
	if (!render->IsWireframe()) {
		WriteNormalShader*ns = new WriteNormalShader();
		

		ns->w = world;
		ns->v = camera.GetViewMatrix();
		ns->p = camera.GetProjMatrix();




		//准备法向量图
		render->SetRenderState(GroundShading);
		render->SetRenderTarget(BlendOff);
		render->SetRenderTarget(BackBufferOn);
		render->SetRenderTarget(RTTOff);
		render->SetRenderTarget(ZwriteOn);
		render->SetCullState(Cullback);
		render->SetVertexBuffer(model.GetVertexsBuffer());
		render->SetIndexBuffer(model.GetIndicesBuffer());
		render->SetShader(ns);
		render->Draw();

		//window->Present_image(render->Present());


	

		SSAOShader*ssao = new SSAOShader();
		
			ssao->depthTex = std::make_shared<Tex2D_1F>(*render->GetDepthBuffer());
			ssao->normalTex = std::make_shared<Tex2D_4F>(*render->GetBackBuffer());
			render->CleanBackAndDepthBuffer();

		ssao->OcclusionFadeStart = 0.1f;
		ssao->OcclusionFadeEnd = 0.2f;


	

		ssao->offsetVec = std::shared_ptr<NtVector4>(new NtVector4[14]);
		NtUtility::RandomOffsetVec(ssao->offsetVec.get());
		ssao->w = world;
		ssao->v = camera.GetViewMatrix();
		ssao->p = camera.GetProjMatrix();
		ssao->ptM = ssao->p*T;
		ssao->sampleCount = 14;
		ssao->surfaceEpsilon = 0.03f;
		ssao->OcclusionRadius = 0.2f;
		
		NtUtility::RandomNormalImage(ssaow, ssaoh, &ssao->randomNormalTex);

		
		Model patch2D = Geometry::Patch2D();
		//渲染
		render->SetRTT(ssaoTex.get());
		render->SetRenderTarget(BlendOff);
		render->SetRenderTarget(ZwriteOff);
		render->SetCullState(NoCull);
		render->SetRenderTarget(BackBufferOn);
		render->SetRenderTarget(RTTOn);
		render->SetVertexBuffer(patch2D.GetVertexsBuffer());
		render->SetIndexBuffer(patch2D.GetIndicesBuffer());
		render->SetShader(ssao);
		render->Draw();
		//window->Present_image(Float4ImageToRGBAImage(*ssaoTex));
		
		//window->Present_image(render->Present());
		//ssaoTex = std::make_shared<Tex2D_4F>(*render->GetBackBuffer());
		render->ResetRTT();
	}
	render->CleanBackAndDepthBuffer();
	//应用SSAO
	AmibentSSAOShader*ambSsao = new AmibentSSAOShader();
	ambSsao->SSAOTex = ssaoTex;
	//window->Present_image(Float4ImageToRGBAImage(*ambSsao->SSAOTex));
	ambSsao->w = world;
	ambSsao->v = camera.GetViewMatrix();
	ambSsao->p = camera.GetProjMatrix();
	ambSsao->ambient = Ambient;
	ambSsao->vpt = vpt;
	render->SetRenderTarget(BlendOff);
	render->SetRenderTarget(BackBufferOn);
	render->SetRenderTarget(ZwriteOn);
	render->SetCullState(Cullback);
	render->SetVertexBuffer(model.GetVertexsBuffer());
	render->SetIndexBuffer(model.GetIndicesBuffer());
	render->SetShader(ambSsao);
	
		
}