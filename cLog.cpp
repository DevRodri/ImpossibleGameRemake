#include "cLog.h"
#include <dxerr9.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

cLog* cLog::_instance = 0;

cLog* cLog::Instance()
{
	FILE *f;

    if (_instance == 0)
	{
        _instance = new cLog;
		
		f=fopen("log.txt","w");
		fclose(f);
    }
    return _instance;
}

/**************************************************************************
	msg
**************************************************************************/
void cLog::Msg(char *msg)
{
	FILE *f;
	char s[256];

	ZeroMemory(s,sizeof(s));

	sprintf(s,"%s\n",msg);
	f=fopen("log.txt","a+");
	fwrite(s,sizeof(char),strlen(s),f);
	fclose(f);
}

/**************************************************************************
	Error: msg
	Code: hr
	Description: desc(hr)
**************************************************************************/
void cLog::Error(HRESULT hr, char *msg)
{
	FILE *f;
	char s[256];

	ZeroMemory(s,sizeof(s));

	sprintf(s,"Error: %s\n",msg);
	sprintf(s,"%sCode: %s\n",s,(char *)DXGetErrorString9(hr));
	sprintf(s,"%sDescription: %s",s,DXGetErrorDescription9(hr));
	
	f=fopen("log.txt","a+");
	fwrite(s,sizeof(char),strlen(s),f);
	fclose(f);
}
