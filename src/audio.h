/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _AUDIO_H
#define _AUDIO_H

#include "psx.h"

//XA enumerations
typedef enum
{
	XA_Menu,   //MENU.XA
	XA_Week1A, //WEEK1A.XA
	XA_Week1B, //WEEK1B.XA
	XA_Week2A, //WEEK2A.XA
	
	XA_Max,
} XA_File;

typedef enum
{
	//MENU.XA
	XA_GettinFreaky, //Gettin' Freaky
	XA_GameOver,     //Game Over
	//WEEK1A.XA
	XA_Bopeebo, //Bopeebo
	XA_Fresh,   //Fresh
	//WEEK1B.XA
	XA_Dadbattle, //DadBattle
	XA_Tutorial,  //Tutorial
	//WEEK2A.XA
	XA_Spookeez, //Spookeez

	
	XA_TrackMax,
} XA_Track;

//Audio functions
void Audio_Init(void);
void Audio_Quit(void);
void Audio_PlayXA_Track(XA_Track track, u8 volume, u8 channel, boolean loop);
void Audio_SeekXA_Track(XA_Track track);
void Audio_PauseXA(void);
void Audio_StopXA(void);
void Audio_ChannelXA(u8 channel);
s32 Audio_TellXA_Sector(void);
s32 Audio_TellXA_Milli(void);
boolean Audio_PlayingXA(void);
void Audio_WaitPlayXA(void);
void Audio_ProcessXA(void);

#endif
