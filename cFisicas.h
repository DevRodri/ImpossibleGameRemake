#ifndef __FISICAS_H__
#define __FISICAS_H__

/*************************************************/
/* clase que aplica la gravedad a los objetos    */
/* clase que calcula las colisiones de un obejeto*/
#include "cGame.h"
#include "cScene.h"

class cFisicas
{
public:
	cFisicas(cGame *game);
	virtual ~cFisicas();

	float Gravity=0.01f;
	//funcion que aplica gravedad a la escena
	void apply_gravity(float dt);
	//funcion que detecta si el  un punto x y esta en colisión con algun objeto.
	bool is_incollision();

private:
	//funcion que nos dice si el player esta grounded
	bool is_grounded(int posx, int posy);
	bool tile_colision(cScene *Scene, int posx, int posy);
	cGame *Game;
};

#endif