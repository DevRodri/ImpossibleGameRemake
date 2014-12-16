#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <windows.h>

class cScene;

class cPlayer
{
public:
	cPlayer(void);
	virtual ~cPlayer(void);

	void SetLocalPosition(int posx, int posy);
	void GetLocalPosition(int *posx, int *posy);

	//calcula la posicion global respecto al tamaño total del mapa
	void SetGlobalPosition(int posx, int posy);
	void GetGlobalPosition(int *posx, int *posy);
	void SetVely(float vely);
	void GetVely(float *vely);
	void SetTileSize(int tsize);
	void GetTileSize(int *tsize);
	void SetVuelta(float v);
	void GetVuelta(float *v);
	void SetVelocidadVuelta(float vvuelta);
	void GetVelocidadVuelta(float *vvuelta);

	void PlayDieAnimation();
	void ResetDieAnimation();
	void GetDieAnimationSeq(int *seq);
	bool IsDeath();


private:
	int lx, ly; //posicion local del jugador (en pantalla)
	int gx, gy; //posicion global del jugador (en todo el mapa)
	int tileSize; //tamaño de la tile del personaje
	float velocity; //velocidad del jugador en el eje Y
	bool death;
	int delay, seq; //control de la animación de muerte
	float vuelta, velocidadvuelta;
};

#endif