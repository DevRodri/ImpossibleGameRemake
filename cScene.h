
#ifndef __SCENE_H__
#define __SCENE_H__

//Resolution
#define SCREEN_RES_X	800
#define SCREEN_RES_Y	600

//Map coordinate beginning
#define SCENE_Xo		0
#define SCENE_Yo		0

//Tiles que caben en pantalla
#define WIDTH_MAX_TILES		25
#define HEIGHT_MAX_TILES	19 //son 18,75 pero si pintamos 18 se quedaria un hueco sin pintar y se veria feo...

//Tamaño del mapa en tiles
#define SCENE_AREA_X	64
#define SCENE_AREA_Y	64	

//Habrá que retocarla para el tema físicas, acorde con el mapa.
#define SCENE_GROUND	32

class cScene
{

public:
	
	cScene();
	virtual ~cScene();

	void LoadMap(char *file);

	void GetGlobalPosition(int *gx, int *gy);
	void SetGlobalPosition(int gx, int gy);

	void CalculateCellonX(int *cell, int gx);


	void GetVelocity(float *vel);
	void SetVelocity(float vel);

	void GetMapPosition(int *val, int x, int y); //Obtiene en "val" la posicion (x,y) del mapa

	int map[SCENE_AREA_X][SCENE_AREA_Y];
	int gx, gy; //posicion global actual en el mapa
	float velocity; //velocidad de desplazamiento del mapa
};


#endif
