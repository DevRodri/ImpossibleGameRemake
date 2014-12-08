#include "cPlayer.h"
#include "cScene.h"

cPlayer::cPlayer()
{
	tileSize = 32;
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

void cPlayer::SetGlobalPosition(cPlayer *Player, cScene *Scene)
{
	//TODO

	//
}

void cPlayer::GetGlobalPosition(int *posx, int *posy)
{
	*posx = gx;
	*posy = gy;
}
void cPlayer::SetTileSize(int size)
{
	tileSize = size;
}
void cPlayer::GetTileSize(int *size)
{
	*size = tileSize;
}
