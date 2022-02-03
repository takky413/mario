#include <vector>
#include "glm\glm.hpp"
#include "course.h"
#include "header.h"
#include "audio.h"
#include "instance.h"

//main.cppがこれをインクルードするため、こちら側に大域変数として定義しておく
//ゴールポールとの当たり判定を行った際に変更する
int whitch_course = 11; //どのコースをプレイしているかを決める変数

void SetWhitchCourse(int _whitch_course) { //セッター
	whitch_course = _whitch_course;
}

int GetWhitchCourse() { //ゲッター
	return whitch_course;
}


//------------------------------------------------------
//フィールドのオブジェクトと干渉しているか調べる関数
//------------------------------------------------------
bool IntersectField(glm::vec2 _position, PLAYER* _player) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_player->isInGround == false) { //地上
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return false;
		}
		switch (course[y][x]) {
		case 'b':
			if (sub_course[y][x] == '0' || sub_course[y][x] == '1') {
				//_player->inPipe = false; //土管に入った後に地面についたら、土管に入り中であること表すフラグをおろす
				return true;
			}
			break;
		case 'p':
			if (_player->inPipe == true) { //土管の上でしゃがんだ時の当たり判定を消す
				//if (sub_course[y][x] == '0' || sub_course[y][x] == '1') {
				if (sub2_course[y][x] == 'i') { //要修正　このままだと、マリオが土管に入っている最中は、敵が土管をすり抜けてしまう
					return false;
				}
				else {
					return true;
				}
			}
			else {
				return true;
			}
			break;
		case 'q':
		case 'f':
			return true;
		}
		return false;
	}
	
	else if (_player->isInGround == true) { //地下
		if ((x < 0) || (x >= GROUND_COURSE_WIDTH) || (y < 0) || (y >= GROUND_COURSE_HEIGHT)) {
			return false;
		}
		switch (ground_course[y][x]) {
		case 'b':
			if (ground_sub_course[y][x] == '0' || ground_sub_course[y][x] == '1') {
				return true;
			}
			break;
		case 'p':
			if (_player->inPipe == true) { //土管の上でしゃがんだ時の当たり判定を消す
				if (ground_sub2_course[y][x] == 'i') { //要修正　このままだと、マリオが土管に入っている最中は、敵が土管をすり抜けてしまう
					return false;
				}
				else {
					return true;
				}
			}
			else {
				return true;
			}
			break;
		case 'q':
			return true;
		}
		return false;
	}
}


//--------------------------------------------------------------------------
//入れる土管と干渉しているか調べる関数
//第二引数には当たり判定を行う箇所（left,right,top,botttomのどれかを入れる）
//--------------------------------------------------------------------------
bool IntersectPipe(glm::vec2 _position, int _intersect_poit, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) {
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return false;
		}
		if (course[y][x] == 'p') { //土管か
			if (sub2_course[y][x] == 'i') {
				if (_intersect_poit == BOTTOM) {
					return true;
				}
			}
		}
		return false;
	}

	else if (_isInGround == true) {
		if ((x < 0) || (x >= GROUND_COURSE_WIDTH) || (y < 0) || (y >= GROUND_COURSE_HEIGHT)) {
			return false;
		}
		if (ground_course[y][x] == 'p') { //土管か
			if (ground_sub2_course[y][x] == 'i') {
				if (_intersect_poit == RIGHT) {
					return true;
				}
			}
		}
		return false;
	}
}


//---------------------------------------------------------------------------------
//どの（破壊できる）ブロックと干渉しているか調べる関数、干渉していなかったら-1を返す
//---------------------------------------------------------------------------------
int IntersectBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) { //地上
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return -1;
		}
		if (_block[_index]->flag == true) {
			if (course[y][x] == 'b') {
				if (sub_course[y][x] == '2') {  //'3'はコインブロック
					if (LetterToNum(sub2_course[y][x]) == _index + 1) {
						return _index;
					}
				}
			}
		}
		return -1;
	}
	
	else if (_isInGround == true) { //地下
		if ((x < 0) || (x >= GROUND_COURSE_WIDTH) || (y < 0) || (y >= GROUND_COURSE_HEIGHT)) {
			return -1;
		}
		if (_block[_index]->flag == true) {
			if (ground_course[y][x] == 'b') {
				if (ground_sub_course[y][x] == '2') {
					if (LetterToNum(ground_sub2_course[y][x]) == _index + 1) {
						printf(" %d",_index);
						return _index;
					}
				}
			}
		}
		return -1;
	}
	
	return -1;
}


//---------------------------------------------------------------------------------
//どのコインブロックと干渉しているか調べる関数、干渉していなかったら-1を返す
//---------------------------------------------------------------------------------
int IntersectCoinBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) { //地上
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return -1;
		}
		//if (_block[_index]->flag == true) { コインブロックは破壊されることがなく、当たり判定が消えることはない
			if (course[y][x] == 'b') {
				if (sub_course[y][x] == '3') {  //'3'はコインブロック
					if (LetterToNum(sub2_course[y][x]) == _index + 1) {
						return _index;
					}
				}
			}
		//}
		return -1;
	}
	else if (_isInGround == true) { //地下
	}

	return -1;
}



//-------------------------------------------------------------------------------------
//指定した一つの（破壊できる）ブロックと干渉しているか調べる関数(アイテムとエネミー用）
//-------------------------------------------------------------------------------------
bool IntersectOneBlock(glm::vec2 _position, BLOCK* _block, int _block_index) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return false;
	}
	if (_block->flag == true) {
		if (course[y][x] == 'b') {
			if (sub_course[y][x] == '2') {
				if (LetterToNum(sub2_course[y][x]) == _block_index + 1) {
					return true;
				}
			}
		}
	}
	return false;
}



//-------------------------------------------------------------------------------------------------------------------------------------
//指定したエネミーが、指定した（破壊できる）ブロックと干渉しているかを、オブジェクトの組合わせを指定して調べて、当たり判定を行う関数
//ブロックの番号を指定するための引数_block_indexが必要だった
//-------------------------------------------------------------------------------------------------------------------------------------
void  DoIntersectEnemyToBlock(ENEMY* _enemy, BLOCK* _block , int _block_index) {
	if (_block->flag == true) {
		if (IntersectOneBlock(_enemy->bottom_left, _block, _block_index) == true || IntersectOneBlock(_enemy->bottom_right, _block, _block_index) == true) {
			_enemy->velocity.y = 0;
			_enemy->position.y = (float)((int)_enemy->position.y / PART_HEIGHT * PART_HEIGHT);
		}
		if (IntersectOneBlock(_enemy->right, _block, _block_index) == true) {
			_enemy->velocity.x = 0;
			_enemy->position.x = (float)((int)_enemy->position.x / PART_WIDTH * PART_WIDTH);
		}
		if (IntersectOneBlock(_enemy->left, _block, _block_index) == true) {
			_enemy->velocity.x = 0;
			_enemy->position.x = (float)(((int)_enemy->position.x / PART_WIDTH + 1) * PART_WIDTH);
		}
		if (IntersectOneBlock(_enemy->top, _block, _block_index) == true) {
			_enemy->velocity.y = 0;
			_enemy->position.y = (float)(((int)_enemy->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
		}
	}
}


//-------------------------------------------------------------------------------------------------------------------------------------
//指定したアイテムが、指定した（破壊できる）ブロックと干渉しているかを、オブジェクトの組合わせを指定して調べて、当たり判定を行う関数
//-------------------------------------------------------------------------------------------------------------------------------------
void  DoIntersectItemToBlock(ITEM* _item, BLOCK* _block, int _block_index) {
	if (_block->flag == true) {
		if (IntersectOneBlock(_item->bottom_left, _block, _block_index) == true || IntersectOneBlock(_item->bottom_right, _block, _block_index) == true) {
			_item->velocity.y = 0;
			_item->position.y = (float)((int)_item->position.y / PART_HEIGHT * PART_HEIGHT);
		}
		if (IntersectOneBlock(_item->right, _block, _block_index) == true) {
			_item->velocity.x = 0;
			_item->position.x = (float)((int)_item->position.x / PART_WIDTH * PART_WIDTH);
		}
		if (IntersectOneBlock(_item->left, _block, _block_index) == true) {
			_item->velocity.x = 0;
			_item->position.x = (float)(((int)_item->position.x / PART_WIDTH + 1) * PART_WIDTH);
		}
		if (IntersectOneBlock(_item->top, _block, _block_index) == true) {
			_item->velocity.y = 0;
			_item->position.y = (float)(((int)_item->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
		}
	}
}


//-------------------------------------------------------------------------------------------------------------------------------------
//指定したファイアボールが、指定した（破壊できる）ブロックと干渉しているかを、オブジェクトの組合わせを指定して調べて、当たり判定を行う関数
//-------------------------------------------------------------------------------------------------------------------------------------
void  DoIntersectFireBallToBlock(FIRE_BALL* _fire_ball, BLOCK* _block, int _block_index) {
	if (_block->flag == true) {
		if (IntersectOneBlock(_fire_ball->bottom_left, _block, _block_index) == true || IntersectOneBlock(_fire_ball->bottom_right, _block, _block_index) == true) {
			_fire_ball->velocity.y = -4.0f;
			_fire_ball->position.y = (float)((int)_fire_ball->position.y / PART_HEIGHT * PART_HEIGHT);
		}
		if (IntersectOneBlock(_fire_ball->right, _block, _block_index) == true) {
			InitFireBallInstance(_fire_ball);
			_fire_ball->position.x = (float)((int)_fire_ball->position.x / PART_WIDTH * PART_WIDTH);
		}
		if (IntersectOneBlock(_fire_ball->left, _block, _block_index) == true) {
			InitFireBallInstance(_fire_ball);
			_fire_ball->position.x = (float)(((int)_fire_ball->position.x / PART_WIDTH + 1) * PART_WIDTH);
		}
		if (IntersectOneBlock(_fire_ball->top, _block, _block_index) == true) {
			_fire_ball->velocity.y = 0;
			_fire_ball->position.y = (float)(((int)_fire_ball->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
		}
	}
}


//--------------------------------------------------------------------------------------------------
//クエスチョンボックスから出たコインと干渉しているか、また干渉しているならどのコインか調べる関数
//--------------------------------------------------------------------------------------------------
int IntersectCoin(glm::vec2 _position, int _index) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return -1;
	}
	if (sub_course[y][x] == 'C') {
		if (LetterToNum(sub_course[y - 1][x]) == _index + 1) {
			return _index; //要修正 return _index+1 でも同じ処理になるってどういうこと？？
		}
	}
	return -1;
}



//-----------------------------------------------------------------------------------
//フィールド上のコインと干渉しているか、また干渉しているならどのコインか調べる関数
//-----------------------------------------------------------------------------------
int IntersectFieldCoin(glm::vec2 _position, int _index, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) { //地上
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return -1;
		}
		if (sub_course[y][x] == 'D') {
			if (LetterToNum(sub_course[y - 1][x]) == _index + 1) {
				return _index;
			}
		}
		return -1;
	}
	else if (_isInGround == true) { //地上
		if ((x < 0) || (x >= GROUND_COURSE_WIDTH) || (y < 0) || (y >= GROUND_COURSE_HEIGHT)) {
			return -1;
		}
		if (ground_sub_course[y][x] == 'D') {
			if (LetterToNum(ground_sub2_course[y][x]) == _index + 1) {
				return _index;
			}
		}
		return -1;
	}
}


//---------------------------------------------------------------------------------
//どのクエスチョンボックスをたたいたかどうか調べる関数、たたいてなかったら-1を返す
//---------------------------------------------------------------------------------
int IsGetQuestionbox(glm::vec2 _position, int _index) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return -1;
	}
	if (course[y][x] == 'q') {
		if (LetterToNum(sub2_course[y][x]) == _index + 1) {
			return _index;
		}
	}
	return -1;
}


//---------------------------------------------------------------------------------
//どのブロックをたたいたかどうか調べる関数、たたいてなかったら-1を返す
//---------------------------------------------------------------------------------
int IsHitBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return -1;
	}
	if (_block[_index]->flag == true) {
		if (course[y][x] == 'b') {
			if (sub_course[y][x] == '2') {
				if (LetterToNum(sub2_course[y][x]) == _index + 1) {
					return _index;
				}
			}
		}
	}
	return -1;
}


//---------------------------------------------------------------------------------
//どのコインブロックをたたいたかどうか調べる関数
//---------------------------------------------------------------------------------
int IsHitCoinBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return -1;
	}
	if (_block[_index]->flag == true) {
		if (course[y][x] == 'b') {
			if (sub_course[y][x] == '3') {
				if (LetterToNum(sub2_course[y][x]) == _index + 1) {
					return _index;
				}
			}
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------------
//敵との当たり判定において、敵の左右上下のどこが当たったかを調べる関数
//実際は敵の左右上下どの位置にいるかを返している
//クリボーなどの他の敵にも適応可能にしてある
//-----------------------------------------------------------------------------------
void SearchIntersectEnemyPoint(glm::vec2 _position, int _index, PLAYER _player, std::vector<ENEMY*> _enemy) {
	if (_position.x + _enemy[_index]->width <= _enemy[_index]->position.x) {
		_enemy[_index]->intersect_left_flag = true;
		_enemy[_index]->intersect_right_flag = false;
		_enemy[_index]->intersect_top_flag = false;
		_enemy[_index]->intersect_bottom_flag = false;
	}
	else if (_position.x >= _enemy[_index]->position.x + _enemy[_index]->width) {
		_enemy[_index]->intersect_left_flag = false;
		_enemy[_index]->intersect_right_flag = true;
		_enemy[_index]->intersect_top_flag = false;
		_enemy[_index]->intersect_bottom_flag = false;
	}
	else if (_position.y + (_player.isBig + 1) * PLAYER_HEIGHT <= _enemy[_index]->position.y) { //_player.isBigに+1しておかないと、大きい状態で立っているだけで敵を踏んでしまう
		_enemy[_index]->intersect_left_flag = false;
		_enemy[_index]->intersect_right_flag = false;
		_enemy[_index]->intersect_top_flag = true;
		_enemy[_index]->intersect_bottom_flag = false;
	}
	//「敵の下に位置している」と判定する範囲を狭めておかないと、ブロックが二段あるときに、下の段のブロックをたたいた時にも、上の段にいる敵が死んでしまう
	else if (_position.y >= _enemy[_index]->position.y + _player.isBig * PLAYER_HEIGHT && _position.y <= _enemy[_index]->position.y + _player.isBig * PLAYER_HEIGHT + PART_HEIGHT*2) {
		_enemy[_index]->intersect_left_flag = false;
		_enemy[_index]->intersect_right_flag = false;
		_enemy[_index]->intersect_top_flag = false;
		_enemy[_index]->intersect_bottom_flag = true;
	}
}


//----------------------------------------------------------------------------------------------
//プレイヤーの敵との当たり判定を調べる関数、当たっているならどの敵かを返す
//----------------------------------------------------------------------------------------------
int IntersectEnemy(int _index, PLAYER _player, std::vector<ENEMY*> _enemy) {
	if ((_player.position.x + _enemy[_index]->width >= _enemy[_index]->position.x)
		&& (_player.position.x <= _enemy[_index]->position.x + _enemy[_index]->width)
		&& (_player.position.y + _player.isBig * PLAYER_WIDTH >= _enemy[_index]->position.y)
		&& (_player.position.y <= _enemy[_index]->position.y + _enemy[_index]->height))
	{
		return _index;
	}
	else {
		return -1;
	}
}


//-----------------------------------------------------------------------------------
//敵同士の当たり判定において、敵の左右上下のどの位置にいるかを調べる関数
//_enemy1を基準として、_enemy2に対してどの位置にいるかを返す
//-----------------------------------------------------------------------------------
int SearchIntersectEnemyPointToEnemy(ENEMY* _enemy1, ENEMY* _enemy2) {
	//位置関係変更の境界が、敵の周囲だと、敵が重なっているときにうまく処理されないため、中心座標の関係を返すことにする
	if (_enemy1->position.x + _enemy1->width / 2.0f <= _enemy2->position.x) { //_enemy1が_enemy2よりも左にいる時
		return LEFT;
	}
	else if (_enemy1->position.x >= _enemy2->position.x + _enemy2->width / 2.0f) { //_enemy1が_enemy2よりも右にいる時
		return RIGHT;
	}
	else if (_enemy1->position.y + _enemy1->height / 2.0f <= _enemy2->position.y) { //_enemy1が_enemy2よりも上にいる時
		return TOP;
	}
	else if (_enemy1->position.y >= _enemy2->position.y + _enemy1->height / 2.0f) { //_enemy1が_enemy2よりも下にいる時
		return BOTTOM;
	}
}


//----------------------------------------------------------------------------------------------
//敵同士の当たり判定を調べる関数、当たっているならどの敵かを返す
//----------------------------------------------------------------------------------------------
bool IntersectEnemyToEnemy(ENEMY* _enemy1, ENEMY* _enemy2) {
	if ((_enemy1->position.x + _enemy2->width >= _enemy2->position.x)
		&& (_enemy1->position.x <= _enemy2->position.x + _enemy1->width)
		&& (_enemy1->position.y + _enemy1->height >= _enemy2->position.y)
		&& (_enemy1->position.y <= _enemy2->position.y + _enemy2->height))
	{
		return true;
	}
	else {
		return false;
	}
}


//----------------------------------------------------------------------------------------------
//敵同士の当たり判定処理を行う関数
//----------------------------------------------------------------------------------------------
void  DoIntersectEnemyToEnemy(ENEMY* _enemy1, ENEMY* _enemy2, int _obj1, int _obj2) {
	switch (_obj1) {

	case KURIBOO_INTERSECT :

		//クリボー同士の干渉
		if (_obj2 == KURIBOO_INTERSECT) {
			if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == LEFT) { //_enemy2の左に_enmey1がいる場合
				if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
					_enemy1->position.x = _enemy2->position.x - _enemy1->width;
					_enemy1->isMirror = false;
				}
			}
			else if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == RIGHT) {
				if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
					_enemy1->position.x = _enemy2->position.x + _enemy1->width;
					_enemy1->isMirror = true;
				}
			}
		}

		//クリボーとノコノコの干渉
		if (_obj2 == NOKONOKO_INTERSECT) {
			//通常の状態のノコノコ、または、止まっている甲羅と干渉したとき
			if (_enemy2->stepped_on_flag == false || ((_enemy2->stepped_on_flag == true) && (_enemy2->isShellMoving == false) )) { 
				if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == LEFT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						_enemy1->position.x = _enemy2->position.x - _enemy1->width;
						_enemy1->isMirror = false;
					}
				}
				else if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == RIGHT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						_enemy1->position.x = _enemy2->position.x + _enemy1->width;
						_enemy1->isMirror = true;
					}
				}
				else if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == TOP) {}
				else if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == BOTTOM) {}
			}
			else if (_enemy2->stepped_on_flag == true && _enemy2->isShellMoving == true) { //動いている甲羅状態のノコノコと干渉した時
				if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == LEFT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						if (_enemy1->isDeath == false) {
							audioPlay(SE_ENEMY_DEATH);
						}
						_enemy1->isDeath = true; //クリボーが死ぬ
					}
				}
				else if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == RIGHT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						if (_enemy1->isDeath == false) {
							audioPlay(SE_ENEMY_DEATH);
						}
						_enemy1->isDeath = true; //クリボーが死ぬ
					}
				}
			}
		}

		break;

	case NOKONOKO_INTERSECT :

		//ノコノコとクリボーの干渉
		if (_obj2 == KURIBOO_INTERSECT) {
			//通常の状態のノコノコ、または、止まっている甲羅と干渉したとき
			if (_enemy1->stepped_on_flag == false || ((_enemy1->stepped_on_flag == true) && (_enemy1->isShellMoving == false))) {
				if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == LEFT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						_enemy1->position.x = _enemy2->position.x - _enemy1->width;
						_enemy1->isMirror = false;
					}
				}
				else if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == RIGHT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						_enemy1->position.x = _enemy2->position.x + _enemy1->width;
						_enemy1->isMirror = true;
					}
				}
			}

			else if (_enemy1->stepped_on_flag == true && _enemy1->isShellMoving == true) { //動いている甲羅状態のノコノコと干渉した時
				if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == LEFT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						//ノコノコ側は動いている甲羅なので、特に何もしない
					}
				}
				else if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == RIGHT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {

					}
				}
			}
		}
		break;
	}
}

//----------------------------------------------------------------------------------------------
//ファイアボールの敵との当たり判定を調べる関数、当たっているならどの敵かを返す
//----------------------------------------------------------------------------------------------
int IntersectEnemyFireBall(int _index, FIRE_BALL* _fire_ball, std::vector<ENEMY*> _enemy) {
	if ((_fire_ball->position.x + _enemy[_index]->width >= _enemy[_index]->position.x)
		&& (_fire_ball->position.x <= _enemy[_index]->position.x + _enemy[_index]->width)
		&& (_fire_ball->position.y + PART_WIDTH / 2.0f >= _enemy[_index]->position.y)
		&& (_fire_ball->position.y <= _enemy[_index]->position.y + _enemy[_index]->height))
	{
		return _index;
	}
	else {
		return -1;
	}
}


//----------------------------------------------------------------------------------------------
//アイテムとの当たり判定を調べる関数、当たっているならどのアイテムかを返す
//キノコ、ファイヤーフラワーなど、引数にアイテムの種類を入れることができるようにする
//アイテムの場合は、当たった位置によって処理を変える必要はない？
//(プレイヤーの左右上下のどこが当たったかによって処理を変える可能性があるため、_positonを渡す)
//----------------------------------------------------------------------------------------------
int IntersectItem(int _index, PLAYER _player, std::vector<ITEM*> _item) {
	if ((_player.position.x + ITEM_WIDTH >= _item[_index]->position.x)
		&& (_player.position.x <= _item[_index]->position.x + ITEM_WIDTH)
		&& (_player.position.y + _player.isBig * PLAYER_HEIGHT >= _item[_index]->position.y)
		&& (_player.position.y <= _item[_index]->position.y + PART_HEIGHT)) //とりあえず、ブロックの下からアイテムを取らないように修正（適切かどうか微妙）
	{
		return _index;
	}
	else {
		return -1;
	}
}


//--------------------------------------------------
//ゴールポールとの当たり判定を調べる関数 
//--------------------------------------------------
int IntersectGoal(glm::vec2 _position) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return 0;
	}
	if (course[y][x] == 'g') {
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------
//引数１を基準として、引数２に干渉した時の処理を記述する関数
//オブジェクトのインスタンスはすべて引数として渡す
//loopでは、forループの回数を渡す
//引数としてplayerを渡して、そのメンバ変数を変更したいときはポインタで渡す必要がある
//他の関数で参照渡しでうまくいっているのは、メンバ変数の値を取得するだけで、変更を加えようとしているわけではないため
//-------------------------------------------------------------------------------------------------------------------------
void DoIntersectDetection(int _obj1, int _obj2, int _loop1, int _loop2, bool _isInGround, 
	PLAYER* _player, std::vector<ENEMY*> _enemy1, std::vector<ENEMY*> _enemy2,
	std::vector<ITEM*> _item, std::vector<FIRE_BALL*> _fire_ball, std::vector<BLOCK*> _block, std::vector<QUESTION*> _question) {
	switch (_obj1) {
		//プレイヤーに関する当たり判定
	case PLAYER_INTERSECT:
		//プレイヤーのコースパーツとの当たり判定
		if (_obj2 == PART_INTERSECT) {
			if (IntersectField(_player->bottom, _player)) {
				_player->velocity.y = 0;
				_player->position.y = (float)((int)_player->position.y / PART_HEIGHT * PART_HEIGHT); //キャラがブロックの上に立てるようにする
				_player->isJumping = false;
			}
			else {
				_player->isJumping = true;
			}
			if (IntersectField(_player->right, _player)) {
				_player->velocity.x = 0;
				_player->position.x = (float)((int)_player->position.x / PART_WIDTH * PART_WIDTH);
			}
			if (IntersectField(_player->left, _player)) {
				_player->velocity.x = 0;
				_player->position.x = (float)(((int)_player->position.x / PART_WIDTH + 1) * PART_WIDTH);
			}
			if (IntersectField(_player->top, _player)) {
				_player->velocity.y = 0;
				_player->position.y = (float)(((int)_player->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				_player->isJumping = false;
			}
		}
		//プレイヤーの（壊せる）ブロックとの当たり判定
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			for (int i = 0; i < _loop2; i++) {
				if (_block[i]->flag == true) {
					if (IntersectBlock(_player->bottom, i, _block, _isInGround) == i) {
						_player->velocity.y = 0;
						_player->position.y = (float)((int)_player->position.y / PART_HEIGHT * PART_HEIGHT); //キャラがブロックの上に立てるようにする
						_player->isJumping = false;
					}
					else {
						//_player->isJumping = true; //要修正　壊せるブロックの上にいる問、アニメーションがジャンプアニメーションになってしまう
						//_player->animation = ANIMATION_STAND; //もっと適切な修正案があるはず・・・
					}
					if (IntersectBlock(_player->right, i, _block, _isInGround) == i) {
						_player->velocity.x = 0;
						_player->position.x = (float)((int)_player->position.x / PART_WIDTH * PART_WIDTH);
					}
					if (IntersectBlock(_player->left, i, _block, _isInGround) == i) {
						_player->velocity.x = 0;
						_player->position.x = (float)(((int)_player->position.x / PART_WIDTH + 1) * PART_WIDTH );
					}
					if (IntersectBlock(_player->top, i, _block, _isInGround) == i) {
						_player->velocity.y = 0;
						_player->position.y = (float)(((int)_player->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
						_player->isJumping = false;
					}
				}
			}
		}
		//プレイヤーのコインブロックとの当たり判定
			if (_obj2 == COIN_BLOCK_INTERSECT) {
				for (int i = 0; i < _loop2; i++) {
					//if (_block[i]->flag == true) { コインブロックは破壊されることがなく、当たり判定が消えることはない
						if (IntersectCoinBlock(_player->bottom, i, _block, _isInGround) == i) {
							_player->velocity.y = 0;
							_player->position.y = (float)((int)_player->position.y / PART_HEIGHT * PART_HEIGHT); //キャラがブロックの上に立てるようにする
							_player->isJumping = false;
						}
						else {
							//_player->isJumping = true; //要修正　壊せるブロックの上にいる問、アニメーションがジャンプアニメーションになってしまう
							//_player->animation = ANIMATION_STAND; //もっと適切な修正案があるはず・・・
						}
						if (IntersectCoinBlock(_player->right, i, _block, _isInGround) == i) {
							_player->velocity.x = 0;
							_player->position.x = (float)((int)_player->position.x / PART_WIDTH * PART_WIDTH);
						}
						if (IntersectCoinBlock(_player->left, i, _block, _isInGround) == i) {
							_player->velocity.x = 0;
							_player->position.x = (float)(((int)_player->position.x / PART_WIDTH + 1) * PART_WIDTH);
						}
						if (IntersectCoinBlock(_player->top, i, _block, _isInGround) == i) {
							_player->velocity.y = 0;
							_player->position.y = (float)(((int)_player->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
							_player->isJumping = false;
						}
					//}
				}
			}
		//プレイヤーのエネミーとの当たり判定
		if (_obj2 == ENEMY_INTERSECT) {
			for (int i = 0; i < ENEMY_NUM; i++) {
				SearchIntersectEnemyPoint(_player->position, i, *_player, _enemy1);

				if (IntersectEnemy(i, *_player, _enemy1) == i) {
					if (_enemy1[i]->intersect_left_flag == true) {
						_player->velocity.x = 0;
						_player->position.x = _enemy1[i]->position.x - 28 * ENEMY_WIDTH / 32.0f;
					}
					else if (_enemy1[i]->intersect_right_flag == true) {
						_player->velocity.x = 0;
						_player->position.x = _enemy1[i]->position.x + 28 * ENEMY_WIDTH / 32.0f;
					}
					else if (_enemy1[i]->intersect_top_flag == true) {
						_player->velocity.y = 0;
						_player->position.y = _enemy1[i]->position.y - _player->isBig * PLAYER_HEIGHT;
					}
					else if (_enemy1[i]->intersect_bottom_flag == true) {
						_player->velocity.y = 0;
						_player->position.y = _enemy1[i]->position.y + _player->isBig * PLAYER_HEIGHT;
					}
					_enemy1[i]->animation = ENEMY_ANIMATION_ATTACK;
					_player->animation = ANIMATION_DAMAGE;
					_player->isBig = 1;
				}
			}
		}
		//プレイヤーのクリボーとの当たり判定
		if (_obj2 == KURIBOO_INTERSECT) {
			if (_player->isInGround == false) {
				for (int i = 0; i < KURIBOO_NUM; i++) {
					if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //踏まれて消えた後と、死んだ後は以下の当たり判定をおこなう必要はない
						SearchIntersectEnemyPoint(_player->position, i, *_player, _enemy1);

						if (IntersectEnemy(i, *_player, _enemy1) == i) {
							if (_enemy1[i]->intersect_left_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->position.x = _enemy1[i]->position.x - KURIBOO_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_right_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->position.x = _enemy1[i]->position.x + KURIBOO_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_top_flag == true) { //敵を踏むということは、敵の頭と干渉することに注意する
								if (_player->isStar == false) { //スター状態でないとき
									if (_enemy1[i]->stepped_on_flag == false) {
										_player->velocity.y = -8;
										audioPlay(SE_STEP_ON);
										_enemy1[i]->stepped_on_flag = true; //踏まれたフラグを立てる
									}
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_bottom_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->velocity.y = 0;
									_player->position.y = _enemy1[i]->position.y + _player->isBig * PLAYER_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
						}
						if (_enemy1[i]->intersect_bottom_flag == true) { //ブロックをはさんで下から突き上げた時の処理
							for (int j = 0; j < BLOCK_NUM; j++) {
								if (IsHitBlock(_player->top, j, _block) == j) {
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH); //敵が死んだときの音を鳴らす
								}
							}
							for (int j = 0; j < QUESTION_BOX_NUM; j++) { //要修正　アイテムが出た後の空のクエスチョンボックスでも敵を倒せてしまう
								if (_question[j]->flag == true) {
									if (IsGetQuestionbox(_player->top, j) == j) {
										_enemy1[i]->isDeath = true;
										audioPlay(SE_ENEMY_DEATH); //敵が死んだときの音を鳴らす
									}
								}
							}
						}
					}
				}
			}
		}
		//プレイヤーのノコノコとの当たり判定
		if (_obj2 == NOKONOKO_INTERSECT) {
			for (int i = 0; i < NOKONOKO_NUM; i++) {
				if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //踏まれて消えた後と、死んだ後は以下の当たり判定をおこなう必要はない
					SearchIntersectEnemyPoint(_player->position, i, *_player, _enemy1);

					if (IntersectEnemy(i, *_player, _enemy1) == i) {
						if (_enemy1[i]->stepped_on_flag == false) { //通常状態のノコノコとの干渉
							if (_enemy1[i]->intersect_left_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->position.x = _enemy1[i]->position.x - NOKONOKO_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_right_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->position.x = _enemy1[i]->position.x + NOKONOKO_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_top_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->velocity.y = -8.0f;
									_player->position.y = _enemy1[i]->position.y - _player->isBig * PLAYER_HEIGHT;
									audioPlay(SE_STEP_ON);
									_enemy1[i]->stepped_on_flag = true; //踏まれたフラグを立てる
									_enemy1[i]->width = NOKONOKO_SHELL_WIDTH; //幅を甲羅の幅に更新
									_enemy1[i]->height = NOKONOKO_SHELL_HEIGHT; //高さを甲羅の高さに更新
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}

							}
							else if (_enemy1[i]->intersect_bottom_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->velocity.y = 0;
									_player->position.y = _enemy1[i]->position.y + _player->isBig * PLAYER_HEIGHT;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
						}

						else if (_enemy1[i]->stepped_on_flag == true) { //踏まれた状態のノコノコとの干渉
							if (_enemy1[i]->intersect_left_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->position.x = _enemy1[i]->position.x - NOKONOKO_WIDTH;

									if (_enemy1[i]->isShellMoving == false) { //甲羅が止まっている時に干渉した場合の処理
										_enemy1[i]->touch_left_flag = true;
										_enemy1[i]->touch_right_flag = false; //同時に両側のフラグが立たないようにする
										_enemy1[i]->isMirror = true; //敵キャラは左方向が正なので、反対方向に進むときはMirrorフラグを立てる（そうしないと反射がうまくいかない）
										_enemy1[i]->isShellMoving = true;
									}
									else if (_enemy1[i]->isShellMoving == true) { //甲羅が動いている時に干渉した場合の処理
										_player->animation = ANIMATION_DAMAGE;
										_player->isBig = 1;
										_player->isFire = false;
									}
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_right_flag == true) { //甲羅が止まっている時に干渉した場合の処理
								if (_player->isStar == false) { //スター状態でないとき
									_player->position.x = _enemy1[i]->position.x + NOKONOKO_WIDTH;

									if (_enemy1[i]->isShellMoving == false) {
										_enemy1[i]->touch_right_flag = true;
										_enemy1[i]->touch_left_flag = false;
										_enemy1[i]->isMirror = false;
										_enemy1[i]->isShellMoving = true;
									}
									else if (_enemy1[i]->isShellMoving == true) { //甲羅が動いている時に干渉した場合の処理
										_player->animation = ANIMATION_DAMAGE;
										_player->isBig = 1;
										_player->isFire = false;
									}
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_top_flag == true) { //要修正　敵の左上なのか右上なのかで甲羅をはじく方向を変えたい
								if (_player->isStar == false) { //スター状態でないとき
									_player->position.y = _enemy1[i]->position.y - _player->isBig * PLAYER_HEIGHT;

									if (_enemy1[i]->isShellMoving == false) { //甲羅が止まっている時に干渉した場合の処理
										_enemy1[i]->touch_top_flag = true;
										_enemy1[i]->isMirror = true;
										_enemy1[i]->isShellMoving = true;
									}
									else if (_enemy1[i]->isShellMoving == true && _enemy1[i]->isMirror == false) { //動いている甲羅を上から踏んだ時の処理
										//_enemy1[i]->isMirror == falseがないと、止まっている甲羅を踏んだ瞬間にこっちのループ処理も行ってしまい、うまくいかない。
										//動いている甲羅を踏むときは、どこかで跳ね返ってきた状態で踏むことに着目し、この条件を加えた。
										_player->velocity.y = -8.0f;
										//_player->position.y = _enemy1[i]->position.y - _player->isBig * PLAYER_HEIGHT;
										audioPlay(SE_STEP_ON);
										_enemy1[i]->isShellMoving = false; //甲羅の動きを止める
										double tmp = _enemy1[i]->position.x; //踏まれた瞬間の甲羅の位置を保持
										_enemy1[i]->position.x = tmp; //甲羅の位置を、踏まれた位置で止める
									}
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}

							}
							else if (_enemy1[i]->intersect_bottom_flag == true) {
								if (_player->isStar == false) { //スター状態でないとき
									_player->velocity.y = 0;
									_player->position.y = _enemy1[i]->position.y + _player->isBig * PLAYER_HEIGHT;
								}
								else if (_player->isStar == true) { //スター状態の時
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
						}
					}
				}
			}
		}
		//プレイヤーのスーパーキノコとの当たり判定
		if (_obj2 == MUSHROOM_INTERSECT) {
			for (int i = 0; i < MUSHROOM_NUM; i++) {
				if (_item[i]->display_flag == true) { //取った後は当たり判定を消す
					if (_item[i]->get_flag == false) {
						if (IntersectItem(i, *_player, _item) == i) {
							_player->position.y = _player->position.y - PART_HEIGHT / 100.0f; //キノコがすぐに消えるなら問題ないが、キノコの上を跳ねるような描画になってしまう
																				//プレイヤーのポジションを少し浮かせて置かないと、大きくなった時に地面に埋まってしまう
							_player->isBig = 2;
							_item[i]->get_flag = true;
							audioPlay(SE_GET_MUSHROOM);
						}
					}
				}
			}
		}
		//プレイヤーの1UPキノコとの当たり判定
		if (_obj2 == MUSHROOM_1UP_INTERSECT) {
			for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
				if (_item[i]->display_flag == true) { //取った後は当たり判定を消す
					if (_item[i]->get_flag == false) {
						if (IntersectItem(i, *_player, _item) == i) {
							_item[i]->get_flag = true;
							audioPlay(SE_1UP);
						}
					}
				}
			}
		}
		//プレイヤーのファイアフラワーとの当たり判定
		if (_obj2 == FIRE_FLOWER_INTERSECT) {
			for (int i = 0; i < FIRE_FLOWER_NUM; i++) {
				if (_item[i]->display_flag == true) { //取った後は当たり判定を消す
					if (_item[i]->get_flag == false) {
						if (IntersectItem(i, *_player, _item) == i) {
							_player->position.y = _player->position.y - PART_HEIGHT / 100.0f;
							_player->isBig = 2;
							_player->isFire = true;
							_item[i]->get_flag = true;
							audioPlay(SE_GET_MUSHROOM); //スーパーキノコを取った時とファイアフラワーを取った時は同じ音
						}
					}
				}
			}
		}
		//プレイヤーのスターとの当たり判定
		if (_obj2 == STAR_INTERSECT) {
			for (int i = 0; i < STAR_NUM; i++) {
				if (_item[i]->display_flag == true) { //取った後は当たり判定を消す
					if (_item[i]->get_flag == false) {
						if (IntersectItem(i, *_player, _item) == i) {
							//スターを取った時の処理をここに書く
							_player->isStar = true;
							_item[i]->get_flag = true;
							audioStop(MUSIC_STAGE); //ステージのBGMを止め、
							audioPlay(MUSIC_STAR); //スターのBGMを流す
						}
					}
				}
			}
		}
		//プレイヤーのゴールとの当たり判定
		if (_obj2 == GOAL_INTERSECT) {
			if (IntersectGoal(_player->right) == 1) {
				if (_player->isGoal == false) {
					_player->isGoal = true;
					//_player->animation = ANIMATION_GOAL;
					audioStop(MUSIC_STAGE);
					//audioPlay(SE_CLEAR);
				}
			}

			if (_player->isGoal == true) {
				if (_player->flag_pos.y <= 540) { //ゴールの旗が下にくるまでの間の処理
					if (_player->position.y < 495) {
						_player->animation = ANIMATION_GOAL;
						_player->velocity.y = 0.0f;
						_player->position.x = 8910.0f - 13.0f;
						_player->position.y += 1.2f;
					}
					else {
						audioPlay(SE_CLEAR);
						_player->animation = ANIMATION_GOAL;
						_player->position.y = 495;
						_player->tmp_goal_frame = _player->frame; //ゴールの旗が下につく直前のフレームを保持
					}
				}
				else { //ゴールの旗が下についた後の処理
					//今はプレイヤーは自由に動ける
					//キャラクターの動きを強制するならここに処理をかく
					if (_player->frame < _player->tmp_goal_frame + 60) { //60フレームの間だけ以下の処理を行う
						_player->animation = ANIMATION_GOAL;
						_player->isMirror = true;
						_player->position.x = 8910.0f + 13.0f;
						_player->position.y = 495;
					}
					else{
						//_player->whitch_course = 2;
						whitch_course = 12;
					}
				}
			}
		}
		break;

		//エネミーに関する当たり判定
	case ENEMY_INTERSECT:
		//エネミー自身の周囲との当たり判定、クリボー自身の周囲との当たり判定
		if (_obj2 == PART_INTERSECT) {
			for (int i = 0; i < _loop1; i++) {
				if (IntersectField(_enemy1[i]->left, _player)) {
					_enemy1[i]->position.x = (float)(((int)_enemy1[i]->position.x / PART_WIDTH + 1) * PART_WIDTH);
					_enemy1[i]->isMirror = true;
				}
				if (IntersectField(_enemy1[i]->right, _player)) {
					_enemy1[i]->position.x = (float)((int)_enemy1[i]->position.x / PART_WIDTH * PART_WIDTH);
					_enemy1[i]->isMirror = false;
				}
				if (IntersectField(_enemy1[i]->top, _player)) {
					_enemy1[i]->velocity.y = 0;
					_enemy1[i]->position.y = (float)(((int)_enemy1[i]->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				}
				if (IntersectField(_enemy1[i]->bottom_left, _player)) {
					_enemy1[i]->velocity.y = 0;
					_enemy1[i]->position.y = (float)((int)_enemy1[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
				if (IntersectField(_enemy1[i]->bottom_right, _player)) {
					_enemy1[i]->velocity.y = 0;
					_enemy1[i]->position.y = (float)((int)_enemy1[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
			}
		}
		//エネミーの（壊せる）ブロックとの当たり判定
		//ここの処理は全走査すると重くなるため、干渉する組み合わせのみ指定する
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			DoIntersectEnemyToBlock(_enemy1[1], _block[0], 0);
			DoIntersectEnemyToBlock(_enemy1[1], _block[1], 1);
			DoIntersectEnemyToBlock(_enemy1[1], _block[2], 2);
		}
		break;

	//クリボーに関する当たり判定
	case KURIBOO_INTERSECT:
		//クリボー自身の周囲との当たり判定は、エネミーの部分と同じであるため、引数を工夫して処理する

		//クリボーの（壊せる）ブロックとの当たり判定
		//要変更　重すぎる、個別に処理することで解決
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			DoIntersectEnemyToBlock(_enemy1[4], _block[3], 3);
			DoIntersectEnemyToBlock(_enemy1[4], _block[4], 4);
			DoIntersectEnemyToBlock(_enemy1[5], _block[3], 3);
			DoIntersectEnemyToBlock(_enemy1[5], _block[4], 4);
			DoIntersectEnemyToBlock(_enemy1[5], _block[5], 5);
			DoIntersectEnemyToBlock(_enemy1[5], _block[6], 6);
			DoIntersectEnemyToBlock(_enemy1[5], _block[7], 7);
			DoIntersectEnemyToBlock(_enemy1[5], _block[8], 8);
		}

		//クリボー同士の当たり判定
		if (_obj2 == KURIBOO_INTERSECT) {
			DoIntersectEnemyToEnemy(_enemy1[2], _enemy1[3], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[3], _enemy1[2], _obj1, _obj2);
		}

		//ノコノコとの当たり判定
		if (_obj2 == NOKONOKO_INTERSECT) {
			DoIntersectEnemyToEnemy(_enemy1[8], _enemy2[0], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[9], _enemy2[0], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[10], _enemy2[0], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[11], _enemy2[0], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[12], _enemy2[0], _obj1, _obj2);

			//DoIntersectEnemyToEnemy(_enemy1[15], _enemy2[2], _obj1, _obj2);
		}
		break;

		//ノコノコに関する当たり判定
	case NOKONOKO_INTERSECT:
		//ノコノコ自身の周囲との当たり判定は、エネミーの部分と同じであるため、引数を工夫して処理する

		//クリボーとの当たり判定
		if (_obj2 == KURIBOO_INTERSECT) {
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[8], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[9], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[10], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[11], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[12], _obj1, _obj2);

			//DoIntersectEnemyToEnemy(_enemy1[2], _enemy2[15], _obj1, _obj2);
		}

		break;

	//スーパーキノコに関する当たり判定
	case MUSHROOM_INTERSECT:
		//スーパーキノコ自身の周囲との当たり判定
		if (_obj2 == PART_INTERSECT) {
			for (int i = 0; i < MUSHROOM_NUM; i++) {
				if (IntersectField(_item[i]->left, _player)) {
					_item[i]->position.x = (float)(((int)_item[i]->position.x / PART_WIDTH + 1) * PART_WIDTH);
					_item[i]->isMirror = false;
				}
				if (IntersectField(_item[i]->right, _player)) {
					_item[i]->position.x = (float)((int)_item[i]->position.x / PART_WIDTH * PART_WIDTH);
					_item[i]->isMirror = true;
				}
				if (IntersectField(_item[i]->top, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)(((int)_item[i]->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				}
				if (IntersectField(_item[i]->bottom_left, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)((int)_item[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
				if (IntersectField(_item[i]->bottom_right, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)((int)_item[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
			}
		}
		//スーパーキノコと破壊できるブロックの当たり判定
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			DoIntersectItemToBlock(_item[0], _block[0], 0);
			DoIntersectItemToBlock(_item[0], _block[1], 1);
			DoIntersectItemToBlock(_item[0], _block[2], 2);
		}
		break;

	//1UPキノコに関する当たり判定
	case MUSHROOM_1UP_INTERSECT:
		//1UPキノコ自身の周囲との当たり判定
		if (_obj2 == PART_INTERSECT) {
			for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
				if (IntersectField(_item[i]->left, _player)) {
					_item[i]->position.x = (float)(((int)_item[i]->position.x / PART_WIDTH + 1) * PART_WIDTH);
					_item[i]->isMirror = false;
				}
				if (IntersectField(_item[i]->right, _player)) {
					_item[i]->position.x = (float)((int)_item[i]->position.x / PART_WIDTH * PART_WIDTH);
					_item[i]->isMirror = true;
				}
				if (IntersectField(_item[i]->top, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)(((int)_item[i]->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				}
				if (IntersectField(_item[i]->bottom_left, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)((int)_item[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
				if (IntersectField(_item[i]->bottom_right, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)((int)_item[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
			}
		}
		break;

	//スターに関する当たり判定
	case STAR_INTERSECT:
		///スター自身の周囲との当たり判定
		if (_obj2 == PART_INTERSECT) {
			for (int i = 0; i < STAR_NUM; i++) {
				if (IntersectField(_item[i]->left, _player)) {
					_item[i]->position.x = (float)(((int)_item[i]->position.x / PART_WIDTH + 1) * PART_WIDTH);
					_item[i]->isMirror = false;
				}
				if (IntersectField(_item[i]->right, _player)) {
					_item[i]->position.x = (float)((int)_item[i]->position.x / PART_WIDTH * PART_WIDTH);
					_item[i]->isMirror = true;
				}
				if (IntersectField(_item[i]->top, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)(((int)_item[i]->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				}
				if (IntersectField(_item[i]->bottom_left, _player)) {
					_item[i]->velocity.y = -8.0f;
					_item[i]->position.y = (float)((int)_item[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
				if (IntersectField(_item[i]->bottom_right, _player)) {
					_item[i]->velocity.y = -8.0f;
					_item[i]->position.y = (float)((int)_item[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
			}
		}
		///スターと破壊できるブロックの当たり判定
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			//DoIntersectItemToBlock(_item[0], _block[31], 31);
		}
		break;

	//ファイヤボールに関する当たり判定
	case FIRE_BALL_INTERSECT :
		//ファイヤボール自身の周囲との当たり判定
		if (_obj2 == PART_INTERSECT) {
			for (int i = 0; i < FIRE_BALL_NUM; i++) {
				if (IntersectField(_fire_ball[i]->left, _player)) {
					_fire_ball[i]->position.x = (float)(((int)_fire_ball[i]->position.x / PART_WIDTH + 1) * PART_WIDTH);
					InitFireBallInstance(_fire_ball[i]);
				}
				if (IntersectField(_fire_ball[i]->right, _player)) {
					_fire_ball[i]->position.x = (float)((int)_fire_ball[i]->position.x / PART_WIDTH * PART_WIDTH) + PART_WIDTH / 2.0f;
					InitFireBallInstance(_fire_ball[i]);
				}
				if (IntersectField(_fire_ball[i]->top, _player)) {
					_fire_ball[i]->velocity.y = 0;
					_fire_ball[i]->position.y = (float)(((int)_fire_ball[i]->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				}
				if (IntersectField(_fire_ball[i]->bottom, _player)) {
					_fire_ball[i]->velocity.y = -3.5f;
					_fire_ball[i]->position.y = (float)((int)_fire_ball[i]->position.y / PART_HEIGHT * PART_HEIGHT + PART_HEIGHT / 2.0f);
					//ファイアボールは他のパーツよりも小さいため、当たり判定の時の処理を調節するために+ PART_HEIGHT / 2.0fを加えている
				}
				if (IntersectField(_fire_ball[i]->bottom_left, _player)) {
					_fire_ball[i]->velocity.y = -3.5f;
					_fire_ball[i]->position.y = (float)((int)_fire_ball[i]->position.y / PART_HEIGHT * PART_HEIGHT + PART_HEIGHT / 2.0f);
				}
				if (IntersectField(_fire_ball[i]->bottom_right, _player)) {
					_fire_ball[i]->velocity.y = -3.5f;
					_fire_ball[i]->position.y = (float)((int)_fire_ball[i]->position.y / PART_HEIGHT * PART_HEIGHT + PART_HEIGHT / 2.0f);
				}
			}
		}
		//ファイアボールの（壊せる）ブロックとの当たり判定
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			for (int i = 0; i < FIRE_BALL_NUM; i++) {
				if (_fire_ball[i]->display_flag == true) {
					for (int j = 0; j < BLOCK_NUM; j++) {
						DoIntersectFireBallToBlock(_fire_ball[i], _block[j], j);
					}
				}
			}
		}
		//ファイアボールのクリボー（ノコノコ）との当たり判定
		if (_obj2 == KURIBOO_INTERSECT || _obj2 == NOKONOKO_INTERSECT) {
			for (int j = 0; j < FIRE_BALL_NUM /*_loop1*/; j++) {
				if (_fire_ball[j]->display_flag == true) {
					for (int i = 0; i < _loop2; i++) {
						if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //踏まれて消えた後と、死んだ後は以下の当たり判定をおこなう必要はない
							SearchIntersectEnemyPoint(_fire_ball[j]->position, i, *_player, _enemy1); //引数の*_playerは、実際にはここでは必要ない

							if (IntersectEnemyFireBall(i, _fire_ball[j], _enemy1) == i) {
								_fire_ball[j]->animation = FIRE_BALL_ANIMATION_BURST;
								_fire_ball[j]->isAttackEnemy = true; //これをtrueにすることで、ファイアボールへ重力を加えることをやめる
								_enemy1[i]->isDeath = true; //敵が死ぬ
								audioPlay(SE_ENEMY_DEATH); //敵が死んだときの音を鳴らす

								if (_enemy1[i]->intersect_left_flag == true) {
									_fire_ball[j]->position.x = _enemy1[i]->position.x - PART_WIDTH / 4.0f;
								}
								else if (_enemy1[i]->intersect_right_flag == true) {
									_fire_ball[j]->position.x = _enemy1[i]->position.x + PART_WIDTH / 4.0f;
								}
								else if (_enemy1[i]->intersect_top_flag == true) {
									_fire_ball[j]->position.y = _enemy1[i]->position.y + PART_WIDTH / 4.0f;
								}
								else if (_enemy1[i]->intersect_bottom_flag == true) {
									_fire_ball[j]->position.y = _enemy1[i]->position.y - PART_WIDTH / 4.0f;
								}
							}
						}
					}
				}
			}
		}
		break;
	}
	
	
}
