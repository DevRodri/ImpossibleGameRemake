
#include "cGame.h"
#include "cLog.h"
#include "cKeyboard.h"
#include "MP3.h"


cGame::cGame() {}
cGame::~cGame(){}

bool cGame::Init(HWND hWnd, HINSTANCE hInst, bool exclusive)
{
	
	Mp3Init();
	Mp3Load("main.mp3");
	bool res;
	cLog *Log = cLog::Instance();

	res = Graphics.Init(hWnd, exclusive);
	if (!res)
	{
		Log->Msg("Error initializing Graphics!");
		return false;
	}

	res = Input.Init(hInst, hWnd, exclusive, USE_MOUSE | USE_KEYBOARD);
	if (!res)
	{
		Log->Msg("Error initializing Input!");
		return false;
	}
	Input.SetMousePosition(SCREEN_RES_X >> 1, SCREEN_RES_Y >> 1);

	//inicializamos deltatime
	//deltaTime = GetTickCount();

	//Carga ficheros graficos
	Graphics.LoadData();

	//Carga mapa lógico
	Scene.LoadMap("map.txt");
	Scene.SetVelocity(7.0f);

	//Inicializa Gravedad
	Physics.SetGravity(1.0f);
	fsalt = -10.1f;
	//Inicializa posicion del jugador
	Player.SetTileSize(32);
	//Player.SetLocalPosition(5,32);
	//Player.SetGlobalPosition(5 * 32, SCENE_GROUND * 32);
	Player.SetLocalPosition(5 * 32, (29 - HEIGHT_MAX_TILES + 4) * 32);
	Scene.SetLastPlayerLY( (29 - HEIGHT_MAX_TILES + 4) * 32 );
	Player.SetGlobalPosition(5 * 32, 29 * 32);
	psx = 5 * 32;
	psy = 29 * 32;
	ssx = 0;
	ssy = (29 - HEIGHT_MAX_TILES + 4) * 32;
	coff = 0;
	
	Interface.InitScore();

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


	lastCounts = actCounts;

	do {
		actCounts = GetTickCount();
	} while (actCounts - lastCounts < 100 / 10);

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
		//si la distancia entre la X global y el final del mapa es menor que WIDTH_MAX_TILES, no muevo mas el mapa.

		Physics.MoveScene(&Player, &Scene);
		res = Physics.ApplyGravity(&Player, &Scene, deltaTime);

		if (Physics.Is_Grounded(&Player, &Scene))
		{
			Player.SetVely(0);
		}

		// si el player entre en colision matarlo
		int colision; //si hay colision indica con que ha colisionado 0,1,2,3,4....
		bool down, right;
		if (Physics.Is_Incollision(&Player, &Scene, &colision, ESFERICO, &down, &right))
		{
			//matamos al jugador
			//incrementar el contador de intentos
			//reiniciar el nivel
			
			if (colision == PINCHO || colision == CUBO || colision == AGUJERO){ 
				
				Mp3Stop();
				Mp3Load("death.mp3");
				Mp3Play();
				state = STATE_DEATH; 
			
			}
			if (colision == SUELO){ 
				res = true;
			} // es el suelo y no cuenta.
			
			if (colision == FINAL){
				state = STATE_END;
			} // final

		
			res = true;
		}

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

		Mp3Play();
		if (Mouse->ButtonDown(LEFT))
		{
			//Play button
			if (Mouse->In(417, 293, 700, 393))
			{

				Mp3Stop();
				Mp3Load("button1.mp3");
				Mp3Play();
				Sleep(250);
				state = STATE_GAME;
				Mp3Stop();
				Mp3Load("level3.mp3");
				Mp3Play();
			}
			//Exit button
			else if (Mouse->In(465, 448, 669, 525))
			{
				Mp3Stop();
				Mp3Load("button1.mp3");
				Mp3Play();
				Sleep(300);
				return false;
			}
		}
		break;

	case STATE_GAME:

		if (Keyboard->KeyDown(DIK_ESCAPE)){
			Mp3Stop();
			Mp3Load("main.mp3");
			ResetLevel();
			state = STATE_MAIN;
		}

		//AQUI MENEO LA CAMARA EN FUNCION DE LA POSICION DEL PERSONAJE
		//if (Physics.Is_Grounded(&Player, &Scene))
		//{
		int lx, ly,
			lply, offsetYCam;

			Player.GetLocalPosition(&lx, &ly);
			Scene.GetLastPlayerLY(&lply);
			Scene.GetOffsetYCamera(&offsetYCam);

			if (ly < CAMERA_MAX_Y) // hay que aumentar offset de camara
			{
				//Scene.SetLastPlayerLY(ly);
				Scene.SetOffsetYCamera(offsetYCam + 16);
				Player.SetLocalPosition(lx, ly + 16);
			}
			else if (ly > CAMERA_MIN_Y)
			{
				//Scene.SetLastPlayerLY(ly);
				Scene.SetOffsetYCamera(offsetYCam - 16);
				Player.SetLocalPosition(lx, ly - 16);
			}
		//}
		Scene.Changebackground(&Player);
		ProcessOrder();
		break;

	case STATE_DEATH:

		//Scene.SetVelocity(0.0f);

		Player.PlayDieAnimation();
		if (Player.IsDeath()){
			Interface.SumScore();
			state = STATE_GAME;
			//si hay punto de guardado lo restauramos , sino reseteamos el level;
			if (Scene.ck.HayCheckPoint()) ResetSaveLevel();
			else {
				Mp3Load("level3.mp3");
				Mp3Play();
				ResetLevel();

			}
		}
		break;
	case STATE_END:
		Scene.PlayEnd();
		
		if (Scene.FinishEndAnimation()){
			state = STATE_MAIN;
			Scene.end = false;
			ResetLevel();
		}
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

	//Salto con teclado
	if (Keyboard->KeyDown(DIK_SPACE)){
		if (Physics.Is_Grounded(&Player, &Scene))
		{
			Player.SetVely(fsalt);
			//segun el juego original el salto de largo es de 4 espacios
			//|X|_|_|_|_|X|
			// de alto salta exactamente 1.5 de su altura
			//|_|_|_|x|_|_|_|
			//| | |X|X|X| | |
			//|_|X|X|_|X|X|_|
			//|X|X| | | |X|X|
			//|X|_|_|_|_|_|X|
			//

		}
		
	}
	if (Keyboard->KeyDown(DIK_RIGHT))
	{
		Physics.MoveScene(&Player, &Scene);
	}

	//Salto con raton
	if (Mouse->ButtonDown(LEFT))
	{
		if (Physics.Is_Grounded(&Player, &Scene))
		{
			Player.SetVely(fsalt);
		}
	}
	if (Keyboard->KeyDown(DIK_C)){
		double tcancion;
		tcancion=GetPosition();
		Scene.SaveCheckPoint(&Player,tcancion);	
	}
}

bool cGame::ManageGraphics()
{
	bool res;

	res = Graphics.Render(Input.GetMouse(), &Scene, state, &Player, &Interface);

	return res;
}

void cGame::ResetLevel()
{
	Player.SetLocalPosition(5 * 32, (29 - HEIGHT_MAX_TILES + 4) * 32);
	Player.SetGlobalPosition(5 * 32, 29 * 32);
	Player.ResetDieAnimation();

	Scene.SetGlobalPosition(0,(29 - HEIGHT_MAX_TILES + 5) * 32);
	Scene.SetOffsetYCamera(0);
	Scene.ck.checkpoint=false;

}
void cGame::ResetSaveLevel()
{
	Mp3Load("level3.mp3");
	SetPosition(Scene.ck.tiempocancion);
	Mp3Play();
	Player.ResetDieAnimation();
	Scene.RestoreCheckPoint(&Player);

}

void cGame::Finalize()
{
	Graphics.UnLoadData();
	Graphics.Finalize();
	Input.UnacquireAll();
	Input.Finalize();
}
