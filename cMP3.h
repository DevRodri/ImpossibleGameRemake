#ifndef __MP3_H__
#define __MP3_H__
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <strmif.h>
#include <control.h>
#include <uuids.h>
#pragma comment(lib,"strmiids.lib")
#pragma once

class cMP3
{
public:

	cMP3();
	//virtual ~cMP3();
	~cMP3();

	IGraphBuilder* g_pGraphBuilder;
	IMediaControl* g_pPlaybackControl;
	IBasicAudio*   g_pVolumeControl;
	int			   g_bReady;


	void Mp3Cleanup();

	void Mp3Init();
	void Mp3InitGraph();
	void Mp3InitIMedia();
	void Mp3InitBasic();
	void Mp3InitReady();

	void Mp3Play();
	void Mp3Load(char filename[]);
	void Mp3Pause();
	void Mp3Stop();

	void Mp3SetVolume(long l);
	long Mp3GetVolume();
	void Mp3SetBalance(long l);
	long Mp3GetBalance();
	
	double GetDuration();
	double GetPosition();
	void SetPosition(double posicion);
	
	void PrepareTrack(char filename[]);
};
#endif
