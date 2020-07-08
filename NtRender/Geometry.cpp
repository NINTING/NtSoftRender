


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


Model Geometry::Sphere(float radius, int levels, int slices)
{
	
	
	int vertexCount =2 + (levels - 1)*(slices + 1);
	int indexCount = 6 * (levels - 1) * slices;
	std::vector<NtVertex> vertexs;
	std::vector<std::vector<Uint32>> indices;
	float theta = 0.f,phi = 0.f;
	float perPhi = M_PI / levels;
	float perTheta = 2*M_PI / slices;
	float x, y, z;
	NtVertex vertex;
	vertex = NtVertex(NtVector3(0, radius, 0), NtVector3(0, 0,0), NtVector3(0, 1, 0),NtVector3(1,0,0));
	vertexs.push_back(vertex);
	for (int i = 1; i < levels; i++)
	{
		phi = perPhi * i;
		for (int j = 0; j <= slices;j++)
		{
			theta = perTheta * j; 

			x = radius * sinf(phi) * cosf(theta);
			y = radius * cosf(phi);
			z = radius * sinf(phi) * sinf(theta);
			vertex.Normal = vertex.Position = NtVector3(x, y, z);
			vertex.Normal.normalize();
			vertex.Diffuse = NtVector3();
			vertex.TangentU = NtVector3(-sinf(theta), 0.0f, cosf(theta));
			vertex.Diffuse = NtVector3(theta / (2* M_PI), phi / M_PI, 0);
			vertexs.push_back(vertex);

		}

	}

	vertexs.push_back({ NtVector3(0, -radius, 0), NtVector3(0, 1, 0), NtVector3(0, -1, 0), NtVector3(-1, 0, 0) });
	std::vector<Uint32>face(3);
	if (levels > 1)
	{
		for (Uint32 j = 1; j <= slices; ++j)
		{
			face = {0,j%(slices+1)+1,j};
			indices.push_back(face);
		}
	}

	for (Uint32 i = 1; i < levels -1; ++i)
	{
		for (Uint32 j = 1; j <= slices; ++j)
		{
		
			

			face[0] = (i - 1) * (slices + 1) + j;
			face[1] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
			face[2] = i * (slices + 1) + j % (slices + 1) + 1;
			indices.push_back(face);
			face[0] = i * (slices + 1) + j % (slices + 1) + 1;
			face[1] = i * (slices + 1) + j;
			face[2] = (i - 1) * (slices + 1) + j;
			indices.push_back(face);
		}
	}

	if (levels > 1)
	{
		for (Uint32 j = 1; j <= slices; ++j)
		{
			face[0] = (levels - 2) * (slices + 1) + j;
			face[1] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
			face[2] = (levels - 1) * (slices + 1) + 1;
			indices.push_back(face);
		}
	}
	return Model(vertexs,indices);
}

Model  Geometry::plane(float w, float h)
{
	std::vector<NtVertex> vertexs;
	std::vector<std::vector<Uint32>> indices;


	float halfW = w/2.f, halfH = h / 2.f;
	
	vertexs.push_back({ NtVector3(halfW, halfH, 0), NtVector3(0, 0, 0), NtVector3(0, 0, 1), NtVector3(-1, 0, 0) });
	vertexs.push_back({ NtVector3(-halfW, halfH, 0), NtVector3(1, 0, 0), NtVector3(0, 0, 1), NtVector3(-1, 0, 0) });
	vertexs.push_back({ NtVector3(-halfW, -halfH, 0), NtVector3(1, 1, 0), NtVector3(0, 0, 1), NtVector3(-1, 0, 0) });
	vertexs.push_back({ NtVector3(halfW, -halfH, 0), NtVector3(0, 1, 0), NtVector3(0, 0, 1), NtVector3(-1, 0, 0) });
	std::vector<Uint32>face(3);
	face[0] = 0;
	face[1] = 1;
	face[2] = 3;
	indices.push_back(face);
	face[0] = 1;
	face[1] = 2;
	face[2] = 3;
	indices.push_back(face);
	return Model(vertexs, indices);
}