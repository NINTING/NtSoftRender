#pragma once
#include"Nt.h"
#include"tgaImage.h"
#include"Model.h"
namespace Geometry{
	//void Line(NtVector3 v0, NtVector3 v1, NtImage*img, NtColor color);

	//void Trigle(NtVector3 v0, NtVector3 v1, NtVector3 v2, NtImage*img, NtColor color);

	//void BarycentricTriangle(NtVertex v0, NtVertex v1, NtVertex v2, NtImage*img, const NtImage *diffuse, float intensity);
	/*level 垂直分层，slice 水平顶点数*/
	Model Sphere(float radius, int levels, int slices);
	Model Patch2D();
	Model plane(float w, float h);
	//Model CreateCube();

};