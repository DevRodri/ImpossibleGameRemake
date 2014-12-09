
#include "cGame.h"
#include "cLog.h"
#include "cKeyboard.h"


cGame::cGame() {}
cGame::~cGame(){}

bool cGame::Init(HWND hWnd,HINSTANCE hInst,bool exclusive)
{
	bool res;
	cLog *Log = cLog::Instance();

	res = Graphics.Init(hWnd,exclusive);
	if(!res)
	{
		Log->Msg("Error initializing Graphics!");
		return false;
	}

	res = Input.Init(hInst,hWnd,exclusive,USE_MOUSE|USE_KEYBOARD);
	if(!res)
	{
		Log->Msg("Error initializing Input!");
		return false;
	}
	Input.SetMousePosition(SCREEN_RES_X >> 1,SCREEN_RES_Y >> 1);

	//inicializamos deltatime
	//deltaTime = GetTickCount();

	//Carga ficheros graficos
	Graphics.LoadData();

	//Carga mapa lógico
	Scene.LoadMap("map.txt");

	//Inicializa Gravedad
	Physics.SetGravity(0.8f);

	//Inicializa posicion del jugador
	Player.SetTileSize(32);
	//Player.SetLocalPosition(5,32);
	//Player.SetGlobalPosition(5 * 32, SCENE_GROUND * 32);
	Player.SetLocalPosition(5,7);
	Player.SetGlobalPosition(5 * 32, 7 * 32);


	return true;
}

//Bucle de juego
bool cGame::Loop()
{
	bool res;
	deltaTime = 1;
	//Input
	res = ManageInputs();
	//if (!res) return false;

	//Process
	res = ManagePhysics();
	if (!res) return false;

	res = ManageLogic();
	if (!res) return false;

	//Output
	res = ManageGraphics();
	if (!res) return false;

	//deltaTime = GetTickCount() - deltaTime;

	return true;
}

bool cGame::ManageInputs()
{
	bool res;
	cLog *Log = cLog::Instance();
	cMouse *Mouse;

	res = Input.Read();
	if (!res)
	{
		Log->Msg("Error reading Input!");
		return false;
	}
	Mouse = Input.GetMouse();
}

bool cGame::ManagePhysics()
{
	bool res; //cuando se añada la clase de fisicas el "= true" va fuera.

	if (state == STATE_GAME){ // si estamos jugando aplicamos las fisicas

		//si el player no toca suelo aplicarle la gravedad, si toca suelo reseteamos la velocidad
		res = Physics.ApplyGravity(&Player, &Scene, deltaTime);
		
		if (Physics.Is_Grounded(&Player, &Scene))
			{ 
			Player.SetVely(0);
			}

		// si el player entre en colision matarlo
		int colision; //si hay colision indica con que ha colisionado 0,1,2,3,4....
		if (Physics.Is_Incollision(&Player, &Scene, &colision))
		{
			//matamos al jugador
			//incrementar el contador de intentos
			//reiniciar el nivel
			if (colision==4){res = true;} // es el suelo y no cuenta.
			res = true;
		}
		//aqui se aplica la gravedad a cada iteración y todo lo relacionado con fisicas.
		//res = Physics.ApplyGravity(&Player,&Scene, deltaTime); //aqui hay que poner el deltatime.

		return true;
	}
	else return true;
}

bool cGame::ManageLogic()
{
	cMouse *Mouse;
	Mouse = Input.GetMouse();

	cKeyboard *Keyboard;
	Keyboard = Input.GetKeyboard();

	switch (state)
	{
	case STATE_MAIN:
		if (Mouse->ButtonDown(LEFT))
		{
			//Play button
			if (Mouse->In(256, 315, 430, 350))
			{
				state = STATE_GAME;
			}
			//Exit button
			else if (Mouse->In(255, 395, 410, 430))
			{
				return false;
			}
		}
		break;

	case STATE_GAME: 

		if (Keyboard->KeyDown(DIK_ESCAPE)){
			state = STATE_MAIN;
		}
		ProcessOrder();
		break;
	}

	return true;
}

void cGame::ProcessOrder()
{
	cMouse *Mouse;
	Mouse = Input.GetMouse();

	cKeyboard *Keyboard;

	Keyboard = Input.GetKeyboard();

	if(Keyboard->KeyDown(1)){
		//bla
	}

	if (Mouse->ButtonDown(LEFT))
	{
		if (Physics.Is_Grounded(&Player, &Scene)) 
		{
			Player.SetVely(-10.0f);
		}
	}
}

bool cGame::ManageGraphics()
{
	bool res;

	res = Graphics.Render(Input.GetMouse(), &Scene, state, &Player);

	return res;
}

void cGame::Finalize()
{
	Graphics.UnLoadData();
	Graphics.Finalize();
	Input.UnacquireAll();
	Input.Finalize();
}
