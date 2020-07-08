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
	std::shared_ptr<Tex2D_4F> GetEmissionTex() const { return  EmissionTexture; }
	std::vector<std::vector<Uint32>> GetIndicesBuffer() const { return Faces_; }
	std::vector<NtVertex> GetVertexsBuffer() const { return Vertexs_; }
	void SetMaterial(const Material&);
	void SetMainTexture(const char*name);
	void SetNormalTexture(const char*name);
	void SetSpecularTexture(const char*name);
	void SetTangentTexture(const char*name);
	void SetEmissionTexture(const char*name);
	void Assemble(NtSofterRender*render);
	void Init();
	void SetWorld(const NtMatrix4x4&world) { World_ = world; };
	NtMatrix4x4 GetWorld() const{ return World_; }
	void mergeVertexs(std::vector<NtVertex>& vertexs);
	void mergeIndices(std::vector<std::vector<Uint32>>& Indicess);
private :
	NtMatrix4x4 World_;
	std::vector<NtVertex> Vertexs_;
	std::vector<std::vector<Uint32>>Faces_;
	Material mat_;
	std::shared_ptr<Tex2D_4F> diffuseTex;
	std::shared_ptr<Tex2D_4F> normalTex;
	std::shared_ptr<Tex2D_4F> specularTex;
	std::shared_ptr<Tex2D_4F> tangentTex;
	std::shared_ptr<Tex2D_4F>  EmissionTexture;
};
/*
void Model::mergeVertexs(std::vector<NtVertex>& vertexs)
{
	Vertexs_.resize(vertexs.size()+Vertexs_.size());
	std::copy(vertexs.begin(), vertexs.end(), Vertexs_.end());
}
void Model::mergeIndices(std::vector<std::vector<Uint32>>& Indicess)
{
	Faces_.resize(Indicess.size()+Faces_.size());
	std::copy(Indicess.begin(), Indicess.end(), Faces_.end());
}

Model MergeModel(std::vector<const Model&>models)
{

	Model ret = models[0];
	for (int i = 1;i<models.size();i++)
	{
		ret.mergeVertexs(models[i]);
		ret.mergeIndices()
	}
}

*/


