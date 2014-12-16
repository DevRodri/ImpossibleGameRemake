

//////////////////////////////////////////////////////////////////////////
//
//	Sentry  (prevents header being included twice)
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _MP3_PLAYER_H_
#define _MP3_PLAYER_H_

//////////////////////////////////////////////////////////////////////////
//
//	DirectShow Initialisation / Uninitialisation
//
//////////////////////////////////////////////////////////////////////////

/// \brief	You must call this function before calling any other Mp3 function
///
extern void Mp3Init(void);


/// \brief	This function is called to kill the DirectShow interface
///
extern void Mp3Cleanup(void);



//////////////////////////////////////////////////////////////////////////
//
//	Load an Mp3 File
//
//////////////////////////////////////////////////////////////////////////

/// \brief	this function loads the specified file for playback
/// \param	filename	-	the name of the file to load
///
extern void Mp3Load(char filename[]);



//////////////////////////////////////////////////////////////////////////
//
//	Mp3 Playback Controls
//
//////////////////////////////////////////////////////////////////////////


/// \brief	call this to start playback of the mp3
///
extern void Mp3Play(void);


/// \brief	call this to pause playback of the mp3
///
extern void Mp3Pause(void);


/// \brief	call this to stop the playback of the mp3
///
extern void Mp3Stop(void);




//////////////////////////////////////////////////////////////////////////
//
//	Audio Controls
//
//////////////////////////////////////////////////////////////////////////

/// \brief	call this to set the current volume level. Valid values are 
///			between -10000 and 0.
///
extern void Mp3SetVolume(long);


/// \brief	call this to get the current volume level. Valid values are 
///			between -10000 and 0.
///
extern long Mp3GetVolume(void);


/// \brief	call this to set the current balance. Valid values are 
///			between -10000 and 10000.
///
extern void Mp3SetBalance(long);


/// \brief	call this to get the current balance. Valid values are 
///			between -10000 and 10000.
///
extern long Mp3GetBalance(void);

double GetPosition();
void SetPosition(double posicion);

#endif
