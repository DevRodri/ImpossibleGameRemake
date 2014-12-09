#ifndef __FISICAS_H__
#define __FISICAS_H__

/*************************************************/
/* clase que aplica la gravedad a los objetos    */
/* clase que calcula las colisiones de un obejeto*/
#include "cPlayer.h"
#include "cScene.h"

class cFisicas
{
public:
	cFisicas();
	virtual ~cFisicas();

	float Gravity;
	//funcion que aplica gravedad a la escena
	bool ApplyGravity(cPlayer *Player,cScene *Scene, float dt);
	//funcion que detecta si el  Player esta en colisión con algun objeto.
	bool Is_Incollision(cPlayer *Player, cScene *Scene,int *type);
	//funcion que te dice si el player esta tocando suelo o pieza solida.
	bool Is_Grounded(cPlayer *Player, cScene *Scene);
	void SetGravity(float gravity);
	void GetGravity(float *gravity);

private:
	//funcion que nos dice si el player esta grounded
	
	bool TileColision(cScene *Scene, int posx, int posy,int *type);
};

#endif