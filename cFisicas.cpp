#include "cFisicas.h"


cFisicas::cFisicas(cGame *game_parameter){Game=game_parameter; }
cFisicas::~cFisicas(){ Game = NULL; }

void cFisicas::apply_gravity(float dt)
{
	float velocity;
	Game->Player.GetVely(&velocity);
	velocity = velocity + Gravity * dt;
	Game->Player.SetVely(velocity);
}

bool cFisicas::tile_colision(cScene *Scene,int posx,int posy)
{
	int objeto;
	objeto = Scene->map[posx][posy];

	//0-vacia no hay colision
	if ((objeto) == 0){ return false; }
	
	//1-cubo,si el cubo esta a la derecha hay que colisionar, si esta debajo no????? este caso hay que mirarlo.
	if ((objeto) == 1){ 
		return true; 
	}
	
	//2-pincho siempre hay colision, este caso se puede afinar si vemos que colisiona demasiado pronto
	if ((objeto) == 2){ return false; }
	
	//3-suelo que mata hay colision
	if ((objeto) == 3){ return true; }
	
	//4-suelo normal no hay colision
	if ((objeto) == 4){ return false; }
	else  { return false; }
}

bool cFisicas::is_incollision()
{
	int posx, posy,tsize;
	//obtener las coordenadas del jugador en el mundo

	Game->Player.GetGlobalPosition(&posx, &posy);
	Game->Player.GetTileSize(&tsize);

	//calculamos los 4 puntos de la caja del player.
	//  x,y------x1,y1
	//  |          |  
	//  |          |  
	//  |          |  
	//x3,y3------x2,y2
	//
	int x, y, x1, y1, x2, y2, x3, y3;

	x = posx;
	y = posy;
	x1 = posx + tsize;
	y1 = posy;
	x2 = posx + tsize;
	y2 = posy + tsize;
	x3 = x;
	y3 = posy + tsize;

	//para cada x,y buscamos en que zona del mapa esta

	if ((posy % tsize == 0) && (posx %tsize == 0))
	{//estamos en un espacio entero es imposible que haya colision.
		return false;
	}
	else
	{ //caso dificil hay que mirar 4 posiciones del mapa.
		int tx,ty, tx1, ty1, tx2, ty2, tx3, ty3;
		bool b1, b2, b3, b;
		div_t divresult;
		
		divresult = div(x, tsize);
		tx = divresult.quot;
		divresult = div(y, tsize);
		ty = divresult.quot;

		b = tile_colision(&Game->Scene, tx, ty);

		divresult = div(x1, tsize);
		tx1 = divresult.quot;
		divresult = div(y1, tsize);
		ty1 = divresult.quot;

		b1 = tile_colision(&Game->Scene, tx1, ty1);

		divresult = div(x2, tsize);
		tx2 = divresult.quot;
		divresult = div(y2, tsize);
		ty2 = divresult.quot;

		b2 = tile_colision(&Game->Scene, tx1, ty1);

		divresult = div(x3, tsize);
		tx3 = divresult.quot;
		divresult = div(y3, tsize);
		ty3 = divresult.quot;

		b3 = tile_colision(&Game->Scene, tx1, ty1);

		return b&&b1&&b2&&b3;

	}
}


bool cFisicas::is_grounded()
{
	int posx, posy,tsize;
	
	Game->Player.GetGlobalPosition(&posx, &posy);
	Game->Player.GetTileSize(&tsize);
	//un objeto toca suelo cuando el objeto que tiene justo debajo es del tipo cubo o es el suelo.
	
	if (posy == 0){ return true; } //esta en el suelo;
	
	if (posy % 32 == 0) //puede ser que este encima de un cubo, pero no se sabe.
	{
		//buscamos los 2 cubos que pueden estar justo debajo.
		int tx, ty;

		div_t divresult;
		divresult = div(posx, tsize);
		tx = divresult.quot;
		divresult = div(posy, tsize);
		ty = divresult.quot;

		if (posx % 32 == 0) //estamos justo en una sola tile, tenemos que mriar solo debajo nuestro
		{
			int objeto;
			objeto = Game->Scene.map[tx][ty + 1];
			if (objeto==1 || objeto==4)//suelo o cubo
			{
				return true;
			}
			return false;
		}
		else //estamos en medio de 2 hay que mirar las 2 y es la suma de las 2.
		{
			int objeto;
			objeto = Game->Scene.map[tx][tx + 1];
			int objeto1;
			objeto1 = Game->Scene.map[tx+1][ty + 1];

			if ((objeto == 1 || objeto == 4) || (objeto1 == 1 || objeto1 == 4))//suelo o cubo
			{
				return true;
			}
			return false;
		}
	}
	else
	{
		return false;
	}
return false;
}