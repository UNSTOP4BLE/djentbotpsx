/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "guitarbot.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Christmas Parents structure
enum
{
	guitarbot_ArcMain_Idle0,
	guitarbot_ArcMain_Idle1,
	guitarbot_ArcMain_Idle2,
	guitarbot_ArcMain_Idle3,
	guitarbot_ArcMain_LeftA0,
	guitarbot_ArcMain_LeftA1,
	guitarbot_ArcMain_LeftB0,
	guitarbot_ArcMain_LeftB1,
	guitarbot_ArcMain_DownA0,
	guitarbot_ArcMain_DownA1,
	guitarbot_ArcMain_DownB0,
	guitarbot_ArcMain_DownB1,
	guitarbot_ArcMain_UpA0,
	guitarbot_ArcMain_UpA1,
	guitarbot_ArcMain_UpB0,
	guitarbot_ArcMain_UpB1,
	guitarbot_ArcMain_RightA0,
	guitarbot_ArcMain_RightA1,
	guitarbot_ArcMain_RightB0,
	guitarbot_ArcMain_RightB1,
	
	guitarbot_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[guitarbot_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_guitarbot;

//Christmas Parents definitions
static const CharFrame char_guitarbot_frame[] = {
	{guitarbot_ArcMain_Idle0, {0,   0, 219, 195}, {130, 182}}, //0 idle 1
	{guitarbot_ArcMain_Idle1, {0,   0, 219, 191}, {131, 178}}, //1 idle 2
	{guitarbot_ArcMain_Idle2, {0,   0, 221, 192}, {130, 179}}, //2 idle 3
	{guitarbot_ArcMain_Idle3, {0,   0, 217, 196}, {127, 183}}, //3 idle 4
	
	{guitarbot_ArcMain_LeftA0, {0,   0, 197, 201}, {123, 188}}, //4 left a 1
	{guitarbot_ArcMain_LeftA1, {0,   0, 201, 201}, {125, 188}}, //5 left a 2
	{guitarbot_ArcMain_LeftB0, {0,   0, 197, 200}, {122, 188}}, //6 left b 1
	{guitarbot_ArcMain_LeftB1, {0,   0, 201, 200}, {125, 188}}, //7 left b 2
	
	{guitarbot_ArcMain_DownA0, {0,   0, 213, 189}, {120, 177}}, //8 down a 1
	{guitarbot_ArcMain_DownA1, {0,   0, 213, 190}, {122, 178}}, //9 down a 2
	{guitarbot_ArcMain_DownB0, {0,   0, 213, 189}, {121, 177}}, //10 down b 1
	{guitarbot_ArcMain_DownB1, {0,   0, 213, 190}, {123, 178}}, //11 down b 2
	
	{guitarbot_ArcMain_UpA0, {0,   0, 199, 203}, {117, 190}}, //12 up a 1
	{guitarbot_ArcMain_UpA1, {0,   0, 201, 203}, {120, 190}}, //13 up a 2
	{guitarbot_ArcMain_UpB0, {0,   0, 197, 204}, {117, 191}}, //14 up b 1
	{guitarbot_ArcMain_UpB1, {0,   0, 201, 202}, {119, 190}}, //15 up b 2
	
	{guitarbot_ArcMain_RightA0, {0,   0, 239, 187}, {128, 174}}, //16 right a 1
	{guitarbot_ArcMain_RightA1, {0,   0, 239, 191}, {130, 178}}, //17 right a 2
	{guitarbot_ArcMain_RightB0, {0,   0, 239, 189}, {128, 176}}, //18 right b 1
	{guitarbot_ArcMain_RightB1, {0,   0, 239, 191}, {130, 178}}, //19 right b 2
};

static const Animation char_guitarbot_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 4,  5, ASCR_BACK, 1}},         //CharAnim_Left
	{2, (const u8[]){ 6,  7, ASCR_BACK, 1}},         //CharAnim_LeftAlt
	{2, (const u8[]){ 8,  9, ASCR_BACK, 1}},         //CharAnim_Down
	{2, (const u8[]){10, 11, ASCR_BACK, 1}},         //CharAnim_DownAlt
	{2, (const u8[]){12, 13, ASCR_BACK, 1}},         //CharAnim_Up
	{2, (const u8[]){14, 15, ASCR_BACK, 1}},         //CharAnim_UpAlt
	{2, (const u8[]){16, 17, ASCR_BACK, 1}},         //CharAnim_Right
	{2, (const u8[]){18, 19, ASCR_BACK, 1}},         //CharAnim_RightAlt
};

//Christmas Parents functions
void Char_guitarbot_SetFrame(void *user, u8 frame)
{
	Char_guitarbot *this = (Char_guitarbot*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_guitarbot_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_guitarbot_Tick(Character *character)
{
	Char_guitarbot *this = (Char_guitarbot*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_guitarbot_SetFrame);
	Character_Draw(character, &this->tex, &char_guitarbot_frame[this->frame]);
}

void Char_guitarbot_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_guitarbot_Free(Character *character)
{
	Char_guitarbot *this = (Char_guitarbot*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Guitarbot_New(fixed_t x, fixed_t y)
{
	//Allocate Christmas Parents object
	Char_guitarbot *this = Mem_Alloc(sizeof(Char_guitarbot));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_guitarbot_New] Failed to allocate Christmas Parents object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_guitarbot_Tick;
	this->character.set_anim = Char_guitarbot_SetAnim;
	this->character.free = Char_guitarbot_Free;
	
	Animatable_Init(&this->character.animatable, char_guitarbot_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 3;
	
	this->character.focus_x = FIXED_DEC(60,1);
	this->character.focus_y = FIXED_DEC(-70,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\GUITARBO.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",   //guitarbot_ArcMain_Idle0
		"idle1.tim",   //guitarbot_ArcMain_Idle1
		"idle2.tim",   //guitarbot_ArcMain_Idle2
		"idle3.tim",   //guitarbot_ArcMain_Idle3
		"lefta0.tim",  //guitarbot_ArcMain_LeftA0
		"lefta1.tim",  //guitarbot_ArcMain_LeftA1
		"leftb0.tim",  //guitarbot_ArcMain_LeftB0
		"leftb1.tim",  //guitarbot_ArcMain_LeftB1
		"downa0.tim",  //guitarbot_ArcMain_DownA0
		"downa1.tim",  //guitarbot_ArcMain_DownA1
		"downb0.tim",  //guitarbot_ArcMain_DownB0
		"downb1.tim",  //guitarbot_ArcMain_DownB1
		"upa0.tim",    //guitarbot_ArcMain_UpA0
		"upa1.tim",    //guitarbot_ArcMain_UpA1
		"upb0.tim",    //guitarbot_ArcMain_UpB0
		"upb1.tim",    //guitarbot_ArcMain_UpB1
		"righta0.tim", //guitarbot_ArcMain_RightA0
		"righta1.tim", //guitarbot_ArcMain_RightA1
		"rightb0.tim", //guitarbot_ArcMain_RightB0
		"rightb1.tim", //guitarbot_ArcMain_RightB1
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
