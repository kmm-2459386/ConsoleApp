#pragma once
//======================================
//	�O���u  AI
//======================================
#ifndef __AI_H
#define __AI_H
#include "Stage.h"
#include "Castle.h"
#include "Command.h"

// NPC�^�[���̓���(�v�l)
Command InputNpcTurn(Stage* stage, Castle* castle, CastleId* outTarget, int* outTroopCount);

#endif //  __AI_H