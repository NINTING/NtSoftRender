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
					//索引从0开始
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
				v0.TangentU.normalize();
				v1.TangentU.normalize();
				v2.TangentU.normalize();
			}
		}
	}
}


void Model::SetMaterial(const Material&mat)
{
	mat_ = mat;
}
void Model::SetMainTexture(const char*name)
{
	//NtUtility::Read_file(name);
	Tex2D_UC diffTex = NtUtility::Read_file(name);
	diffuseTex = std::make_shared<Tex2D_4F>();

	
	*diffuseTex = RGBAImageToFloat4Image(diffTex);// (std::move());
	
}

void Model::SetNormalTexture(const char*name)
{
	Tex2D_UC norTex = NtUtility::Read_file(name);
	normalTex = std::make_shared<Tex2D_4F>();
	*normalTex = (std::move(RGBAImageToFloat4Image(norTex)));
}
void Model::SetSpecularTexture(const char*name)
{
	Tex2D_UC specuTex = NtUtility::Read_file(name);
	specularTex = std::make_shared<Tex2D_4F>();
	*specularTex = (std::move(RGBAImageToFloat4Image(specuTex)));
}
void Model::SetTangentTexture(const char*name)
{
	Tex2D_UC tanTex = NtUtility::Read_file(name);
	tangentTex = std::make_shared<Tex2D_4F>();
	*tangentTex = (std::move(RGBAImageToFloat4Image(tanTex)));
}

void Model::SetEmissionTexture(const char*name)
{
	Tex2D_UC EmissionTex = NtUtility::Read_file(name);
	EmissionTexture = std::make_shared<Tex2D_4F>();
	*EmissionTexture = (std::move(RGBAImageToFloat4Image(EmissionTex)));
}

void Model::Assemble(NtSofterRender*render)
{

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