
#ifndef __INPUTLAYER_H__
#define __INPUTLAYER_H__

#pragma comment(lib,"dinput8.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class cMouse;
class cKeyboard;

#define USE_KEYBOARD	0x01
#define USE_MOUSE		0x02

class cInputLayer  
{
public:
	cInputLayer();
	virtual ~cInputLayer();

	bool	Init(HINSTANCE hInst, HWND hWnd, bool isExclusive = true, DWORD flags = 0);
	bool	Finalize();

	void	AcquireAll();
	void	UnacquireAll();

	cKeyboard	*GetKeyboard();
	cMouse		*GetMouse();
	
	bool	Read();

	bool	KeyDown(char key);
	bool	KeyUp(char key);

	bool	ButtonDown(int button);
	bool	ButtonUp(int button);
	void	GetMouseMovement(int *dx, int *dy);
	int		GetMouseWheelMovement();
	void	SetMousePosition(int xo, int yo);
	void	GetMousePosition(int *xpos, int *ypos);
	
private:
	cKeyboard *m_pKeyboard;
	cMouse *m_pMouse;

	LPDIRECTINPUT8 m_pDI;
};

#endif
