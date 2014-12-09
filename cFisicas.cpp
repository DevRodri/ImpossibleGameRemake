#include "cFisicas.h"


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

bool cFisicas::ApplyGravity(cPlayer *Player, cScene *Scene, float dt)
{
	float velocity;
	int x, y, type,tsize,posfy, posfx;
	bool b;

	//velocidad de la escena (movimiento de la misma). Falta definir bien los atributos y no hacerlo tan chapuza.
	Scene->x -= Scene->velocity * dt;


	Player->GetVely(&velocity);
	velocity = velocity + Gravity * dt;
	Player->SetVely(velocity);
	
	Player->GetTileSize(&tsize);
	Player->GetGlobalPosition(&x, &y);
	
	//aqui vamos a situar el player en su posición actual y le sumamos la caida de la gravedad
	posfy = y + velocity*dt;

	//nueva posicion del player en funcion de la velocidad de la escena
	posfx = x + Scene->velocity/4 * dt; // si no divido por 4 va más rapido y no se porque.
	
	// prueba de movimiento de player

	//int posfx;	
	//if (velocity >0) posfx = x + velocity*dt;
	//else posfx = x - velocity*dt;
	//x=posfx;
	//Player->SetGlobalPosition(x, posfy);
	//
	

	// pero si esta suma situa a la pieza encima de una base hay que ponerle la altura de la base
	//es decir, si la velocidad es de bajada (positiva) y colisiona con un objeto lo ponemos a la altura del objeto.
	b = Is_Incollision(Player,Scene, &type);
	
	if (b && velocity>0)
	{
		//hemos colisionado, si la colision es con un cubo de suelo actualizamos la vel a 0 y reposicionamos el player encima del cubo
		if (type == 1)
			{
				
				Player->SetVely(0);
				
				div_t divresult;
				divresult = div(posfy, tsize);
				posfy = divresult.quot * tsize;

			}
		if (type == 4)
			{
				
				Player->SetVely(0);
				
				div_t divresult;
				divresult = div(posfy, tsize);
				posfy = divresult.quot * tsize;

			}
	}

	Player->SetGlobalPosition(posfx, posfy);
	//controlamos que no caiga mas alla del suelo hay 32 niveles de altura, si esta en el 33 es que esta ya fuera del suelo
	
	if (posfy > (tsize * SCENE_GROUND)) { 
			//Player->SetGlobalPosition(x, SCENE_GROUND % tsize);  
			posfy = SCENE_GROUND*tsize;
			Player->SetGlobalPosition(x, posfy);
			Player->SetVely(0);
	}
	
	//* if temporal */
	/*if (posfy > 224)
		{ 
			posfy = 224;
			Player->SetGlobalPosition(x, posfy);
			Player->SetVely(0);
		}*/
	///////
	return true;
}

bool cFisicas::TileColision(cScene *Scene,int posx,int posy,int *type)
{

	*type = Scene->map[posy][posx];

	//0-vacia no hay colision
	if ((*type) == 0){ return false; }
	
	//1-cubo,si el cubo esta a la derecha hay que colisionar, si esta debajo no????? este caso hay que mirarlo.
	if ((*type) == 1){
		return true; 
	}
	//2-pincho siempre hay colision, este caso se puede afinar si vemos que colisiona demasiado pronto
	if ((*type) == 2){ return true; }
	
	//3-suelo que mata hay colision
	if ((*type) == 3){ return true; }
	
	//4-suelo normal no hay colision, ponemos que si de momento.
	if ((*type) == 4){ return true; }
	else  { return false; }
}

bool cFisicas::Is_Incollision(cPlayer *Player, cScene *Scene, int *type)
{
	int posx, posy,tsize;
	//obtener las coordenadas del jugador en el mundo

	Player->GetGlobalPosition(&posx, &posy);
	Player->GetTileSize(&tsize);

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

		b = TileColision(Scene, tx, ty, type);

		if (b) return true;

		divresult = div(x1, tsize);
		tx1 = divresult.quot;
		divresult = div(y1, tsize);
		ty1 = divresult.quot;

		b1 = TileColision(Scene, tx1, ty1, type);
		if (b1) return true;

		divresult = div(x2, tsize);
		tx2 = divresult.quot;
		divresult = div(y2, tsize);
		ty2 = divresult.quot;

		b2 = TileColision(Scene, tx2, ty2, type);
		if (b2) return true;

		divresult = div(x3, tsize);
		tx3 = divresult.quot;
		divresult = div(y3, tsize);
		ty3 = divresult.quot;

		b3 = TileColision(Scene, tx3, ty3, type);
		if (b3) return true;

		return b||b1||b2||b3; // o false que es lo mismo

	}
}


bool cFisicas::Is_Grounded(cPlayer *Player, cScene *Scene)
{
	int posx, posy,tsize;
	
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
			if (objeto==1 || objeto==4)//suelo o cubo
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