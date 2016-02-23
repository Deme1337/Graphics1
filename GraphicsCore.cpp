#include "stdafx.h"
#include <glew.h>
#include <wglew.h>
#include "GraphicsCore.h"
#include "Shader.h"
#include "Render.h"
#include "SoundClass.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
Logger logs2;

std::string sb1[18] = { "CloudyLightRays\\CloudyLightRaysBack2048.png", "CloudyLightRays\\CloudyLightRaysFront2048.png", "CloudyLightRays\\CloudyLightRaysRight2048.png",
"CloudyLightRays\\CloudyLightRaysLeft2048.png", "CloudyLightRays\\CloudyLightRaysUp2048.png", "CloudyLightRays\\CloudyLightRaysDown2048.png",
"TropicalSunnyDay\\TropicalSunnyDayBack2048.png", "TropicalSunnyDay\\TropicalSunnyDayFront2048.png", "TropicalSunnyDay\\TropicalSunnyDayRight2048.png", "TropicalSunnyDay\\TropicalSunnyDayLeft2048.png",
"TropicalSunnyDay\\TropicalSunnyDayUp2048.png", "TropicalSunnyDay\\TropicalSunnyDayDown2048.png" };



// define jutut
#define iMajorVersion 4
#define iMinorVersion 2

//Tunnistamaan näppäinten painallukset
char Keys::kp[256] = { 0 };
int Keys::key(int iKey)
{
	return (GetAsyncKeyState(iKey) >> 15) & 1;
}

int Keys::onekey(int iKey)
{
	if (key(iKey) && !kp[iKey]){ kp[iKey] = 1; return 1; }
	if (!key(iKey))kp[iKey] = 0;
	return 0;
}

//globaalit oliotyms
HWND ViewPort;
HDC hDC;
HGLRC hRC;
Render render;
glm::mat4 mProjection1;
bool ContextDone = false;
glm::mat4 mOrtho1;
SoundClass *m_Audio;
btRigidBody* bodies[200];
GLcore::GLcore()
{

}

HWND GLcore::GetViewPort()
{
	return ViewPort;
}

void GLcore::setOrtho2d(int width, int height)
{
	mOrtho1 = glm::ortho(0.0f, float(4), 0.0f, float(3));
}
void GLcore::setProjection3D(float fFOV, float fAspectRatio, float fNear, float fFar)
{
	//mProjection = glm::perspective(fFOV, fAspectRatio, fNear, fFar);
}
int iFPSCount, iCurrentFPS;
clock_t tLastSecond;

int GLcore::GetFps()
{
	return iCurrentFPS;
}

glm::mat4* GLcore::getOrthoMatrix()
{
	RECT vpRec;
	GetWindowRect(ViewPort, &vpRec);
	mOrtho1 = glm::ortho(0.0f, float(vpRec.right), 0.0f, float(vpRec.bottom));
	return &mOrtho1;
}
glm::mat4* GLcore::getProjectionMatrix()
{
	mProjection1 = glm::perspective(45.0f, float(4) / float(3), 0.1f, 100000.0f);
	return &mProjection1;
}

void GLcore::Destroy()
{
	DisableOpenGl(ViewPort, hDC, hRC);
	render.DestroyRender();
	DestroyWindow(ViewPort);
	m_Audio->Shutdown();
}

//Opengl context creation
bool GLcore::GlContext(HWND hWnd, HWND MainWindow)
{
	hDC = GetDC(hWnd); // Get the device context for our window

	PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
	pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
	pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
	pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
	pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

	int nPixelFormat = ChoosePixelFormat(hDC, &pfd); // Check if our PFD is valid and get a pixel format back
	if (nPixelFormat == 0)
	{// If it fails
		MessageBox(MainWindow, L"choose pf error", L"ERROR", MB_OK);
		return false;
	}
	bool bResult = SetPixelFormat(hDC, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
	if (!bResult) // If it fails
	{

		MessageBox(MainWindow, L"set pf error", L"ERROR", MB_OK);
		return false;
	}
	HGLRC tempOpenGLContext = wglCreateContext(hDC); // Create an OpenGL 2.1 context for our device context
	wglMakeCurrent(hDC, tempOpenGLContext); // Make the OpenGL 2.1 context current and active

	GLenum error = glewInit(); // Enable GLEW
	if (error != GLEW_OK) // If GLEW fails
	{

		MessageBox(MainWindow, L"glewError", L"ERROR", MB_OK);
		return false;
	}
	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4.5, // Set the MAJOR version of OpenGL to 3
		WGL_CONTEXT_MINOR_VERSION_ARB, 2, // Set the MINOR version of OpenGL to 2
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1) { // If the OpenGL 3.x context creation extension is available
		hRC = wglCreateContextAttribsARB(hDC, NULL, attributes); // Create and OpenGL 3.x context based on the given attributes
		wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
		wglDeleteContext(tempOpenGLContext); // Delete the temporary OpenGL 2.1 context
		wglMakeCurrent(hDC, hRC); // Make our OpenGL 3.0 context current
	}
	else {
		hRC = tempOpenGLContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
	}

	int glVersion[2] = { -1, -1 }; // Set some default values for the version
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using


	std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl; // Output which version of OpenGL we are using
	std::string mesg = "Using opengl: " + glVersion[0];
	Logger::Log(mesg);
	return true; // We have successfully created a context, return true

}
void GLcore::DisableOpenGl(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}

//Render looppi, lasketaan myös frameintervalli ja fps loopin ympärillä
void GLcore::RenderLoop(LPVOID lpParam)
{
	clock_t tCurrent = clock();
	double finterval = tCurrent - tLastSecond;

	if ((tCurrent - tLastSecond) >= CLOCKS_PER_SEC)
	{
		tLastSecond += CLOCKS_PER_SEC;
		iFPSCount = iCurrentFPS;
		iCurrentFPS = 0;
	}
	
	render.RenderAll(finterval,lpParam);
	iCurrentFPS++;
}
//bufferien vaihto
void GLcore::SwapBuffersm()
{
	SwapBuffers(hDC);
}
Renderables object[200];

void AddSpidersandSuzanne()
{
	

	
	object[0].InitializeObject("Models\\cubewdt.3DS", 1.0f, "");
	object[0].SetObjectSpecular(float(10), 0.04f);
	object[0].SetObjectSize(glm::vec3(50.0f, 0.1f, 50.0f));
	object[0].SetObjectLocation(glm::vec3(2.0f, -50.0f, -300.0f));
	object[0].SetObjectColor(glm::vec3(0.7f, 0.7f, 0.7f));
	object[0].usesphysics = false;
	object[0].SetPhysics(bodies[0]);
	render.addRenderObject(object[0]);
	 
	
	object[1].InitializeObject("Models\\spider.obj", 1.0f, "");
	object[1].SetObjectSpecular(float(10), 0.04f);
	object[1].SetObjectSize(glm::vec3(0.1, 0.1, 0.1));
	object[1].SetObjectLocation(glm::vec3(0, 10.0f, 1000));
	object[1].SetObjectColor(glm::vec3(1.0f, 1.0f, 1.0f));
	object[1].SetPhysics(bodies[1]);	
	render.addRenderObject(object[1]);
	
	object[2].InitializeObject("Models\\House\\HouseRemodel2.obj", 1.0f, "Models\\House\\HouseRemodel2.mtl");
	object[2].SetObjectSpecular(float(10), 0.04f);
	object[2].SetObjectSize(glm::vec3(2.0f, 2.0f, 2.0f));
	object[2].SetObjectLocation(glm::vec3(0.0f, -47.5f, 350.0f));
	object[2].SetObjectColor(glm::vec3(1.0f, 1.0f, 1.0f));
	object[2].SetPhysics(bodies[2]);
	object[2].usesphysics = false;
	render.addRenderObject(object[2]);

	object[3].InitializeObject("Models\\House\\HouseRemodel2.obj", 1.0f, "Models\\House\\HouseRemodel2.mtl");
	object[3].SetObjectSpecular(float(10), 0.04f);
	object[3].SetObjectSize(glm::vec3(2.0f, 2.0f, 2.0f));
	object[3].SetObjectLocation(glm::vec3(300.0f, -47.5f, 700.0f));
	object[3].SetObjectColor(glm::vec3(1.0f, 1.0f, 1.0f));
	object[3].SetPhysics(bodies[3]);
	object[3].usesphysics = false;
	render.addRenderObject(object[3]);
	//object[2].InitializeObject("Models\\suzanne.obj", 1.0f);
	//object[2].SetObjectSpecular(float(10), 0.04f);
	//object[2].SetObjectSize(glm::vec3(50.0f, 50.0f, 50.0f));
	//object[2].SetObjectLocation(glm::vec3(200.0f, 5050.0f, 0.0f));
	//object[2].SetObjectColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//object[2].SetPhysics(bodies[2]);
	//render.addRenderObject(object[2]);
}

void AddHouse()
{
	object[0].InitializeObject("Models\\House\\HouseRemodel2.obj", 1.0f, "");
	object[0].SetObjectSpecular(float(10), 0.04f);
	object[0].SetObjectSize(glm::vec3(10.0f, 10.0f, 10.0f));
	object[0].SetObjectLocation(glm::vec3(0.0f,0.0f, 0.0f));
	object[0].SetObjectColor(glm::vec3(1.0f, 1.0f, 1.0f));

	render.addRenderObject(object[0]);
}

void PoolHouse()
{
	object[0].InitializeObject("Models\\Pool\\pool.3DS", 1.0f, "");
	object[0].SetObjectSpecular(float(10), 0.04f);
	object[0].SetObjectSize(glm::vec3(0.5f, 0.5f, 0.5f));
	object[0].SetObjectLocation(glm::vec3(0.0f, 0.0f, 0.0f));
	object[0].SetObjectColor(glm::vec3(1.0f, 1.0f, 1.0f));

	render.addRenderObject(object[0]);

}

void City()
{
	object[0].InitializeObject("Models\\LordGoodReexport.obj", 1.0f, "");
	//object[0].SetObjectTexture("Textures\\stone_wall.jpg");
	object[0].SetObjectSpecular(float(10), 0.04f);
	object[0].SetObjectSize(glm::vec3(1.0f, 1.0f, 1.0f));
	object[0].SetObjectLocation(glm::vec3(1.0f, 0.0f, 1.0f));
	object[0].SetObjectColor(glm::vec3(1.0f, 1.0f, 1.0f));
	render.addRenderObject(object[0]);
}

void Sponza()
{
	object[0].InitializeObject("Models\\Crytek\\sponza.obj", 1.0f, "");
	object[0].SetObjectSpecular(float(10), 0.04f);
	object[0].SetObjectSize(glm::vec3(0.2f, 0.2f, 0.2f));
	object[0].SetObjectLocation(glm::vec3(float(1) + 1, 0.0f, 00.0f));
	object[0].SetObjectColor(glm::vec3(1.0f, 1.0f, 1.0f));
	render.addRenderObject(object[0]);
}

void GLcore::InitRender()
{
	SkyBox skybox;
	AddSpidersandSuzanne();
	render.ShaderPrograms();
	int ChooseObject = 0;


	skybox.loadSkybox("Textures\\skyboxes\\", sb1[6], sb1[7], sb1[8], sb1[9], sb1[10], sb1[11]);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	

	m_Audio = new SoundClass();

	m_Audio->Initialize(ViewPort, "Audio\\arcademusic.wav", -5000);
	


	Renderables::FinalizeVBOS();
	if (!m_Audio->PlayWaveFile())
	{
		Logger::Log("Audio cannot be played");
	}

	render.addSkyBox(skybox);

	Logger::Log("Initialization complete");

}

void GLcore::InitGraphics(HWND hWnd, LPCTSTR szsWindowClass, LPCTSTR szsTitle, HINSTANCE hInstance, int XWP, int YWP, int width, int height, bool FullScreen)
{
	bool enOGL = false;
	// Initialize GLEW
	//bool glewstuffinit = InitGLEW(hInstance);

	HMONITOR hmon = MonitorFromWindow(ViewPort,MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = { sizeof(mi) };

	ViewPort = CreateWindow(szsWindowClass, szsTitle, WS_POPUP | WS_VISIBLE, mi.rcMonitor.left + 100, mi.rcMonitor.top+20 , 1280, 960, NULL, NULL, hInstance, NULL);
	bool glewstuffinit = GlContext(ViewPort, hWnd);
	if (glewstuffinit)
	{
		Logger::Log("Initialized glew");
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		MessageBox(hWnd, L"GLEW INITIALIZATION ERROR!", L"ERROR", MB_OK);
	}


}
