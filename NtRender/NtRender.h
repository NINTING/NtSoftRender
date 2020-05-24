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

class NtSofterRender
{

public:
	struct MaterialConstant;
	enum RenderState
	{
		Wireframe,
		GroundShading,
	};
	struct VertexOut
	{
		NtVector3 PosW;//世界坐标
		NtVector4 PosH;//齐次裁剪空间坐标
		NtColor Color;
		VertexOut Interpolation(const VertexOut& rhs, float t)const;
		VertexOut&& PerspectiveCorrectionInterpolation(const VertexOut& rhs, float t);
	};
	using VertexOut = NtSofterRender::VertexOut;
private:
	RenderState rs_ = RenderState::GroundShading;
public:
	static std::shared_ptr<NtSofterRender>& Instance(int Width = 0, int Height = 0)
	{
		static std::shared_ptr<NtSofterRender> instance (new NtSofterRender(Width, Height));
		return instance;
	}
	void Draw();

	NtImage<Uint32> Present();
	/*初始化*/
	NtSofterRender(int w, int h);
public:
	NtMatrix4x4& GetWorldMatrix() {
		return this->World_;
	};
	NtMatrix4x4& GetViewMatrix() {
		return this->View_;
	};
	NtMatrix4x4& GetProjMatrix() {
		return this->Proj_;
	};
	NtVector3& GetLightDir() {
		return LightDir_;
	};
	bool GetBackCull()
	{
		return IsBackCull;
	}
	Uint32 GetTextureFregment32( float x, float y)
	{
		return CurrTex_->Get(x, y);
	}
	MaterialConstant* GetMaterial()
	{
		return CurrMat_.get();
	}
public:
	void SetViewport(NtViewport Viewport) {
		this->Viewport_ = Viewport;
	};
	/*输入装配*/
	void SetWorldMatrix(const NtMatrix4x4& w) {
		this->World_ = w;
	};
	void SetLightDir(const NtVector3  &ld) {
		this->LightDir_ = ld;
	};
	void SetViewMatrix(const NtMatrix4x4& v) {
		this->View_ = v;
	};
	void SetProjMatrix(const NtMatrix4x4& p) {
		this->Proj_ = p;
	};
	void SetBackCull(bool flag)
	{
		IsBackCull = flag;
	}
	void SetTexture(int slot)
	{
		CurrTex_ = Texture_[slot];
	}
	void SetMaterial(int slot)
	{
		CurrMat_ = Mat_[slot];
	}
	void SetVertexBuffer(const std::vector<NtVertex>&vs);
	void SetIndexBuffer(const std::vector<std::vector<Uint32>>&vs);
	void SetShader(NtShader* s) { shader_.reset(s); }
	NtVertex GetVertexs(size_t i) { return VertexBuffer_[i]; }
public:
	void AddTexture(int slot, std::shared_ptr<NtImage<Uint32>>tex)
	{
		Texture_[slot] = tex;
	}
	void AddMaterial(int slot, std::shared_ptr<Material>mat)
	{
		Mat_[slot] = mat;
	}
public:
	void SetRenderState(RenderState rs);
private:
	/*顶点变换*/
	void VertexProcessing();
	void PrimitiveAssemblyAndClip();
	void ClearPrimitive();
	void  HomogeneousCoordinateCropping(ptrBase  Tri[]);
	/*光栅化*/
	void Rasterization();
	void Wirframe(const NtVertexOutBaseFormat* v0, const NtVertexOutBaseFormat* v1, const  NtVertexOutBaseFormat* v2);
	void BarycentricTriangle(const NtVertexOutBaseFormat* v0, const NtVertexOutBaseFormat* v1, const  NtVertexOutBaseFormat* v2);
	void Line(const NtVertexOutBaseFormat* v0, const  NtVertexOutBaseFormat* v1);
	void OutputMerge(int x, int y, float z, NtColor color);



private:
	std::shared_ptr<NtShader> shader_;
	std::vector<NtVertex> VertexBuffer_;
	std::vector<std::vector<Uint32>> IndexBuffer_;
	std::shared_ptr<NtImage<Uint32>> Texture_[10];
	std::shared_ptr<MaterialConstant> Mat_[10];

	std::shared_ptr<NtImage<Uint32>> CurrTex_;
	std::shared_ptr<MaterialConstant>CurrMat_;

	NtImage<Uint32>* FrontBuffer_;
	NtImage<Uint32>* BackBuffer_;
	NtImage<float>*ZBuffer_;
	NtMatrix4x4 World_;
	NtMatrix4x4 View_;
	NtMatrix4x4 Proj_;
	NtViewport Viewport_;
	std::vector<std::shared_ptr<NtVertexOutBaseFormat>>VertexOuts;
	std::vector<std::vector<std::shared_ptr<NtVertexOutBaseFormat>>>Primitive;
	NtVector3 LightDir_;
	bool IsBackCull;
};



#endif // !RENDER_H
