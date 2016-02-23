#pragma once
#include <glm.hpp>

using namespace glm;

class FlyingCam
{
public:
	void setCamGLcore(LPVOID lpparam);
	glm::vec3 vEye, vView, vUp;
	float fSpeed;
	float fSensitivity; // How many degrees to rotate per pixel moved by mouse (nice value is 0.10)
	float fFrameInterval;
	// Main functions
	void rotateWithMouse();
	void update();
	glm::mat4 look();

	void setMovingKeys(int a_iForw, int a_iBack, int a_iLeft, int a_iRight);
	void resetMouse();

	// Functions that get viewing angles
	float getAngleX(), getAngleY();

	// Constructors
	FlyingCam();
	FlyingCam(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity);

private:
	POINT pCur; // For mosue rotation
	int iForw, iBack, iLeft, iRight;
};

//extern FlyingCam fcMain;
