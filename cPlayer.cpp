#include "cPlayer.h"
#include "cScene.h"

cPlayer::cPlayer()
{

}

cPlayer::~cPlayer()
{

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