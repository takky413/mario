#pragma once

#include <vector>
#include "header.h"

PLAYER* MakePlayerInstance(void);

std::vector<ENEMY*> MakeEnemyInstance(double _width, double _height, int _enemy_num);

std::vector<ITEM*> MakeMushroomInstance(int _loop);

std::vector<ITEM*> MakeFireFlowerInstance(int _loop);

std::vector<FIRE_BALL*> MakeFireBallInstance(void);

std::vector<ITEM*> MakeStarInstance(void);

void InitFireBallInstance(FIRE_BALL* _fire_ball);

std::vector<QUESTION*> MakeQuestionInstance(int _loop);

std::vector<BLOCK*> MakeBlockInstance(int _loop);

std::vector<BLOCK*> MakeCoinBlockInstance(void);

std::vector<COIN*> MakeCoinInstance(int _loop);

std::vector<COIN*> MakeFieldCoinInstance(int _loop);

std::vector<COIN*> MakeBlockCoinInstance(void);

std::vector<LIFT*> MakeLiftInstance(void);
