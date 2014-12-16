#include "cCheckPoint.h"

cCheckPoint::cCheckPoint()
{
	velocidadCPoint = 0.0f;
	pointlx = 0;
	pointly=0;
	pointgx = 0;
	pointgy=0;
	scenex = 0;
	sceney=0;
	coff=0;
}

cCheckPoint::~cCheckPoint()
{
}
void cCheckPoint::SetCameraoff(int camoff)
{
	coff = camoff;
}
void cCheckPoint::GetCameraoff(int *camoff)
{
	*camoff = coff;
}
void cCheckPoint::SetVelCPoint(float vely)
{
	velocidadCPoint = vely;
}
void cCheckPoint::GetVelCPoint(float *vely)
{
	*vely = velocidadCPoint;
}
void cCheckPoint::SetSceneCPoint(int x, int y)
{
	scenex = x;
	sceney = y;
}
void cCheckPoint::GetSceneCPoint(int *x, int *y)
{
	*x = scenex;
	*y = sceney;
}
void cCheckPoint::SetGlobalCPoint(int x, int y)
{
	pointgx = x;
	pointgy = y;
}
void cCheckPoint::GetGlobalCPoint(int *x, int *y)
{
	*x = pointgx;
	*y = pointgy;
}
void cCheckPoint::SetLocalCPoint(int x, int y)
{
	pointlx = x;
	pointly = y;
}
void cCheckPoint::GetLocalCPoint(int *x, int *y)
{
	*x = pointlx;
	*y = pointly;
}