#include "stdafx.h"
#include "TTexture.h"

#include "Usefulheaders.h"
#include <FreeImage.h>

#pragma comment(lib, "FreeImage.lib")
#include <iostream>
std::string Error;

CTexture::CTexture()
{
	bMipMapsGenerated = false;
}

std::string CTexture::GetPath()
{
	return sPath;
}

void CTexture::CreateRotationTexture(int a_iWidth, int a_iHeight)
{
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int size = a_iWidth * a_iHeight;

	glm::vec4 *data = new glm::vec4[size];

	srand(GetTickCount());

	float randomAngle = 3.14f * 2.0f * (float)rand() / (float)RAND_MAX;

	for (int i = 0; i < size; i++)
	{
		data[i].x = cos(randomAngle);
		data[i].y = sin(randomAngle);
		data[i].z = -sin(randomAngle);
		data[i].w = cos(randomAngle);

		data[i] *= 0.5f;
		data[i] += 0.5f;

		randomAngle += 3.14f * 2.0f * (float)rand() / (float)RAND_MAX;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, a_iWidth, a_iHeight, 0, GL_RGBA, GL_FLOAT, data);

	delete[] data;
}


/*-----------------------------------------------

Name:		createFromData

Params:	a_sPath - path to the texture
format - format of data
bGenerateMipMaps - whether to create mipmaps

Result:	Creates texture from provided data.

/*---------------------------------------------*/

void CTexture::createFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps)
{
	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	if (format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	// We must handle this because of internal format parameter
	else if (format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	if (bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	glGenSamplers(1, &uiSampler);

	sPath = "";
	bMipMapsGenerated = bGenerateMipMaps;
	iWidth = a_iWidth;
	iHeight = a_iHeight;
	iBPP = a_iBPP;
}

bool CTexture::ReloadTexture()
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sPath.c_str(), 0); // Check the file signature and deduce its format

	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(sPath.c_str());

	if (fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, sPath.c_str());
	if (!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	// If somehow one of these failed (they shouldn't), return failure
	if (bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;

	GLenum format;
	int bada = FreeImage_GetBPP(dib);
	if (FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
	if (FreeImage_GetBPP(dib) == 24)format = GL_BGR;
	if (FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;

	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, iWidth, iHeight, format, GL_UNSIGNED_BYTE, bDataPointer);
	if (bMipMapsGenerated)glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(dib);

	return true; // Success
}

void CTexture::CreateDepthTexture(int a_iWidth, int a_iHeight)
{
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, a_iWidth, a_iHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}


void CTexture::CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format)
{
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	if (format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	// We must handle this because of internal format parameter
	else if (format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);

	glGenSamplers(1, &uiSampler);
}

/*-----------------------------------------------

Name:		loadTexture2D

Params:	a_sPath - path to the texture

Result:	Loads texture from a file, supports most
graphics formats.

/*---------------------------------------------*/

bool CTexture::loadTexture2D(std::string a_sPath, bool bGenerateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(a_sPath.c_str(), 0); // Check the file signature and deduce its format

	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());

	if (fif == FIF_UNKNOWN) // If still unkown, return failure
	{
		printf("Texture file type unknown.\n");
		return false;
	}
	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, a_sPath.c_str());
	if (!dib)
	{
		printf("Cannot load texture file\n ");

		return false;
	}
	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	iWidth = FreeImage_GetWidth(dib); // Get the image width and height
	iHeight = FreeImage_GetHeight(dib);
	iBPP = FreeImage_GetBPP(dib);

	// If somehow one of these failed (they shouldn't), return failure
	if (bDataPointer == NULL || iWidth == 0 || iHeight == 0)
	{

		printf("Datapointer = null... not supposed to happen\n");
		return false;
	}
	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	int iFormat = iBPP == 24 ? GL_BGR : iBPP == 8 ? GL_LUMINANCE : 0;
	int iInternalFormat = iBPP == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, bDataPointer);

	if (bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(dib);

	glGenSamplers(1, &uiSampler);

	sPath = a_sPath;
	bMipMapsGenerated = bGenerateMipMaps;
	std::cout << "texture loaded: " + sPath << std::endl;
	return true; // Success
}



void CTexture::setSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(uiSampler, parameter, value);
}

/*-----------------------------------------------

Name:		setFiltering

Params:	tfMagnification - mag. filter, must be from
ETextureFiltering enum
tfMinification - min. filter, must be from
ETextureFiltering enum

Result:	Sets magnification and minification
texture filter.

/*---------------------------------------------*/

void CTexture::setFiltering(int a_tfMagnification, int a_tfMinification)
{
	// Set magnification filter
	if (a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	tfMinification = a_tfMinification;
	tfMagnification = a_tfMagnification;
}

/*-----------------------------------------------

Name:		bindTexture

Params:	iTextureUnit - texture unit to bind texture to

Result:	Guess what it does :)

/*---------------------------------------------*/

void CTexture::bindTexture(int iTextureUnit)
{
	if (isShadow == 1)
	{
		glActiveTexture(GL_TEXTURE0 + iTextureUnit);
		glBindTexture(GL_TEXTURE_2D, uiTexture);
		glBindSampler(iTextureUnit, uiSampler);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + iTextureUnit);
		glBindTexture(GL_TEXTURE_2D, uiTexture);
		glBindSampler(iTextureUnit, uiSampler);
	}

}

/*-----------------------------------------------

Name:		releaseTexture

Params:	none

Result:	Frees all memory used by texture.

/*---------------------------------------------*/

void CTexture::releaseTexture()
{
	glDeleteSamplers(1, &uiSampler);
	glDeleteTextures(1, &uiTexture);
}

int CTexture::getMinificationFilter()
{
	return tfMinification;
}

int CTexture::getMagnificationFilter()
{
	return tfMagnification;
}
int CTexture::getWidth()
{
	return iWidth;
}

int CTexture::getHeight()
{
	return iHeight;
}

int CTexture::getBPP()
{
	return iBPP;
}

UINT CTexture::GetTextureID()
{
	return uiTexture;
}

void CTexture::SetWrap()
{
	glBindSampler(0, uiSampler);
	glSamplerParameteri(uiSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(uiSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}