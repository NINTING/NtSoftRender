#pragma once
#ifndef NT_H
#define NT_H
#include<assert.h>
#include"SDL.h"
#include <memory>
#include <algorithm>
#include<vector>
#include<list>
#include"Ntmath.h"
#include"NtImage.h"
#include"NtShader.h"

class NtColor;


class NtWindow;


class NtSofterRender;
class NtShader;



class NtVertex
{
public:
	NtVertex() {};
	
	NtVertex(NtVector3 Position, NtVector3 Diffuse = NtVector3(), NtVector3 Normal = NtVector3(), NtVector3 TangentU = NtVector3())
		:Position(Position), Diffuse(Diffuse), Normal(Normal) {}
	NtVector3 GetPostion()const { return Position; }
	NtVector3 GetDiffuse()const { return Diffuse; }
	NtVector3 GetNormal()const { return Normal; }
	void SetPosition(NtVector3 Position) { Position = Position; }
	void SetDiffuse(NtVector3 Diffuse) { Position = Diffuse; }
	void SetNormal(NtVector3 Normoal) { Position = Normoal; }
	NtVertex Interpolation(const NtVertex& rhs, float t);

	NtVector3 Position;
	NtVector3 Diffuse;
	NtVector3 Normal;
	NtVector3 TangentU;
};


class NtViewport
{
public:
	float TopLeftX;
	float TopLeftY;
	float width;
	float height;
	float MinDepth;
	float MaxDepth;
};

class NtWindow
{
public:
	NtWindow(int Width,int Height)
	{
		init(Width,Height);
	}
	static std::shared_ptr<NtWindow>& Instance(int Width=0,int Height=0)
	{
		static std::shared_ptr<NtWindow> instance = std::make_shared<NtWindow>(Width, Height);
		return instance;
	}

	void init(int Width, int height);
	void PresentWindow();
	void Present_image(const Tex2D_UC &img);

	void FillWindow(const Tex2D_UC&image);
	SDL_Renderer* GetSDL_Render() { return gRender_; }
	int GetWindowWidth();
	int GetWindowHeight();
	void SetTitle(const char* s)
	{
		SDL_SetWindowTitle(gWindow_, s);
	}
	~NtWindow()
	{
		
		SDL_DestroyRenderer(gRender_);
		SDL_DestroyTexture(gTexture_);
		SDL_DestroyWindow(gWindow_);
		SDL_Quit();
	}
	float AspectRadio();
private:
	
	SDL_Window* gWindow_ = NULL;
	SDL_Renderer* gRender_ = NULL;
	SDL_Texture* gTexture_ = NULL;
};

struct Frustum
{
	float nearZ, farZ;
	float aspect;
	float nearH, farH;
	float fovY;
};
struct VRectangle
{
	float Width, Heiht;
	float Near, Far;
};


class NtCamera
{
public:
	static std::vector<NtVector4> NDCface;
	NtCamera() {};
	void LookAt(const NtVector3& Pos,const NtVector3& Target,const NtVector3& Up);
	void Update();
	void SetViewFrustum(float FovAngleY, float Aspect, float NearZ, float FarZ);
	void SetVisualBody(float Width, float Height, float NearZ, float FarZ);
	NtMatrix4x4 GetViewMatrix()const;
	NtMatrix4x4 GetProjMatrix()const;
	void Walk(float d);
	void Strafe(float d);
	void RotateY(float d);
	void Pitch(float d);
	NtVector3 GetPos() const{ return Pos_; }
private:
	//摄像机
	NtVector3 Pos_ = NtVector3(0, 0, 0);
	NtVector3 Up_ = NtVector3(0, 1, 0);
	NtVector3 Right_ = NtVector3(1, 0, 0);
	NtVector3 Look_ = NtVector3(0, 0, 1);
	bool ViewDirty_;
	//视锥
	union body
	{
		Frustum frustum;
		VRectangle rectangle;
	}VisualBody;

	//变换矩阵
	NtMatrix4x4 View_;
	NtMatrix4x4 Proj_;
};

NtMatrix4x4 NtMatrixViewport(const NtViewport&vp);

//外部使用
struct Material
{
	std::string name;		//材质对应的名称
	int diffTextureId = -1;	//漫反射纹理在纹理缓冲区中的索引
	int MaterialId = -1;	//材质缓冲区中的索引

	//存入缓冲区中的数据
	
	NtVector4 diffuseAlbedo = { 1.f, 1.f, 1.f, 1.f };		//漫反射率 
	NtVector3 FresnelR0 = { 0.01f, 0.01f, 0.01f };	//菲涅尔反射率
	float Roughness = 0.25;									//粗糙度  shininess = 1- Roughness 光滑度
	//NtMatrix4x4 MatTransform;	//材质纹理伸缩变换
}; 


//缓存使用
struct MaterialConstant
{
	MaterialConstant() {}
	MaterialConstant(NtVector4 Albedo, NtVector3 R0, float roughness):diffuseAlbedo(Albedo), FresnelR0(R0),Roughness(roughness) {}

	MaterialConstant(const Material& m)
		:diffuseAlbedo(m.diffuseAlbedo), FresnelR0(m.FresnelR0), Roughness(m.Roughness) {}
	NtVector4 diffuseAlbedo;		//漫反射率 
	NtVector3 FresnelR0;	//菲涅尔反射率
	float Roughness;
};


#endif