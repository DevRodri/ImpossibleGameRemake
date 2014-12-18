#include "cGraphicsLayer.h"
#include "cGame.h"
#include "cLog.h"
#include <stdio.h>



cGraphicsLayer::cGraphicsLayer()
{
	g_pD3D = NULL;
	g_pD3DDevice = NULL;
	g_pSprite = NULL;
}

cGraphicsLayer::~cGraphicsLayer(){}

bool cGraphicsLayer::Init(HWND hWnd, bool exclusive)
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	//D3DVIEWPORT9 viewPort = { 0, 0, SCREEN_RES_X, SCREEN_RES_Y, 0.0f, 1.0f };

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		Log->Msg("Error creating Direct3D object");
		return false;
	}

	//Get device capabilities
	ZeroMemory(&d3dCaps, sizeof(d3dCaps));
	if (FAILED(g_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
		return FALSE;


	ZeroMemory(&d3dpp, sizeof(d3dpp));

	//Setup present parameters
	d3dpp.Windowed = !exclusive;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //Efficient page flipping


	///////////////////////////////////////////////////////////////////////////
	LONG resWidth, resHeight;
	if (exclusive)//fullscreen
	{
		d3dpp.Windowed = false;
		d3dpp.BackBufferWidth = SCREEN_RES_X;
		d3dpp.BackBufferHeight = SCREEN_RES_Y;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		resWidth = SCREEN_RES_X;
		resHeight = SCREEN_RES_Y;
	}
	else //no full screen
	{
		D3DDISPLAYMODE d3ddm;
		RECT rWindow;

		//Get display mode
		g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		//Get window bounds
		GetClientRect(hWnd, &rWindow);

		//Setup screen dimensions
		resWidth = rWindow.right - rWindow.left;
		resHeight = rWindow.bottom - rWindow.top;

		//Setup backbuffer
		d3dpp.Windowed = true;
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.BackBufferWidth = rWindow.right - rWindow.left;
		d3dpp.BackBufferHeight = rWindow.bottom - rWindow.top;
	}

	//Check if hardware vertex processing is available
	//if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
//	{
		//Create device with hardware vertex processing
	//	hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
	//		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
	//}
	//else
	//{
		//Create device with software vertex processing
		hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
	//}

	if (FAILED(hr))
	{
		Log->Error(hr, "Creating Direct3D device");
		return false;
	}

	//Setup Direct3D for rendering
	InitRendering(resWidth, resHeight);

	return true;
}
//Setup the quad
void cGraphicsLayer::SetupQuad()
{
	TLVERTEX* vertices = NULL;
	vertexBuffer->Lock(0, 4 * sizeof(TLVERTEX), (VOID**)&vertices, 0);

	//Setup vertices
	vertices[0].colour = 0xffffffff;
	vertices[0].x = 0.0f;
	vertices[0].y = 0.0f;
	vertices[0].z = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].colour = 0xffffffff;
	vertices[1].x = 1.0f;
	vertices[1].y = 0.0f;
	vertices[1].z = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].colour = 0xffffffff;
	vertices[2].x = 1.0f;
	vertices[2].y = -1.0f;
	vertices[2].z = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3].colour = 0xffffffff;
	vertices[3].x = 0.0f;
	vertices[3].y = -1.0f;
	vertices[3].z = 1.0f;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

	vertexBuffer->Unlock();
}
//Fill the index buffer
void cGraphicsLayer::FillIndexBuffer()
{
	int index = 0;
	short* indices = NULL;

	//Lock index buffer
	indexBatchBuffer->Lock(0, BATCH_BUFFER_SIZE * 3, (void**)&indices, 0);

	for (int vertex = 0; vertex < BATCH_BUFFER_SIZE; vertex += 4)
	{
		indices[index] = vertex;
		indices[index + 1] = vertex + 2;
		indices[index + 2] = vertex + 3;
		indices[index + 3] = vertex;
		indices[index + 4] = vertex + 1;
		indices[index + 5] = vertex + 2;
		index += 6;
	}

	//Unlock index buffer
	indexBatchBuffer->Unlock();
}
void cGraphicsLayer::InitRendering(int resWidth, int resHeight)
{
	D3DXMATRIX matOrtho;
	D3DXMATRIX matIdentity;

	//Setup an orthographic perspective
	D3DXMatrixOrthoLH(&matOrtho, (float)resWidth, (float)resHeight, 1.0f, 10.0f);
	D3DXMatrixIdentity(&matIdentity);

	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matIdentity);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	//Create a vertex buffer and set stream source
	g_pD3DDevice->CreateVertexBuffer(4 * sizeof(TLVERTEX), D3DUSAGE_WRITEONLY,
		D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &vertexBuffer, NULL);
	g_pD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));

	//Create batching vertex and index buffers
	g_pD3DDevice->CreateVertexBuffer(BATCH_BUFFER_SIZE * sizeof(TLVERTEX), D3DUSAGE_WRITEONLY,
		D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &vertexBatchBuffer, NULL);
	g_pD3DDevice->CreateIndexBuffer(BATCH_BUFFER_SIZE * 3, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &indexBatchBuffer, NULL);
	g_pD3DDevice->SetIndices(indexBatchBuffer);
	numBatchVertices = 0;

	//Fill the index buffer
	FillIndexBuffer();

	//Setup vertex format
	g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetFVF(D3DFVF_TLVERTEX);

	//Setup the quad
	SetupQuad();

	//Set render states
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	alpha1 = 0; alpha2 = 0; alpha3 = 0;
}

bool cGraphicsLayer::Finalize()
{
	//Clear stream source
	g_pD3DDevice->SetStreamSource(0, NULL, 0, 0);

	//Release vertex buffer
	if (vertexBuffer)
		vertexBuffer->Release();
	//Release batching buffers
	if (vertexBatchBuffer)
		vertexBatchBuffer->Release();
	if (indexBatchBuffer)
		indexBatchBuffer->Release();

	//Successfully closed Direct3D
	return TRUE;
	if (g_pD3DDevice)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

IDirect3DTexture9 *cGraphicsLayer::LoadTexture(char *fileName, D3DCOLOR colorkey)
{
	IDirect3DTexture9 *d3dTexture;
	D3DXIMAGE_INFO SrcInfo;         //Optional

	// Load image from file
	if (FAILED(D3DXCreateTextureFromFileEx(g_pD3DDevice, fileName, 0, 0, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		colorkey, &SrcInfo, NULL, &d3dTexture)))
	{
		return NULL;
	}

	//Return the newly made texture
	return d3dTexture;
}

void cGraphicsLayer::LoadData()
{
	D3DCOLOR colorkey = 0xFFFF00FF;
	D3DXCreateSprite(g_pD3DDevice, &g_pSprite);
	//Main menu
	texMain = LoadTexture("main.png", NULL);
	//GUI game
	texEnd = LoadTexture("end.png", NULL);
	//GUI game
	texGame3 = LoadTexture("game3.png", NULL);
	//GUI game
	texGame2 = LoadTexture("game2.png", NULL);
	//GUI game
	texGame = LoadTexture("game.png", NULL);
	//Tiles
	texTiles = LoadTexture("tiles.png", 0x00ff00ff);
	//Characters
	texCharacters = LoadTexture("characters.png", 0x00ff00ff);
	//Mouse pointers
	texMouse = LoadTexture("mouse.png", 0x00ff00ff);
	//Textura animación muerte
	texDie = LoadTexture("dieAnim.png", 0x00ff00ff);
	//Score
	texNumbers = LoadTexture("numbers.png", 0x00ff00ff);
}

void cGraphicsLayer::UnLoadData()
{
	if (texMain)
	{
		texMain->Release();
		texMain = NULL;
	}
	if (texGame)
	{
		texGame->Release();
		texGame = NULL;
	}
	if (texGame2)
	{
		texGame2->Release();
		texGame2 = NULL;
	}
	if (texGame3)
	{
		texGame3->Release();
		texGame3 = NULL;
	}
	if (texTiles)
	{
		texTiles->Release();
		texTiles = NULL;
	}
	if (texCharacters)
	{
		texCharacters->Release();
		texCharacters = NULL;
	}
	if (texMouse)
	{
		texMouse->Release();
		texMouse = NULL;
	}
	if (texDie)
	{
		texDie->Release();
		texMouse = NULL;
	}
	if (texNumbers)
	{
		texNumbers->Release();
		texNumbers = NULL;
	}
	if (texEnd)
	{
		texEnd->Release();
		texEnd = NULL;
	}
}

//Clear backbuffer
int cGraphicsLayer::ClearBuffer() { return g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, 255, 0.0f, 0); }

//Present
int cGraphicsLayer::Present() { return g_pD3DDevice->Present(NULL, NULL, NULL, NULL); }

//Begin drawing
int cGraphicsLayer::BeginDrawing() { return g_pD3DDevice->BeginScene(); }

//End drawing
int cGraphicsLayer::EndDrawing() { return g_pD3DDevice->EndScene(); }

//Get ready for batch drawing
void cGraphicsLayer::BeginBatchDrawing(IDirect3DTexture9* texture, float rotate)
{	
	D3DXMATRIX matTransform;
	D3DXMATRIX matIdentity;
	D3DSURFACE_DESC surfDesc;

	//Lock the batching vertex buffer
	numBatchVertices = 0;
	vertexBatchBuffer->Lock(0, BATCH_BUFFER_SIZE * sizeof(TLVERTEX), (void **)&batchVertices, 0);

	//Get texture dimensions
	texture->GetLevelDesc(0, &surfDesc);
	batchTexWidth = (float)surfDesc.Width;
	batchTexHeight = (float)surfDesc.Height;

	//Set texture
	g_pD3DDevice->SetTexture(0, texture);

	//Set world matrix to an identity matrix
	D3DXMatrixIdentity(&matIdentity);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	
	//Set stream source to batch buffer
	g_pD3DDevice->SetStreamSource(0, vertexBatchBuffer, 0, sizeof(TLVERTEX));
}


//Add a quad to the batching buffer
void cGraphicsLayer::AddQuad(RECT rSource, RECT rDest, D3DCOLOR colour)
{
	float X;
	float Y;
	float destWidth;
	float destHeight;

	//Calculate coordinates
	X = rDest.left - (float)(d3dpp.BackBufferWidth) / 2;
	Y = -rDest.top + (float)(d3dpp.BackBufferHeight) / 2;
	destWidth = (float)(rDest.right - rDest.left);
	destHeight = (float)(rDest.bottom - rDest.top);

	//Setup vertices in buffer
	batchVertices[numBatchVertices].colour = colour;
	batchVertices[numBatchVertices].x = X;
	batchVertices[numBatchVertices].y = Y;
	batchVertices[numBatchVertices].z = 1.0f;
	batchVertices[numBatchVertices].u = rSource.left / batchTexWidth;
	batchVertices[numBatchVertices].v = rSource.top / batchTexHeight;

	batchVertices[numBatchVertices + 1].colour = colour;
	batchVertices[numBatchVertices + 1].x = X + destWidth;
	batchVertices[numBatchVertices + 1].y = Y;
	batchVertices[numBatchVertices + 1].z = 1.0f;
	batchVertices[numBatchVertices + 1].u = rSource.right / batchTexWidth;
	batchVertices[numBatchVertices + 1].v = rSource.top / batchTexHeight;

	batchVertices[numBatchVertices + 2].colour = colour;
	batchVertices[numBatchVertices + 2].x = X + destWidth;
	batchVertices[numBatchVertices + 2].y = Y - destHeight;
	batchVertices[numBatchVertices + 2].z = 1.0f;
	batchVertices[numBatchVertices + 2].u = rSource.right / batchTexWidth;
	batchVertices[numBatchVertices + 2].v = rSource.bottom / batchTexHeight;

	batchVertices[numBatchVertices + 3].colour = colour;
	batchVertices[numBatchVertices + 3].x = X;
	batchVertices[numBatchVertices + 3].y = Y - destHeight;
	batchVertices[numBatchVertices + 3].z = 1.0f;
	batchVertices[numBatchVertices + 3].u = rSource.left / batchTexWidth;
	batchVertices[numBatchVertices + 3].v = rSource.bottom / batchTexHeight;

	//Increase vertex count
	numBatchVertices += 4;

	//Flush buffer if it's full
	if (numBatchVertices == BATCH_BUFFER_SIZE)
	{
		//Unlock vertex buffer
		vertexBatchBuffer->Unlock();

		//Draw quads in the buffer
		g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numBatchVertices, 0,
			numBatchVertices / 2);

		//Reset vertex count        
		numBatchVertices = 0;

		//Lock vertex buffer
		vertexBatchBuffer->Lock(0, BATCH_BUFFER_SIZE * sizeof(TLVERTEX), (void **)
			&batchVertices, 0);
	}

}


//Finish batch drawing
void cGraphicsLayer::EndBatchDrawing()
{
	//Unlock vertex buffer
	vertexBatchBuffer->Unlock();

	//Draw the quads in the buffer if it wasn't just flushed
	if (numBatchVertices)
		g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numBatchVertices, 0,
		numBatchVertices / 2);

	//Set stream source to regular buffer
	g_pD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));

	//Reset vertex count        
	numBatchVertices = 0;
}

bool cGraphicsLayer::Render(cMouse *Mouse, cScene *Scene, int state, cPlayer *Player, cInterface *Interface)
{

	RECT rc_o;
	RECT rc_d;
	ClearBuffer();
	BeginDrawing();

	switch (state)
	{
	case STATE_MAIN:

		//pintar pantalla principal
		BeginBatchDrawing(texMain,0.0f);
		//rectangulo de toda la pantalla
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		//pintamos.
		EndBatchDrawing();

		PintaRaton(Mouse);

		break;

	case STATE_GAME:
		PintaFondo(Scene);
		PintaEscena(Scene);	
		PintaPlayer(Scene, Player);
		PintaScore(Scene, Interface);
		PintaCheckPoint(Scene,Player);
		break;

	case STATE_DEATH:

		//para pintar el fondo con QUADS
		/*BeginBatchDrawing(texGame, 0.0f);
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		EndBatchDrawing();*/
		PintaFondo(Scene);
		//pintar escena
		PintaEscena(Scene);
		//
		PintaScore(Scene, Interface);
		//pintar animacion de muerte
		PintaMuerte(Player);
		PintaCheckPoint(Scene,Player);
		break;
	case STATE_END:
		
		PintaFondo(Scene);
		PintaEscena(Scene);
		PintaPlayer(Scene, Player);
		PintaScore(Scene, Interface);
		PintaCheckPoint(Scene, Player);
		PintaFinal(Scene);
		break;
	}

	EndDrawing();
	Present();

	return true;
}

bool cGraphicsLayer::PintaMuerte(cPlayer *Player)
{
	int px, py;
	int seq;
	Player->GetDieAnimationSeq(&seq);

	RECT rc_o;
	RECT rc_d;

	Player->GetLocalPosition(&px, &py);
	BeginBatchDrawing(texDie, 0.0f); //texturaMuerte
	SetRect(&rc_o, seq * 64, 0, 64 + seq * 64, 64);
	SetRect(&rc_d, px - 48, py - 48, px + 96, py + 96);
	AddQuad(rc_o, rc_d, 0xFFFFFFFF);
	EndBatchDrawing();

	return true;
}

bool cGraphicsLayer::PintaRaton(cMouse *Mouse)
{
	RECT rc;
	int mx, my, posx, posy;

	//Mouse selection box
	Mouse->GetPosition(&mx, &my);

	//Mouse
	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	Mouse->GetRect(&rc, &posx, &posy);
	HRESULT hr = g_pSprite->Draw(texMouse, &rc, NULL, &D3DXVECTOR3(float(mx + posx), float(my + posy), 0.0f), 0xFFFFFFFF);
	if (FAILED(hr))
	{
		cLog *Log = cLog::Instance();
		Log->Error(hr, "mouse pointer");
		return false;
	}

	g_pSprite->End();

	return true;
}

bool cGraphicsLayer::PintaEscena(cScene *Scene)
{
	RECT rc_o;
	RECT rc_d;

	int n,
		fx, fy,
		pantx, panty,
		offy_cam;

	//Celda inicial a pintar
	int scenegx, scenegy;
	int cellx;
	int celly;
	Scene->GetGlobalPosition(&scenegx, &scenegy);
	Scene->GetOffsetYCamera(&offy_cam);

	cellx = scenegx / 32;
	//celly = scenegy / 32;
	celly = (scenegy - offy_cam) / 32;

	//Celda final a pintar
	fx = cellx + WIDTH_MAX_TILES + 1;
	fy = celly + HEIGHT_MAX_TILES + 1;

	for (int y = celly; y < fy; y++)
	{
		panty = (y - scenegy) + 31 * y  + offy_cam;
		for (int x = cellx; x < fx; x++)
		{

			pantx = (x - scenegx) + 31 * x;
			Scene->GetMapPosition(&n, y, x);

  			if (y >= 0) {
				int temp_i;
				temp_i = fx - x;
				if ((temp_i <= 2) || (temp_i >= 26)) { AplicaAlpha(0); }
				if ((temp_i == 3) || (temp_i == 25)) { AplicaAlpha(50); }
				if ((temp_i == 4) || (temp_i == 24)) { AplicaAlpha(100); }
				if ((temp_i == 5) || (temp_i == 23)) { AplicaAlpha(150); }
				if ((temp_i == 6) || (temp_i == 22)) { AplicaAlpha(200); }

				BeginBatchDrawing(texTiles, 0.0f);
				SetRect(&rc_o, n << 5, 0, (n + 1) << 5, 32);
				SetRect(&rc_d, pantx, panty, pantx + 32, panty + 32);
				AddQuad(rc_o, rc_d, 0xFFFFFFFF);
				EndBatchDrawing();
 
				AplicaAlpha(255);
			}
		}
	}
	return true;
}

bool cGraphicsLayer::PintaPlayer(cScene *Scene, cPlayer *Player)
{
	int gx,gy,px, py, tsize;

	RECT rc_o;
	RECT rc_d;

	Player->GetGlobalPosition(&gx, &gy);
	Player->GetLocalPosition(&px, &py);
	Player->GetTileSize(&tsize);
	BeginBatchDrawing(texCharacters, 0.0f);

	//AÑADO OFFSET DE CAMARA
	//int oy;
	//Scene->GetOffsetYCamera(&oy);
	//py += oy;

	int temp = gy % tsize;
	float vuelta, velocidadvuelta;
	Player->GetVuelta(&vuelta);
	Player->GetVelocidadVuelta(&velocidadvuelta);
	
	if ((temp == 0))	{ SetRect(&rc_o, 240, 32, 288, 80); vuelta = 0.0f;}
	else
	{
		if (vuelta >= 5){ SetRect(&rc_o, 192, 32, 240, 80); vuelta = 0.0f; }
		else if (vuelta >= 4){ SetRect(&rc_o, 144, 32, 192, 80); vuelta=vuelta+velocidadvuelta; }
		else if (vuelta >= 3){ SetRect(&rc_o, 96, 32, 144, 80); vuelta = vuelta + velocidadvuelta; }
		else if (vuelta >= 2){ SetRect(&rc_o, 48, 32, 96, 80); vuelta = vuelta + velocidadvuelta; }
		else if (vuelta >= 1){ SetRect(&rc_o, 0, 32, 48, 80); vuelta = vuelta + velocidadvuelta; }
		else if (vuelta >= 0){ SetRect(&rc_o, 240, 32, 288, 80); vuelta = vuelta + velocidadvuelta; }
	}
	Player->SetVuelta(vuelta);
	Player->SetVelocidadVuelta(velocidadvuelta);

	SetRect(&rc_d, px-10, py-10, px + 40, py + 40);
	AddQuad(rc_o, rc_d, 0xFFFFFFFF);
	EndBatchDrawing();
	//pintamos.
	return true;
}
void cGraphicsLayer::AplicaAlpha(int alpha)
{
	DWORD AlphaValue;
	AlphaValue = D3DCOLOR_ARGB(alpha, 0, 0, 0);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_CONSTANT, AlphaValue);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

void cGraphicsLayer::PintaFondo(cScene *Scene)
{
	RECT rc_o;
	RECT rc_d;

	BeginBatchDrawing(texGame3, 0.0f);
	SetRect(&rc_o, 0, 0, 800, 600);
	SetRect(&rc_d, 0, 0, 800, 600);
	AddQuad(rc_o, rc_d, 0xFFFFFFFF);
	EndBatchDrawing();

	int fondo;
	Scene->GetFondo(&fondo);

	if (fondo == 0){
		alpha1 = alpha1 + 10;
		if (alpha1 > 255) alpha1 = 255;
		AplicaAlpha(alpha1);

		BeginBatchDrawing(texGame, 0.0f);
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		EndBatchDrawing();
		AplicaAlpha(255);
		alpha2 = 0;
		alpha3 = 0;
	}
	if (fondo == 1){

		alpha2 = alpha2 + 10;
		if (alpha2 > 255) alpha2 = 255;

		BeginBatchDrawing(texGame, 0.0f);
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		EndBatchDrawing();

		AplicaAlpha(alpha2);
		BeginBatchDrawing(texGame3, 0.0f);
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		EndBatchDrawing();

		AplicaAlpha(255);
		alpha1 = 0;
		alpha3 = 0;

	}
	if (fondo == 2){

		alpha3 = alpha3 + 10;
		if (alpha3 > 255) alpha3 = 255;
		AplicaAlpha(alpha3);

		BeginBatchDrawing(texGame2, 0.0f);
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		EndBatchDrawing();

		AplicaAlpha(255);
		alpha2 = 0;
		alpha1 = 0;
	}
	
}
bool cGraphicsLayer::PintaScore(cScene *Scene, cInterface *Interface)
{

	int TempScore;
	Interface->GetScore(&TempScore);
	int Marcador1, Marcador2, Marcador3;

	Marcador1 = TempScore % 10;
	Marcador2 = TempScore % 100 / 10;
	Marcador3 = TempScore % 1000 / 100;

	RECT Attempt;
	RECT m1;
	RECT m2;
	RECT m3;
	RECT rc_sc2;
	BeginBatchDrawing(texNumbers, 0.0f);
	//Attempt
	SetRect(&Attempt, 0, 50, 256, 110);
	SetRect(&rc_sc2, 300, SCORE_Y, 300 + 128, SCORE_Y + 30);
	AddQuad(Attempt, rc_sc2, 0xFFFFFFFF);
	int cent = 0;

	if (TempScore > 100){
		cent = 42;
		SetRect(&m1, 0 + (40 * Marcador3), 0, 40 + (40 * Marcador3), 50);
		SetRect(&rc_sc2, SCORE_X + cent, SCORE_Y, SCORE_X + 18 + cent, SCORE_Y + 28);//primer marcador
		AddQuad(m1, rc_sc2, 0xFFFFFFFF);
	}

	if (TempScore >= 10){

		SetRect(&m2, 0 + (40 * Marcador2), 0, 40 + (40 * Marcador2), 50);
		SetRect(&rc_sc2, SCORE_X + 20 + cent, SCORE_Y, SCORE_X + 38 + cent, SCORE_Y + 28);//segon marcador
		AddQuad(m2, rc_sc2, 0xFFFFFFFF);
	}

	SetRect(&m3, 0 + (40 * Marcador1), 0, 40 + (40 * Marcador1), 50);
	SetRect(&rc_sc2, SCORE_X + 40 + cent, SCORE_Y, SCORE_X + 56 + cent, SCORE_Y + 28);//tercer marcador
	AddQuad(m3, rc_sc2, 0xFFFFFFFF);
	EndBatchDrawing();

	return true;

}
bool cGraphicsLayer::PintaCheckPoint(cScene *cScene,cPlayer *Player)
{
	int px, py, tsize,cpgx,cpgy,cplx,cply;

	RECT rc_o;
	RECT rc_d;

	cScene->ck.GetGlobalCPoint(&cpgx, &cpgy);
	cScene->ck.GetLocalCPoint(&cplx, &cply);

	Player->GetGlobalPosition(&px,&py);
	
	//pintamos si esta en pantalla, esta en pantalla si la resta de la posicion x del player y la de la bandera de menos de 5*32 que es el espacio que hay hasta el final.
	if ((px - cpgx<5 * 32) && (cpgx != 0))
	{
		BeginBatchDrawing(texCharacters, 0.0f);
		SetRect(&rc_o, 0, 0, 32, 32);
		SetRect(&rc_d, 5 * 32 + cpgx - px, cply, 5 * 32 + cpgx - px + 32, cply + 32); //aqui hay un pequeño error al pintar en las y's globales.
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);

		EndBatchDrawing();

	}
	
	return true;
}
bool cGraphicsLayer::PintaFinal(cScene *Scene)
{
		RECT rc_o;
		RECT rc_d;

		AplicaAlpha(Scene->alpha);

		BeginBatchDrawing(texEnd, 0.0f);
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		EndBatchDrawing();
		//AplicaAlpha(255);
		return true;
}