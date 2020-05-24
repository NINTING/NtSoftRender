#include"Model.h"
#include"NtUtility.h"
#include"NtRender.h"
#include<fstream>
#include<sstream>

Model::Model(std::string fileName)
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
				for (int i = 0; i < 3; i++)
					iss >> vt[i];
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
				while (iss>>v>>ttrash>>vt>>ttrash>>vn)
				{
					//索引从0开始
					Vertexs_.push_back(NtVertex(Position[v-1], Texture[vt-1], Normal[vn-1]));
					f.push_back(VCount++);
				}
				Faces_.push_back(f);
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
	NtUtility::Read_Tga_file(name, diffuse.get());
}

void Model::Assemble(NtSofterRender*render)
{


	Model cube("cube.obj");
	render->SetVertexBuffer(Vertexs_);
	render->SetIndexBuffer(Faces_);

	NtMatrix4x4 world = NtMatrix4x4();
	render->SetWorldMatrix(world);
	std::shared_ptr< NtImage<Uint32>>difftexture = std::make_shared<NtImage<Uint32>>();
	NtUtility::Read_Tga_file("african_head_diffuse.tga", difftexture.get());
	render->AddTexture(0, difftexture);
	GroudShader* gs = new GroudShader();

	TGroudShader*tgs = new TGroudShader();
	render->SetShader(tgs);
}

void Model::Init()
{
	
}


Model::Model(std::vector<NtVertex> vertexs, std::vector<std::vector<Uint32>> Faces)
{
	Vertexs_ = vertexs;
	Faces_ = Faces;
}