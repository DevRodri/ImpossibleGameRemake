
#include "cScene.h"
#include "cMouse.h"
#include <stdio.h>


cScene::cScene()
{
	velocity = 3;
	gx = 0;
	gy = 0;
}
cScene::~cScene(){}

void cScene::LoadMap(char *file)
{
	int i,j,n;
	
	FILE *f;
	f=fopen("map.txt","r");

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

void cScene::GetVelocity(float *vel)
{
	*vel = velocity;
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
