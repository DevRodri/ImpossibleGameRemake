#include "cPlayer.h"
#include "cScene.h"

cPlayer::cPlayer()
{
	tileSize = 32;
	velocity = 0;
}

cPlayer::~cPlayer()
{

}

void cPlayer::SetVely(float vely)
{
	velocity=vely; //velocidad del jugador en el eje Y
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
	//TODO
	gx = posx;
	gy = posy;
	//
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
