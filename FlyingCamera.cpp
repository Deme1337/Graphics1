#include "stdafx.h"
#include "FlyingCamera.h"
#include "GraphicsCore.h"
#include "Input.h"





const float PI = atan(1.0)*4.0;
GLcore *ViewPortContr;

void FlyingCam::setCamGLcore(LPVOID lpparam)
{
	ViewPortContr = (GLcore*)lpparam;
}


FlyingCam::FlyingCam()
{
	vEye = glm::vec3(0.0f, 0.0f, 0.0f);
	vView = glm::vec3(0.0f, 0.0, -1.0f);
	vUp = glm::vec3(0.0f, 1.0f, 0.0f);
	fSpeed = 1500.0f;
	fSensitivity = 0.05f;
}

FlyingCam::FlyingCam(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity)
{
	vEye = a_vEye; vView = a_vView; vUp = a_vUp;
	fSpeed = a_fSpeed;
	fSensitivity = a_fSensitivity;
}

/*-----------------------------------------------

Name:	rotateWithMouse

Params:	none

Result:	Checks for moving of mouse and rotates
camera.

/*---------------------------------------------*/

void FlyingCam::rotateWithMouse()
{

	if (Keys::key(VKEY_RIGHT_BUTTON))
	{

		GetCursorPos(&pCur);
		RECT rRect; GetWindowRect(ViewPortContr->GetViewPort(), &rRect);

		int iCentX = rRect.right / 2,
			iCentY = rRect.bottom / 2;

		float deltaX = (int)(iCentX - pCur.x)* (fSensitivity / 50);
		float deltaY = (int)(iCentY - pCur.y)* (fSensitivity / 50);

		if (deltaX > 0.002 || deltaX < -0.002)
		{
			vView -= vEye;

			vView = glm::rotate(vView, deltaX, glm::vec3(0.0f, 1.0f, 0.0f));
			vView += vEye;
		}
		if (deltaY > 0.002 || deltaY < -0.002)
		{
			glm::vec3 vAxis = glm::cross(vView - vEye, vUp);
			vAxis = glm::normalize(vAxis);
			float fAngle = deltaY;
			float fNewAngle = fAngle + getAngleX();
			if (fNewAngle > -100.80f && fNewAngle < 100.80f)
			{
				vView -= vEye;
				vView = glm::rotate(vView, deltaY, vAxis);
				vView += vEye;
			}
		}
		SetCursorPos((int)iCentX, (int)iCentY);
	}

}

/*-----------------------------------------------

Name:	getAngleY

Params:	none

Result:	Gets Y angle of camera (head turning left
and right).

/*---------------------------------------------*/

float FlyingCam::getAngleY()
{
	glm::vec3 vDir = vView - vEye; vDir.y = 0.0f;
	glm::normalize(vDir);
	float fAngle = acos(glm::dot(glm::vec3(0, 0, -1), vDir))*(180.0f / PI);
	if (vDir.x < 0)fAngle = 360.0f - fAngle;
	return fAngle;
}

/*-----------------------------------------------

Name:	getAngleX

Params:	none

Result:	Gets X angle of camera (head turning up
and down).

/*---------------------------------------------*/

float FlyingCam::getAngleX()
{
	glm::vec3 vDir = vView - vEye;
	vDir = glm::normalize(vDir);
	glm::vec3 vDir2 = vDir; vDir2.y = 0.0f;
	vDir2 = glm::normalize(vDir2);
	float fAngle = acos(glm::dot(vDir2, vDir))*(180.0f / PI);
	if (vDir.y < 0)fAngle *= -1.0f;
	return fAngle;
}

/*-----------------------------------------------

Name:	setMovingKeys

Params:	a_iForw - move forward key
a_iBack - move backward key
a_iLeft - strafe left key
a_iRight - strafe right key

Result:	Sets keys for moving camera.

/*---------------------------------------------*/

void FlyingCam::setMovingKeys(int a_iForw, int a_iBack, int a_iLeft, int a_iRight)
{
	iForw = a_iForw;
	iBack = a_iBack;
	iLeft = a_iLeft;
	iRight = a_iRight;
}

/*-----------------------------------------------

Name:	update

Params:	none

Result:	Performs updates of camera - moving and
rotating.

/*---------------------------------------------*/

void FlyingCam::update()
{
	rotateWithMouse();
	float FrameInter = fFrameInterval;
	// Get view direction
	glm::vec3 vMove = vView - vEye;
	vMove = glm::normalize(vMove);
	vMove *= fSpeed;

	glm::vec3 vStrafe = glm::cross(vView - vEye, vUp);
	vStrafe = glm::normalize(vStrafe);
	vStrafe *= fSpeed;

	int iMove = 0;
	glm::vec3 vMoveBy;
	// Get vector of move
	if (Keys::key(VKEY_RIGHT_BUTTON))
	{
    	if (Keys::key(VKEY_W))vMoveBy += vMove* FrameInter;
		if (Keys::key(VKEY_S))vMoveBy -= vMove* FrameInter;
		if (Keys::key(VKEY_A))vMoveBy -= vStrafe*FrameInter;
		if (Keys::key(VKEY_D))vMoveBy += vStrafe*FrameInter;
		vEye += vMoveBy; vView += vMoveBy;
		SetFocus(ViewPortContr->GetViewPort());
	}

}

/*-----------------------------------------------

Name:	resetMouse

Params:	none

Result:	Sets mouse cursor back to the center of
window.

/*---------------------------------------------*/

void FlyingCam::resetMouse()
{
	RECT rRect; GetWindowRect(ViewPortContr->GetViewPort(), &rRect);
	int iCentX = (rRect.left + rRect.right) >> 1,
		iCentY = (rRect.top + rRect.bottom) >> 1;
	SetCursorPos(iCentX, iCentY);
}

/*-----------------------------------------------

Name:	look

Params:	none

Result:	Returns proper modelview matrix to make
camera look.

/*---------------------------------------------*/

glm::mat4 FlyingCam::look()
{
	return glm::lookAt(vEye, vView, vUp);
}