#pragma once

#include <vector>
#include "glm\glm.hpp"

void SetWhitchCourse(int _whitch_course);

int GetWhitchCourse();

bool IntersectField(glm::vec2 _position, PLAYER* _player);

bool IntersectPipe(glm::vec2 _position, int _intersect_poit, bool _isInGround);

int IntersectBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block, bool _isInGround);

int IntersectCoin(glm::vec2 _position, int _index);

int IntersectFieldCoin(glm::vec2 _position, int _index, bool _isInGround);

int IsGetQuestionbox(glm::vec2 _position, int _index);

int IsHitBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block);

int IsHitCoinBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block);

bool IntersectOneBlock(glm::vec2 _position, BLOCK* _block, int _block_index);

void DoIntersectEnemyToBlock(ENEMY* _enemy, BLOCK* _block, int _block_index);

void  DoIntersectItemToBlock(ITEM* _item, BLOCK* _block, int _block_index);

void  DoIntersectFireBallToBlock(FIRE_BALL* _fire_ball, BLOCK* _block, int _block_index);

void SearchIntersectEnemyPoint(glm::vec2 _position, int _index, PLAYER _player, std::vector<ENEMY*> _enemy);

int SearchIntersectEnemyPointToEnemy(ENEMY* _enemy1, ENEMY* _enemy2);

int IntersectEnemy(int _index, PLAYER _player, std::vector<ENEMY*> _enemy);

bool IntersectEnemyToEnemy(ENEMY* _enemy1, ENEMY* _enemy2);

void  DoIntersectEnemyToEnemy(ENEMY* _enemy1, ENEMY* _enemy2, int _obj1, int _obj2);

int IntersectEnemyFireBall(int _index, FIRE_BALL* _fire_ball, std::vector<ENEMY*> _enemy);

int IntersectItem(int _index, PLAYER _player, std::vector<ITEM*> _item);

int IntersectGoal(glm::vec2 _position);

void DoIntersectDetection(
	int _obj1, //当たり判定を行う一つ目の種類
	int _obj2,
	int _loop1, //一つ目のオブジェクトのインスタンスの数
	int _loop2,
	bool _isInGround,
	PLAYER* _player, std::vector<ENEMY*> _enemy, std::vector<ENEMY*> _enemy2,
	std::vector<ITEM*> _item, std::vector<FIRE_BALL*> _fire_ball, std::vector<BLOCK*> _block, std::vector<QUESTION*> _question);

