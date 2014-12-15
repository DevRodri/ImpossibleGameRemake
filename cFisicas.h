#ifndef __FISICAS_H__
#define __FISICAS_H__

/*************************************************/
/* clase que aplica la gravedad a los objetos    */
/* clase que calcula las colisiones de un obejeto*/
#include "cPlayer.h"
#include "cScene.h"

#define VACIO	0
#define CUBO	1
#define PINCHO	2
#define AGUJERO	3
#define SUELO	4
#define FINAL	4

#define CUBICO		0
#define ESFERICO	1

class cFisicas
{
public:
	cFisicas();
	virtual ~cFisicas();

	float Gravity;
	//funcion que aplica gravedad a la escena
	bool ApplyGravity(cPlayer *Player, cScene *Scene, float dt);
	//funcion que detecta si el  Player esta en colisión con algun objeto.
	bool Is_Incollision(cPlayer *Player, cScene *Scene, int *type, int boundigbox, bool *down, bool *right);
	//funcion que te dice si el player esta tocando suelo o pieza solida.
	bool Is_Grounded(cPlayer *Player, cScene *Scene);
	void SetGravity(float gravity);
	void GetGravity(float *gravity);
	void MoveScene(cPlayer *Player, cScene *Scene);

private:
	//funcion que nos dice si el player esta grounded
	bool doesCubeIntersectSphere(int cx1, int cy1, int cx2, int cy2, int spx, int spy, int Radio);
	bool TileColisionCubic(cScene *Scene, int posx, int posy, int *type, int tsize);
	bool TileColisionSferic(cScene *Scene, int playerx, int playery, int posx, int posy, int *type, int tsize);
};

#endif