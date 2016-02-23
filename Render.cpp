#include "stdafx.h"
#include <glew.h>
#include "Render.h"
#include <cmath>
#include <stdio.h>
#include <tchar.h>
#include "Usefulheaders.h"
#include "Input.h"
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <gtx\norm.hpp>
#include <string>
#include "FreeTypeFont.h"
#include "vertexbufferobject.h"
#include "FlyingCamera.h"
#include "GraphicsCore.h"



#define PI 3.14159265
Logger logs1;

CShader shader[20];

CShaderProgram sProgram, sFontProgram, sSkyboxProgram, spShadowMapper, spShadowMapRender, sOrtho2D;

CFreeTypeFont ftFont;
float x, y, z;
FlyingCam cam;

GLuint depthTexture;
GLuint FramebufferName = 0;


#include "FrameBuffer.h"

CFramebuffer fboShadowMap;
bool bShadowsOn = true;
bool bDisplayShadowMap = true;
int iShadowMapTextureSize = 2056;

vertexbufferobject vboShadowMapQuad;
UINT uiVAOShadowMapQuad;
CTexture rotationTexture;

Render::Render()
{
}


void Render::addSkyBox(SkyBox sb)
{
	skybox = sb;
}


glm::vec3 vShadowMapQuad[] =
{
	glm::vec3(0.75f, 1.0f, 0.0f),
	glm::vec3(0.75f, 0.5f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 0.5f, 0.0f)
};

glm::vec2 vShadowMapQuadTC[] =
{
	glm::vec2(0.0f, 1.0f),
	glm::vec2(0.0f, 0.0f),
	glm::vec2(1.0f, 1.0f),
	glm::vec2(1.0f, 0.0f)
};

void Render::InitShadowMapping()
{

	vboShadowMapQuad.createVBO();
	FOR(i, 4)
	{
		vboShadowMapQuad.addData(&vShadowMapQuad[i], sizeof(glm::vec3));
		vboShadowMapQuad.addData(&vShadowMapQuadTC[i], sizeof(glm::vec2));
	}

	glGenVertexArrays(1, &uiVAOShadowMapQuad);
	glBindVertexArray(uiVAOShadowMapQuad);

	vboShadowMapQuad.bindVBO();
	vboShadowMapQuad.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));


	fboShadowMap.CreateFramebufferWithTexture(iShadowMapTextureSize , iShadowMapTextureSize );
	if (fboShadowMap.AddDepthBuffer())
	{
		Logger::Log("Depthbuffer done");
	}

	fboShadowMap.AddDepthBuffer();
	fboShadowMap.SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST);
	rotationTexture.CreateRotationTexture(128, 128);
}

glm::mat4 mProjection;
glm::mat4 mOrtho;
bool ShowLogger;
void Render::ShaderPrograms()
{
	std::string ShaderFolder = "Shaders\\";
	shader[0].loadShader(ShaderFolder + "Shader.frag", GL_FRAGMENT_SHADER);
	shader[1].loadShader(ShaderFolder + "Shader.vert", GL_VERTEX_SHADER);
	shader[2].loadShader(ShaderFolder + "font2D.frag", GL_FRAGMENT_SHADER);
	shader[3].loadShader(ShaderFolder + "ortho2D.frag", GL_FRAGMENT_SHADER);
	shader[4].loadShader(ShaderFolder + "ortho2D.vert", GL_VERTEX_SHADER);
	shader[5].loadShader(ShaderFolder + "DirectionalLight.txt", GL_FRAGMENT_SHADER);
	shader[6].loadShader(ShaderFolder + "shadowMapper.vert", GL_VERTEX_SHADER);
	shader[7].loadShader(ShaderFolder + "shadowMapper.frag", GL_FRAGMENT_SHADER);
	shader[8].loadShader(ShaderFolder + "shadowMapRender.frag", GL_FRAGMENT_SHADER);
	shader[9].loadShader(ShaderFolder + "shadowMapRender.vert", GL_VERTEX_SHADER);
	shader[10].loadShader(ShaderFolder + "shadows.frag", GL_FRAGMENT_SHADER);
	shader[11].loadShader(ShaderFolder + "skybox.vert", GL_VERTEX_SHADER);
	shader[12].loadShader(ShaderFolder + "skybox.frag", GL_FRAGMENT_SHADER);

	ShowLogger = false;

	//shaderit 3d
	sProgram.createProgram();
	sProgram.addShaderToProgram(&shader[5]);
	sProgram.addShaderToProgram(&shader[1]);
	sProgram.addShaderToProgram(&shader[0]);
	sProgram.addShaderToProgram(&shader[10]);
	sProgram.linkProgram();

	//2d shaderit fontteja varten
	sOrtho2D.createProgram();
	sOrtho2D.addShaderToProgram(&shader[4]);
	sOrtho2D.addShaderToProgram(&shader[3]);
	sOrtho2D.linkProgram();
	//2d shaderit fontteja varten
	sFontProgram.createProgram();
	sFontProgram.addShaderToProgram(&shader[2]);
	sFontProgram.addShaderToProgram(&shader[4]);
	sFontProgram.linkProgram();



	//Skyboxille oma shaderi
	sSkyboxProgram.createProgram();
	sSkyboxProgram.addShaderToProgram(&shader[11]);
	sSkyboxProgram.addShaderToProgram(&shader[12]);
	sSkyboxProgram.linkProgram();

	//shaderit shadow mappingiin
	spShadowMapper.createProgram();
	spShadowMapper.addShaderToProgram(&shader[6]);
	spShadowMapper.addShaderToProgram(&shader[7]);
	spShadowMapper.linkProgram();

	spShadowMapRender.createProgram();
	spShadowMapRender.addShaderToProgram(&shader[8]);
	spShadowMapRender.addShaderToProgram(&shader[9]);
	spShadowMapRender.linkProgram();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	cam.vEye = glm::vec3(10.0f, 500.0f, 150.0f);

	//fonttien valmistelu (ei toimi atm, jotain vikaa fonttitiedoston luvussa ehkä)
	Logger::Log("Loading fonts");
	ftFont.SetShaderProgram(&sFontProgram);
	ftFont.LoadFont("arial.ttf", 32, 1024);
	//ftFont.LoadFont("arial.ttf", 24, 5000);
	mOrtho = glm::ortho(0.0f, float(800), 0.0f, float(800));
    mProjection = glm::perspective(45.0f, float(4) / float(3), 0.1f, 100000.0f);

	// Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

	//shadowmappien valmistelu
	this->InitShadowMapping();
	for (size_t i = 0; i < Objects.size()-1; i++)
	{
		dynamicsWorld->addRigidBody(Objects[i].rigidbodys);
	}
	
	
	
}

void Render::addRenderObject(Renderables rparam)
{

	Objects.push_back(rparam);
}

static double tLastFrame = 0;
double fFrameInterval;
static float TimeCounter = 0;


float sunY = 1000;

glm::vec3 sunPos = glm::vec3(100.0f,8500.0f, 8500.0f);
void Render::RenderAll(double fps, LPVOID lpParam)
{
	dynamicsWorld->stepSimulation(1 / 60.0f, 10);
	glClearColor(1.0f, 0.2f, 0.5f, 1.0f);
	// Typecast lpParam to glcore pointer
	GLcore *glcore = (GLcore*)lpParam;
	cam.fFrameInterval = 0.0005f;
	glm::mat4 mDepthBiasMVP;

	bShadowsOn = true;
	glEnable(GL_DEPTH_TEST);

	
	if (bShadowsOn) // So if the shadows are on
	{
		// We are going to render scene from the light's point of view
	
		
		fboShadowMap.BindFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//spShadowMapper.useProgram();
		glCullFace(GL_FRONT);


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		spShadowMapper.useProgram(); 
		// Because we have a directional light, we just set it high enough (vLightPos) so that it sees all objects on scene
		// We also create orthographics projection matrix for the purposes of rendering shadows
		const float fRangeX =500, fRangeY =500, fMinZ = 0.1f, fMaxZ = glm::length(glm::vec3(0.0f, 0.0f, 0.0f) - sunPos) + 500;
		glm::mat4 mPROJ = glm::ortho<float>(-fRangeX, fRangeX, -fRangeY, fRangeY, fMinZ, fMaxZ);
		
		glm::mat4 mViewFromLight = glm::lookAt(sunPos, glm::vec3(0, 0, 0), glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), glm::normalize(sunPos)));

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		// Calculate depth bias matrix to calculate shadow coordinates in shader programs
		mDepthBiasMVP = biasMatrix * mPROJ * mViewFromLight;


		

		for (int i = 0; i < Objects.size() ; i++)
		{
			
			Objects[i].RenderObjectInit(&sProgram,dynamicsWorld);
			glm::mat4 depthMVP = mPROJ * mViewFromLight * Objects[i].GetModelMatrix();
			spShadowMapper.setUniform("depthMVP", depthMVP);
			Objects[i].RenderObjectGL();
		}






	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	RECT viewportrec;
	GetWindowRect(glcore->GetViewPort(), &viewportrec);
	glViewport(0, 0, viewportrec.right, viewportrec.bottom);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glm::mat4 mView = cam.look();




	sProgram.useProgram();

	sProgram.setUniform("matActive.fSpecularIntensity", 0.04f);

	sProgram.setUniform("sunLight.vColor", glm::vec3(1.0f,1.0f,1.0f));
	sProgram.setUniform("sunLight.fAmbient", 0.01f);

	sProgram.setUniform("sunLight.vDirection", -glm::normalize(sunPos));
	sProgram.setUniform("sunLight.iSkybox", 0);

	sProgram.setUniform("gSampler", 0);


	sProgram.setUniform("matrices.projMatrix", glcore->getProjectionMatrix());
	sProgram.setUniform("matrices.viewMatrix", &cam.look());


	sProgram.setUniform("vEyePosition", cam.vEye);

	sProgram.setUniform("matrices.depthBiasMVP", mDepthBiasMVP);

	// Bind shadow map to 5th texture unit (index is 5, if we count from 1 it's 6th)
	// You can use whichever texture unit you want, I used 5 because of 5 textures in terrain and 6th first available
	fboShadowMap.BindFramebufferTexture(50, false);
	sProgram.setUniform("shadowMap", 50);
	sProgram.setUniform("bShadowsOn", bShadowsOn);

	sProgram.setUniform("rotationTexture", 49);
	rotationTexture.bindTexture(49);
	sProgram.setUniform("scale", 1.0f / 128.0f);
	sProgram.setUniform("radius", (iShadowMapTextureSize / 2056.0f) / iShadowMapTextureSize);

	for (int i = 0; i < Objects.size(); i++)
	{
		Objects[i].RenderObjectInit(&sProgram,dynamicsWorld);
		fboShadowMap.BindFramebufferTexture(50, false);
		Objects[i].RenderObjectGL();
	}
	if (Keys::onekey(VKEY_G))
	{
		if (ShowLogger)
		{
			ShowLogger = false;
		}
		else
		{
			ShowLogger = true;
		}
		
	}
	if (Keys::key(VKEY_UP_ARROW))
	{
		
		sunPos.y = sunPos.y+1.0f;
	}
	if (Keys::key(VKEY_DOWN_ARROW))
	{
		
		sunPos.y = sunPos.y - 1.0f;
	}
	if (Keys::key(VKEY_LEFT_ARROW))
	{

		sunPos.x = sunPos.x + 1.0f;
	}
	if (Keys::key(VKEY_LEFT_ARROW))
	{

		sunPos.x = sunPos.x - 1.0f;
	}
	if (Keys::key(VKEY_H))
	{
		cam.vEye = sunPos;
		cam.vView = -normalize(sunPos);
	}
	if (Keys::onekey(VKEY_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if (Keys::onekey(VKEY_F))
	{
		if (bDisplayShadowMap)
		{
			bDisplayShadowMap = false;
		}
		else
		{
			bDisplayShadowMap = true;
		}
	}

	sSkyboxProgram.useProgram();
	sSkyboxProgram.setUniform("matrices.projMatrix", glcore->getProjectionMatrix());
	sSkyboxProgram.setUniform("matrices.viewMatrix", &cam.look());
	skybox.CenterSkybox(cam.vEye);
	skybox.renderSkybox(&sSkyboxProgram);

	cam.update();

	//Quadi shadowmappia varten.. Näkee vähän miten piirtää
	if (!bDisplayShadowMap)
	{
		// Display shadow map
		spShadowMapRender.useProgram();
		glBindVertexArray(uiVAOShadowMapQuad);
		fboShadowMap.BindFramebufferTexture();
		spShadowMapRender.setUniform("samplerShadow", 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}



	//2D piirustukset
	UpdateWindow(glcore->GetViewPort());
	GetWindowRect(glcore->GetViewPort(), &viewportrec);
	glViewport(viewportrec.top, viewportrec.left, viewportrec.right, viewportrec.bottom);
	sFontProgram.useProgram();
	glDisable(GL_DEPTH_TEST);
	ftFont.SetShaderProgram(&sFontProgram);
	sFontProgram.setUniform("matrices.projMatrix", glcore->getOrthoMatrix());
	sFontProgram.setUniform("vColor", glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
	ftFont.Print(L"FPS: " + std::to_wstring(glcore->GetFps()) + L" 'F' Shadomap" + L" 'G' Show Log", 15, viewportrec.top + 790, 20);
	ftFont.Print(L"Loc: " + std::to_wstring(Objects[1].GetLocation().y) + L" " +std::to_wstring(Objects[1].GetLocation().x), 15, viewportrec.top + 750, 20);

	if (ShowLogger)
	{
		for (int i = 0; i < logs1.GetLogList().size(); i++)
		{
			std::wstring loggeritem = logs1.GetLogListW()[i];
			ftFont.Print(loggeritem, 15, viewportrec.top + 775 - (i * 15), 20);
		}
	}
	glEnable(GL_DEPTH_TEST);
}

void Render::DestroyRender()
{
	skybox.releaseSkybox();

	for (int i = 0; i < Objects.size(); i++)
	{
		Objects[i].DestroyObject();
	}
}

Render::~Render()
{

}
