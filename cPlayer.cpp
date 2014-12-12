#include "cPlayer.h"
#include "cScene.h"

cPlayer::cPlayer()
{
	tileSize = 32;
	velocity = 0;
	death = false;
	delay = 0;
	seq = 0;
}

cPlayer::~cPlayer()
{

}

void cPlayer::SetVely(float vely)
{
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

void cPlayer::PlayDieAnimation()
{
	delay++;
	if (delay >= 5)
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
