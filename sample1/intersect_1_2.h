#pragma once

#include <vector>
#include "glm\glm.hpp"

int IsGetQuestionbox_1_2(glm::vec2 _position, int _index);

int IsHitBlock_1_2(glm::vec2 _position, int _index, std::vector<BLOCK*> _block);

int IntersectItem_1_2(int _index, PLAYER _player, std::vector<ITEM*> _item);

void SearchIntersectEnemyPoint_1_2(glm::vec2 _position, int _index, PLAYER _player, std::vector<ENEMY*> _enemy);

int IntersectEnemy_1_2(int _index, PLAYER _player, std::vector<ENEMY*> _enemy);

bool IntersectPipe_1_2(glm::vec2 _position, int _intersect_poit, int _course);

int IntersectEnemyFireBall_1_2(int _index, FIRE_BALL* _fire_ball, std::vector<ENEMY*> _enemy);

void SearchIntersectLiftPoint(glm::vec2 _position, int _index, PLAYER _player, std::vector<LIFT*> _lift);

int IntersectLift(int _index, PLAYER _player, std::vector<LIFT*> _lift);

int IntersectGoal_1_2(glm::vec2 _position);

bool IntersectField_1_2(glm::vec2 _position, PLAYER* _player);

void DoIntersectDetection_1_2(int _obj1, int _obj2, int _loop1, int _loop2, bool _isInGround,
	PLAYER* _player, std::vector<ENEMY*> _enemy1, std::vector<ENEMY*> _enemy2,
	std::vector<ITEM*> _item, std::vector<FIRE_BALL*> _fire_ball, std::vector<BLOCK*> _block, std::vector<QUESTION*> _question,
	std::vector<LIFT*> _lift);

