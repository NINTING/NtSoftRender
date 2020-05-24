#pragma once
#include"Nt.h"
#include<vector>
#include<string>




class Model
{
public:

	Model(std::string fileName);
	Model(std::vector<NtVertex> vertexs, std::vector<std::vector<Uint32>> Faces);
	NtVertex   GetVertex(int i)const { return Vertexs_[i]; }
	std::vector<Uint32>Getface(int i) const{ return Faces_[i]; }
	int GetVertexSize() const{ return Vertexs_.size(); }
	int GetFacesSize() const{ return Faces_.size(); }
	std::vector<std::vector<Uint32>> GetIndicesBuffer() { return Faces_; }
	std::vector<NtVertex> GetVertexsBuffer() { return Vertexs_; }
	void SetMaterial(const Material&);
	void SetTexture(const char*name);
	void Assemble(NtSofterRender*render);
	void Init();
	void SetWorld(const NtMatrix4x4&world) { World_ = world; };
private :
	NtMatrix4x4 World_;
	std::vector<NtVertex> Vertexs_;
	std::vector<std::vector<Uint32>>Faces_;
	Material mat_;
	std::shared_ptr<NtImage<Uint32>> diffuse;
	
};




