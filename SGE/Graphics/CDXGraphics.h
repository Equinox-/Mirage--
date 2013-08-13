#ifndef INCLUDED_DXGRAPHICS_H
#define INCLUDED_DXGRAPHICS_H

//====================================================================================================
// Filename:	CDXGraphics.h
// Created by:	Peter Chan
// Description:	Singleton class for the graphics system. This is a wrapper class for Direct3D and it
//			  manages all the D3D initialization and creation of a Direct3D device interface. To
//			  access the interface, just make a call to Device(). The singleton can be accessed via
//			  a call to CDXGraphics::Get().
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "../Misc/CIniFile.h"
#include "../GLUtil/GLColor.h"
//====================================================================================================
// Class Declarations
//====================================================================================================

class CDXGraphics
{
public:
	// Accessor function for singleton instance
	static CDXGraphics* Get(void);

	// Destructor
	virtual ~CDXGraphics(void);

	// Functions to startup and shutdown the graphics system
	void Initialize(int hWindow, int iWidth, int iHeight, bool bFullscreen);
	void Terminate(void);

	// Function to access the Direct3D device
	// IDirect3DDevice9* D3DDevice(void) const;

	// Functions to be called before and after any rendering
	void BeginRender(GLColor clearColor = GLColor(0.0,0.0,0.0));
	void EndRender(void);

protected:
	// Protected constructor for singleton
	CDXGraphics(void);

private:
	static CDXGraphics* s_pInstance;	// Static instance for singleton

	//IDirect3D9*		 mpD3D;		 // Pointer to Direct3D interface
	//IDirect3DDevice9*   mpD3DDevice;   // Pointer to Direct3D device interface

	unsigned int		  mD3DDM;	// Current display mode
	//D3DPRESENT_PARAMETERS   mD3DPP;	// Presentation parameters
	int iWidth, iHeight;
	bool mInitialized;				 // Init flag
};

#endif // #ifndef INCLUDED_DXGRAPHICS_H
