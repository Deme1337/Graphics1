#include "stdafx.h"
#include "SkyBox.h"
#include "Usefulheaders.h"

/*-----------------------------------------------

Name:	loadSkybox

Params:	paths of skybox pictures

Result:	Loads skybox and creates VAO and VBO for it.

/*---------------------------------------------*/

void SkyBox::loadSkybox(std::string a_sDirectory, std::string a_sFront, std::string a_sBack, std::string a_sLeft, std::string a_sRight, std::string a_sTop, std::string a_sBottom)
{
	tTextures[0].loadTexture2D(a_sDirectory + a_sFront);
	tTextures[1].loadTexture2D(a_sDirectory + a_sBack);
	tTextures[2].loadTexture2D(a_sDirectory + a_sLeft);
	tTextures[3].loadTexture2D(a_sDirectory + a_sRight);
	tTextures[4].loadTexture2D(a_sDirectory + a_sTop);
	tTextures[5].loadTexture2D(a_sDirectory + a_sBottom);

	sDirectory = a_sDirectory;

	sFront = a_sFront;
	sBack = a_sBack;
	sLeft = a_sLeft;
	sRight = a_sRight;
	sTop = a_sTop;
	sBottom = a_sBottom;

	FOR(i, 6)
	{
		tTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
		tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	vboRenderData.createVBO();

	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);

	
	vboRenderData.bindVBO();

	glm::vec3 vSkyBoxVertices[24] =
	{
		// Front face
		glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
		// Back face
		glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f),
		// Left face
		glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f),
		// Right face
		glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f),
		// Top face
		glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(50.0f, 50.0f, 50.0f),
		// Bottom face
		glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
	};


	glm::vec2 vSkyBoxTexCoords[4] =
	{
		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
	};

	glm::vec3 vSkyBoxNormals[6] =
	{
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f)
	};

	glm::vec3 vSkyBoxNormals1[6] =
	{
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f)
	};
	glm::vec3 vSkyBoxNormals2[6] =
	{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	glm::vec3 newSkybv[24];
	glm::vec3 scale_sb = glm::vec3(2.0, 2.0, 2.0);

	FOR(i, 24)
	{
		newSkybv[i] = vSkyBoxVertices[i] * scale_sb;
		vboRenderData.addData(&newSkybv[i], sizeof(glm::vec3));
		vboRenderData.addData(&vSkyBoxTexCoords[i % 4], sizeof(glm::vec2));
		vboRenderData.addData(&vSkyBoxNormals[i / 4], sizeof(glm::vec3));
	}

	vboRenderData.uploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void SkyBox::CenterSkybox(glm::vec3 loc)
{
	sbloc = loc;
}

/*-----------------------------------------------

Name:	renderSkybox

Params:	none

Result: Guess what it does :)

/*---------------------------------------------*/

void SkyBox::renderSkybox(CShaderProgram *sProgram)
{
	
	
	glDepthMask(0);
	glBindVertexArray(uiVAO);
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(glm::mat4(1.0), sbloc);
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100.0f,100.0f,100.0f));
	sProgram->SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", ModelMatrix);

	sProgram->setUniform("vColor", glm::vec4(1, 1, 1, 1));
	sProgram->setUniform("gSampler", 0);

	FOR(i, 6)
	{
		tTextures[i].bindTexture();
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
	}
	glDepthMask(1);
	glBindVertexArray(0);
}

/*-----------------------------------------------

Name:	loadSkybox

Params:	paths of skybox pictures

Result:	Loads skybox and creates VAO and VBO for it.

/*---------------------------------------------*/

void SkyBox::releaseSkybox()
{
	FOR(i, 6)tTextures[i].releaseTexture();
	glDeleteVertexArrays(1, &uiVAO);
	vboRenderData.releaseVBO();
}