
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "cInputLayer.h"

//Mouse buttons
#define LEFT		0
#define RIGHT		1

class cMouse 
{
public:
	cMouse(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive = true);
	~cMouse();

	bool	ButtonDown(int button);
	bool	ButtonUp(int button);
	int		GetWheelMovement();
	void	GetMovement(int *dx, int *dy);

	bool Acquire();
	bool Unacquire();

	void SetPosition(int xo, int yo);
	void GetPosition(int *xpos, int *ypos);
	void GetRect(RECT *rc, int *posx, int *posy);
	bool Read();

	bool In(int xo,int yo,int xf,int yf);
	
	int x,y;

private:
	LPDIRECTINPUTDEVICE8 m_pDIDev;
	DIMOUSESTATE	m_state;
};

#endif 
