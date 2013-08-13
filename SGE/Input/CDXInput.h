#ifndef INCLUDED_DXINPUT_H
#define INCLUDED_DXINPUT_H

//====================================================================================================
// Filename:	CDXInput.h
// Created by:	Peter Chan
// Description:	Singleton class for the input system. It provides support for keyboard, mouse, and
//			  gamepad control. Internally it uses DirectInput devices to handle all input
//			  detections. The singleton can be accessed via a call to CDXInput::Get().
//====================================================================================================

//====================================================================================================
// Defines
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================
class CDXInput {
public:
	static int cdxInput_glutKeyToSGE(int glutKey);
	static int cdxInput_characterToSGE(char c);
	static void cdxInput_mouseMotion(int x, int y);
	static void cdxInput_mouseButton(int button, int state, int x, int y);
	static void cdxInput_keyPress(unsigned char c, int x, int y);
	static void cdxInput_keyRelease(unsigned char c, int x, int y);
	static void cdxInput_keyPressSpecial(int c, int x, int y);
	static void cdxInput_keyReleaseSpecial(int c, int x, int y);
	// Accessor function for singleton instance
	static CDXInput* Get(void);
protected:
	// Protected constructor for singleton
	CDXInput(void);
public:
	// Functions to startup/shutdown the input system
	void Initialize(int hWindow);
	void Terminate(void);

	// Function to update the input device states
	void Update(void);

	// Functions to query for a key state
	bool IsKeyDown(int iKey) const;
	bool IsKeyPressed(int iKey) const;

	// Functions to query for a mouse state
	bool IsMouseDown(int iMouse) const;
	bool IsMousePressed(int iMouse) const;
	bool IsCloseRequested(void) const;

	// Functions to get the mouse position
	int GetMouseScreenX(void) const;
	int GetMouseScreenY(void) const;

	// Functions to check screen boundary
	bool IsMouseLeftEdge(void) const;
	bool IsMouseRightEdge(void) const;
	bool IsMouseTopEdge(void) const;
	bool IsMouseBottomEdge(void) const;

	// Functions to get the game pad analog stick
	//float GetLeftAnalogX(void) const;
	//float GetLeftAnalogY(void) const;
	//float GetRightAnalogX(void) const;
	//float GetRightAnalogY(void) const;

	int mCurrMouseState;		  // Current mouse state
	int mPrevMouseState;		  // Previous mouse state

	int mMouseX;						   // Current mouse position X
	int mMouseY;						   // Current mouse position Y
	int mScreenWidth;					  // Screen width
	int mScreenHeight;					 // Screen height
	static const int kKeyBufferSize = 256;
	char mCurrKeyBuffer[kKeyBufferSize]; // Buffer for current key states
	void updateKeyModifiers();
private:
	// Members
	static CDXInput* s_Instance;			// Static instance for singleton
	char mPrevKeyBuffer[kKeyBufferSize]; // Buffer for previous key states

	bool mInitialized;					 // Init flag
};

#endif // #ifndef INCLUDED_DXINPUT_H
