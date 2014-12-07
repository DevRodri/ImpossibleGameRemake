
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
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		//Create device with hardware vertex processing
		hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
	}
	else
	{
		//Create device with software vertex processing
		hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
	}

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
	texGame = LoadTexture("game.png", NULL);
	//Tiles
	texTiles = LoadTexture("tiles.png", 0x00ff00ff);
	//Characters
	texCharacters = LoadTexture("characters.png", 0x00ff00ff);
	//Mouse pointers
	texMouse = LoadTexture("mouse.png", 0x00ff00ff);
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
void cGraphicsLayer::BeginBatchDrawing(IDirect3DTexture9* texture)
{
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

bool cGraphicsLayer::Render(cMouse *Mouse, cScene *Scene, int state /*, cPlayer *Player, DWORD deltaTime*/)
{

	RECT rc_o;
	RECT rc_d;
	ClearBuffer();
	BeginDrawing();

	switch (state)
	{
	case STATE_MAIN:

		//pintar pantalla principal
		BeginBatchDrawing(texMain);
		//rectangulo de toda la pantalla
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		//pintamos.
		EndBatchDrawing();

		break;

	case STATE_GAME:
		//Graphic User Interface
		//pintar fondo de pantalla

		//para pintar el fondo con QUADS
		BeginBatchDrawing(texGame);
		SetRect(&rc_o, 0, 0, 800, 600);
		SetRect(&rc_d, 0, 0, 800, 600);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		EndBatchDrawing();

		//para pintar el fondo con SPRITES
		//g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		//g_pSprite->Draw(texGame, NULL, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//g_pSprite->End();

		////////////////////////////////////

		//pintar escena
		BeginBatchDrawing(texTiles);
		PintaEscena(Scene);
		EndBatchDrawing();


		//Scene->cx += 0.05 * deltaTime;
		//if (Scene->cx == SCENE_AREA)Scene->cx = 0;

		///aqui hay que cargar los elementos y pintarlos donde toca.


		/*//pintar player
		BeginBatchDrawing(texCharacters);
		//rectangulo de toda la pantalla
		int px, py;
		//Player->GetPosition(&px, &py);
		SetRect(&rc_o, 0, 0, 32, 32);
		SetRect(&rc_d, px, py, px + 32, py + 32);
		AddQuad(rc_o, rc_d, 0xFFFFFFFF);
		//pintamos.
		EndBatchDrawing();
		break;*/
	}

	/*BeginBatchDrawing(texture);

	//Draw 16 quads
	for (int quads = 0; quads < 16; quads++)
	AddQuad(&rSource[quads], &rDest[quads], 0xFFFFFFFF);

	EndBatchDrawing();*/

	PintaRaton(Mouse);

	EndDrawing();
	Present();

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
	//esto hay que cambiarlo entero. hay que pintar la escena en funcion de la velocidad desplazamiento, y donde se encuentra ahora.
	RECT rc_o;
	RECT rc_d;

	int x, y, n,
		fx, fy,
		pantx, panty;

	fx = Scene->cx + SCENE_WIDTH;
	fy = Scene->cy + SCENE_HEIGHT;

	for (y = Scene->cy; y<fy; y++)
	{
		panty = SCENE_Yo + ((y - Scene->cy) << 5);

		for (x = Scene->cx; x<fx; x++)
		{
			pantx = SCENE_Xo + ((x - Scene->cx) << 5);

			n = Scene->map[y][x];

			SetRect(&rc_o, n << 5, 0, (n + 1) << 5, 32);

			SetRect(&rc_d, pantx, panty, pantx + 32, panty + 32);

			AddQuad(rc_o, rc_d, 0xFFFFFFFF);

		}
	}
	return true;
}