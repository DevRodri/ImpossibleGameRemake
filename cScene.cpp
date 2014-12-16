
#include "cScene.h"
#include "cMouse.h"
#include <stdio.h>


cScene::cScene()
{
	velocity = 3;
	gx = 0;
	gy = (29 - HEIGHT_MAX_TILES + 5) * 32;
	oy = 0;
}
cScene::~cScene(){}

void cScene::LoadMap(char *file)
{
	int i,j,n;
	
	FILE *f;
	f=fopen("map.txt","r");
	//f = fopen("map_debug.txt", "r");

	for(i=0;i<SCENE_AREA_X;i++)
	{
		for(j=0;j<SCENE_AREA_Y;j++)
		{
			fscanf(f,"%d",&n);
			map[i][j]=n;
		}
	}

	fclose(f);
}

void cScene::GetGlobalPosition(int *gpx, int *gpy)
{
	*gpx = gx;
	*gpy = gy;
}

void cScene::SetGlobalPosition(int gpx, int gpy)
{
	gx = gpx;
	gy = gpy;
}

void cScene::GetLastPlayerLY(int *lply)
{
	*lply = lastPlayerLocalY;
}

void cScene::SetLastPlayerLY(int value)
{
	lastPlayerLocalY = value;
}

void cScene::CalculateCellonX(int *cell, int gpx)
{
	*cell = gpx / 32;
}

void cScene::GetOffsetYCamera(int *offset)
{
	*offset = oy;
}

void cScene::SetOffsetYCamera(int value)
{
	oy = value;
}

void cScene::GetVelocity(float *vel)
{
	*vel = velocity;
}
void cScene::GetFondo(int *f)
{
	*f = fondo;
}

void cScene::SetVelocity(float vel)
{
	velocity = vel;
}

void cScene::GetMapPosition(int *val, int x, int y)
{
	if (x < SCENE_AREA_X && y < SCENE_AREA_Y)
	{
		*val = map[x][y];
	}
	else //SEGMENTATION FAULT
		*val = -1;
}
int cScene::Changebackground(cPlayer *Player)
{
	int x, y, tsize;
	Player->GetGlobalPosition(&x, &y);
	Player->GetTileSize(&tsize);
	x = x / tsize;

	//324 fondo negro
	//360 fondo amarillo
	//660 fondo negro
	//684 fondo azul
	fondo = 0;
	if (x >= 324){ fondo = 1; }
	if (x >= 360){ fondo = 2; }
	if (x >= 660){ fondo = 1; }
	if (x >= 684){ fondo = 0; }

	return fondo;
}
