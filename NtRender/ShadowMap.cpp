#include"ShadowMap.h"
#include"NtRender.h"
#include"NtImage.h"
#include"Effect.h"
#include"Model.h"
ShadowMap::ShadowMap(NtSofterRender*render, int w, int h)
{
	render_ = render;
	Width_ = w;
	Height_ = h;
	vp_ = {0.f,0.f,float(w),float(h),0.01f,1.f};
	

}	
std::shared_ptr<NtImage> ShadowMap::BuildDepthMap(const NtMatrix4x4&w, const Model&model)
{
	float* DepthMap = new float[Width_*Height_];

	std::fill(DepthMap,DepthMap+(Width_*Height_),255);
	render_->SetRenderTarget(RenderTarget::OnlyDepthWtite);

//	std::shared_ptr<ShadowShader>shadowshader = std::make_shared<ShadowShader>();
	ShadowShader* shadowshader = new ShadowShader();
	shadowshader->p = lightCamera_.GetProjMatrix();
	shadowshader->v = lightCamera_.GetViewMatrix();
	shadowshader->w = w;
	render_->SetViewport(vp_);
	render_->SetShader(shadowshader);
	render_->SetVertexBuffer(model.GetVertexsBuffer());
	render_->SetIndexBuffer(model.GetIndicesBuffer());
	render_->SetDepthBuffer(DepthMap);
	render_->Draw();

	render_->SetDepthBuffer(nullptr);
	NtColor depthC;
	DepthMap_ = std::make_shared<NtImage>(Width_, Height_, 1);
	int size = Width_ * Height_;
	for (int i = 0; i < size; i++)
	{
	
		depthC.R_ = DepthMap[i];	//zBuffer记录了1/z的深度需要计算为1/z*255==255/z;
		DepthMap_->Set(i, depthC);
	}
	delete[] DepthMap;
	return DepthMap_;
}
	
void ShadowMap::SetMainLight(const NtCamera&Light)
{
	lightCamera_ = Light;
}