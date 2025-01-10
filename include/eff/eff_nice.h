#pragma once

#include "driver/effdrv.h"
#include <dolphin/mtx.h>

typedef struct NiceEffectWork {
	s32 type; //0x0
	Vec position; //0x4
	f32 field_0x10; //0x10, Vec scale;?
	f32 field_0x14; //0x14
	f32 field_0x18; //0x18
	f32 field_0x1C; //0x1C
	s32 field_0x20; //0x20
	u8 field_0x24[0x28 - 0x24]; //0x24
	s32 field_0x28; //0x28
	u8 field_0x2C[0x30 - 0x2C]; //0x2C
	s32 poseId; //0x30
	u8 field_0x34[0x64 - 0x34]; //0x34
} NiceEffectWork;

BOOL effNiceAsync(s32 group);
EffectEntry* effNiceEntry(s32 type, f32 x, f32 y, f32 z);
