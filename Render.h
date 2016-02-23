#pragma once
#include "Renderables.h"
#include "FreeTypeFont.h"
#include "SkyBox.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
class Render
{
public:
	Render();
	void addRenderObject(Renderables lpObject);
	void addSkyBox(SkyBox sb);
	void RenderAll(double fps, LPVOID lpParam);
	
	void ShaderPrograms();
	void ResetTimer();
	double UpdateTimer();
	~Render();
	std::vector<Renderables> Objects;
	void DestroyRender();
	glm::mat4 GetModelMatrix();
	std::string fps;

	btDiscreteDynamicsWorld* dynamicsWorld;
private:
	void InitShadowMapping();
	
	glm::mat4 mModelMat;
	SkyBox skybox;
	CFreeTypeFont ftFont;
};

