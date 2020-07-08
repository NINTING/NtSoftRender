#pragma once
#include<memory>
#include"Nt.h"
#include"Ntmath.h"


class NtSofterRender;
class Model;
class ShadowMap
{
public:
	ShadowMap() {};
	ShadowMap(NtSofterRender*render, int w, int h);

	std::shared_ptr<Tex2D_1F> BuildDepthMap(const std::vector<Model>&models);
	void SetMainLight(const NtCamera&camera);
	std::shared_ptr<Tex2D_1F> GetShadowMap()const { return DepthMap_; }
	NtMatrix4x4 GetViewMatrix()const { return lightCamera_.GetViewMatrix();}
	NtMatrix4x4 GetProjMatrix()const { return lightCamera_.GetProjMatrix();}
	~ShadowMap() {  }
private:
	NtCamera lightCamera_;
	NtSofterRender*render_;
	NtViewport vp_;
	int Width_;
	int Height_;
	//float* DepthMap_;
	std::shared_ptr<Tex2D_1F> DepthMap_;
};