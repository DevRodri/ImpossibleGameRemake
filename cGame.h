
#ifndef __GAME_H__
#define __GAME_H__

#include "cGraphicsLayer.h"
#include "cInputLayer.h"
#include "cScene.h"
#include "cPlayer.h"
#include "cFisicas.h"
#include "cMP3.h"
//#include "cPhysics.h"

#define STATE_MAIN	0
#define STATE_GAME	1
#define STATE_PAUSE	2
#define STATE_DEATH 3
#define STATE_END 4

class cGame
{
public:
	cGame();
	virtual ~cGame();

	bool Init(HWND hWnd, HINSTANCE hInst, bool exclusive);

	bool Loop();
	bool ManageInputs();
	bool ManagePhysics();
	bool ManageLogic();
	bool ManageGraphics();
	void Finalize();
	void ProcessOrder();
	void ResetLevel();
	void ResetSaveLevel();

	//Clases que posee el juego
	cGraphicsLayer Graphics;
	cInputLayer Input;
	cScene Scene;
	cPlayer Player;
	cFisicas Physics;
	cInterface Interface;
	cMP3 Mp3Inicio;
	cMP3 Mp3Muerte;
	cMP3 Mp3Nivel;
	cMP3 Mp3;

	int psx, psy, ssx, ssy, coff;
	float fsalt;
	float deltaTime;
	DWORD lastCounts, actCounts;

	int state;
};


#endif
