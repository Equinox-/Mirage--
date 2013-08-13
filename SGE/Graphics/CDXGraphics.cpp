//====================================================================================================
// Filename:	CDXGraphics.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CDXGraphics.h"

#include <GL/freeglut.h>
#include <GL/gl.h>

#include "../Core/CLog.h"

//====================================================================================================
// Statics
//====================================================================================================

CDXGraphics* CDXGraphics::s_pInstance = NULL;

//====================================================================================================
// Class Definitions
//====================================================================================================

CDXGraphics* CDXGraphics::Get(void) {
	// If we do not yet have an instance created
	if (NULL == s_pInstance) {
		// Create a new instance
		s_pInstance = new CDXGraphics;
	}

	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

CDXGraphics::CDXGraphics(void) :
		mInitialized(false), iWidth(0), iHeight(0) {
	mD3DDM = GLUT_SINGLE | GLUT_RGB;
//	ZeroMemory(&mD3DDM, sizeof(mD3DDM));
//	ZeroMemory(&mD3DPP, sizeof(mD3DPP));
}

//----------------------------------------------------------------------------------------------------

CDXGraphics::~CDXGraphics(void) {
	// Terminate the graphics system
	Terminate();
}

//----------------------------------------------------------------------------------------------------
void CDXGraphics::Initialize(int hWindow, int iWidth, int iHeight,
		bool bFullscreen) {
	this->iWidth = iWidth;
	this->iHeight = iHeight;
	// Check if we have already initialized the system
	if (mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[DXGraphics] System already initialized.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[DXGraphics] Initializing...");

	// Obtain an interface to Direct3D
//	mpD3D = Direct3DCreate9(D3D_SDK_VERSION);

// Get device capabilities
// http://msdn2.microsoft.com/en-us/library/bb172513.aspx
//D3DCAPS9 sD3DCaps;
//mpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,   // Get primary display adapter
//		D3DDEVTYPE_HAL,	   // Device type (HAL or REF)
//		&sD3DCaps);		  // Direct3D capability struct

// HAL - Hardware Abstraction Layer (Hardware)
// REF - Reference device (Software)

// Check for hardware vertex processing
//int iVertexProcessing = 0;
//if (sD3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
//	iVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
//} else {
//	iVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
//}

// Check for shader versions

// Set presentation parameters
// http://msdn2.microsoft.com/en-us/library/bb172588.aspx
//ZeroMemory(&mD3DPP, sizeof(mD3DPP));
//mD3DPP.BackBufferWidth = iWidth;
//mD3DPP.BackBufferHeight = iHeight;
//mD3DPP.BackBufferFormat = D3DFMT_A8R8G8B8;
//mD3DPP.BackBufferCount = 1;
//mD3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
//mD3DPP.MultiSampleQuality = 0;
//mD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
//mD3DPP.hDeviceWindow = hWindow;
//mD3DPP.Windowed = !bFullscreen;
//mD3DPP.EnableAutoDepthStencil = true;
//mD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;
//mD3DPP.Flags = 0;
//mD3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
//if (CIniFile::Get()->GetBool("LOCKFPS", true))
//	mD3DPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
//else
//	mD3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

// Create Direct3D device
//if (FAILED(
//		mpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow,
//				iVertexProcessing, &mD3DPP, &mpD3DDevice))) {
//	// Write to log
//	CLog::Get()->Write(ELogMessageType_ERROR,
//			"[DXGraphics] Failed to create Direct3D device.");
//	return;
//}

// Set flag
	mInitialized = true;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE,
			"[DXGraphics] System initialized.");
}

//----------------------------------------------------------------------------------------------------

void CDXGraphics::Terminate(void) {
	// Check if we have already terminated the manager
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING,
				"[DXGraphics] System already terminated.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[DXGraphics] Terminating...");

	// Release Direct3D device
	//if (NULL != mpD3DDevice) {
	//	mpD3DDevice->Release();
	//	mpD3DDevice = NULL;
	//}

	// Release Direct3D
	//if (NULL != mpD3D) {
	//	mpD3D->Release();
	//	mpD3D = NULL;
	//}

	// Set flag
	mInitialized = false;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE,
			"[DXGraphics] System terminated");
}

//----------------------------------------------------------------------------------------------------

void CDXGraphics::BeginRender(GLColor sClearColor) {
	// Check if the system is initialized
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[DXGraphics] Graphics system not initialized.");
		return;
	}

	/*HRESULT hr = mpD3DDevice->TestCooperativeLevel();
	 if (FAILED(hr)) {
	 // If device is lost, rendering is not possible and we will have to try again later
	 if (hr == D3DERR_DEVICELOST) {
	 CLog::Get()->Write(ELogMessageType_ERROR,
	 "[DXGraphics] Graphics device lost.");
	 Sleep(100);
	 return;
	 }

	 // If the device is ready again, then we can go ahead and reset it
	 if (hr == D3DERR_DEVICENOTRESET) {
	 CLog::Get()->Write(ELogMessageType_ERROR,
	 "[DXGraphics] Resetting graphics device.");

	 // Release all device objects
	 //InvalidateDeviceObjects();

	 // Proceed and reset the device. Note that the reset will fail
	 // unless the application releases all resources that are
	 // allocated in D3DPOOL_DEFAULT, including any render targets
	 // and buffers.
	 hr = mpD3DDevice->Reset(&mD3DPP);
	 if (FAILED(hr))
	 return;

	 // Restore all device objects
	 //RestoreDeviceObjects();

	 CLog::Get()->Write(ELogMessageType_ERROR,
	 "[DXGraphics] Graphics device reset succeeded.");
	 }
	 }*/

	// Clear the screen and the back buffer
	/*mpD3DDevice->Clear(0, NULL,
	 D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, sClearColor,
	 1.0f, 0);*/
	glClearColor(sClearColor.r, sClearColor.g, sClearColor.b, sClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Begin rendering
	//mpD3DDevice->BeginScene();
	glViewport(0, 0, iWidth, iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, iWidth, iHeight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//----------------------------------------------------------------------------------------------------

void CDXGraphics::EndRender(void) {
	// Check if the system is initialized
	if (!mInitialized) {
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR,
				"[DXGraphics] Graphics system not initialized.");
		return;
	}

	// End rendering
	//mpD3DDevice->EndScene();

	// Show rendered scene
	//mpD3DDevice->Present(NULL, NULL, NULL, NULL);
	glFlush();
	glutPostRedisplay();
}
