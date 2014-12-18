
#include "cScene.h"
#include "cMouse.h"
#include <stdio.h>


cScene::cScene()
{
	velocity = 3;
	gx = 0;
	gy = (29 - HEIGHT_MAX_TILES + 5) * 32;
	oy = 0;
	end = false;
	delay = 0;
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
	// fondo de final de fase
	fondo = 0;
	if (x >= 288){ fondo = 1; }
	if (x >= 320){ fondo = 2; }
<<<<<<< HEAD
	if (x >= 630){ fondo = 1; }
	if (x >= 662){ fondo = 0; }
=======
	if (x >= 734){ fondo = 1; }
	if (x >= 750){ fondo = 0; }
>>>>>>> parent of f472e78... segundo pintado.. da igual pk la musica va a su bola dependiendo del pc XD

	return fondo;
}

void cScene::SaveCheckPoint(cPlayer *Player,double tcancion)
{
	//psx, psy, ssx, ssy, coff,pslx,psly;
	float vely;
	int psx, psy, pslx, psly;

	Player->GetGlobalPosition(&psx, &psy);
	Player->GetLocalPosition(&pslx, &psly);
	Player->GetVely(&vely);
	
	ck.SetSceneCPoint(gx,gy);
	ck.SetGlobalCPoint(psx, psy);
	ck.SetLocalCPoint(pslx, psly);
	ck.SetVelCPoint(vely);
	ck.SetCameraoff(oy);
	ck.tiempocancion=tcancion;

}
void cScene::RestoreCheckPoint(cPlayer *Player)
{
	float vely;
	int psx, psy, pslx, psly;
	
	ck.GetGlobalCPoint(&psx, &psy);
	ck.GetLocalCPoint(&pslx, &psly);
	ck.GetVelCPoint(&vely);
	
	ck.GetSceneCPoint(&gx, &gy);
	ck.GetCameraoff(&oy);

	Player->SetGlobalPosition(psx, psy);
	Player->SetLocalPosition(pslx, psly);
	Player->SetVely(vely);

}
void cScene::PlayEnd()
{
	//delay++;
	//if (delay >= 0)
	//{
		alpha = alpha + 5;
		
		if (alpha > 100) alpha = alpha ++;
		if (alpha <= 100) alpha = alpha + 3;
		if (alpha <= 50) alpha = alpha +5;


		if (alpha > 255) {
			
			alpha = 255;
			delay++;
			if (delay >= 200){
				alpha = 0;
				end = true;
				delay = 0;
			}

		}
		//delay = 0;
	//}
}

bool cScene::FinishEndAnimation()
{
	return end;
}