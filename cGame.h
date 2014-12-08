
#ifndef __GAME_H__
#define __GAME_H__

#include "cGraphicsLayer.h"
#include "cInputLayer.h"
#include "cScene.h"
#include "cPlayer.h"
//#include "cPhysics.h"

#define STATE_MAIN	0
#define STATE_GAME	1
#define STATE_PAUSE	2

class cGame
{
public:
	cGame();
	virtual ~cGame();

	bool Init(HWND hWnd,HINSTANCE hInst,bool exclusive);

	bool Loop(); 
	bool ManageInputs();
	bool ManagePhysics();
	bool ManageLogic();
	bool ManageGraphics();
	void Finalize();

	//Clases que posee el juego
	cGraphicsLayer Graphics;
	cInputLayer Input;
	cScene Scene;
	cPlayer Player;
	//cPhysics Physics;

	int state;
};


#endif