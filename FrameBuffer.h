#pragma once
#include "TTexture.h"
#include "Shader.h"
class CFramebuffer
{
public:

	bool CreateFrameBufferForDepthShadow(int a_iWidth, int a_iHeight);


	bool CreateFramebufferWithTexture(int a_iWidth, int a_iHeight);

	bool AddDepthBuffer();
	void BindFramebuffer(bool bSetFullViewport = true);
	void BindFramebufferShadowMap();

	void SetFramebufferTextureFiltering(int a_tfMagnification, int a_tfMinification);
	void BindFramebufferTexture(int iTextureUnit = 0, bool bRegenMipMaps = false);

	glm::mat4 CalculateProjectionMatrix(float fFOV, float fNear, float fFar);
	glm::mat4 CalculateOrthoMatrix();

	void DeleteFramebuffer();

	int GetWidth();
	int GetHeight();

	CFramebuffer();
private:
	int iWidth, iHeight;
	UINT uiFramebuffer;
	UINT uiDepthRenderbuffer;
	CTexture tFramebufferTex;
	UINT depthTexture;
};

