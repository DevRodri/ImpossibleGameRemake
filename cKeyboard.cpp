#include "cKeyboard.h"
#include "cLog.h"
#include <dxerr9.h>

/************************************************************************
cKeyboard::Constructor

Initializes the DI device
*************************************************************************/
cKeyboard::cKeyboard(LPDIRECTINPUT8 pDI, HWND hwnd)
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDev, NULL);
	if(FAILED(hr))
	{
		Log->Error(hr,"Creating keyboard device!");
	}
	//IDirectInput8::CreateDevice http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/input/ref/ifaces/idirectinput9/createdevice.asp

	hr = m_pDIDev->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		Log->Error(hr,"Setting keyboard data format!");
	}
	//IDirectInput8::SetDataFormat http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/input/ref/ifaces/idirectinputdevice9/setdataformat.asp
	//c_dfDIKeyboard - an array of 256 characters, each representing a key. If the high bit is set
	//					the key was down;otherwise, it was up. 

	//hr = m_pDIDev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY);
	hr = m_pDIDev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hr))
	{
		Log->Error(hr,"Setting keyboard cooperative level!");
	}
	//IDirectInputDevice8::SetCooperativeLevel  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/input/ref/ifaces/idirectinputdevice9/SetCooperativeLevel.asp

	hr = m_pDIDev->Acquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Acquiring keyboard!");
	}
	//IDirectInputDevice8::Acquire Method - http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/input/ref/ifaces/idirectinputdevice9/Acquire.asp

	Clear();
}


/**************************************************************************
cKeyboard::Destructor

Releases the DI device
**************************************************************************/
cKeyboard::~cKeyboard()
{
	if (m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}


/*************************************************************************
cKeyboard::Update()

Queueries current state of keyboard and stores it in the member variables.
*************************************************************************/
bool cKeyboard::Read()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);
	if(FAILED(hr))
	{
		Log->Error(hr,"Getting keyboard device state");

		hr = m_pDIDev->Acquire();
		if(FAILED(hr))
		{
			Log->Error(hr,"Acquiring keyboard!");
			return false;
		}

		hr = m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);
		if(FAILED(hr))
		{
			Log->Error(hr,"Getting keyboard device state");
			return false;
		}
	}
//IDirectInputDevice8::GetDeviceState  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/input/ref/ifaces/idirectinputdevice9/GetDeviceState.asp

	return true;
}


/***********************************************************************
cKeyboard::Acquire()

Acquire the keyboard
************************************************************************/
bool cKeyboard::Acquire()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	Clear();
	hr = m_pDIDev->Acquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Acquiring keyboard!");
		return false;
	}
	return true;
}


/************************************************************************
cKeyboard::Unacquire()

Unacquires the keyboard
*************************************************************************/
bool cKeyboard::Unacquire()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	Clear();
	hr = m_pDIDev->Unacquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Unacquiring keyboard!");
		return false;
	}
	return true;
}
