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
std::shared_ptr<Tex2D_1F> ShadowMap::BuildDepthMap(const NtMatrix4x4&w, const Model&model)
{
	DepthMap_ = std::make_shared<Tex2D_1F>(Width_,Height_,1.f);

	//std::fill(DepthMap_.get(),DepthMap_.get()+(Width_*Height_),1.f);
	render_->SetRenderTarget(RenderTarget::BackBufferOff);

//	std::shared_ptr<ShadowShader>shadowshader = std::make_shared<ShadowShader>();
	ShadowShader* shadowshader = new ShadowShader();
	shadowshader->p = lightCamera_.GetProjMatrix();
	shadowshader->v = lightCamera_.GetViewMatrix();
	shadowshader->w = w;
	render_->SetViewport(vp_);
	render_->SetShader(shadowshader);
	render_->SetVertexBuffer(model.GetVertexsBuffer());
	render_->SetIndexBuffer(model.GetIndicesBuffer());
	render_->SetDepthBuffer(DepthMap_.get());
	render_->Draw();

	render_->SetDepthBuffer(nullptr);

	
	return DepthMap_;
}
	
void ShadowMap::SetMainLight(const NtCamera&Light)
{
	lightCamera_ = Light;
}