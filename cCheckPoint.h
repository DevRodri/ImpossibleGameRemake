#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

class cCheckPoint
{
public:
	cCheckPoint();
	virtual ~cCheckPoint();
	void SetCameraoff(int coff);
	void GetCameraoff(int *coff);
	void SetVelCPoint(float vely);
	void GetVelCPoint(float *vely);
	void SetGlobalCPoint(int x, int y);
	void GetGlobalCPoint(int *x, int *y);
	void SetLocalCPoint(int x, int y);
	void GetLocalCPoint(int *x, int *y);
	void SetSceneCPoint(int x, int y);
	void GetSceneCPoint(int *x, int *y);
private:
	float velocidadCPoint;
	int pointlx, pointly;
	int pointgx, pointgy;
	int scenex, sceney;
	int coff;
};

#endif