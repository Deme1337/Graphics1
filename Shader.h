#pragma once
#include "Usefulheaders.h"

/********************************

Class:	CShader

Purpose:	Wraps OpenGL shader loading
and compiling.

********************************/

class CShader
{
public:
	bool loadShader(std::string sFile, int a_iType);
	void deleteShader();
	bool getLinesFromFile(std::string sFile, bool bIncludePart, std::vector<std::string>* vResult);
	bool isLoaded();
	UINT getShaderID();

	CShader();

private:
	UINT uiShader; // ID of shader
	int iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool bLoaded; // Whether shader was loaded and compiled
};

/********************************

Class:	CShaderProgram

Purpose:	Wraps OpenGL shader program
and make its usage easy.

********************************/

class CShaderProgram
{
public:
	void createProgram();
	void deleteProgram();

	bool addShaderToProgram(CShader* shShader);
	bool linkProgram();

	void useProgram();

	UINT getProgramID();


	// Setting vectors
	void setUniform(std::string sName, glm::vec2* vVectors, int iCount = 1);
	void setUniform(std::string sName, const glm::vec2 vVector);
	void setUniform(std::string sName, glm::vec3* vVectors, int iCount = 1);
	void setUniform(std::string sName, const glm::vec3 vVector);
	void setUniform(std::string sName, glm::vec4* vVectors, int iCount = 1);
	void setUniform(std::string sName, const glm::vec4 vVector);

	// Setting floats
	void setUniform(std::string sName, float* fValues, int iCount = 1);
	void setUniform(std::string sName, const float fValue);

	// Setting 3x3 matrices
	void setUniform(std::string sName, glm::mat3* mMatrices, int iCount = 1);
	void setUniform(std::string sName, const glm::mat3 mMatrix);

	// Setting 4x4 matrices
	void setUniform(std::string sName, glm::mat4* mMatrices, int iCount = 1);
	void setUniform(std::string sName, const glm::mat4 mMatrix);

	// Setting integers
	void setUniform(std::string sName, int* iValues, int iCount = 1);
	void setUniform(std::string sName, const int iValue);

	// Model and normal matrix setting is pretty common
	void SetModelAndNormalMatrix(std::string sModelMatrixName, std::string sNormalMatrixName, glm::mat4 mModelMatrix);
	void SetModelAndNormalMatrix(std::string sModelMatrixName, std::string sNormalMatrixName, glm::mat4* mModelMatrix);

	CShaderProgram();

private:
	UINT uiProgram; // ID of program
	bool bLinked; // Whether program was linked and is ready to use
};