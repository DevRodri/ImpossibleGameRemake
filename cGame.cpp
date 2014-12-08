
#include "cGame.h"
#include "cLog.h"


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
	deltaTime = GetTickCount();

	//Carga ficheros graficos
	Graphics.LoadData();

	//Carga mapa lógico
	Scene.LoadMap("map.txt");

	//Inicializa Gravedad
	Physics.SetGravity(0.01f);

	//Inicializa posicion del jugador
	Player.SetLocalPosition(0,0);
	Player.SetGlobalPosition(&Player, &Scene);


	return true;
}

//Bucle de juego
bool cGame::Loop()
{
	bool res;
	
	//Input
	res = ManageInputs();
	if (!res) return false;

	//Process
	res = ManagePhysics();
	if (!res) return false;

	res = ManageLogic();
	if (!res) return false;

	//Output
	res = ManageGraphics();
	if (!res) return false;
	
	deltaTime = GetTickCount() - deltaTime;

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

	//aqui se aplica la gravedad a cada iteración y todo lo relacionado con fisicas.
	res = Physics.ApplyGravity(&Player, deltaTime); //aqui hay que poner el deltatime.

	return res;
}

bool cGame::ManageLogic()
{
	cMouse *Mouse;
	Mouse = Input.GetMouse();

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
		break;
	}

	return true;
}

bool cGame::ManageGraphics()
{
	bool res;

	res = Graphics.Render(Input.GetMouse(), &Scene, state);

	return res;
}

void cGame::Finalize()
{
	Graphics.UnLoadData();
	Graphics.Finalize();
	Input.UnacquireAll();
	Input.Finalize();
}
