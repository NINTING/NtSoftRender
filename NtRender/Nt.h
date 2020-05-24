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
template<typename T> class NtImage;

class NtWindow;


class NtSofterRender;
class NtShader;



class NtVertex
{
public:
	NtVertex() {};
	NtVertex(NtVector3 Position, NtVector3 Diffuse = NtVector3(), NtVector3 Normal = NtVector3())
		:Position_(Position), Diffuse_(Diffuse), Normal_(Normal) {}
	NtVector3 GetPostion()const { return Position_; }
	NtVector3 GetDiffuse()const { return Diffuse_; }
	NtVector3 GetNormal()const { return Normal_; }
	void SetPosition(NtVector3 Position) { Position_ = Position; }
	void SetDiffuse(NtVector3 Diffuse) { Position_ = Diffuse; }
	void SetNormal(NtVector3 Normoal) { Position_ = Normoal; }
	NtVertex Interpolation(const NtVertex& rhs, float t);
private:
	NtVector3 Position_;
	NtVector3 Diffuse_;
	NtVector3 Normal_;
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
	void FillWindow(const NtImage<Uint32>&image);

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





class NtCamera
{
public:
	static std::vector<NtVector4> NDCface;
	NtCamera() {};
	void LookAt(const NtVector3& Pos,const NtVector3& Target,const NtVector3& Up);
	void Update();
	void SetViewFrustum(float FovAngleY, float Aspect, float NearZ, float FarZ);
	NtMatrix4x4 GetViewMatrix();
	NtMatrix4x4 GetProjMatrix();
	void Walk(float d);
	void Strafe(float d);
	void RotateY(float d);
	void Pitch(float d);

private:
	//摄像机
	NtVector3 Pos_ = NtVector3(0, 0, 0);
	NtVector3 Up_ = NtVector3(0, 1, 0);
	NtVector3 Right_ = NtVector3(1, 0, 0);
	NtVector3 Look_ = NtVector3(0, 0, 1);
	bool ViewDirty_;
	//视锥
	float nearZ_, farZ_;
	float aspect_;
	float nearH_, farH_;
	float fovY;

	//变换矩阵
	NtMatrix4x4 View_;
	NtMatrix4x4 Proj_;
};

NtMatrix4x4 NtMatrixViewport(const NtViewport&vp);


struct Material
{
	std::string name;		//材质对应的名称
	int diffTextureId = -1;	//漫反射纹理在纹理缓冲区中的索引
	int MaterialId = -1;	//材质缓冲区中的索引

	//存入缓冲区中的数据
	
	NtColor diffuseAlbedo = { 1.f, 1.f, 1.f, 1.f };		//漫反射率 
	NtColor FresnelR0 = { 0.01f, 0.01f, 0.01f, 0.01f };	//菲涅尔反射率
	float Roughness = 0.25;									//粗糙度  shininess = 1- Roughness 光滑度
	//NtMatrix4x4 MatTransform;	//材质纹理伸缩变换
}; 
#endif