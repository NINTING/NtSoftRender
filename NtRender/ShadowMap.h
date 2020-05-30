#pragma once
#include<memory>
#include"Nt.h"
#include"Ntmath.h"

class NtImage;
class NtSofterRender;
class Model;
class ShadowMap
{
public:
	ShadowMap() {};
	ShadowMap(NtSofterRender*render, int w, int h);

	std::shared_ptr<NtImage> BuildDepthMap(const NtMatrix4x4&w, const Model&model);
	void SetMainLight(const NtCamera&camera);
	std::shared_ptr<NtImage> GetShadowMap()const { return DepthMap_; }
	NtMatrix4x4 GetViewMatrix()const { return lightCamera_.GetViewMatrix(); }
	NtMatrix4x4 GetProjMatrix()const { return lightCamera_.GetProjMatrix(); }
	~ShadowMap() {  }
private:
	NtCamera lightCamera_;
	NtSofterRender*render_;
	NtViewport vp_;
	int Width_;
	int Height_;
	//float* DepthMap_;
	std::shared_ptr<NtImage> DepthMap_;
};