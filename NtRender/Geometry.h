#pragma once
#include"Nt.h"
#include"tgaImage.h"
#include"Model.h"
namespace Geometry{
	void Line(NtVector3 v0, NtVector3 v1, NtImage<Uint32>*img, NtColor color);

	void Trigle(NtVector3 v0, NtVector3 v1, NtVector3 v2, NtImage<Uint32>*img, NtColor color);

	void BarycentricTriangle(NtVertex v0, NtVertex v1, NtVertex v2, NtImage<Uint32>*img, const NtImage<Uint32> *diffuse, float intensity);


	Model CreateCube();

};