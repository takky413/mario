#include <vector>
#include "glm\glm.hpp"
#include "course.h"
#include "header.h"
#include "audio.h"
#include "instance.h"


//---------------------------------------------------------------------------------
//どのクエスチョンボックスをたたいたかどうか調べる関数、たたいてなかったら-1を返す
//---------------------------------------------------------------------------------
int IsGetQuestionbox_1_2(glm::vec2 _position, int _index) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return -1;
	}
	if (course_1_2[y][x] == 'q') {
		if (LetterToNum(sub2_course_1_2[y][x]) == _index + 1) {
			return _index;
		}
	}
	return -1;
}


//---------------------------------------------------------------------------------
//どのブロックをたたいたかどうか調べる関数、たたいてなかったら-1を返す
//---------------------------------------------------------------------------------
int IsHitBlock_1_2(glm::vec2 _position, int _index, std::vector<BLOCK*> _block) {
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


//----------------------------------------------------------------------------------------------
//アイテムとの当たり判定を調べる関数、当たっているならどのアイテムかを返す
//----------------------------------------------------------------------------------------------
int IntersectItem_1_2(int _index, PLAYER _player, std::vector<ITEM*> _item) {
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


//-----------------------------------------------------------------------------------
//敵との当たり判定において、敵の左右上下のどこが当たったかを調べる関数
//実際は敵の左右上下どの位置にいるかを返している
//クリボーなどの他の敵にも適応可能にしてある
//-----------------------------------------------------------------------------------
void SearchIntersectEnemyPoint_1_2(glm::vec2 _position, int _index, PLAYER _player, std::vector<ENEMY*> _enemy) {
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
	else if (_position.y >= _enemy[_index]->position.y + _player.isBig * PLAYER_HEIGHT && _position.y <= _enemy[_index]->position.y + _player.isBig * PLAYER_HEIGHT + PART_HEIGHT * 2) {
		_enemy[_index]->intersect_left_flag = false;
		_enemy[_index]->intersect_right_flag = false;
		_enemy[_index]->intersect_top_flag = false;
		_enemy[_index]->intersect_bottom_flag = true;
	}
}


//----------------------------------------------------------------------------------------------
//プレイヤーの敵との当たり判定を調べる関数、当たっているならどの敵かを返す
//----------------------------------------------------------------------------------------------
int IntersectEnemy_1_2(int _index, PLAYER _player, std::vector<ENEMY*> _enemy) {
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


//--------------------------------------------------------------------------
//入れる土管と干渉しているか調べる関数
//第二引数には当たり判定を行う箇所（left,right,top,botttomのどれかを入れる）
//--------------------------------------------------------------------------
bool IntersectPipe_1_2(glm::vec2 _position, int _intersect_poit, int _course) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_course == MAIN) {
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return false;
		}
		if (course_1_2[y][x] == 'p') { //土管か
			if (sub2_course_1_2[y][x] == 'i') {
				if (_intersect_poit == RIGHT) {
					return true;
				}
			}
		}
		return false;
	}
	else if (_course == START) {
		if ((x < 0) || (x >= START_COURSE_WIDTH_1_2) || (y < 0) || (y >= START_COURSE_HEIGHT_1_2)) {
			return false;
		}
		if (start_course_1_2[y][x] == 'p') { //土管か
			if (start_sub2_course_1_2[y][x] == 'i') {
				if (_intersect_poit == RIGHT) {
					return true;
				}
			}
		}
		return false;
	}
}


//----------------------------------------------------------------------------------------------
//ファイアボールの敵との当たり判定を調べる関数、当たっているならどの敵かを返す
//----------------------------------------------------------------------------------------------
int IntersectEnemyFireBall_1_2(int _index, FIRE_BALL* _fire_ball, std::vector<ENEMY*> _enemy) {
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


//-----------------------------------------------------------------------------------
//リフトとの当たり判定において、リフトの左右上下のどこが当たったかを調べる関数
//実際はリフトの上にいるかどうかを調べている。
//-----------------------------------------------------------------------------------
void SearchIntersectLiftPoint(glm::vec2 _position, int _index, PLAYER _player, std::vector<LIFT*> _lift) {

	if (_position.y + (_player.isBig + 1) * PLAYER_HEIGHT <= _lift[_index]->position.y) {
		_lift[_index]->intersect_top_flag = true;
	}

	if (_position.x + PLAYER_WIDTH <= _lift[_index]->position.x) {
		_lift[_index]->intersect_top_flag = false;
	}
	else if (_position.x >= _lift[_index]->position.x + LIFT_WIDTH) {
		_lift[_index]->intersect_top_flag = false;
	}
	else if (_position.y + (_player.isBig + 1) * PLAYER_HEIGHT <= _lift[_index]->position.y) { //_player.isBigに+1しておかないと、大きい状態で立っているだけで敵を踏んでしまう
		_lift[_index]->intersect_top_flag = true;
	}
	else if (_position.y >= _lift[_index]->position.y + LIFT_HEIGHT) {
		_lift[_index]->intersect_top_flag = false;
	}
}



//----------------------------------------
//リフトと干渉しているか調べる関数
//----------------------------------------
int IntersectLift(int _index, PLAYER _player, std::vector<LIFT*> _lift) {
	if ((_player.position.x + PLAYER_WIDTH >= _lift[_index]->position.x)
		&& (_player.position.x <= _lift[_index]->position.x + LIFT_WIDTH)
		&& (_player.position.y + _player.isBig * PLAYER_WIDTH >= _lift[_index]->position.y)
		&& (_player.position.y <= _lift[_index]->position.y + LIFT_HEIGHT))
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
int IntersectGoal_1_2(glm::vec2 _position) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return 0;
	}
	if (goal_course_1_2[y][x] == 'g') {
		return 1;
	}
	return 0;
}



//------------------------------------------------------
//フィールドのオブジェクトと干渉しているか調べる関数
//------------------------------------------------------
bool IntersectField_1_2(glm::vec2 _position, PLAYER* _player) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return false;
	}

	if (_player->course == MAIN) { //メインコースとの当たり判定
		switch (course_1_2[y][x]) {
		case 'b':
			//if (sub_course_1_2[y][x] == '0' || sub_course_1_2[y][x] == '1') {
			return true;
			//}
			break;
		case 'p':
			if (_player->inPipe == true) { //土管の上でしゃがんだ時の当たり判定を消す
				if (sub2_course_1_2[y][x] == 'i') { //要修正　このままだと、マリオが土管に入っている最中は、敵が土管をすり抜けてしまう
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
	else if (_player->course == START) { //スタートコースとの当たり判定
		switch (start_course_1_2[y][x]) {
		case 'p':
			if (_player->inPipe == true) { //土管の上でしゃがんだ時の当たり判定を消す
				if (start_sub2_course_1_2[y][x] == 'i') { //要修正　このままだと、マリオが土管に入っている最中は、敵が土管をすり抜けてしまう
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
		case 'b':
		case 'q':
		case 'f':
			return true;
		}
		return false;
	}
	else if (_player->course == GOAL) { //ゴールコースとの当たり判定
		switch (goal_course_1_2[y][x]) {
		case 'p':
			if (_player->inPipe == true) {
				if (start_sub2_course_1_2[y][x] == 'i') {
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
		case 'b':
		case 'q':
		case 'f':
			return true;
		}
		return false;
	}

}


//-------------------------------------
//当たり判定を実際に行う関数
//-------------------------------------
void DoIntersectDetection_1_2(int _obj1, int _obj2, int _loop1, int _loop2, bool _isInGround,
	PLAYER* _player, std::vector<ENEMY*> _enemy1, std::vector<ENEMY*> _enemy2,
	std::vector<ITEM*> _item, std::vector<FIRE_BALL*> _fire_ball, std::vector<BLOCK*> _block, std::vector<QUESTION*> _question,
	std::vector<LIFT*> _lift) {
	switch (_obj1) {
		//プレイヤーに関する当たり判定
	case PLAYER_INTERSECT:
		//プレイヤーのコースパーツとの当たり判定
		if (_obj2 == PART_INTERSECT) {
			if (IntersectField_1_2(_player->bottom, _player)) {
				_player->velocity.y = 0;
				_player->position.y = (float)((int)_player->position.y / PART_HEIGHT * PART_HEIGHT); //キャラがブロックの上に立てるようにする
				_player->isJumping = false;
			}
			else {
				_player->isJumping = true;
			}
			if (IntersectField_1_2(_player->right, _player)) {
				_player->velocity.x = 0;
				_player->position.x = (float)((int)_player->position.x / PART_WIDTH * PART_WIDTH);
			}
			if (IntersectField_1_2(_player->left, _player)) {
				_player->velocity.x = 0;
				_player->position.x = (float)(((int)_player->position.x / PART_WIDTH + 1) * PART_WIDTH);
			}
			if (IntersectField_1_2(_player->top, _player)) {
				_player->velocity.y = 0;
				_player->position.y = (float)(((int)_player->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				_player->isJumping = false;
			}
		}
		//プレイヤーのクリボーとの当たり判定
		if (_obj2 == KURIBOO_INTERSECT) {
			if (_player->course == MAIN) {
				for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
					if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //踏まれて消えた後と、死んだ後は以下の当たり判定をおこなう必要はない
						SearchIntersectEnemyPoint_1_2(_player->position, i, *_player, _enemy1);

						if (IntersectEnemy_1_2(i, *_player, _enemy1) == i) {
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
						/*
						if (_enemy1[i]->intersect_bottom_flag == true) { //ブロックをはさんで下から突き上げた時の処理
							for (int j = 0; j < BLOCK_NUM; j++) {
								if (IsHitBlock_1_2(_player->top, j, _block) == j) {
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH); //敵が死んだときの音を鳴らす
								}
							}
							for (int j = 0; j < QUESTION_BOX_NUM; j++) { //要修正　アイテムが出た後の空のクエスチョンボックスでも敵を倒せてしまう
								if (_question[j]->flag == true) {
									if (IsGetQuestionbox_1_2(_player->top, j) == j) {
										_enemy1[i]->isDeath = true;
										audioPlay(SE_ENEMY_DEATH); //敵が死んだときの音を鳴らす
									}
								}
							}
						}*/
					}
				}
			}
		}
		//プレイヤーのスーパーキノコとの当たり判定
		if (_obj2 == MUSHROOM_INTERSECT) {
			for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
				if (_item[i]->display_flag == true) { //取った後は当たり判定を消す
					if (_item[i]->get_flag == false) {
						if (IntersectItem_1_2(i, *_player, _item) == i) {
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
		//プレイヤーのファイアフラワーとの当たり判定
		if (_obj2 == FIRE_FLOWER_INTERSECT) {
			for (int i = 0; i < FIRE_FLOWER_NUM_1_2; i++) {
				if (_item[i]->display_flag == true) { //取った後は当たり判定を消す
					if (_item[i]->get_flag == false) {
						if (IntersectItem_1_2(i, *_player, _item) == i) {
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
		//プレイヤーのリフトとの当たり判定
		if (_obj2 == LIFT_INTERSECT) {
			for (int i = 0; i < LIFT_NUM; i++) {
				SearchIntersectLiftPoint(_player->position, i, *_player, _lift);

				if (IntersectLift(i, *_player, _lift) == i) {
					if (_lift[i]->intersect_top_flag == true) {
						_player->velocity.y = 0.8f; //リフトに乗っている時だけジャンプ力が低くなってしまう。
						_player->position.y = _lift[i]->position.y - _player->isBig * PLAYER_HEIGHT;
						_player->isJumping = false;
					}
				}
			}
		}
		//プレイヤーのゴールとの当たり判定
		if (_obj2 == GOAL_INTERSECT) {
			if (_player->course == GOAL) {
				if (IntersectGoal_1_2(_player->right) == 1) {
					if (_player->isGoal == false) {
						_player->isGoal = true;
						audioStop(MUSIC_STAGE);
					}
				}
				if (_player->isGoal == true) {
					if (_player->flag_pos.y <= 540) { //ゴールの旗が下にくるまでの間の処理
						if (_player->position.y < 495) {
							_player->animation = ANIMATION_GOAL;
							_player->velocity.y = 0.0f;
							_player->position.x = 990.0f - 13.0f;
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
							_player->position.x = 990.0f + 13.0f;
							_player->position.y = 495;
						}
						else {
						}
					}
				}
			}
		}
		break;
		//スーパーキノコに関する当たり判定
	case MUSHROOM_INTERSECT:
		//スーパーキノコ自身の周囲との当たり判定
		if (_obj2 == PART_INTERSECT) {
			for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
				if (IntersectField_1_2(_item[i]->left, _player)) {
					_item[i]->position.x = (float)(((int)_item[i]->position.x / PART_WIDTH + 1) * PART_WIDTH);
					_item[i]->isMirror = false;
				}
				if (IntersectField_1_2(_item[i]->right, _player)) {
					_item[i]->position.x = (float)((int)_item[i]->position.x / PART_WIDTH * PART_WIDTH);
					_item[i]->isMirror = true;
				}
				if (IntersectField_1_2(_item[i]->top, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)(((int)_item[i]->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				}
				if (IntersectField_1_2(_item[i]->bottom_left, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)((int)_item[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
				if (IntersectField_1_2(_item[i]->bottom_right, _player)) {
					_item[i]->velocity.y = 0;
					_item[i]->position.y = (float)((int)_item[i]->position.y / PART_HEIGHT * PART_HEIGHT);
				}
			}
		}
		//スーパーキノコと破壊できるブロックの当たり判定
		/*
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			DoIntersectItemToBlock(_item[0], _block[0], 0);
			DoIntersectItemToBlock(_item[0], _block[1], 1);
			DoIntersectItemToBlock(_item[0], _block[2], 2);
		}
		*/
		break;
		//ファイヤボールに関する当たり判定
	case FIRE_BALL_INTERSECT:
		//ファイヤボール自身の周囲との当たり判定
		if (_obj2 == PART_INTERSECT) {
			for (int i = 0; i < FIRE_BALL_NUM; i++) {
				if (IntersectField_1_2(_fire_ball[i]->left, _player)) {
					_fire_ball[i]->position.x = (float)(((int)_fire_ball[i]->position.x / PART_WIDTH + 1) * PART_WIDTH);
					InitFireBallInstance(_fire_ball[i]);
				}
				if (IntersectField_1_2(_fire_ball[i]->right, _player)) {
					_fire_ball[i]->position.x = (float)((int)_fire_ball[i]->position.x / PART_WIDTH * PART_WIDTH) + PART_WIDTH / 2.0f;
					InitFireBallInstance(_fire_ball[i]);
				}
				if (IntersectField_1_2(_fire_ball[i]->top, _player)) {
					_fire_ball[i]->velocity.y = 0;
					_fire_ball[i]->position.y = (float)(((int)_fire_ball[i]->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
				}
				if (IntersectField_1_2(_fire_ball[i]->bottom, _player)) {
					_fire_ball[i]->velocity.y = -3.5f;
					_fire_ball[i]->position.y = (float)((int)_fire_ball[i]->position.y / PART_HEIGHT * PART_HEIGHT + PART_HEIGHT / 2.0f);
					//ファイアボールは他のパーツよりも小さいため、当たり判定の時の処理を調節するために+ PART_HEIGHT / 2.0fを加えている
				}
				if (IntersectField_1_2(_fire_ball[i]->bottom_left, _player)) {
					_fire_ball[i]->velocity.y = -3.5f;
					_fire_ball[i]->position.y = (float)((int)_fire_ball[i]->position.y / PART_HEIGHT * PART_HEIGHT + PART_HEIGHT / 2.0f);
				}
				if (IntersectField_1_2(_fire_ball[i]->bottom_right, _player)) {
					_fire_ball[i]->velocity.y = -3.5f;
					_fire_ball[i]->position.y = (float)((int)_fire_ball[i]->position.y / PART_HEIGHT * PART_HEIGHT + PART_HEIGHT / 2.0f);
				}
			}
		}
		//ファイアボールのクリボー（ノコノコ）との当たり判定
		if (_obj2 == KURIBOO_INTERSECT || _obj2 == NOKONOKO_INTERSECT) {
			for (int j = 0; j < FIRE_BALL_NUM /*_loop1*/; j++) {
				if (_fire_ball[j]->display_flag == true) {
					for (int i = 0; i < _loop2; i++) {
						if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //踏まれて消えた後と、死んだ後は以下の当たり判定をおこなう必要はない
							SearchIntersectEnemyPoint_1_2(_fire_ball[j]->position, i, *_player, _enemy1); //引数の*_playerは、実際にはここでは必要ない

							if (IntersectEnemyFireBall_1_2(i, _fire_ball[j], _enemy1) == i) {
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
	//クリボーの関する当たり判定
	case KURIBOO_INTERSECT:
		//クリボー自身の周囲との当たり判定
		if (_obj2 == PART_INTERSECT) {
			if (_player->course == MAIN) {
				for (int i = 0; i < _loop1; i++) {
					if (IntersectField_1_2(_enemy1[i]->left, _player)) {
						_enemy1[i]->position.x = (float)(((int)_enemy1[i]->position.x / PART_WIDTH + 1) * PART_WIDTH);
						_enemy1[i]->isMirror = true;
					}
					if (IntersectField_1_2(_enemy1[i]->right, _player)) {
						_enemy1[i]->position.x = (float)((int)_enemy1[i]->position.x / PART_WIDTH * PART_WIDTH);
						_enemy1[i]->isMirror = false;
					}
					if (IntersectField_1_2(_enemy1[i]->top, _player)) {
						_enemy1[i]->velocity.y = 0;
						_enemy1[i]->position.y = (float)(((int)_enemy1[i]->position.y / PART_HEIGHT + 1) * PART_HEIGHT);
					}
					if (IntersectField_1_2(_enemy1[i]->bottom_left, _player)) {
						_enemy1[i]->velocity.y = 0;
						_enemy1[i]->position.y = (float)((int)_enemy1[i]->position.y / PART_HEIGHT * PART_HEIGHT);
					}
					if (IntersectField_1_2(_enemy1[i]->bottom_right, _player)) {
						_enemy1[i]->velocity.y = 0;
						_enemy1[i]->position.y = (float)((int)_enemy1[i]->position.y / PART_HEIGHT * PART_HEIGHT);
					}
				}
			}

		}
	}
}


