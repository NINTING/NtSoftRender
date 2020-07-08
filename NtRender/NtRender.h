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
	Skybox,
};
enum ProjMatrix
{
	frustum,
	virsualbody
};
enum RenderTarget
{
	BackBufferOn = 0x0001,
	BackBufferOff = 0x11110,
	ZwriteOn = 0x0010,
	ZwriteOff = 0x11101,
	BlendOn = 0x0100,
	BlendOff = 0x11011,
	RTTOn = 0x01000,
	RTTOff = 0x10111
};
enum CullEnum
{
	Cullback,
	CullFront,
	NoCull
};
enum ZTestEnum
{
	Less,LessEqual,Equal
};

enum BlendFactor
{
	One,
	Zero,
	AlphaSrc,
	OneMinusAlphaDest,
};



class NtSofterRender
{

public:
	
	void CleanBackAndDepthBuffer();

private:
	RenderState rs_ = RenderState::Wireframe;
	int rt_ = RenderTarget::BackBufferOn|ZwriteOn;
	ProjMatrix pm_ = frustum;
	CullEnum CullState = Cullback;
	ZTestEnum zTest = Less;
public:
	static std::shared_ptr<NtSofterRender>& Instance(int Width = 0, int Height = 0)
	{
		static std::shared_ptr<NtSofterRender> instance (new NtSofterRender(Width, Height));
		return instance;
	}

	void Draw();

	Tex2D_UC  Present();
	/*初始化*/
	NtSofterRender(int w, int h);
public:

	Tex2D_1F* GetDepthBuffer();
	Tex2D_4F* GetBackBuffer();
	CullEnum GetCullState(){return CullState;}
	bool IsWireframe();
public:
	void SetViewport(NtViewport Viewport) {
		this->Viewport_ = Viewport;
	};
	/*输入装配*/
	void SetDepthBuffer(Tex2D_1F *depthBuffer) { DepthBuffer_= depthBuffer; }

	void SetWorldMatrix(const NtMatrix4x4& w) {	this->World_ = w;};
	void SetCullState(CullEnum flag){ CullState = flag;}
	//void SetDiffuseTexture(int slot){	if (slot < 0)return;	CurrTex_ = Texture_[slot];}
	//void SetMaterial(int slot){	if (slot < 0)return;	CurrMat_ = Mat_[slot];}
	//void SetAmbient(const NtVector4&amb){	Ambient_ = amb;}
	void SetVertexBuffer(const std::vector<NtVertex>&vs);
	void SetIndexBuffer(const std::vector<std::vector<Uint32>>&vs);
	void SetShader(NtShader* s) { shader_.reset(s); }
	//void SetCameraPos(const NtVector3& pos) { EyePosW_ = pos; }
	//NtVertex GetVertexs(size_t i) { return VertexBuffer_[i]; }
	void SetRTT(Tex2D_4F*target);
	void ResetRTT();

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
	void SetZTestStat(ZTestEnum zt);
	void SetRenderState(RenderState rs);
	void SetRenderTarget(RenderTarget target);
	void SetBlendFactor(BlendFactor src, BlendFactor dest);

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
	//输出合并
	bool ZTest(int x, int y, float z);
	void OutputMerge(int x, int y, float z, NtVector4 color);
	NtVector4 Blend(const NtVector4& src,const NtVector4& dest);
	void CleanZBuffer() { 
		ZBuffer_->FillImage(1);
	}

private:
	std::shared_ptr<NtShader> shader_;
	std::vector<NtVertex> VertexBuffer_;
	std::vector<std::vector<Uint32>> IndexBuffer_;
	std::shared_ptr<Tex2D_UC> Texture_[10];
	std::shared_ptr<MaterialConstant> Mat_[10];

	std::shared_ptr<Tex2D_UC> CurrTex_;
	std::shared_ptr<MaterialConstant>CurrMat_;

	Tex2D_4F * FrontBuffer_;
	Tex2D_4F * BackBuffer_;
	Tex2D_4F * tmpBackBuffer_;	//临时放置后缓存区
	Tex2D_1F * DepthBuffer_;
	Tex2D_1F * ZBuffer_;
	Tex2D_4F * RenderTexture_;
	NtMatrix4x4 World_;
	NtMatrix4x4 View_;
	NtMatrix4x4 Proj_;
	NtViewport Viewport_;
	std::vector<std::shared_ptr<NtVertexOutBaseFormat>>VertexOuts;
	std::vector<std::vector<std::shared_ptr<NtVertexOutBaseFormat>>>Primitive;
	NtVector3 EyePosW_;
	NtVector4 Ambient_;
	
	std::vector<std::shared_ptr<LightConstant>>DirectionalLights_;



	BlendFactor srcFactor = One,destFactor = Zero;
	

};



#endif // !RENDER_H
