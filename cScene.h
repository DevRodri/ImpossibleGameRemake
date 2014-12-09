
#ifndef __SCENE_H__
#define __SCENE_H__

//Resolution
#define SCREEN_RES_X	800
#define SCREEN_RES_Y	600

//Map coordinate beginning
#define SCENE_Xo		0
#define SCENE_Yo		0

//Visible part
#define SCENE_WIDTH		24
#define SCENE_HEIGHT	17

//Dimension = Area^2
#define SCENE_AREA		64
#define SCENE_GROUND	32

class cScene
{

public:
	
	cScene();
	virtual ~cScene();

	void LoadMap(char *file);

	int map[SCENE_AREA][SCENE_AREA];
	int cx,cy;
	float velocity;
	int x;
};


#endif
