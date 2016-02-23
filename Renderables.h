#pragma once
#include <glew.h>
#include "vertexbufferobject.h"
#include "Shader.h"
#include "TTexture.h"
#include "AssimpModel.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class Renderables
{
public:

	
	Renderables();
	void ReleaseTextureData();
	void InitializeObject(int objectType, float Tiling);
	void InitializeObject(char* path, float Tiling, std::string matfilepath);
	void SetObjectLocation(glm::vec3 loc);
	void SetObjectTexture(char *TexPath);
	void SetObjectColor(glm::vec3 col);
	void SkyObject(int val);
	void MakeGlow();
	void rotateObject(float rotator, glm::vec3 axis);
	void SetPhysics(btRigidBody* rgbody);
	static void FinalizeVBOS();
	void SetMaterialPath();
	void SetObjectSpecular(float val, float val2);
	void SetObjectSize(glm::vec3 size);
	void SetObjectTextureFromData(BYTE* data, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps);
	void RenderObjectInit(CShaderProgram *sProgram, btDynamicsWorld *dyn);
	void RenderObjectGL();
	void DestroyObject();
	void SetModelMatrix(glm::mat4 mModelMat);
	glm::mat4 GetModelMatrix();
	~Renderables();
	int HasTexture = 0;
	CAssimpModel ObjectModel;
	glm::vec3 GetLocation();

	glm::vec3 GetScale();

	btRigidBody* rigidbodys;
	bool usesphysics = true;
private:
	int objType;
	float TextureTiling;
	char *TexturePath;
	btDefaultMotionState* motionstate;
	btCollisionShape* boxCollisionShape;
	bool glowingtex = false;
	bool PartOfSky;
	CTexture texture;
	GLuint VertexArrayID;
	vertexbufferobject vbo;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	glm::mat4 mModelMat;

	struct MaterialProperties
	{
		glm::vec3 MatColor;
		glm::vec3 MatSpecular;
	};

	float specular = 1.0f; float specI = 0.04f;
	float rotateSpeed = 0.0f;
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 location = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotateAxis = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 scaleSize = glm::vec3(1.0f, 1.0f, 1.0f);


};
