
#ifndef __GRAPHICSLAYER_H__
#define __GRAPHICSLAYER_H__


#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxerr8.lib")
#pragma comment(lib,"winmm.lib")

#include <D3D9.h>
#include <D3DX9.h>
#include "cScene.h"
#include "cMouse.h"
#include "cPlayer.h"
#include "cInterface.h"

class cGraphicsLayer
{
	//Custom vertex format
	static const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	//Custom vertex
	struct TLVERTEX
	{
		float x;
		float y;
		float z;
		D3DCOLOR colour;
		float u;
		float v;
	};

public:

	cGraphicsLayer();
	virtual ~cGraphicsLayer();

	bool Init(HWND hWnd, bool exclusive);
	bool Finalize();

	void LoadData();
	void UnLoadData();
	bool Render(cMouse *Mouse, cScene *Scene, int state, cPlayer *Player,cInterface *Interface);
	bool PintaScore(cScene *Scene, cInterface *Interface);
	bool PintaEscena(cScene *Scene);
	bool PintaRaton(cMouse *Mouse);
	bool cGraphicsLayer::PintaPlayer(cScene *Scene, cPlayer *Player);
	bool DrawRect(RECT rc, D3DCOLOR color);
	bool PintaMuerte(cPlayer *Player);
	bool PintaBandera(cPlayer *Player);

	IDirect3DTexture9 *LoadTexture(char *fileName, D3DCOLOR colorkey);
	int ClearBuffer();
	int Present();
	int BeginDrawing();
	int EndDrawing();
	void InitRendering(int resWidth, int resHeight);
	void SetupQuad();
	void FillIndexBuffer();
	void BeginBatchDrawing(IDirect3DTexture9* texture,float rotate);
	void AplicaAlpha(int alpha);
	void PintaFondo(cScene *Scene);
	//Add a quad to the batching buffer
	void AddQuad(RECT rSource, RECT rDest, D3DCOLOR colour);
	//End batched drawing (put all quads in the buffer on screen)
	void EndBatchDrawing();


private:

	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DDEVICE9 g_pD3DDevice;
	LPD3DXSPRITE g_pSprite;

	//Texturas de juego
	LPDIRECT3DTEXTURE9 texMain, texGame3, texGame2, texGame, texDie;
	LPDIRECT3DTEXTURE9 texTiles, texCharacters, texMouse, texNumbers;

	//Capabilities of graphics adapter
	D3DCAPS9 d3dCaps;

	//Vertex buffer for drawing quads
	IDirect3DVertexBuffer9* vertexBuffer;

	//Vertex buffer and index buffer for batched drawing
	IDirect3DVertexBuffer9* vertexBatchBuffer;
	IDirect3DIndexBuffer9* indexBatchBuffer;

	//Max amount of vertices that can be put in the batching buffer
	static const int BATCH_BUFFER_SIZE = 1000;

	//Vertices currently in the batching buffer
	int numBatchVertices;
	TLVERTEX* batchVertices;

	//Info on texture used for batched drawing
	float batchTexWidth;
	float batchTexHeight;
	int alpha1, alpha2, alpha3;
	cInterface Interface;
};


#endif