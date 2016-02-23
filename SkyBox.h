#pragma once
#include <glew.h>
#include <string>
#include "Shader.h"
#include "TTexture.h"
#include "vertexbufferobject.h"



class SkyBox
{
public:
	void loadSkybox(std::string a_sDirectory, std::string a_sFront, std::string a_sBack, std::string a_sLeft, std::string a_sRight, std::string a_sTop, std::string a_sBottom);
	void renderSkybox(CShaderProgram *sProgram);
	void CenterSkybox(glm::vec3 pos);
	void releaseSkybox();

private:

#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()

	GLuint uiVAO;
	glm::vec3 sbloc;
	vertexbufferobject vboRenderData;
	CTexture tTextures[6];
	std::string sDirectory;
	std::string sFront, sBack, sLeft, sRight, sTop, sBottom;
};

