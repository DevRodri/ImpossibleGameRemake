#include "cPlayer.h"
#include "cScene.h"

cPlayer::cPlayer()
{
	tileSize = 32;
	velocity = 0;
	death = false;
	delay = 0;
	seq = 0;
	vuelta = 0.0f; 
	velocidadvuelta = 0.7f;
	banderalx = 0;
	banderaly = 0;
	banderagx = 0;  
	banderagy = 0;
}

cPlayer::~cPlayer()
{

}
void cPlayer::SetVuelta(float v)
{
	vuelta = v; //velocidad del jugador en el eje Y
}

void cPlayer::GetVelocidadVuelta(float *vvuelta)
{
	*vvuelta = velocidadvuelta;
}
void cPlayer::SetVelocidadVuelta(float vvuelta)
{
	velocidadvuelta = vvuelta; //velocidad del jugador en el eje Y
}

void cPlayer::GetVuelta(float *v)
{
	*v = vuelta;
}
void cPlayer::SetVely(float vely)
{
	if (vely > 20) vely = 20;
	velocity = vely; //velocidad del jugador en el eje Y
}

void cPlayer::GetVely(float *vely)
{
	*vely = velocity;
}

void cPlayer::SetLocalPosition(int posx, int posy)
{
	lx = posx;
	ly = posy;
}

void cPlayer::GetLocalPosition(int *posx, int *posy)
{
	*posx = lx;
	*posy = ly;
}

void cPlayer::SetGlobalPosition(int posx, int posy)
{
	gx = posx;
	gy = posy;
}

void cPlayer::GetGlobalPosition(int *posx, int *posy)
{
	*posx = gx;
	*posy = gy;
}
void cPlayer::SetTileSize(int tsize)
{
	tileSize = tsize;
}
void cPlayer::GetTileSize(int *tsize)
{
	*tsize = tileSize;
}


void cPlayer::SetLocalBandera(int x,int y)
{
	banderalx = x; 
	banderaly = y;
}
void cPlayer::GetLocalBandera(int* x, int *y)
{
	*x = banderalx;
	*y = banderaly;
}
void cPlayer::SetGlobalBandera(int x, int y)
{
	banderagx = x;
	banderagy = y;
}
void cPlayer::GetGlobalBandera(int *x, int *y)
{
	*x = banderagx;
	*y = banderagy;
}
void cPlayer::PlayDieAnimation()
{
	delay++;
	if (delay >= 3)
	{
		seq++;
		if (seq > 5) {
			seq = 0;
			death = true;
		}
		delay = 0;
	}
}

void cPlayer::ResetDieAnimation()
{
	delay = seq = 0;
	death = false;
}

void cPlayer::GetDieAnimationSeq(int *val)
{
	*val = seq;
}

bool cPlayer::IsDeath()
{
	return death;
}
