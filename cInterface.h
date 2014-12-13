#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#define SCORE_X	 420
#define SCORE_Y	 32

class cInterface
{
public:
	cInterface();
	virtual ~cInterface();
	int	 SCORE;
	void InitScore();
	void SumScore();
	void GetScore(int *score);

private:


};

#endif
