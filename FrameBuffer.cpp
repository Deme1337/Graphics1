#include "stdafx.h"
#include "Framebuffer.h"
#include "Usefulheaders.h"

#include <gtc/matrix_transform.hpp>

CFramebuffer::CFramebuffer()
{
	uiFramebuffer = 0;
	uiDepthRenderbuffer = 0;
}

bool CFramebuffer::CreateFrameBufferForDepthShadow(int a_iWidth, int a_iHeight)
{
	if (uiFramebuffer != 0)
	{
		return false;
	}

	glGenFramebuffers(1, &uiFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer);

	tFramebufferTex.CreateDepthTexture(a_iWidth, a_iHeight);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tFramebufferTex.GetTextureID(), 0);

	glDrawBuffers(0, NULL); glReadBuffer(GL_NONE);

	iWidth = a_iWidth;
	iHeight = a_iHeight;

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

/*-----------------------------------------------

Name:	CreateFramebufferWithTexture

Params:	a_iWidth - framebuffer width
a_iHeight - framebuffer height

Result:	Creates a framebuffer and a texture to
render to.

/*---------------------------------------------*/

bool CFramebuffer::CreateFramebufferWithTexture(int a_iWidth, int a_iHeight)
{
	if (uiFramebuffer != 0)return false;

	glGenFramebuffers(1, &uiFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer);

	tFramebufferTex.CreateEmptyTexture(a_iWidth, a_iHeight, GL_RGB);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tFramebufferTex.GetTextureID(), 0);

	iWidth = a_iWidth;
	iHeight = a_iHeight;

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

/*-----------------------------------------------

Name:	AddDepthBuffer

Params:	none

Result:	Adds depth renderbuffer to framebuffer,
so rendering can perform depth testing.

/*---------------------------------------------*/

bool CFramebuffer::AddDepthBuffer()
{
	if (uiFramebuffer == 0)return false;

	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer);

	glGenRenderbuffers(1, &uiDepthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, uiDepthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, iWidth, iHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, uiDepthRenderbuffer);

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

/*-----------------------------------------------

Name:	BindFramebuffer

Params:	bSetFullViewport - set full framebuffer
viewport, default is true

Result:	Binds this framebuffer.

/*---------------------------------------------*/

void CFramebuffer::BindFramebuffer(bool bSetFullViewport)
{
	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer);
	if (bSetFullViewport)glViewport(0, 0, iWidth, iHeight);
}

void CFramebuffer::BindFramebufferShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer);
	glViewport(0, 0, 1024, 1024);
}

/*-----------------------------------------------

Name:	BindFramebufferTexture

Params:	iTextureUnit - texture unit to bind to (defaul 0)
bRegenMipMaps - regenerate mipmaps? (default false)

Result:	Binds framebuffer texture, where renderings
are stored.

/*---------------------------------------------*/

void CFramebuffer::BindFramebufferTexture(int iTextureUnit, bool bRegenMipMaps)
{
	tFramebufferTex.isShadow = 1;
	tFramebufferTex.bindTexture(iTextureUnit);
	if (bRegenMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
}

/*-----------------------------------------------

Name:	SetFramebufferTextureFiltering

Params:	a_tfMagnification - magnification filter
a_tfMinification - miniification filter

Result:	Sets filtering of framebuffer texture,
poor texturing here results in greater
performance.

/*---------------------------------------------*/

void CFramebuffer::SetFramebufferTextureFiltering(int a_tfMagnification, int a_tfMinification)
{
	tFramebufferTex.setFiltering(a_tfMagnification, a_tfMinification);
	tFramebufferTex.SetWrap();
}

/*-----------------------------------------------

Name:	DeleteFramebuffer

Params:	none

Result:	Deletes framebuffer and frees memory.

/*---------------------------------------------*/

void CFramebuffer::DeleteFramebuffer()
{
	if (uiFramebuffer)
	{
		glDeleteFramebuffers(1, &uiFramebuffer);
		uiFramebuffer = 0;
	}
	if (uiDepthRenderbuffer)
	{
		glDeleteRenderbuffers(1, &uiDepthRenderbuffer);
		uiDepthRenderbuffer = 0;
	}
	tFramebufferTex.releaseTexture();
}

/*-----------------------------------------------

Name:	CalculateProjectionMatrix

Params:	fFOV - field of view angle
fNear, fFar - distance of near and far clipping plane

Result:	Calculates projection matrix.

/*---------------------------------------------*/

glm::mat4 CFramebuffer::CalculateProjectionMatrix(float fFOV, float fNear, float fFar)
{
	return glm::perspective(fFOV, float(iWidth) / float(iHeight), fNear, fFar = 1500);
}

/*-----------------------------------------------

Name:	CalculateOrthoMatrix

Params:	none

Result:	Calculates ortho 2D projection matrix.

/*---------------------------------------------*/

glm::mat4 CFramebuffer::CalculateOrthoMatrix()
{
	return glm::ortho(0.0f, float(iWidth), 0.0f, float(iHeight));
}

/*-----------------------------------------------

Name:	Getters

Params:	none

Result:	... They get something :D

/*---------------------------------------------*/

int CFramebuffer::GetWidth()
{
	return iWidth;
}

int CFramebuffer::GetHeight()
{
	return iHeight;
}