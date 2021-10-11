/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "gfa.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"


#include "../stage/week7.h"

//gfa character structure
enum
{
	gfa_ArcMain_BopLeft,
	gfa_ArcMain_BopRight,
	gfa_ArcMain_Cry,
	
	gfa_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[gfa_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
	
	//Pico test
	u16 *pico_p;
} Char_gfa;

//gfa character definitions
static const CharFrame char_gfa_frame[] = {
	{gfa_ArcMain_BopLeft, {  0,   0,  123, 103}, { 40,  73}}, //0 bop left 1
	{gfa_ArcMain_BopLeft, {  0,   0,  123, 103}, { 39,  73}}, //1 bop left 2
	{gfa_ArcMain_BopLeft, {147,   0,  151, 102}, { 39,  73}}, //2 bop left 3
	{gfa_ArcMain_BopLeft, {147, 103,  151, 103}, { 39,  74}}, //3 bop left 4
	{gfa_ArcMain_BopLeft, {141, 102,  255, 105}, { 43,  76}}, //4 bop left 5
	{gfa_ArcMain_BopLeft, {141, 102,  255, 105}, { 43,  76}}, //5 bop left 6
	
	{gfa_ArcMain_BopRight, {  0,   0,  126, 103}, { 43,  74}}, //6 bop right 1
	{gfa_ArcMain_BopRight, {  0,   0,  126, 103}, { 43,  74}}, //7 bop right 2
	{gfa_ArcMain_BopRight, {  0,   0,  125, 103}, { 42,  74}}, //8 bop right 3
	{gfa_ArcMain_BopRight, {  0,   0,  125, 103}, { 41,  74}}, //9 bop right 4
	{gfa_ArcMain_BopRight, {152, 103,  255, 105}, { 35,  76}}, //10 bop right 5
	{gfa_ArcMain_BopRight, {152, 103,  255, 104}, { 35,  75}}, //11 bop right 6
};

static const Animation char_gfa_anim[CharAnim_Max] = {
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_Idle
	{1, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  4,  4,  5, ASCR_BACK, 1}}, //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_LeftAlt
	{2, (const u8[]){12, 13, ASCR_REPEAT}},                                  //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_DownAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_UpAlt
	{1, (const u8[]){ 6,  6,  7,  7,  8,  8,  9, 10, 10, 11, ASCR_BACK, 1}}, //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Left}},                           //CharAnim_RightAlt
};

//gfa character functions
void Char_gfa_SetFrame(void *user, u8 frame)
{
	Char_gfa *this = (Char_gfa*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_gfa_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_gfa_Tick(Character *character)
{
	Char_gfa *this = (Char_gfa*)character;
	
	//Initialize Pico test
	if (stage.stage_id == StageId_7_3 && stage.back != NULL && this->pico_p == NULL)
		this->pico_p = ((Back_Week7*)stage.back)->pico_chart;
	
	if (this->pico_p != NULL)
	{
		if (stage.note_scroll >= 0)
		{
			//Scroll through Pico chart
			u16 substep = stage.note_scroll >> FIXED_SHIFT;
			while (substep >= ((*this->pico_p) & 0x7FFF));
											
			
		}
	}
	else
	{
		if (stage.flag & STAGE_FLAG_JUST_STEP)
		{
			//Perform dance
			if ((stage.song_step % stage.gf_speed) == 0)
			{
				//Switch animation
				if (character->animatable.anim == CharAnim_Left)
					character->set_anim(character, CharAnim_Right);
				else
					character->set_anim(character, CharAnim_Left);
				
			}
		}
	}
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_gfa_SetFrame);
	Character_Draw(character, &this->tex, &char_gfa_frame[this->frame]);
	
}

void Char_gfa_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
}

void Char_gfa_Free(Character *character)
{
	Char_gfa *this = (Char_gfa*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_GFA_New(fixed_t x, fixed_t y)
{
	//Allocate gfa object
	Char_gfa *this = Mem_Alloc(sizeof(Char_gfa));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_GFA_New] Failed to allocate gfa object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_gfa_Tick;
	this->character.set_anim = Char_gfa_SetAnim;
	this->character.free = Char_gfa_Free;
	
	Animatable_Init(&this->character.animatable, char_gfa_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(16,1);
	this->character.focus_y = FIXED_DEC(-50,1);
	this->character.focus_zoom = FIXED_DEC(13,10);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\GFA.ARC;1");
	
	const char **pathp = (const char *[]){
		"bopleft.tim",  //gfa_ArcMain_BopLeft
		"bopright.tim", //gfa_ArcMain_BopRight
		"cry.tim",      //gfa_ArcMain_Cry
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	
	//Initialize Pico test
	if (stage.stage_id == StageId_7_3 && stage.back != NULL)
		this->pico_p = ((Back_Week7*)stage.back)->pico_chart;
	else
		this->pico_p = NULL;
	
	return (Character*)this;
}
