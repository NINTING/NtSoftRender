#pragma once
#include"Nt.h"
#include"tgaImage.h"
#include"Model.h"
namespace Geometry{
	void Line(NtVector3 v0, NtVector3 v1, NtImage*img, NtColor color);

	void Trigle(NtVector3 v0, NtVector3 v1, NtVector3 v2, NtImage*img, NtColor color);

	void BarycentricTriangle(NtVertex v0, NtVertex v1, NtVertex v2, NtImage*img, const NtImage *diffuse, float intensity);


	Model CreateCube();

};