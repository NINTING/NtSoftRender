#include"Model.h"
#include"NtUtility.h"
#include"NtRender.h"
#include<fstream>
#include<sstream>

void Model::init(std::string fileName)
{
	std::string suffix = fileName.substr(fileName.find_last_of(".")+1);
	if (suffix == "obj")
	{
		std::ifstream in(fileName);
		if (in.fail())return;
		std::string line;
		std::vector<NtVector3> Position;
		std::vector<NtVector3> Texture;
		std::vector<NtVector3> Normal;
		int VCount = 0;
		while (!in.eof())
		{
			std::string trash;
		
			std::getline(in, line);
			std::istringstream iss(line);

			if (!line.compare(0, 2, "v "))
			{
				iss >> trash;
				NtVector3 v;
				for (int i = 0; i < 3; i++)
					iss >> v[i];
				Position.push_back(v);
			}
			else if (!line.compare(0, 2, "vt"))
			{
				iss >> trash;
				NtVector3 vt;
				for (int i = 0; i < 3; i++) {
					iss >> vt[i];
				}
				Texture.push_back(vt);
			}
			else if (!line.compare(0, 2, "vn"))
			{
				iss >> trash;
				NtVector3 vn;
				for (int i = 0; i < 3; i++)
					iss >> vn[i];
				Normal.push_back(vn);
			}
			else if (!line.compare(0, 2, "f "))
			{
				char ttrash;
				iss >> ttrash;
				int vt, vn;
				int v;
				std::vector<Uint32>f;
				while (iss >> v >> ttrash >> vt >> ttrash >> vn)
				{
					//������0��ʼ
					Vertexs_.push_back(NtVertex(Position[v - 1], Texture[vt - 1], Normal[vn - 1]));
					f.push_back(VCount++);
				}
				Faces_.push_back(f);

				NtVertex& v0 = Vertexs_[f[0]];
				NtVertex& v1 = Vertexs_[f[1]];
				NtVertex& v2 = Vertexs_[f[2]];
				NtVector3 e0 = v1.Position - v0.Position;
				NtVector3 e1 = v2.Position - v0.Position;
				NtMatrix2x3 sideM({ e0, e1 });
				NtVector2 t0 = NtVector3To2(v1.Diffuse - v0.Diffuse);
				NtVector2 t1 = NtVector3To2(v2.Diffuse - v0.Diffuse);
				NtMatrix2x2 texCoordM({t0,t1});
				NtMatrix2x3 tagent =  ComputerTangent(texCoordM, sideM);
				v0.TangentU = tagent[0];
				v1.TangentU = tagent[0];
				v2.TangentU = tagent[0];
			}
		}
	}
}


void Model::SetMaterial(const Material&mat)
{
	mat_ = mat;
}
void Model::SetTexture(const char*name)
{
	diffuseTex = std::make_shared<NtImage>();
	NtUtility::Read_Tga_file(name, diffuseTex.get());
}

void Model::SetNotmalTexture(const char*name)
{
	normalTex = std::make_shared<NtImage>();
	NtUtility::Read_Tga_file(name, normalTex.get());
}
void Model::SetSpecularTexture(const char*name)
{
	specularTex = std::make_shared<NtImage>();
	NtUtility::Read_Tga_file(name, specularTex.get());
}
void Model::Assemble(NtSofterRender*render)
{

	render->SetVertexBuffer(Vertexs_);
	render->SetIndexBuffer(Faces_);
	render->SetWorldMatrix(World_);
	render->AddTexture(mat_.diffTextureId, diffuseTex);
	render->SetDiffuseTexture(mat_.diffTextureId);
	render->AddMaterial(mat_.MaterialId, mat_);
	render->SetMaterial(mat_.MaterialId);
}

void Model::Init()
{
	
}


Model::Model(std::vector<NtVertex>& vertexs, std::vector<std::vector<Uint32>>& Faces)
{
	init(vertexs, Faces);
}



void Model::init(std::vector<NtVertex>& vertexs, std::vector<std::vector<Uint32>>& Faces)
{
	Vertexs_ = vertexs;
	Faces_ = Faces;
}

Model::Model(std::string fileName)
{
	init(fileName);
}