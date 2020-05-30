#pragma once
#ifndef RENDER_H
#define RENDER_H
#include"Ntmath.h"
#include"NtImage.h"
#include"NtShader.h"
#include"Nt.h"
#include<SDL.h>
#include<vector>
#include"NtImage.h"
#include"LightHelper.h"

enum RenderState
{
	Wireframe,
	GroundShading,
};
enum RenderTarget
{
	OnlyDepthWtite,
	BackBufferWrite
};

class NtSofterRender
{

public:
	
	void CleanBackAndDepthBuffer();
	

private:
	RenderState rs_ = RenderState::Wireframe;
	RenderTarget rt_ = RenderTarget::BackBufferWrite;
public:
	static std::shared_ptr<NtSofterRender>& Instance(int Width = 0, int Height = 0)
	{
		static std::shared_ptr<NtSofterRender> instance (new NtSofterRender(Width, Height));
		return instance;
	}

	void Draw();

	NtImage Present();
	/*初始化*/
	NtSofterRender(int w, int h);
public:
//	NtMatrix4x4& GetWorldMatrix() {return this->World_;}
//	NtMatrix4x4& GetViewMatrix() {return this->View_;}
//	NtMatrix4x4& GetProjMatrix() {return this->Proj_;}
//	std::vector<std::shared_ptr<LightConstant>>& GetDirectionLights() {return DirectionalLights_;}
	float* GetDepthBuffer();
//	NtVector4& GetAmbient() { return Ambient_; }
//	NtVector3& GetCameraPos() { return EyePosW_ ; }

	bool GetBackCull(){return IsBackCull;}
//	Uint32 GetTextureFregment32( float x, float y){return CurrTex_->Get(x, y);}
//	MaterialConstant* GetMaterial(){return CurrMat_.get();}
//	NtVector3 GetLightDir() { return DirectionalLights_[0]->Direction; };
public:
	void SetViewport(NtViewport Viewport) {
		this->Viewport_ = Viewport;
	};
	/*输入装配*/
	void SetDepthBuffer(float*depthBuffer) { DepthBuffer_= depthBuffer; }

	void SetWorldMatrix(const NtMatrix4x4& w) {	this->World_ = w;};
	//void SetViewMatrix(const NtMatrix4x4& v) {	this->View_ = v;};
	//void SetProjMatrix(const NtMatrix4x4& p) {	this->Proj_ = p;};
	void SetBackCull(bool flag){IsBackCull = flag;}
	//void SetDiffuseTexture(int slot){	if (slot < 0)return;	CurrTex_ = Texture_[slot];}
	//void SetMaterial(int slot){	if (slot < 0)return;	CurrMat_ = Mat_[slot];}
	//void SetAmbient(const NtVector4&amb){	Ambient_ = amb;}
	void SetVertexBuffer(const std::vector<NtVertex>&vs);
	void SetIndexBuffer(const std::vector<std::vector<Uint32>>&vs);
	void SetShader(NtShader* s) { shader_.reset(s); }
	//void SetCameraPos(const NtVector3& pos) { EyePosW_ = pos; }
	//NtVertex GetVertexs(size_t i) { return VertexBuffer_[i]; }
public:
	/*
	void AddTexture(int slot, std::shared_ptr<NtImage>tex)
	{
		if (slot < 0)return;
		Texture_[slot] =  tex;
	}
	void AddMaterial(int slot,const Material& mat)
	{
		if (slot < 0)return;
		Mat_[slot] = std::make_shared<MaterialConstant>(mat);
	}
	void AddLight(const std::shared_ptr<Light> lc)
	{
		if (lc->type == DirectionalLight) {
			lc->index = DirectionalLights_.size();
			DirectionalLights_.push_back(std::shared_ptr<LightConstant>(lc->GetLightConstant()));
		}
	}
	*/
public:
	void SetRenderState(RenderState rs);
	void SetRenderTarget(RenderTarget target) { rt_ = target; }
private:
	/*顶点变换*/
	void VertexProcessing();
	void PrimitiveAssemblyAndClip();
	void ClearPrimitive();
	void  HomogeneousCoordinateCropping(ptrBase  Tri[]);
	bool AllInside(ptrBase Tri[]);
	/*光栅化*/
	void Rasterization();
	void Wirframe(const NtVertexOutBaseFormat* v0, const NtVertexOutBaseFormat* v1, const  NtVertexOutBaseFormat* v2);
	void BarycentricTriangle(const NtVertexOutBaseFormat* v0, const NtVertexOutBaseFormat* v1, const  NtVertexOutBaseFormat* v2);
	void Line(const NtVertexOutBaseFormat* v0, const  NtVertexOutBaseFormat* v1);
	void OutputMerge(int x, int y, float z, NtColor color);
	
	void CleanZBuffer() { 
		memset(ZBuffer_, 0, BackBuffer_->GetImageSize() * sizeof(float));
	}

private:
	std::shared_ptr<NtShader> shader_;
	std::vector<NtVertex> VertexBuffer_;
	std::vector<std::vector<Uint32>> IndexBuffer_;
	std::shared_ptr<NtImage> Texture_[10];
	std::shared_ptr<MaterialConstant> Mat_[10];

	std::shared_ptr<NtImage> CurrTex_;
	std::shared_ptr<MaterialConstant>CurrMat_;

	NtImage* FrontBuffer_;
	NtImage* BackBuffer_;
	float*DepthBuffer_;
	float*ZBuffer_;
	NtMatrix4x4 World_;
	NtMatrix4x4 View_;
	NtMatrix4x4 Proj_;
	NtViewport Viewport_;
	std::vector<std::shared_ptr<NtVertexOutBaseFormat>>VertexOuts;
	std::vector<std::vector<std::shared_ptr<NtVertexOutBaseFormat>>>Primitive;
	NtVector3 EyePosW_;
	NtVector4 Ambient_;
	
	std::vector<std::shared_ptr<LightConstant>>DirectionalLights_;
	bool IsBackCull;
	
};



#endif // !RENDER_H
