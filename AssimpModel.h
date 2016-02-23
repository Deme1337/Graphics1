#pragma once

#include "Shader.h"
#include "vertexbufferobject.h"
#include "TTexture.h"




class CAssimpModel
{
public:
	bool LoadModelFromFile(char* sFilePath);
	bool LoadModelFromFileWithMaterials(char* sFilePath);
	static void FinalizeVBO();
	static void BindModelsVAO();
	static void ReleaseAllModelData();
	static void UnBindModelsVAO();
	void RenderModel(GLenum = GL_TRIANGLES);
	int GetModelTex();
	std::string MaterialFilename;
	CAssimpModel();
private:
	bool bLoaded;

	static vertexbufferobject vboModelData;
	static UINT uiVAO;
	static vector<CTexture> tTextures;
	vector<int> iMeshStartIndices;
	vector<int> iMeshSizes;
	vector<int> iMaterialIndices;
	int iNumMaterials;
};
