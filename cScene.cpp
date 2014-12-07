
#include "cScene.h"
#include "cMouse.h"
#include <stdio.h>


cScene::cScene()
{
	cx=0;
	cy=0;
}
cScene::~cScene(){}

void cScene::LoadMap(char *file)
{
	int i,j,n;
	
	FILE *f;
	f=fopen("map.txt","r");

	for(i=0;i<SCENE_AREA;i++)
	{
		for(j=0;j<SCENE_AREA;j++)
		{
			fscanf(f,"%d",&n);
			map[i][j]=n;
		}
	}

	fclose(f);
}
