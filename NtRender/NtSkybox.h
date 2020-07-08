#pragma once
#include"NtCube.h"
#include"Model.h"


class NtSofterRender;
class NtCamera;
class NtSkybox
{
public:
	NtSkybox();

	void draw(NtSofterRender* render, const NtCamera& camera);
	Model unitsphere;
	std::shared_ptr<NtCubeTex4F> skyimg;
};