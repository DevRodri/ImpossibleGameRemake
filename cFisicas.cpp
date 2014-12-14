#include "cFisicas.h"
#include <math.h> 

cFisicas::cFisicas(){}
cFisicas::~cFisicas(){}

void cFisicas::SetGravity(float gravity)
{
	Gravity = gravity;
}

void cFisicas::GetGravity(float *gravity)
{
	*gravity = Gravity;
}


void cFisicas::MoveScene(cPlayer *Player, cScene *Scene)
{
	int y, x,plx,ply;
	float vel;
	Scene->GetVelocity(&vel);
	Player->GetGlobalPosition(&x, &y);

	//Movimiento en X
	x = x + vel;
	int scenegx, scenegy;
	Scene->GetGlobalPosition(&scenegx, &scenegy);
	Scene->SetGlobalPosition(scenegx + vel, scenegy);
	//Scene->gx +=vel;

	//Movimiento en Y ( limite superior y límite inferior )

	//volvemos a guardar las variables
	Player->SetGlobalPosition(x, y);
}

bool cFisicas::ApplyGravity(cPlayer *Player, cScene *Scene, float dt)
{
	float velocity;
	int x, y, type, tsize, posfy, posfx, localx, localy;
	bool b, down, right;


	//velocidad de la escena (movimiento de la misma). Falta definir bien los atributos y no hacerlo tan chapuza.

	Player->GetVely(&velocity);
	velocity = velocity + Gravity * dt;
	Player->SetVely(velocity);

	Player->GetTileSize(&tsize);
	Player->GetGlobalPosition(&x, &y);
	Player->GetLocalPosition(&localx, &localy);

	posfx = y;
	posfx = x;

	//aqui vamos a situar el player en su posición actual y le sumamos la caida de la gravedad
	posfy = y + velocity*dt;
	localy = localy + velocity*dt;
	//nueva posicion del player en funcion de la velocidad de la escena

	// pero si esta suma situa a la pieza encima de una base hay que ponerle la altura de la base
	//es decir, si la velocidad es de bajada (positiva) y colisiona con un objeto lo ponemos a la altura del objeto.
	Player->SetGlobalPosition(posfx, posfy);
	Player->SetLocalPosition(localx, localy);

	b = Is_Incollision(Player, Scene, &type, ESFERICO, &down, &right);

	if (b && velocity>0)
	{
		//hemos colisionado, si la colision es con un cubo de suelo actualizamos la vel a 0 y reposicionamos el player encima del cubo
		if ((type == CUBO))
		{
			Player->SetVely(0);

			div_t divresult;
			divresult = div(posfy, tsize);
			posfy = divresult.quot * tsize;
			localy = (localy / tsize)*tsize;

		}
		if (type == SUELO)
		{

			Player->SetVely(0);

			div_t divresult;
			divresult = div(posfy, tsize);
			posfy = divresult.quot * tsize;
			localy = (localy / tsize)*tsize;

		}
	}

	//controlamos que no caiga mas alla del suelo hay 32 niveles de altura, si esta en el 33 es que esta ya fuera del suelo

	if (posfy > (tsize * SCENE_GROUND)) {
		posfy = SCENE_GROUND*tsize;
		localy = SCENE_GROUND*tsize;
		Player->SetVely(0);
	}

	Player->SetGlobalPosition(posfx, posfy);
	Player->SetLocalPosition(localx, localy);

	return true;
}

bool cFisicas::TileColisionCubic(cScene *Scene, int posx, int posy, int *type, int tsize)
{

	div_t divresult;
	int tx, ty;

	divresult = div(posx, tsize);
	tx = divresult.quot;
	divresult = div(posy, tsize);
	ty = divresult.quot;

	*type = Scene->map[ty][tx];

	if ((*type) == VACIO){ return false; }
	if ((*type) == CUBO){ return true; }
	if ((*type) == PINCHO){ return true; }
	if ((*type) == AGUJERO){ return true; }
	if ((*type) == SUELO){ return true; }
	else  { return false; }
}


bool cFisicas::TileColisionSferic(cScene *Scene, int playerx, int playery, int posx, int posy, int *type, int tsize)
{

	div_t divresult;
	int tx, ty;

	divresult = div(posx, tsize);
	tx = divresult.quot;
	divresult = div(posy, tsize);
	ty = divresult.quot;

	int pctx1, pcty1;
	float distancia;

	//centramos el punto del player
	playerx = playerx + tsize / 2;
	playery = playery + tsize / 2;

	pctx1 = (tx*tsize) + (tsize / 2);
	pcty1 = (ty*tsize) + (tsize / 2);

	//distancia d(a,b)=sqtr((x2-x1)^2+(y2-y1)^2)
	int tipo;
	tipo = Scene->map[ty][tx];

	///vamos a hacer que la colision con cubos vs cubos:
	if ((tipo == CUBO) || (tipo == SUELO))
	{
		*type = tipo;
		return true;
	}
	else{
		////////////////////////////el resto es esferico.
		distancia = sqrt(pow(float(playerx - pctx1), 2) + pow(float(playery - pcty1), 2));

		if (distancia < tsize)
		{ //posible impacto!!!

			if ((tipo) == VACIO){ return false; }
			if ((tipo) == CUBO){ *type = CUBO; return true; }
			if ((tipo) == PINCHO){ *type = PINCHO; return true; }
			if ((tipo) == AGUJERO){ *type = AGUJERO; return true; }
			if ((tipo) == SUELO){ *type = SUELO; return true; }
			else  { return false; }
		}
	}

	return false;
}

bool cFisicas::Is_Incollision(cPlayer *Player, cScene *Scene, int *type, int boundigbox, bool *down, bool *right)
{
	int posx, posy, tsize;

	*down = false;
	*right = false;

	//obtener las coordenadas del jugador en el mundo
	Player->GetGlobalPosition(&posx, &posy);
	Player->GetTileSize(&tsize);

	if (boundigbox == CUBICO)
	{
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
			bool b1, b2, b3, b;

			b = TileColisionCubic(Scene, x, y, type, tsize);//colision arriba a la izquierda??? no deberiamos ni mirar esto.

			b1 = TileColisionCubic(Scene, x1, y1, type, tsize); //colision lateral derecha
			if (b1) { *right = true; return b1; }

			b2 = TileColisionCubic(Scene, x2, y2, type, tsize); //colision abajo derecha
			if (b2)	{ *down = true; return b2; }

			b3 = TileColisionCubic(Scene, x3, y3, type, tsize); // colision abajo izquierda
			if (b3) { *down = true; return b3; }

			return b || b1 || b2 || b3; // o false que es lo mismo
		}
	}
	if (boundigbox == ESFERICO)
	{
		if (posx%tsize == 0) //estamos en una celda fija de las x
		{
			if (posy%tsize == 0)
			{//estamos justo en una tile concreta. aqui podemos mirar pero nunca habrá una colision

				//return TileColisionSferic(Scene, posx, posy, posx, posy, type, tsize);
				return TileColisionCubic(Scene, posx, posy, type, tsize);
			}
			else
			{// estamos en una x fija pero en el aire con la ficha
				//	Puede haber colision solamente abajo

				//no es necesario calcular distancias porque estan alineados es igual.
				bool b;
				//b = TileColisionSferic(Scene,posx, posy, posx, posy + tsize, type, tsize); //miramos justo debajo a ver que hay.
				b = TileColisionCubic(Scene, posx, posy + tsize, type, tsize);
				*down = b;
				return b;
			}
		}
		else {
			if (posy%tsize == 0)
			{//estamos justo en una altura. aqui solo puede haber colision lateral, mirar solo a la derecha

				bool b;
				//b = TileColisionSferic(Scene, posx, posy, posx, posy + tsize, type, tsize);
				b = TileColisionCubic(Scene, posx + tsize, posy, type, tsize); //miramos justo a la derecha.
				*right = b;
				return b;
			}
			else
			{ //la chunga, no coincide na de na, estamos en medio de 4 tiles. puede haber colision lateral o colision por abajo (un misterio)

				//Calculamos la distancia de nuestro punto a las 4 tiles que estamos tocando, toda distancia inferior al radio*2 es un posible impacto

				//que 4 posiciones ocupo?
				//calculamos los 4 puntos de la caja del player.
				//  x,y------x1,y1
				//  |          |  
				//  |          |  
				//  |          |  
				//x3,y3------x2,y2

				int x, y, x1, y1, x2, y2, x3, y3;

				x = posx;
				y = posy;
				x1 = posx + tsize;
				y1 = posy;
				x2 = posx + tsize;
				y2 = posy + tsize;
				x3 = posx;
				y3 = posy + tsize;

				bool b, b1, b2, b3;
				int tipo1, tipo2, tipo3;

				b = TileColisionSferic(Scene, posx, posy, x, y, type, tsize); // colision con la tile de arriba a la izquierda, no la vamos a mirar no nos interesa.	
				b1 = TileColisionSferic(Scene, posx, posy, x1, y1, &tipo1, tsize); // colision con la tile de arriba a la derecha, la miramos porque podemos tener una colision lateral aqui
				if (b1) { *right = true; *type = tipo1; }
				b2 = TileColisionSferic(Scene, posx, posy, x2, y2, &tipo2, tsize); // colision con la tile de abajo a la derecha, la miramos proque podemos tener una colision abajo
				if (b2) { *down = true; *type = tipo2; }
				b3 = TileColisionSferic(Scene, posx, posy, x3, y3, &tipo3, tsize);	//  colision con la tile de abajo a la izquierda, improbable pero puede dar una colision en down
				if (b3) { *down = true; *type = tipo3; }

				//ok, si ahora tenemos varias colisiones, hay que ordenarlas por prioridad:
				//si down la colision de cubo es prioritaria a la colision de pincho, ya que no debes morir

				if (down)
				{
					if ((tipo2 == CUBO) || (tipo3 == CUBO)) { *type = CUBO; }
				}

				return b || b1 || b2 || b3;

			}
		}
	}
	return false; //no hay bounding box
}


bool cFisicas::Is_Grounded(cPlayer *Player, cScene *Scene)
{
	int posx, posy, tsize;

	Player->GetGlobalPosition(&posx, &posy);
	Player->GetTileSize(&tsize);
	//un objeto toca suelo cuando el objeto que tiene justo debajo es del tipo cubo o es el suelo.

	if (posy >= SCENE_GROUND * tsize) { return true; } //esta en el suelo;

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
			//objeto = Scene->map[tx][ty + 1];
			objeto = Scene->map[ty + 1][tx];
			if (objeto == 1 || objeto == 4)//suelo o cubo
			{
				return true;
			}
			return false;
		}
		else //estamos en medio de 2 hay que mirar las 2 y es la suma de las 2.
		{
			int objeto;
			objeto = Scene->map[ty + 1][tx];
			int objeto1;
			objeto1 = Scene->map[ty + 1][tx + 1];

			if ((objeto == CUBO || objeto == SUELO) || (objeto1 == CUBO || objeto1 == SUELO))//suelo o cubo
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