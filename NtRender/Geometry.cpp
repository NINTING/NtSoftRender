


#include"Geometry.h"
#include"math.h"
#include"tgaImage.h"
#include<algorithm>
using namespace Geometry;




Model Geometry::Patch2D()
{
	std::vector<NtVertex> vertexs({ NtVertex(NtVector3(0, 0, 0)) ,NtVertex(NtVector3(1, 0, 0)) 
		,NtVertex(NtVector3(0, 1, 0)) ,NtVertex(NtVector3(1, 1, 0)) });
	std::vector<std::vector<Uint32>> Faces;
	Faces.push_back({ 0,1,2 });
	Faces.push_back({ 2,1,3 });
	return Model(vertexs, Faces);
}


