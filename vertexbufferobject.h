#pragma once
#include "stdafx.h"
#include <vector>
#include <stdlib.h>
#include <iostream>


#include <string>
#include <glew.h>
#include <glm.hpp>
#include <Windows.h>

using namespace std;
using namespace glm;
class vertexbufferobject
{
public:
	void createVBO(int a_iSize = 0);
	void releaseVBO();

	void* mapBufferToMemory(int iUsageHint);
	void* mapSubBufferToMemory(int iUsageHint, UINT uiOffset, UINT uiLength);
	void unmapBuffer();

	void bindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
	void uploadDataToGPU(int iUsageHint);

	void addData(void* ptrData, UINT uiDataSize);

	void* getDataPointer();
	UINT getBuffer();
	int GetCurrentSize();
	vertexbufferobject();

private:
	UINT uiBuffer;
	int iSize;
	int iCurrentSize;
	int iBufferType;
	vector<BYTE> data;

	bool bDataUploaded;

};

