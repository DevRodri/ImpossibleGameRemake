#include "cInterface.h"

cInterface::cInterface(){}
cInterface::~cInterface(){}

void cInterface::InitScore()
{
	
	SCORE = 1;

}

void cInterface::SumScore()
{

	SCORE = SCORE + 1;

}

void cInterface::GetScore(int *score)
{
	*score = SCORE;

}