#pragma once
#include"Nt.h"
#include<vector>
#include<string>




class Model
{
public:
	Model()=default;
	Model(std::string fileName);
	
	Model(std::vector<NtVertex>& vertexs, std::vector<std::vector<Uint32>>& Faces);
	void init(std::string fileName);
	void init(std::vector<NtVertex>& vertexs, std::vector<std::vector<Uint32>>& Faces);
	NtVertex   GetVertex(int i)const { return Vertexs_[i]; }
	std::vector<Uint32>Getface(int i) const{ return Faces_[i]; }
	int GetVertexSize() const{ return Vertexs_.size(); }
	int GetFacesSize() const{ return Faces_.size(); }
	Material GetMaterial() const{ return mat_; }
	std::shared_ptr<Tex2D_4F> GetDiffuseTex()const { return diffuseTex; }
	std::shared_ptr<Tex2D_4F> GetNormalTex() const { return  normalTex; }
	std::shared_ptr<Tex2D_4F> GetTangentTex() const { return  tangentTex; }
	std::shared_ptr<Tex2D_4F> GetSpecularTex() const { return  specularTex; }
	std::vector<std::vector<Uint32>> GetIndicesBuffer() const { return Faces_; }
	std::vector<NtVertex> GetVertexsBuffer() const { return Vertexs_; }
	void SetMaterial(const Material&);
	void SetMainTexture(const char*name);
	void SetNormalTexture(const char*name);
	void SetSpecularTexture(const char*name);
	void SetTangentTexture(const char*name);
	void Assemble(NtSofterRender*render);
	void Init();
	void SetWorld(const NtMatrix4x4&world) { World_ = world; };
private :
	NtMatrix4x4 World_;
	std::vector<NtVertex> Vertexs_;
	std::vector<std::vector<Uint32>>Faces_;
	Material mat_;
	std::shared_ptr<Tex2D_4F> diffuseTex;
	std::shared_ptr<Tex2D_4F> normalTex;
	std::shared_ptr<Tex2D_4F> specularTex;
	std::shared_ptr<Tex2D_4F> tangentTex;
};




