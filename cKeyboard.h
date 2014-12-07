
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "cInputLayer.h"

class cKeyboard
{
public:
	cKeyboard(LPDIRECTINPUT8 pDI, HWND hwnd);
	~cKeyboard();

	bool KeyDown(int key) { return (m_keys[key] & 0x80) ? true : false; }
	bool KeyUp(int key) { return (m_keys[key] & 0x80) ? false : true; }

	bool Read();
	void Clear() { ZeroMemory(m_keys, 256 * sizeof(char)); }
	
	bool Acquire();
	bool Unacquire();

private:
	LPDIRECTINPUTDEVICE8 m_pDIDev;

	char m_keys[256];
};

#endif 