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
	//funcion que detecta si el  Player esta en colisi�n con algun objeto.
	bool is_incollision();
	//funcion que te dice si el player esta tocando suelo o pieza solida.
	bool is_grounded();

private:
	//funcion que nos dice si el player esta grounded
	
	bool tile_colision(cScene *Scene, int posx, int posy);
	cGame *Game;
};

#endif