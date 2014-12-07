
#include "cMouse.h"
#include "cLog.h"
#include "cScene.h"
#include <dxerr9.h>

/************************************************************************
cMouse::Constructor()

Initialize DI device
*************************************************************************/
cMouse::cMouse(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive)
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = pDI->CreateDevice(GUID_SysMouse, &m_pDIDev, NULL);
	if(FAILED(hr))
	{
		Log->Error(hr,"Creating mouse device");
	}
	hr = m_pDIDev->SetDataFormat(&c_dfDIMouse);
	if(FAILED(hr))
	{
		Log->Error(hr,"Setting mouse data format");
	}
	//DIMOUSESTATE Structure - http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/input/ref/structs/dimousestate.asp
	
	DWORD flags;
	if (isExclusive)
	{
		flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
	}
	else
	{
		flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
		//ShowCursor(false);
	}

	hr = m_pDIDev->SetCooperativeLevel(hwnd, flags);
	if (FAILED(hr))
	{
		Log->Error(hr,"Setting mouse cooperative level");
	}

	hr = m_pDIDev->Acquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Acquiring mouse");
	}

	hr = m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state);
	if(FAILED(hr))
	{
		Log->Error(hr,"Getting mouse device state");
	}
}


/***************************************************************************
cMouse::Destructor()

Release DI device
***************************************************************************/
cMouse::~cMouse()
{
	if(m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}

/******************************************************************************
cMouse::Read()

Queuries the current state of the mouse and stores it in the member variables
********************************************************************************/
bool cMouse::Read()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state);
	if(FAILED(hr))
	{
		Log->Error(hr,"Getting mouse device state");

		hr = m_pDIDev->Acquire();
		if(FAILED(hr))
		{
			Log->Error(hr,"Acquiring mouse");
			return false;
		}
		hr = m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state);
		if(FAILED(hr))
		{
			Log->Error(hr,"Getting mouse device state");
			return false;
		}
	}
	x+=m_state.lX;
	y+=m_state.lY;
	
	if(x<0) x=0;
	else if(x>=SCREEN_RES_X) x=SCREEN_RES_X-1;

	if(y<0) y=0;
	else if(y>=SCREEN_RES_Y) y=SCREEN_RES_Y-1;

	return true;
}

/******************************************************************************
cMouse::Acquire()

Acquires mouse
********************************************************************************/
bool cMouse::Acquire()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = m_pDIDev->Acquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Acquiring mouse");
		return false;
	}
	return true;
}


/******************************************************************************
cMouse::Acquire()

Unacquires mouse
********************************************************************************/
bool cMouse::Unacquire()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = m_pDIDev->Unacquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Unacquiring mouse");
		return false;
	}
	return true;
}

/******************************************************************************
Mouse queries
********************************************************************************/
bool cMouse::ButtonDown(int button)
{
	return (m_state.rgbButtons[button] & 0x80) ? true : false;
}
bool cMouse::ButtonUp(int button)
{
	return (m_state.rgbButtons[button] & 0x80) ? false : true;
}
int  cMouse::GetWheelMovement()
{
	return m_state.lZ;
}
void cMouse::GetMovement(int *dx, int *dy)
{
	*dx = m_state.lX;
	*dy = m_state.lY;
}
void cMouse::SetPosition(int xo, int yo)
{
	x=xo;
	y=yo;
}
void cMouse::GetPosition(int *xpos, int *ypos)
{
	*xpos=x;
	*ypos=y;
}

void cMouse::GetRect(RECT *rc, int *posx, int *posy)
{
	SetRect(rc, 0, 0, 32, 32); *posx = 0; *posy = 0;
}

/******************************************************************************
cMouse::In()

Mouse position in rectangle
********************************************************************************/
bool cMouse::In(int xo,int yo,int xf,int yf)
{
	return ((x>=xo)&&(x<xf)&&(y>=yo)&&(y<yf)) ? true : false;
}
