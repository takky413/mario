#include <vector>
#include "header.h"
#include "course.h"

//--------------------------------------------
//マリオのインスタンスを生成して初期化する
//--------------------------------------------
PLAYER* MakePlayerInstance(void) {

	PLAYER* _player = new PLAYER(); //メモリを確保する

	//インスタンスを作成すると同時に初期化
	_player->animation = ANIMATION_RUN;
	_player->frame = 0;
	_player->isBig = 1;
	_player->isRunning = 1;
	_player->isFire = false;
	_player->isStar = false;
	_player->isInGround = false;
	_player->current_fire_ball = 0;
	_player->fire_ball_counter = 0;
	_player->position = { WINDOW_WIDTH / 2.0f,  PART_HEIGHT * (13 - (_player->isBig - 1)) };
	//_player->position = { WINDOW_WIDTH / 2.0f + 630 ,  PART_HEIGHT * (13 - (_player->isBig - 1)) };//
	//_player->position = { 8000,  PART_HEIGHT * 5 }; //ゴールポールのデバック
	//_player->position = { 2000,  PART_HEIGHT * 2 }; 
	_player->isMirror = false;
	_player->isGoal = false;
	_player->flag_pos = { 8865 + 22.5, 180 };
	//_player->whitch_course = 11; //初期値を1-1に設定

	return _player;
}



//--------------------------------------------------------
//敵のインスタンスを生成する
//--------------------------------------------------------
std::vector<ENEMY*> MakeEnemyInstance(double _width, double _height, int _enemy_num) {

	std::vector<ENEMY*> _enemy;

	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < _enemy_num; i++) {
		ENEMY* hoge = new ENEMY();
		hoge->frame = 0;
		hoge->width = _width;
		hoge->height = _height;
		hoge->isMirror = false;
		hoge->touch_left_flag = false;
		hoge->touch_right_flag = false;
		hoge->touch_top_flag = false;
		hoge->isShellMoving = false;
		hoge->intersect_left_flag = false;
		hoge->intersect_right_flag = false;
		hoge->intersect_top_flag = false;
		hoge->intersect_bottom_flag = false;
		hoge->animation_flag = false;
		hoge->display_flag = true;
		hoge->isDeath = false;
		hoge->delta_psition_calculate_flag = true;
		_enemy.push_back(hoge);
	}

	return _enemy;
}


//--------------------------------------------------------
//キノコのインスタンスを生成する
//--------------------------------------------------------
std::vector<ITEM*> MakeMushroomInstance(int _loop) {

	std::vector<ITEM*> _mushroom;

	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < _loop; i++) {
		ITEM* hoge = new ITEM();
		hoge->isMirror = false;
		hoge->intersect_left_flag = false;
		hoge->intersect_right_flag = false;
		hoge->intersect_top_flag = false;
		hoge->intersect_bottom_flag = false;
		hoge->get_flag = false; //入手したかどうかのフラグ
		hoge->display_flag = false; //存在するかどうかのフラグ
		_mushroom.push_back(hoge);
	}

	return _mushroom;
}


//--------------------------------------------------------
//ファイアフラワーのインスタンスを生成する
//--------------------------------------------------------
std::vector<ITEM*> MakeFireFlowerInstance(int _loop) {

	std::vector<ITEM*> _fire_flower;

	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < _loop; i++) {
		ITEM* hoge = new ITEM();
		hoge->isMirror = false;
		hoge->intersect_left_flag = false;
		hoge->intersect_right_flag = false;
		hoge->intersect_top_flag = false;
		hoge->intersect_bottom_flag = false;
		hoge->get_flag = false; //入手したかどうかのフラグ
		hoge->display_flag = false; //存在するかどうかのフラグ
		_fire_flower.push_back(hoge);
	}

	return _fire_flower;
}


//--------------------------------------------------------
//スターのインスタンスを生成する
//--------------------------------------------------------
std::vector<ITEM*> MakeStarInstance(void) {

	std::vector<ITEM*> _star;

	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < STAR_NUM; i++) {
		ITEM* hoge = new ITEM();
		hoge->isMirror = false;
		hoge->intersect_left_flag = false;
		hoge->intersect_right_flag = false;
		hoge->intersect_top_flag = false;
		hoge->intersect_bottom_flag = false;
		hoge->get_flag = false; //入手したかどうかのフラグ
		hoge->display_flag = false; //存在するかどうかのフラグ
		_star.push_back(hoge);
	}

	return _star;
}



//--------------------------------------------------------
//ファイアボールのインスタンスを生成する
//--------------------------------------------------------
std::vector<FIRE_BALL*> MakeFireBallInstance(void) {

	std::vector<FIRE_BALL*> _fire_ball;

	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		FIRE_BALL* hoge = new FIRE_BALL();
		hoge->intersect_left_flag = false;
		hoge->intersect_right_flag = false;
		hoge->intersect_top_flag = false;
		hoge->intersect_bottom_flag = false;
		hoge->isMirror = false;
		hoge->display_flag = false; //存在するかどうかのフラグ
		_fire_ball.push_back(hoge);
	}

	return _fire_ball;
}


//---------------------------------------------------------------------------------------------------------
//ファイアボールのインスタンスを初期化する
//ボールが画面外に消えたら描画や当たり判定を停止し、初期化して使いまわすため。初期化関数を別に定義する
//---------------------------------------------------------------------------------------------------------
void InitFireBallInstance(FIRE_BALL* _fire_ball) {
	_fire_ball->intersect_left_flag = false;
	_fire_ball->intersect_right_flag = false;
	_fire_ball->intersect_top_flag = false;
	_fire_ball->intersect_bottom_flag = false;
	if (_fire_ball->isLookingLeft == true) {
		_fire_ball->isMirror = true;
	}
	else {
		_fire_ball->isMirror = false;
		_fire_ball->isLookingLeft = false;
	}
	_fire_ball->display_flag = false; //存在するかどうかのフラグ
	_fire_ball->isAttackEnemy = false;
	_fire_ball->frame = 0;
	_fire_ball->animation = FIRE_BALL_ANIMATION_BOUND;
}


//--------------------------------------------------------
//クエスチョンボックスのインスタンスを生成する
//--------------------------------------------------------
std::vector<QUESTION*> MakeQuestionInstance(int _loop) {

	std::vector<QUESTION*> _question;
	/*
	int counter = 0;
	for (int y = 0; y < COURSE_HEIGHT; y++) {
		for (int x = 0; x < COURSE_WIDTH; x++) {
			if (course[y][x] == 'q') {
				counter += 1;
			}
		}
	}
	*/
	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < _loop; i++) {
		QUESTION* hoge = new QUESTION();
		hoge->number = i;
		hoge->flag = true;
		hoge->sound_flag = true;
		_question.push_back(hoge);
	}

	return _question;
}


//--------------------------------------------------------
//(破壊できる)ブロックのインスタンスを生成する
//--------------------------------------------------------
std::vector<BLOCK*> MakeBlockInstance(int _loop) {

	std::vector<BLOCK*> _block;

	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < _loop; i++) {
		BLOCK* hoge = new BLOCK();
		hoge->number = i;
		hoge->flag = true; //存在するかどうかのフラグ
		hoge->sound_flag = true;
		hoge->delta_position = { 0, 0 }; //ブロックの位置の変化量
		hoge->fring_delta_position_1 = { 0, 0 };  //破片の位置の変化
		hoge->fring_delta_position_2 = { 0, 0 };  //破片の位置の変化
		hoge->fring_delta_position_3 = { 0, 0 };  //破片の位置の変化
		hoge->fring_delta_position_4 = { 0, 0 };  //破片の位置の変化
		hoge->animation_flag = false;
		hoge->reach_top_flag = false;
		hoge->fring_calculate_flag = true;
		_block.push_back(hoge);
	}

	return _block;
}


//--------------------------------------------------------
//コインが出るレンガブロックのインスタンスを生成する
//--------------------------------------------------------
std::vector<BLOCK*> MakeCoinBlockInstance(void) {

	std::vector<BLOCK*> _coin_block;

	int counter = 0;
	for (int y = 0; y < COURSE_HEIGHT; y++) {
		for (int x = 0; x < COURSE_WIDTH; x++) {
			if (course[y][x] == 'b') {
				if (sub_course[y][x] == '3') {
					counter += 1;
				}
			}
		}
	}
	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < counter; i++) {
		BLOCK* hoge = new BLOCK();
		hoge->number = i;
		hoge->flag = true; //存在するかどうかのフラグ
		hoge->sound_flag = true;
		hoge->delta_position = { 0, 0 }; //ブロックの位置の変化量
		hoge->fring_delta_position_1 = { 0, 0 };  //破片の位置の変化
		hoge->fring_delta_position_2 = { 0, 0 };  //破片の位置の変化
		hoge->fring_delta_position_3 = { 0, 0 };  //破片の位置の変化
		hoge->fring_delta_position_4 = { 0, 0 };  //破片の位置の変化
		hoge->animation_flag = false;
		hoge->reach_top_flag = false;
		hoge->fring_calculate_flag = true;
		hoge->haveCoinNum = 10;
		_coin_block.push_back(hoge);
	}

	return _coin_block;
}


//-----------------------------------------------------------------
//クエスチョンボックスの数だけコインのインスタンスを生成する
//-----------------------------------------------------------------
std::vector<COIN*> MakeCoinInstance(int _loop) {

	std::vector<COIN*> _coin;

	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < _loop; i++) {
		COIN* hoge = new COIN();
		hoge->number = i;
		hoge->flag = false;
		hoge->sub_flag = true;
		hoge->sound_flag = true;
		hoge->delta_pos = { 0, 0 };
		hoge->velocity = { 0, 0 };
		_coin.push_back(hoge);
	}

	return _coin;
}


//-----------------------------------------------------------------
//フィールド上のコインのインスタンスを生成する
//-----------------------------------------------------------------
std::vector<COIN*> MakeFieldCoinInstance(int _loop) {

	std::vector<COIN*> _field_coin;

	/*
	int counter = 0;
	for (int y = 0; y < COURSE_HEIGHT; y++) {
		for (int x = 0; x < COURSE_WIDTH; x++) {
			if (sub_course[y][x] == 'D') {
				counter += 1;
			}
		}
	}
	*/
	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < _loop; i++) {
		COIN* hoge = new COIN();
		hoge->number = i;
		hoge->flag = true;
		hoge->sub_flag = true;
		hoge->sound_flag = true;
		_field_coin.push_back(hoge);
	}

	return _field_coin;
}


//-----------------------------------------------------------------
//コインブロックの数だけコインのインスタンスを生成する
//-----------------------------------------------------------------
std::vector<COIN*> MakeBlockCoinInstance(void) {

	std::vector<COIN*> _block_coin;

	int counter = 0;
	for (int y = 0; y < COURSE_HEIGHT; y++) {
		for (int x = 0; x < COURSE_WIDTH; x++) {
			if (course[y][x] == 'b') {
				if (sub_course[y][x] == '3') {
					counter += 1;
				}
			}
		}
	}
	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < counter; i++) {
		COIN* hoge = new COIN();
		
		for (int j = 0; j < BLOCK_COIN_NUM; j++) {
			hoge->vector_frame.push_back(0);
			hoge->vector_flag.push_back(false); //false//まだメモリを確保していないため、hoge->vector_flag[j] = falseとしない。push_backで動的にメモリを確保する
			hoge->vector_sub_flag.push_back(true); //true
			hoge->vector_sound_flag.push_back(true); //true
			hoge->vector_delta_pos.push_back({ 0, 0 });
			hoge->vector_velocity.push_back({ 0, 0 });
		}
		
		_block_coin.push_back(hoge);
	}

	return _block_coin;
}


//------------------------------------------
//リフトのインスタンスを生成する
//------------------------------------------
std::vector<LIFT*> MakeLiftInstance(void) {

	std::vector<LIFT*> _lift;

	//インスタンスを作成すると同時に初期化
	for (int i = 0; i < LIFT_NUM; i++) {
		LIFT* hoge = new LIFT();
		
		//特に初期化する必要はない

		_lift.push_back(hoge);
	}

	return _lift;
}
