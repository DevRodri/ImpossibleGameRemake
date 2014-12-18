#include "cMP3.h"

cMP3::cMP3(){}
cMP3::~cMP3(){}

void cMP3::Mp3InitGraph() {
	g_pGraphBuilder = NULL;
}

void cMP3::Mp3InitIMedia() {
	g_pPlaybackControl = NULL;
}

void cMP3::Mp3InitBasic() {
	g_pVolumeControl = NULL;
}

void cMP3::Mp3InitReady() {
	g_bReady = 0;
}


void cMP3::Mp3Init(void) 
{
	CoInitialize(NULL);
}


void cMP3::Mp3Load(char filename[])
{
	// DirectShow requires unicode (16bit) characters rather than 8bit ascii
	WCHAR wFile[MAX_PATH];

	// convert ascii to unicode
	MultiByteToWideChar(CP_ACP, 0, filename, -1, wFile, MAX_PATH);

	// create a graph build to allow us to make the audio signal path
	if( SUCCEEDED(CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void**)&g_pGraphBuilder)))
	{
		// create a simple controller for the sound file
		g_pGraphBuilder->QueryInterface( IID_IMediaControl, (void **)&g_pPlaybackControl );

		// create a simple interface for volume and balance
		g_pGraphBuilder->QueryInterface( IID_IBasicAudio  , (void **)&g_pVolumeControl );
		 
		// load and start playback of the specified sound file
		if( SUCCEEDED( g_pGraphBuilder->RenderFile(wFile, NULL) ) )
		{
			g_bReady = 1;
		}
	}
}


void cMP3::Mp3Cleanup(void)
{
	
	if( g_pPlaybackControl )
	{
		g_pPlaybackControl->Stop();
	}

	if( g_pVolumeControl )
	{
		g_pVolumeControl->Release();
		g_pVolumeControl = NULL;
	}
	
	if( g_pPlaybackControl )
	{
		g_pPlaybackControl->Release();
		g_pPlaybackControl = NULL;
	}
	
	if( g_pGraphBuilder )
	{
		g_pGraphBuilder->Release();
		g_pGraphBuilder = NULL;
	}

	CoUninitialize();
}


void cMP3::Mp3Play(void)
{
	if(g_bReady)
	{
		g_pPlaybackControl->Run();
	}
}


void cMP3::Mp3Pause(void)
{
	if(g_bReady)
	{
		g_pPlaybackControl->Pause();
	}
}


void cMP3::Mp3Stop(void)
{
	if(g_bReady)
	{
		g_pPlaybackControl->Stop();
	}
}



void cMP3::Mp3SetVolume(long val)
{
	if(g_bReady)
	{
		g_pVolumeControl->put_Volume(val);
	}
}


long cMP3::Mp3GetVolume(void)
{
	long val=0;
	if(g_bReady)
	{
		g_pVolumeControl->get_Volume(&val);
	}
	return val;
}


void cMP3::Mp3SetBalance(long val)
{
	if(g_bReady)
	{
		g_pVolumeControl->put_Balance(val);
	}
}


long cMP3::Mp3GetBalance(void)
{
	long val=0;
	if(g_bReady)
	{
		g_pVolumeControl->get_Balance(&val);
	}
	return val;
}

double cMP3::GetDuration()
  {
  double temp;
  IMediaPosition* pMP;
  HRESULT hrErr = g_pGraphBuilder->QueryInterface(IID_IMediaPosition,(void**)&pMP);
  pMP->get_Duration(&temp);
  pMP->Release();
  return temp;
  }


void cMP3::SetPosition(double posicion)
  {
 
  IMediaPosition* pMP;
  HRESULT hrErr = g_pGraphBuilder->QueryInterface(IID_IMediaPosition,(void**)&pMP);
  hrErr = pMP->put_CurrentPosition( posicion);
  pMP->Release();
  }


double cMP3::GetPosition()
  {
  double temp;
  IMediaPosition* pMP;
  HRESULT hrErr = g_pGraphBuilder->QueryInterface(IID_IMediaPosition,(void**)&pMP);
  pMP->get_CurrentPosition(&temp);
  pMP->Release();
  return temp;
  }


void cMP3::PrepareTrack(char filename[], double inicio)
{
	Mp3Cleanup();
	Mp3Init();
	Mp3Load(filename);
	SetPosition(inicio);
	Mp3Play();

}