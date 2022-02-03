#include <stdio.h>
#include <vector>
#include <Windows.h>
#include <GL/glut.h>
#include <AL\alut.h>
#include "glm\glm.hpp"
#include "font.h"
#include "audio.h"
#include "course.h"
#include "header.h"
#include "instance.h"
#include "intersect.h"
#include "main1-2.h"
#include "intersect_1_2.h"

#include "voice_recognition.h"
#include <thread>


//メモ
//スターがクエスチョンボックスからではなく、レンガブロックから出てくるようにする、
//地上のレンガブロックと地下のレンガブロックが同じインスタンスになってしまっている（地上のブロックを破壊したら、地価も破壊された。）
// 

//改善した方がいいと思う部分
//処理ができるだけ軽くなるように、forループの順番を見直して、できるだけ走査が少ないようにするといい

//要修正
//隠しクエスチョンボックスをの当たり判定が、ずっとある
//プレイヤーの右側が何かと干渉していると時に、ファイアボールが出ない
//クリボー同士の当たり判定が、クリボーが死んだ後にも残ってしまう
//レンガブロックを下からたたいた時に、コースの最初の方のブロック(indexが１のブロック)だけ、マリオの表示に違和感がある
//地下のレンガブロックのインデックスの判定が、BLOCK_NUMに依存してしまっている
//ファイアボールが出てくる高さが毎回違う気がする


//---------------------------------------------
//実行時にオプションとして決めておく値
//---------------------------------------------
#define COURSE 1 //「１」なら1-1をプレイ、「２」なら2-2をプレイ //とりあえず今はheader.hに記述
#define COMMAND_MODE 1 //「１」ならキーボード入力、「２」なら音声入力


//--------------------------------------
//グローバル変数の定義
//--------------------------------------
int coin_flag = 0;
int coin_num = 0; //合計で何枚のコインを取ったか保持する

bool voice_command_flag; //音声認識による速度の加算を一回だけ行うためのフラグ

/*
int whitch_course = 11; //どのコースをプレイしているかを決める変数

void SetWhitchCourse(int _whitch_course) { //セッター
	whitch_course = _whitch_course;
}
*/

bool keysIsPressed[256];


//----------------------------------------------
//各構造体のインスタンスの生成
//----------------------------------------------
/*
ANIMATION animations[ANIMATION_MAX] = {
	{4, {0, 1, 2, 1}},// ANIMATION_STAND
	{2, {3, 4}},// ANIMATION_DAMAGE
	{8, {7, 8, 9, 10, 14, 15, 16, 17}},// ANIMATION_RUNG
	{12, {21, 22, 23, 24, 25, 26, 27, 26, 25, 24, 23, 22}}// ANIMATION_JUMP
};
*/


ANIMATION animations[ANIMATION_MAX] = {
	{4, {0, 0, 0, 0}},// ANIMATION_STAND
	{2, {0, 0}},// ANIMATION_DAMAGE
	{3, {1, 2, 3}},// ANIMATION_RUNG
	{2, {5, 5}},// ANIMATION_JUMP
	{2, {4, 4}}, //ANIMATION_TURN,
	{2, {8, 8}}, //ANIMATION_SQUAT,
	{2, {6,7}}, //ANIMATION_GOAL
	{3, {0, 1, 2}},// ANIMATION_STAR_STAND
	{3, {0, 1, 2}},// ANIMATION_STAR_DAMAGE
	{3, {3, 4, 5}},// ANIMATION_STAR_RUNG
	{3, {6, 7, 8}},// ANIMATION_STAR_JUMP
	{3, {9, 10, 11}}, //ANIMATION_STAR_TURN,
	{3, {12, 13, 14}} //ANIMATION_STAR_SQUAT,
};

ANIMATION coin_animations = {
	4, {0, 1, 2, 3}  // コインの回転
};

ANIMATION question_box_animations = {
	4, {0, 0, 1, 2} // クエスチョンボックスの点滅
};

ANIMATION enemy_animations[ENEMY_ANIMATION_MAX] = {
	4, {0, 0, 0, 1}, // ENEMY_ANIMATION_STAND
	3, {2, 2, 2} // ENEMY_ANIMATION_ATTACK
};

ANIMATION kuriboo_animations[KURIBOO_ANIMATION_MAX] = {
	4, {0, 1, 0, 1}, // KURIBOO_ANIMATION_STAND
	3, {2, 2, 2}, // KURIBOO_ANIMATION_CRUSHED
};
//注意！！ANIMATIONの要素数が一つだととなぜかエラーが起きる

ANIMATION nokonoko_animations[NOKONOKO_ANIMATION_MAX] = {
	4, {0, 1, 0, 1} // NOKONOKO_ANIMATION_STAND
};

ANIMATION fire_flower_animations[FIRE_FLOWER_ANIMATION_MAX] = {
	6, {0, 1, 2, 0, 1, 2} // ファイアフラワーの点滅 //注意！！要素数が３つだととなぜかエラーが起きる
};

ANIMATION fire_ball_animations[FIRE_BALL_ANIMATION_MAX] = {
	4, {0, 1, 2, 3},
	3, {4, 5, 6}
};

PLAYER* player;
std::vector<COIN*> coin;
std::vector<COIN*> field_coin;
std::vector<COIN*> ground_field_coin;
std::vector<COIN*> block_coin;
std::vector<QUESTION*> question;
std::vector<BLOCK*> block;
std::vector<BLOCK*> coin_block;
std::vector<BLOCK*> ground_block;
std::vector<ENEMY*> enemy;
std::vector<ENEMY*> kuriboo;
std::vector<ENEMY*> nokonoko;
std::vector<ITEM*> mushroom;
std::vector<ITEM*> mushroom_1up;
std::vector<ITEM*> fire_flower;
std::vector<ITEM*> star;
std::vector<FIRE_BALL*> fire_ball;
std::vector<LIFT*> lift;

std::vector<ENEMY*> empty_enemy; //実際には使わない引数に入れる


//１－２用のインスタンス
std::vector<COIN*> coin_1_2;
std::vector<BLOCK*> block_1_2;
std::vector<QUESTION*> question_1_2;
std::vector<ENEMY*> kuriboo_1_2;
std::vector<ENEMY*> nokonoko_1_2;
std::vector<ITEM*> mushroom_1_2;
std::vector<ITEM*> fire_flower_1_2;


//-------------------------------------------
//音声認識による指示を出す関数
//-------------------------------------------
void VoiceCommand() {
	float acceleration = 0.2f;

	if (GetNewHorizontalVoiceInput() == false) {
		player->voice_old_pos = player->position;
		//player->animation = ANIMATION_STAND;
	}
	else {
		if (SetVoiceCommand() == 1) {
			if (GetNewVerticalVoiceInput() == true) {
				player->velocity.y -= 10.0f;
				audioPlay(VOICE_JUMP_1);
				SetNewVerticalVoiceInput(false);
			}
		}
		if (SetVoiceCommand() == 2) {
			if (GetNewVerticalVoiceInput() == true) { //垂直方向速度指示
				player->velocity.y -= 10.0f;
				audioPlay(VOICE_JUMP_1);
				SetNewVerticalVoiceInput(false);
			}
			if (player->position.x - player->voice_old_pos.x < 45 * VOICE_COMMAND_AMOUNT) { //水平方向速度指示
				player->velocity.x += acceleration * player->isRunning;
				player->isMirror = false;
			}
			else {
				SetNewHorizontalVoiceInput(false);
			}
		}
		if (SetVoiceCommand() == 3) {
			if (player->position.x - player->voice_old_pos.x < 45 * VOICE_COMMAND_AMOUNT) { //１ます分だけ進ませる
				player->velocity.x += acceleration * player->isRunning;
				player->isMirror = false;
				player->animation = ANIMATION_RUN;
			}
			else {
				SetNewHorizontalVoiceInput(false);
			}
		}
		if (SetVoiceCommand() == 7) {
			if (player->position.x - player->voice_old_pos.x > -45 * VOICE_COMMAND_AMOUNT) {
				player->velocity.x -= acceleration * player->isRunning;
				player->isMirror = true;
				player->animation = ANIMATION_RUN;
			}
			else {
				SetNewHorizontalVoiceInput(false);
			}
		}
		if (SetVoiceCommand() == 8) {
			if (GetNewVerticalVoiceInput() == true) { //垂直方向速度指示
				player->velocity.y -= 10.0f;
				audioPlay(VOICE_JUMP_1);
				SetNewVerticalVoiceInput(false);
			}
			if (player->position.x - player->voice_old_pos.x > -45 * VOICE_COMMAND_AMOUNT) { //水平方向速度指示
				player->velocity.x -= acceleration * player->isRunning;
				player->isMirror = true;
			}
			else {
				SetNewHorizontalVoiceInput(false);
			}
		}
	}
}


//-------------------------------------------
//すべてのテクスチャを読み込む関数
//-------------------------------------------
void AllTexFromFile(void) {
	TexFromFile("Mario.bmp", TEX_MARIO, { 16, 16 });
	TexFromFile("MarioBig.bmp", TEX_MARIO_BIG, { 16, 32 });
	TexFromFile("MarioFire.bmp", TEX_MARIO_FIRE, { 16, 32 });
	TexFromFile("MarioStar.bmp", TEX_MARIO_STAR, { 16, 16 });
	TexFromFile("MarioStarBig.bmp", TEX_MARIO_STAR_BIG, { 16, 32 });
	TexFromFile("UnityChan.bmp", TEX_UNITY_CHAN, { 64, 64 });
	TexFromFile("UnityChanFire.bmp", TEX_UNITY_CHAN_FIRE, { 64, 64 });
	TexFromFile("Luigi.bmp", TEX_LUIGI, { 16, 16 });
	TexFromFile("Kinopio.bmp", TEX_KINOPIO, {16, 16});
	TexFromFile("MarioCar.bmp", TEX_MARIO_CAR, { 16, 16 });
	TexFromFile("Empty.bmp", TEX_EMPTY, { 16, 16 });
	TexFromFile("Block.bmp", TEX_BLOCK, { 16, 16 });
	TexFromFile("BreakedBlock.bmp", TEX_BREAKED_BLOCK, { 16, 16 });
	TexFromFile("b0.bmp", TEX_BLOCK_0, { 16, 16 });
	TexFromFile("b1.bmp", TEX_BLOCK_1, { 16, 16 });
	TexFromFile("b2.bmp", TEX_BLOCK_2, { 16, 16 });
	TexFromFile("b0Ground.bmp", TEX_GROUND_BLOCK_0, { 16, 16 });
	TexFromFile("b1Ground.bmp", TEX_GROUND_BLOCK_1, { 16, 16 });
	TexFromFile("b2Ground.bmp", TEX_GROUND_BLOCK_2, { 16, 16 });
	TexFromFile("p0.bmp", TEX_PIPE_0, { 16, 16 });
	TexFromFile("p1.bmp", TEX_PIPE_1, { 16, 16 });
	TexFromFile("p2.bmp", TEX_PIPE_2, { 16, 16 });
	TexFromFile("p3.bmp", TEX_PIPE_3, { 16, 16 });
	TexFromFile("p4.bmp", TEX_PIPE_4, { 16, 16 });
	TexFromFile("p5.bmp", TEX_PIPE_5, { 16, 16 });
	TexFromFile("p6.bmp", TEX_PIPE_6, { 16, 16 });
	TexFromFile("p7.bmp", TEX_PIPE_7, { 16, 16 });
	TexFromFile("p26Joint.bmp", TEX_JOINT_PIPE_26, { 16, 16 });
	TexFromFile("p27Joint.bmp", TEX_JOINT_PIPE_27, { 16, 16 });
	TexFromFile("p0Ground.bmp", TEX_GROUND_PIPE_0, { 16, 16 });
	TexFromFile("p1Ground.bmp", TEX_GROUND_PIPE_1, { 16, 16 });
	TexFromFile("p2Ground.bmp", TEX_GROUND_PIPE_2, { 16, 16 });
	TexFromFile("p3Ground.bmp", TEX_GROUND_PIPE_3, { 16, 16 });
	TexFromFile("p4Ground.bmp", TEX_GROUND_PIPE_4, { 16, 16 });
	TexFromFile("p5Ground.bmp", TEX_GROUND_PIPE_5, { 16, 16 });
	TexFromFile("p6Ground.bmp", TEX_GROUND_PIPE_6, { 16, 16 });
	TexFromFile("p7Ground.bmp", TEX_GROUND_PIPE_7, { 16, 16 });
	TexFromFile("p26JointGround.bmp", TEX_JOINT_GROUND_PIPE_26, { 16, 16 });
	TexFromFile("p27JointGround.bmp", TEX_JOINT_GROUND_PIPE_27, { 16, 16 });
	TexFromFile("g0.bmp", TEX_GOAL_0, { 16, 16 });
	TexFromFile("g1.bmp", TEX_GOAL_1, { 16, 16 });
	TexFromFile("f0.bmp", TEX_FORT_0, { 16, 16 });
	TexFromFile("f1.bmp", TEX_FORT_1, { 16, 16 });
	TexFromFile("f2.bmp", TEX_FORT_2, { 16, 16 });
	TexFromFile("f3.bmp", TEX_FORT_3, { 16, 16 });
	TexFromFile("f4.bmp", TEX_FORT_4, { 16, 16 });
	TexFromFile("f5.bmp", TEX_FORT_5, { 16, 16 });
	TexFromFile("f6.bmp", TEX_FORT_6, { 16, 16 });
	TexFromFile("t0.bmp", TEX_TREE_0, { 16, 16 });
	TexFromFile("t1.bmp", TEX_TREE_1, { 16, 16 });
	TexFromFile("t2.bmp", TEX_TREE_2, { 16, 16 });
	TexFromFile("c0.bmp", TEX_CLOUD_0, { 16, 16 });
	TexFromFile("c1.bmp", TEX_CLOUD_1, { 16, 16 });
	TexFromFile("c2.bmp", TEX_CLOUD_2, { 16, 16 });
	TexFromFile("c3.bmp", TEX_CLOUD_3, { 16, 16 });
	TexFromFile("c4.bmp", TEX_CLOUD_4, { 16, 16 });
	TexFromFile("c5.bmp", TEX_CLOUD_5, { 16, 16 });
	TexFromFile("m0.bmp", TEX_MOUNTAIN_0, { 16, 16 });
	TexFromFile("m1.bmp", TEX_MOUNTAIN_1, { 16, 16 });
	TexFromFile("m2.bmp", TEX_MOUNTAIN_2, { 16, 16 });
	TexFromFile("m3.bmp", TEX_MOUNTAIN_3, { 16, 16 });
	TexFromFile("m4.bmp", TEX_MOUNTAIN_4, { 16, 16 });
	TexFromFile("m5.bmp", TEX_MOUNTAIN_5, { 16, 16 });
	TexFromFile("Coin2.bmp", TEX_COIN, { 16, 16 });
	TexFromFile("QuestionBox.bmp", TEX_QUESTION_BOX, { 16, 16 });
	TexFromFile("EmptyQuestionBox.bmp", TEX_EMPTY_QUESTION_BOX, { 16, 16 });
	TexFromFile("EmptyGroundQuestionBox.bmp", TEX_EMPTY_GROUND_QUESTION_BOX, { 16, 16 });
	TexFromFile("Enemy.bmp", TEX_ENEMY, { 32, 32 });
	TexFromFile("Kuriboo.bmp", TEX_KURIBOO, { 16, 16 });
	TexFromFile("KuribooGround.bmp", TEX_GROUND_KURIBOO, { 16, 16 });
	TexFromFile("Nokonoko.bmp", TEX_NOKONOKO, { 16, 32 });
	TexFromFile("NokonokoShell.bmp", TEX_NOKONOKO_SHELL, { 16, 16 });
	TexFromFile("NokonokoGround.bmp", TEX_GROUND_NOKONOKO, { 16, 32 });
	TexFromFile("Kuppa.bmp", TEX_KUPPA, { 32, 32 });
	TexFromFile("Mushroom.bmp", TEX_MUSHROOM, { 16, 16 });
	TexFromFile("Mushroom1Up.bmp", TEX_MUSHROOM_1UP, { 16, 16 });
	TexFromFile("FireFlower.bmp", TEX_FIRE_FLOWER, { 16, 16 });
	TexFromFile("FireBall.bmp", TEX_FIRE_BALL, { 16, 16 });
	TexFromFile("Star.bmp", TEX_STAR, { 16, 16 });
	TexFromFile("Flag.bmp", TEX_FLAG, { 16, 16 });
	TexFromFile("Surprise.bmp", TEX_SURPRISE, { 16, 16 });

	TexFromFile("Lift.bmp", TEX_LIFT, { 80, 16 });
}


//---------------------
//初期化を行う関数
//---------------------
void Init(void) {
	
	AllTexFromFile();

	//インスタンスの生成と初期化
	player = MakePlayerInstance();
	coin = MakeCoinInstance(QUESTION_BOX_NUM); //クエスチョンボックスから出るコイン
	field_coin = MakeFieldCoinInstance(FIELD_COIN_NUM); //フィールドに初めから置いてあるコイン
	ground_field_coin = MakeFieldCoinInstance(GROUND_FIELD_COIN_NUM); //地下に初めから置いてあるコイン
	block_coin = MakeBlockCoinInstance(); //レンガブロックから出てくるコイン
	question = MakeQuestionInstance(QUESTION_BOX_NUM);
	block = MakeBlockInstance(BLOCK_NUM);
	coin_block = MakeCoinBlockInstance();
	ground_block = MakeBlockInstance(GROUND_BLOCK_NUM);
	enemy = MakeEnemyInstance(28 * ENEMY_WIDTH / 32.0f, 28 * ENEMY_HEIGHT / 32.0f, ENEMY_NUM); //ここでは、画像のサイズではなく、当たり判定で使うサイズを渡す。
	kuriboo = MakeEnemyInstance(KURIBOO_WIDTH, KURIBOO_HEIGHT, KURIBOO_NUM);
	nokonoko = MakeEnemyInstance(NOKONOKO_WIDTH, NOKONOKO_HEIGHT, NOKONOKO_NUM);
	mushroom = MakeMushroomInstance(MUSHROOM_NUM);
	mushroom_1up = MakeMushroomInstance(MUSHROOM_1UP_NUM);
	fire_flower = MakeFireFlowerInstance(FIRE_FLOWER_NUM);
	fire_ball = MakeFireBallInstance();
	star = MakeStarInstance();

	//敵のアニメーションの初期設定
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i]->animation = ENEMY_ANIMATION_STAND;
	}

	//クリボーのアニメーションの初期設定
	for (int i = 0; i < KURIBOO_NUM; i++) {
		kuriboo[i]->animation = KURIBOO_ANIMATION_STAND;
	}

	//ノコノコのアニメーションの初期設定
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		nokonoko[i]->animation = NOKONOKO_ANIMATION_STAND;
	}

	//ファイアフラワーのアニメーションの初期設定
	for (int i = 0; i < FIRE_FLOWER_NUM; i++) {
		fire_flower[i]->animation = FIRE_FLOWER_ANIMATION;
	}

	//ファイアボールのアニメーションの初期設定
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		//fire_ball[i]->animation = FIRE_BALL_ANIMATION_BOUND;
	}

	/*
	//敵の位置を個別に設定 サムネ用
	enemy[0]->position = { 945,  PART_HEIGHT * 13 }; //(180, 585)
	enemy[1]->position = { 1125,  PART_HEIGHT * 11 };
	enemy[2]->position = { 1035,  PART_HEIGHT * 9 };
	*/

	//クリボーの位置を設定
	kuriboo[0]->position = { 1030 /* + 2250*/,  PART_HEIGHT * 13};//
	kuriboo[1]->position = { 1980 /*+ 2250*/,  PART_HEIGHT * 13 };//
	kuriboo[2]->position = { 2370 /*+ 2250*/,  PART_HEIGHT * 13 };//
	kuriboo[3]->position = { 2475 /*+ 2250*/,  PART_HEIGHT * 13 };//
	kuriboo[4]->position = { 3555,  PART_HEIGHT * 9 }; 
	kuriboo[5]->position = { 3645,  PART_HEIGHT * 5 }; 
	kuriboo[6]->position = { 4350,  PART_HEIGHT * 13 }; 
	kuriboo[7]->position = { 4410,  PART_HEIGHT * 13 };
	kuriboo[8]->position = { 5215,  PART_HEIGHT * 13 };
	kuriboo[9]->position = { 5620,  PART_HEIGHT * 13 };
	kuriboo[10]->position = { 5680,  PART_HEIGHT * 13 };
	kuriboo[11]->position = { 5810,  PART_HEIGHT * 13 };
	kuriboo[12]->position = { 5870,  PART_HEIGHT * 13 };
	kuriboo[13]->position = { 7800,  PART_HEIGHT * 13 };
	kuriboo[14]->position = { 7860,  PART_HEIGHT * 13 };

	//kuriboo[15]->position = { 1530,  PART_HEIGHT * 13 };

	//ノコノコの位置を設定
	nokonoko[0]->position = { 4770,  PART_HEIGHT * 13 };
	nokonoko[1]->position = { 1430 /* + 2250*/,  PART_HEIGHT * 13};//

	//nokonoko[2]->position = { 1430,  PART_HEIGHT * 13 };//

	//キノコの初期位置を設定
	mushroom[0]->position = { 985,  PART_HEIGHT * 9 };

	//1UPキノコの初期位置を設定
	mushroom_1up[0]->position = { 2925,  PART_HEIGHT * 8 };

	//ファイアフラワーの初期位置を設定
	fire_flower[0]->position = { 3510,  PART_HEIGHT * 9 };
	
	//ファイアボールの初期位置を設定
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		fire_ball[i]->position = player->position + glm::vec2(22.5, 0);
	}

	//スターの初期位置を設定
	star[0]->position = { 4590,  PART_HEIGHT * 9 };
}


//----------------------------------------
//クエスチョンボックスを描画する関数
//----------------------------------------
void DrawQuestionBox(glm::vec2 _position, char _c, char _sub2_c, int _index, int _course) {

	if (_course == COURSE_1_1) { //1-1
		if (_c == 'q') {
			if (LetterToNum(_sub2_c) == _index + 1 && _index != 13) { //隠しクエスチョンボックスを除く
				if (question[_index]->flag == true) {
					DrawSprite(TEX_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, question_box_animations.frames[(question[_index]->frame / 45) % question_box_animations.count], _position);
				}
				else {
					DrawSprite(TEX_EMPTY_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, 0, _position);
				}
			}
			//隠しクエスチョンボックスを個別に処理
			if (LetterToNum(_sub2_c) == 13 + 1) {
				if (question[13]->flag == true) {
					//たたかれていない時は何も書かない
				}
				else {
					DrawSprite(TEX_EMPTY_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, 0, _position);
				}
			}
		}
	}
	else if(_course == COURSE_1_2) { //1-2
		if (_c == 'q') {
			if (LetterToNum(_sub2_c) == _index + 1 && _index != 6) {
				if (question_1_2[_index]->flag == true) {
					DrawSprite(TEX_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, question_box_animations.frames[(question_1_2[_index]->frame / 45) % question_box_animations.count], _position);
				}
				else {
					DrawSprite(TEX_EMPTY_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, 0, _position);
				}
			}
			//レンガブロックの見た目をしたクエスチョンボックス（ファイアーが出る）を個別に処理
			if (LetterToNum(_sub2_c) == 6) { //隠しクエスチョンボックスを除く
				if (question_1_2[_index]->flag == true) {
					DrawSprite(TEX_GROUND_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position);
				}
				else {
					DrawSprite(TEX_EMPTY_GROUND_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, 0, _position);
				}
			}
		}
	}
}


//----------------------------------------
//(破壊できる)ブロックを描画する関数
//----------------------------------------
void DrawBlock(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, int _index, bool _isInGround, int _course) {

	if (_course == COURSE_1_1) { //1-1
		if (_c == 'b') { //ブロッかどうか
			if (_sub_c == '2') { //破壊できる種類か
				if (LetterToNum(_sub2_c) == _index + 1) {
					if (_isInGround == false) { //地上
						if (block[_index]->flag == true) {
							DrawSprite(TEX_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position + block[_index]->delta_position);
						}
					}
					else if (_isInGround == true) { //地下
						if (ground_block[_index]->flag == true) {
							DrawSprite(TEX_GROUND_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position + ground_block[_index]->delta_position);
						}
					}
				}
			}
		}
	}
	else if (_course == COURSE_1_2) { //1-2
		if (_c == 'b') { //ブロッかどうか
			if (_sub_c == '2') { //破壊できる種類か
				if (LetterToNum(_sub2_c) == _index + 1) {
					if (block_1_2[_index]->flag == true) {
						DrawSprite(TEX_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position + block_1_2[_index]->delta_position);
					}
				}
			}
		}
	}
}


//---------------------------------------------
//コインが出るレンガブロックを描画する関数
//---------------------------------------------
void DrawCoinBlock(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, int _index) {

	if (_c == 'b') { //ブロッかどうか
		if (_sub_c == '3') { //コインが出るものか
			if (LetterToNum(_sub2_c) == _index + 1) {
				if (coin_block[_index]->flag == true) {
					DrawSprite(TEX_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position + coin_block[_index]->delta_position);
				}
				else {
					DrawSprite(TEX_EMPTY_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, 0, _position);
				}
			}
		}
	}
}


//----------------------------------------
//破壊されたブロックを描画する関数
//----------------------------------------
void DrawBreakedBlock(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, int _index) {

	if (_c == 'b') { //ブロッかどうか
		if (_sub_c == '2') { //破壊できる種類か
			if (LetterToNum(_sub2_c) == _index + 1) {
				DrawSprite(TEX_BREAKED_BLOCK, BREAKED_BLOCK_WIDTH, BREAKED_BLOCK_WIDTH, 0, _position + block[_index]->fring_delta_position_1);
				DrawSprite(TEX_BREAKED_BLOCK, BREAKED_BLOCK_WIDTH, BREAKED_BLOCK_WIDTH, 0, _position + block[_index]->fring_delta_position_2);
				DrawSprite(TEX_BREAKED_BLOCK, BREAKED_BLOCK_WIDTH, BREAKED_BLOCK_WIDTH, 0, _position + block[_index]->fring_delta_position_3);
				DrawSprite(TEX_BREAKED_BLOCK, BREAKED_BLOCK_WIDTH, BREAKED_BLOCK_WIDTH, 0, _position + block[_index]->fring_delta_position_4);
			}
		}
	}
}


//-------------------------------------
//コースパーツを描画する関数
//-------------------------------------
void DrawPart(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) { //地上にいる場合
		glClearColor(0.31372549, 0.50196078, 1.0, 1.0);  //(80,128, 255)背景色を空色にする

		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return;
		}

		if (_c == 'b') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_BLOCK_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_BLOCK_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2':  //「_c == 'b'」と「_sub_c == '2'」をDrawBlock内で二重確認してしまっているため、直した方がいいかも
				for (int i = 0; i < BLOCK_NUM; i++) {
					DrawBlock(_position, _c, _sub_c, _sub2_c, i, false, COURSE_1_1);
					//DrawBlock(_position, _c, _sub_c, _sub2_c, block, i, false);
				}
				break;
			case '3':
				for (int i = 0; i < COIN_BLOCK_NUM; i++) {
					DrawCoinBlock(_position, _c, _sub_c, _sub2_c, i);
				}
				break;
			default: break;
			}
		}
		else if (_c == 'q') { //「_c == 'q'」をDrawQuestionBox内で二重確認してしまっているため、直した方がいいかも
			//(注意！結構重くなる)改善済み。コースの走査を二重でループしていたため、そこを修正
			for (int i = 0; i < QUESTION_BOX_NUM; i++) {
				DrawQuestionBox(_position, _c, _sub2_c, i, COURSE_1_1);
			}
		}
		else if (_c == 'p') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_PIPE_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_PIPE_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_PIPE_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_PIPE_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '5': DrawSprite(TEX_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '6': DrawSprite(TEX_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '7': DrawSprite(TEX_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			default: break;
			}
		}
		else if (_c == 'g') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_GOAL_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_GOAL_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			default: break;
			}
		}
		else if (_c == 'f') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_FORT_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_FORT_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_FORT_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_FORT_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_FORT_4, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '5': DrawSprite(TEX_FORT_5, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '6': DrawSprite(TEX_FORT_6, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			default: break;
			}
		}
		else if (_c == 't') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_TREE_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_TREE_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_TREE_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			default: break;
			}
		}
		else if (_c == 'c') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_CLOUD_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_CLOUD_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_CLOUD_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_CLOUD_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_CLOUD_4, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '5': DrawSprite(TEX_CLOUD_5, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			default: break;
			}
		}
		else if (_c == 'm') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_MOUNTAIN_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_MOUNTAIN_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_MOUNTAIN_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_MOUNTAIN_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_MOUNTAIN_4, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '5': DrawSprite(TEX_MOUNTAIN_5, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			default: break;
			}
		}
		else if (_c == ' ') {
			//DrawSprite(TEX_EMPTY, PART_WIDTH, PART_HEIGHT, 0, _position); //何もないところは空（全部透過色）の画像を表示する）
			//空の描画を背景色の指定で行うと、壊したブロックの破片が空の裏に隠れてしまう

			glColor3ub(0x50, 0x80, 0xff);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			glPushMatrix();
			{
				glTranslatef(_position.x, _position.y, 0);
				glBegin(GL_QUADS);
				{
					glVertex2f(0, 0);
					glVertex2f(0, PART_HEIGHT);
					glVertex2f(PART_WIDTH, PART_HEIGHT);
					glVertex2f(PART_WIDTH, 0);
				}
				glEnd();
			}
			glPopMatrix();
		}
	}

	else  { //地下にいる場合
	glClearColor(0.0, 0.0, 0.0, 1.0); //背景色を黒にする

		if ((x < 0) || (x >= GROUND_COURSE_WIDTH) || (y < 0) || (y >= GROUND_COURSE_HEIGHT)) {
			return;
		}

		if (_c == 'b') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_GROUND_BLOCK_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_GROUND_BLOCK_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2':
				for (int i = 0; i < GROUND_BLOCK_NUM; i++) {
					DrawBlock(_position, _c, _sub_c, _sub2_c, i, true, COURSE_1_1);
					//DrawBlock(_position, _c, _sub_c, _sub2_c, ground_block, i, true);
				}
				break;
			default: break;
			}
		}
		else if (_c == 'q') {
		}
		else if (_c == 'p') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_GROUND_PIPE_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_GROUND_PIPE_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_GROUND_PIPE_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_GROUND_PIPE_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_GROUND_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '5': DrawSprite(TEX_GROUND_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '6': DrawSprite(TEX_GROUND_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '7': DrawSprite(TEX_GROUND_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '8': DrawSprite(TEX_JOINT_GROUND_PIPE_26, PART_WIDTH, PART_HEIGHT, 0, _position); break; //接合部
			case '9': DrawSprite(TEX_JOINT_GROUND_PIPE_27, PART_WIDTH, PART_HEIGHT, 0, _position); break; //接合部
			default: break;
			}
		}
		else if (_c == 'g') {
		}
		else if (_c == 'f') {
		}
		else if (_c == 't') {
		}
		else if (_c == 'c') {
		}
		else if (_c == 'm') {
		}
		else if (_c == ' ') {
			glColor3ub(0x00, 0x00, 0x00); //地下にいるとき、背景は黒
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			glPushMatrix();
			{
				glTranslatef(_position.x, _position.y, 0);
				glBegin(GL_QUADS);
				{
					glVertex2f(0, 0);
					glVertex2f(0, PART_HEIGHT);
					glVertex2f(PART_WIDTH, PART_HEIGHT);
					glVertex2f(PART_WIDTH, 0);
				}
				glEnd();
			}
			glPopMatrix();

		}
	}
}


//----------------------------------------------------
//クエスチョンボックスから出たコインを描画する関数
//----------------------------------------------------
void DrawCoin(int _index, int _course) {
	float scrollX = player->position.x - WINDOW_WIDTH / 2 + PLAYER_WIDTH / 2;

	if (_course == COURSE_1_1) { //1-1
		if (coin[_index]->sub_flag == true) {
			if (coin[_index]->flag == true) {
				for (int y = 0; y < COURSE_HEIGHT; y++) {
					for (int x = 0; x < COURSE_WIDTH; x++) {
						if (sub_course[y][x] == 'C') {
							if (course[y + 1][x] == 'q') {
								if (LetterToNum(sub_course[y - 1][x]) == _index + 1) {
									DrawSprite(TEX_COIN, PART_WIDTH, PART_HEIGHT,
										coin_animations.frames[(coin[_index]->frame / 23) % coin_animations.count],
										{ x * PART_WIDTH - scrollX + coin[_index]->delta_pos.x, y * PART_HEIGHT + coin[_index]->delta_pos.y });
								}
							}
						}
					}
				}
			}
		}
	}

	if (_course == COURSE_1_2) { //1-2
		if (coin_1_2[_index]->sub_flag == true) {
			if (coin_1_2[_index]->flag == true) {
				for (int y = 0; y < COURSE_HEIGHT; y++) {
					for (int x = 0; x < COURSE_WIDTH; x++) {
						if (sub_course_1_2[y][x] == 'C') {
							if (course_1_2[y + 1][x] == 'q') {
								if (LetterToNum(sub_course_1_2[y - 1][x]) == _index + 1) {
									DrawSprite(TEX_COIN, PART_WIDTH, PART_HEIGHT,
										coin_animations.frames[(coin_1_2[_index]->frame / 23) % coin_animations.count],
										{ x * PART_WIDTH - scrollX + coin_1_2[_index]->delta_pos.x, y * PART_HEIGHT + coin_1_2[_index]->delta_pos.y });
								}
							}
						}
					}
				}
			}
		}
	}
}


//----------------------------------------------------
//フィールド上のコインを描画する関数
//----------------------------------------------------
void DrawFieldCoin(int _index, bool _isInGround) {
	float scrollX = player->position.x - WINDOW_WIDTH / 2 + PLAYER_WIDTH / 2;

	if (_isInGround == false) { //地上)
		if (field_coin[_index]->sub_flag == true) {
			if (field_coin[_index]->flag == true) {
				for (int y = 0; y < COURSE_HEIGHT; y++) {
					for (int x = 0; x < COURSE_WIDTH; x++) {
						if (sub_course[y][x] == 'D') {
							if (LetterToNum(sub_course[y - 1][x]) == _index + 1) {
								DrawSprite(TEX_COIN, PART_WIDTH, PART_HEIGHT,
									coin_animations.frames[(field_coin[_index]->frame / 23) % coin_animations.count],
									{ x * PART_WIDTH - scrollX, y * PART_HEIGHT });
							}
						}
					}
				}
			}
		}
	}
	else if (_isInGround == true) { //地下
		if (ground_field_coin[_index]->sub_flag == true) {
			if (ground_field_coin[_index]->flag == true) {
				for (int y = 0; y < GROUND_COURSE_HEIGHT; y++) {
					for (int x = 0; x < GROUND_COURSE_WIDTH; x++) {
						if (ground_sub_course[y][x] == 'D') {
							if (LetterToNum(ground_sub2_course[y][x]) == _index + 1) {
								DrawSprite(TEX_COIN, PART_WIDTH, PART_HEIGHT,
									coin_animations.frames[(ground_field_coin[_index]->frame / 23) % coin_animations.count],
									{ x * PART_WIDTH, y * PART_HEIGHT });
							}
						}
					}
				}
			}
		}
	}
}


//----------------------------------------------------
//レンガブロックから出たコインを描画する関数
//----------------------------------------------------
void DrawBlockCoin(int _index) {
	float scrollX = player->position.x - WINDOW_WIDTH / 2 + PLAYER_WIDTH / 2;

	for (int j = 0; j < BLOCK_COIN_NUM; j++) {
		if (block_coin[_index]->vector_sub_flag[j] == true) {
			if (block_coin[_index]->vector_flag[j] == true) {
				for (int y = 0; y < COURSE_HEIGHT; y++) {
					for (int x = 0; x < COURSE_WIDTH; x++) {
						if (course[y + 1][x] == 'b') {
							if (sub_course[y + 1][x] == '3') {
								if (LetterToNum(sub2_course[y + 1][x]) == _index + 1) {
									DrawSprite(TEX_COIN, PART_WIDTH, PART_HEIGHT,
										coin_animations.frames[(block_coin[_index]->vector_frame[j] / 23) % coin_animations.count],
										{ x * PART_WIDTH - scrollX + block_coin[_index]->vector_delta_pos[j].x, y * PART_HEIGHT + block_coin[_index]->vector_delta_pos[j].y });
								}
							}
						}
					}
				}
			}
		}
	}
}



//-------------------
//描画関数本体
//-------------------
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	float scrollX = player->position.x - WINDOW_WIDTH / 2 + PLAYER_WIDTH / 2;
	
	
	//コースパーツを描画
	if (player->isInGround == false) { //地上
		for (int y = 0; y < COURSE_HEIGHT; y++) {
			for (int x = 0; x < COURSE_WIDTH; x++) {
				DrawPart({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, course[y][x], sub_course[y][x], sub2_course[y][x], player->isInGround);
			}
		}
	}
	if (player->isInGround == true) { //地下
		for (int y = 0; y < GROUND_COURSE_HEIGHT; y++) {
			for (int x = 0; x < GROUND_COURSE_WIDTH; x++) {
				DrawPart({ x * PART_WIDTH, y * PART_HEIGHT }, ground_course[y][x], ground_sub_course[y][x], ground_sub2_course[y][x], player->isInGround);
			}
		}
	}
	

	//クエスチョンボックスから出たコインを描画
	for (int i = 0; i < QUESTION_BOX_COIN_NUM; i++) {
		DrawCoin(i, COURSE_1_1);
	}

	//フィールド上のコインを描画
	if (player->isInGround == false) {
		for (int i = 0; i < FIELD_COIN_NUM; i++) {
			DrawFieldCoin(i, false); //地上
		}
	}
	else if (player->isInGround == true) {//地下のコインを描画
		for (int i = 0; i < GROUND_FIELD_COIN_NUM; i++) {
			DrawFieldCoin(i, true); //地下
		}
	}

	//レンガブロックから出るのコインを描画
	for (int i = 0; i < COIN_BLOCK_NUM; i++) { //コインが出てくるレンガブロックと、そこから出てきたコインのインスタンスの数は同じであることに注意する
		DrawBlockCoin(i);
	}
	
	//ゴールの旗を描画
	//これを先に描かないと、キャラクターがゴールの旗に隠されてしまう
	DrawSprite(TEX_FLAG, PART_WIDTH, PART_HEIGHT, 0, player->flag_pos + glm::vec2(-scrollX, 0));

	//マリオを描画
	if (player->isBig == 1) {
		if (player->isStar == false) {
			DrawSprite(TEX_MARIO, PLAYER_WIDTH, PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame * player->isRunning / 31) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (!player->isInGround), 0), //地下でスクロールしないようにするために、player->isInGroundの否定をかける
				player->isMirror
			);
		}
		else if (player->isStar == true) {
			DrawSprite(TEX_MARIO_STAR, PLAYER_WIDTH, PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame / 15) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (!player->isInGround), 0),
				player->isMirror
			);
		}
	}
	else if (player->isBig == 2) {
		if (player->isFire == false && player->isStar == false) {
			DrawSprite(TEX_MARIO_BIG, PLAYER_WIDTH, 2 * PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame / 31) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (!player->isInGround), 0),
				player->isMirror
			);
		}

		else if (player->isFire == true && player->isStar == false) {
			DrawSprite(TEX_MARIO_FIRE, PLAYER_WIDTH, 2 * PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame / 31) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (!player->isInGround), 0),
				player->isMirror
			);
		}

		else if (player->isStar == true) {
			DrawSprite(TEX_MARIO_STAR_BIG, PLAYER_WIDTH, 2 * PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame / 15) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (!player->isInGround), 0),
				player->isMirror
			);
		}
	}

	//入れる土管だけ上書き
	if (player->isInGround == false) {
		DrawSprite(TEX_PIPE_0, PART_WIDTH, PART_HEIGHT, 0, { 2565 - scrollX, 450 });
		DrawSprite(TEX_PIPE_1, PART_WIDTH, PART_HEIGHT, 0, { 2610 - scrollX, 450 });
		DrawSprite(TEX_PIPE_2, PART_WIDTH, PART_HEIGHT, 0, { 2565 - scrollX, 495 });
		DrawSprite(TEX_PIPE_3, PART_WIDTH, PART_HEIGHT, 0, { 2610 - scrollX, 495 });
		DrawSprite(TEX_PIPE_2, PART_WIDTH, PART_HEIGHT, 0, { 2565 - scrollX, 540 });
		DrawSprite(TEX_PIPE_3, PART_WIDTH, PART_HEIGHT, 0, { 2610 - scrollX, 540 });
	}
	else if (player->isInGround == true) {
		DrawSprite(TEX_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, { 585, 540 });
		DrawSprite(TEX_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, { 585, 585 });
		DrawSprite(TEX_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, { 630, 540 });
		DrawSprite(TEX_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, { 630, 585 });
	}
	
	/*
	//.サムネ用のクッパとビックリマークを描画
	DrawSprite(TEX_KUPPA, 64, 64, 0, enemy[0]->position + glm::vec2(-scrollX, -20), true);
	DrawSprite(TEX_KUPPA, 64, 64, 1, enemy[1]->position + glm::vec2(-scrollX - 20, 0), false);
	DrawSprite(TEX_KUPPA, 64, 64, 0, enemy[2]->position + glm::vec2(-scrollX, -20), false);
	DrawSprite(TEX_SURPRISE, 45, 45, 0, glm::vec2(1050, PART_HEIGHT*12) + glm::vec2(-scrollX, 0), false);
	*/

	//クリボーを描画
	if (player->isInGround == false) {
		for (int i = 0; i < KURIBOO_NUM; i++) {
			if (kuriboo[i]->display_flag == true && kuriboo[i]->isDeath == false) { //踏まれる、またはブロックの下からたたかれて死ぬまで描画する
				DrawSprite(TEX_KURIBOO, KURIBOO_WIDTH, KURIBOO_HEIGHT,
					kuriboo_animations[kuriboo[i]->animation].frames[(kuriboo[i]->frame / 63) % animations[kuriboo[i]->animation].count],
					kuriboo[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(KURIBOO_WIDTH / 16.0f, KURIBOO_HEIGHT / 16.0f),
					kuriboo[i]->isMirror
				);
			}
			//クリボーがひっくり返って死ぬアニメーションを描画
			if (kuriboo[i]->isDeath == true) {
				DrawSprite(TEX_KURIBOO, KURIBOO_WIDTH, KURIBOO_HEIGHT,
					3, //Kuriboo.bmpファイルの４番目の（ひっくり返った）画像を表示する
					kuriboo[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(KURIBOO_WIDTH / 16.0f, KURIBOO_HEIGHT / 16.0f) + kuriboo[i]->delta_position,
					kuriboo[i]->isMirror
				);
			}
		}
	}

	//ノコノコを描画
	if (player->isInGround == false) {
		for (int i = 0; i < NOKONOKO_NUM; i++) {
			if (nokonoko[i]->display_flag == true && nokonoko[i]->isDeath == false) { //踏まれる、または死ぬまで描画する
				if (nokonoko[i]->stepped_on_flag == false) {
					DrawSprite(TEX_NOKONOKO, NOKONOKO_WIDTH, NOKONOKO_HEIGHT,
						nokonoko_animations[nokonoko[i]->animation].frames[(nokonoko[i]->frame / 31) % animations[nokonoko[i]->animation].count],
						nokonoko[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_WIDTH / 15.0f, NOKONOKO_HEIGHT / 15.0f),
						nokonoko[i]->isMirror
					);
				}
				else {
					DrawSprite(TEX_NOKONOKO_SHELL, NOKONOKO_SHELL_WIDTH, NOKONOKO_SHELL_HEIGHT,
						0,
						nokonoko[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(0/*NOKONOKO_SHELL_WIDTH / 18.0f*/, 0),
						nokonoko[i]->isMirror
					);
				}
			}
			//ノコノコがひっくり返って死ぬアニメーションを描画
			if (nokonoko[i]->isDeath == true) {
				if (nokonoko[i]->stepped_on_flag == false) { //ノコノコが歩いて入りるときにファイアボールに殺された場合
					DrawSprite(TEX_NOKONOKO, NOKONOKO_WIDTH, NOKONOKO_HEIGHT,
						2, //ファイルの2番目の（ひっくり返った）画像を表示する
						nokonoko[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_WIDTH / 16.0f, NOKONOKO_HEIGHT / 16.0f) + nokonoko[i]->delta_position,
						nokonoko[i]->isMirror
					);
				}
				else { //ノコノコが甲羅の時にファイアボールに殺された場合
					DrawSprite(TEX_NOKONOKO_SHELL, NOKONOKO_SHELL_WIDTH, NOKONOKO_SHELL_HEIGHT,
						2, //ファイルの2番目の（ひっくり返った）画像を表示する
						nokonoko[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_SHELL_WIDTH / 16.0f, NOKONOKO_SHELL_HEIGHT / 16.0f) + nokonoko[i]->delta_position,
						nokonoko[i]->isMirror
					);
				}
			}
		}
	}

	//スーパーキノコを描画
	for (int i = 0; i < MUSHROOM_NUM; i++) {
		if (mushroom[i]->display_flag == true) {
			if (mushroom[i]->get_flag == false) {
				DrawSprite(TEX_MUSHROOM, PART_WIDTH, PART_HEIGHT, 0,
					mushroom[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)
				);
			}
		}
	}

	//1UPキノコを描画
	for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
		if (mushroom_1up[i]->display_flag == true) {
			if (mushroom_1up[i]->get_flag == false) {
				DrawSprite(TEX_MUSHROOM_1UP, PART_WIDTH, PART_HEIGHT, 0,
					mushroom_1up[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)
				);
			}
		}
	}
	
	//ファイアフラワーを描画
	for (int i = 0; i < FIRE_FLOWER_NUM; i++) {
		if (fire_flower[i]->display_flag == true) {
			if (fire_flower[i]->get_flag == false) {
				DrawSprite(TEX_FIRE_FLOWER, PART_WIDTH, PART_HEIGHT,
					fire_flower_animations[fire_flower[i]->animation].frames[(fire_flower[i]->frame / 15) % animations[fire_flower[i]->animation].count],
					fire_flower[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)
				);
			}
		}
	}

	//ファイアボールを描画
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			DrawSprite(TEX_FIRE_BALL, PART_WIDTH , PART_HEIGHT ,
				//0,
				fire_ball_animations[fire_ball[i]->animation].frames[(fire_ball[i]->frame / 31) % animations[fire_ball[i]->animation].count],
				fire_ball[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 16.0f, PART_HEIGHT / 16.0f)
			);
		}
	}

	//スターを描画
	for (int i = 0; i < STAR_NUM; i++) {
		if (star[i]->display_flag == true) {
			if (star[i]->get_flag == false) {
				DrawSprite(TEX_STAR, PART_WIDTH, PART_HEIGHT, 0,
					star[i]->position + glm::vec2(-scrollX, 0) /* - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)*/
				);
			}
		}
	}
	
	//壊れたブロックの破片を描画
	if (player->isInGround == false) {
		for (int i = 0; i < BLOCK_NUM; i++) {
			if (block[i]->flag == false) {
				for (int y = 0; y < COURSE_HEIGHT; y++) {
					for (int x = 0; x < COURSE_WIDTH; x++) {
						DrawBreakedBlock({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, course[y][x], sub_course[y][x], sub2_course[y][x], i);
					}
				}
			}
		}
	}
	/*
	else if (player->isInGround == true) {
		for (int i = 0; i < GROUND_BLOCK_NUM; i++) {
			if (ground_block[i]->flag == false) {
				for (int y = 0; y < GROUND_COURSE_HEIGHT; y++) {
					for (int x = 0; x < GROUND_COURSE_WIDTH; x++) {
						DrawBreakedBlock({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, ground_course[y][x], ground_sub_course[y][x], ground_sub2_course[y][x], i);
					}
				}
			}
		}
	}*/
	
	
	//--------------------------------
	//フォントの描画
	//--------------------------------
	fontBegin();
	fontSetWeight(2.5); //最小0.5、最大10.0
	fontSetColor(0xff - 0x10, 0xff - 0x10, 0xff - 0x10); //微妙に暗めの白色
	fontFont(FONT_FONT_MONO_ROMAN);
	fontSetSize(FONT_DEFAULT_SIZE/5.5);

	/*
	fontSetPosition(WINDOW_WIDTH*0.1 , fontGetsize()*0.7);
	fontDraw("MARIO");
	fontSetPosition(WINDOW_WIDTH*0.6 , fontGetsize()*0.7);
	fontDraw("WORLD");
	fontSetPosition(WINDOW_WIDTH * 0.62, fontGetsize() * 2.0);
	fontDraw("1-1");
	fontSetPosition(WINDOW_WIDTH * 0.8, fontGetsize() * 0.7);
	fontDraw("TIME");
	*/

	fontSetSize(FONT_DEFAULT_SIZE / 5);////deback
	fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 0.5);////deback
	//fontDraw("player->x  %lf", player->position.x);////deback
	fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 2);////deback
	//fontDraw("player->y  %lf", player->position.y);////deback

	//fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 3.5);////deback
	//if (player->isGoal == true) {
	//	fontDraw("Clear!!");////deback
	//}

	fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 5);////deback
	//fontDraw("player->flag_pos.y %lf", player->flag_pos.y);////deback
	fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 6);////deback
	//fontDraw("player->frame %d", player->frame);////deback
	fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 7);////deback
	//fontDraw("player->tmp_frame %d", player->tmp_frame);////deback
	//fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 8);////deback
	//fontDraw(" fire_ball[3].x %lf", fire_ball[3]->position.x);////deback
	//fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 9);////deback
	//fontDraw(" fire_ball[4].x %lf", fire_ball[4]->position.x);////deback
	//fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 10);////deback
	//fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 8.0);////deback
	//fontDraw("kuriboo[0]->intersect_bottom_flag  %d", kuriboo[0]->intersect_bottom_flag);////deback
	fontEnd();


	glutSwapBuffers();
}


//------------------------
//アイドル時の処理
//------------------------
//void Idle(void) {
void Idle(int value) {

	float acceleration = 0.2f; //0.155f;だと歩く速度としてちょうどよい

	//キーが押されている間、行う処理
	if (!keysIsPressed['c']) {
		player->isRunning = 1;
	}
	else if (keysIsPressed['c']) { //ダッシュしている場合
		player->isRunning = 2;
	}

	if (keysIsPressed['j']) {
		player->velocity.x -= acceleration * player->isRunning;
	}

	if (keysIsPressed['l']) {
		player->velocity.x += acceleration * player->isRunning;
	}

	//縦の土管に入るときの処理
	if (!(IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true)) {
		player->tmp1_pos = player->position;
	}
	else if (IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.y += 0.1f;
		if (player->position.y > player->tmp1_pos.y + 135) { //マリオが土管に入りきったらワープ
			player->isInGround = true;
			player->position = {45*2, 0}; 
			player->inPipe = false; //ワープしたら土管に入り中であるというフラグをおろす
			audioStop(MUSIC_STAGE); //地上の音楽を止めて
			audioPlay(MUSIC_UNDER_GROUND_STAGE); //地下の音楽を流す
		}
	}
	//横の土管に入るときの処理
	if (!(IntersectPipe(player->right, RIGHT, true) && player->inPipe == true)) {
		player->tmp2_pos = player->position;
	}
	else if (IntersectPipe(player->right, RIGHT, true) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.x += 0.15f;
		if (player->position.x > player->tmp2_pos.x + 50) { //マリオが土管に入りきったらワープ
			player->isInGround = false;
			player->position = { 7355, PART_HEIGHT * 11 };
			player->inPipe = false; //ワープしたら土管に入り中であるというフラグをおろす
			audioStop(MUSIC_UNDER_GROUND_STAGE); //地下の音楽を止め
			audioPlay(MUSIC_STAGE); //地上の音楽を流す
		}
	}

	//glutSpecialFunc(SpecialKey); //矢印キーでも動くようにする

	//キャラクターに物理法則を適用（抵抗と重力）
	player->velocity.x *= 0.93f;
	player->velocity.y += 0.2f;
	player->position += player->velocity;

	//マリオのスターの秒数を制御する
	if(player->isStar == false){
		player->tmp_star_frame = player->frame;
	}
	else if (player->isStar == true) {
		if (player->frame > player->tmp_star_frame + 180 * 8) { //いま、180fpsで描画しているため、スターを８秒間続けるために180*8
			player->isStar = false;
			audioStop(MUSIC_STAR); //スターのBGMを止め、
			audioPlay(MUSIC_STAGE); //ステージのBGMを流す
		}
	}
	
	/*
	//敵に重力を与える
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i]->velocity.y += 0.2f;
		enemy[i]->position += enemy[i]->velocity;
	}
	*/

	//クリボーに重力を与える
	for (int i = 0; i < KURIBOO_NUM; i++) {
		kuriboo[i]->velocity.y += 0.2f;
		kuriboo[i]->position += kuriboo[i]->velocity;
	}

	//ノコノコに重力を与える
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		nokonoko[i]->velocity.y += 0.2f;
		nokonoko[i]->position += nokonoko[i]->velocity;
	}

	//スーパーキノコに重力を与える
	for (int i = 0; i < MUSHROOM_NUM; i++) {
		mushroom[i]->velocity.y += 0.2f;
		mushroom[i]->position += mushroom[i]->velocity;
	}

	//1UPキノコに重力を与える
	for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
		mushroom_1up[i]->velocity.y += 0.2f;
		mushroom_1up[i]->position += mushroom_1up[i]->velocity;
	}

	//スターに重力を与える
	for (int i = 0; i < STAR_NUM; i++) {
		if (star[i]->display_flag == true) {
			star[i]->velocity.y += 0.2f;
			star[i]->position += star[i]->velocity;
		}
	}

	//ファイアボールに重力を与える
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				fire_ball[i]->velocity.y += 0.2f;
				fire_ball[i]->position += fire_ball[i]->velocity;
			}
		}
	}

	//キャラクターがコース外に行った時のリセット処理
	//if ((player.position.x < 0) || (player.position.x >= COURSE_WIDTH) || (player.position.y < 0) || (player.position.y >= COURSE_HEIGHT)) {
	if ((player->position.y >= WINDOW_HEIGHT + PART_HEIGHT * 100)) {
		player->position = { WINDOW_WIDTH / 2.0f,  PART_HEIGHT * (13 - (player->isBig - 1)) };
		player->isBig = 1;
		player->isFire = false;
	}

	//-------------------------------------
	//キャラクターの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	player->left = player->position + glm::vec2(0, player->isBig * PLAYER_HEIGHT / 2.0f);
	player->right = player->position + glm::vec2(PLAYER_WIDTH, player->isBig * PLAYER_HEIGHT / 2.0f);
	player->top = player->position + glm::vec2(PLAYER_WIDTH / 2.0f, 0);
	player->bottom = player->position + glm::vec2(PLAYER_WIDTH / 2.0f, player->isBig * PLAYER_HEIGHT);

	//プレイヤーのコースパーツとの当たり判定
	//なぜかうまくいかないため要修正
	//引数としてplayerを渡して、そのメンバ変数を変更したいときはポインタで渡す必要がある
	DoIntersectDetection(PLAYER_INTERSECT, PART_INTERSECT, 1, 1, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question); //最後の引数は使わないけど、とりあえず与えておくPART_INTERSECT

	//プレイヤーの（壊せる）ブロックとの当たり判定
	if (player->isInGround == false) {
		DoIntersectDetection(PLAYER_INTERSECT, BREAKABLE_BLOCK_INTERSECT, 1, BLOCK_NUM, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question);
	}
	//プレイヤーの地下のレンガブロックとの当たり判定
	if (player->isInGround == true) {
		DoIntersectDetection(PLAYER_INTERSECT, BREAKABLE_BLOCK_INTERSECT, 1, GROUND_BLOCK_NUM, true, player, enemy, empty_enemy, mushroom, fire_ball, ground_block, question);
	}

	//プレイヤーのコインブロックとの当たり判定
	DoIntersectDetection(PLAYER_INTERSECT, COIN_BLOCK_INTERSECT, 1, COIN_BLOCK_NUM, false, player, enemy, empty_enemy, mushroom, fire_ball, coin_block, question);

	//マリオのアニメーションの処理
	//(これを先に行わないと、当たり判定によるアニメーションの変更が上書きされてしまうため、当たり判定よりも先に処理する)
	//(さらに、ブロックとの当たり判定よりもあとに行わないと、ブロック上でジャンプアニメーションになってしまう)
	if (player->isJumping) {
		player->animation = ANIMATION_JUMP;
		if (player->isStar == true){
			player->animation = ANIMATION_STAR_JUMP;
		}
	}
	else if (keysIsPressed['j'] || keysIsPressed['l']) {
		player->animation = ANIMATION_RUN;
		if (player->isStar == true){
			player->animation = ANIMATION_STAR_RUN;
		}
	}
	else if (keysIsPressed['m']) {
		player->animation = ANIMATION_SQUAT;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_SQUAT;
		}
	}
	else {
		player->animation = ANIMATION_STAND;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_STAND;
		}
	}

	if (keysIsPressed['j'] && keysIsPressed['l'] && keysIsPressed['c']) { //マリオがダッシュ中に切り返すとき
		player->animation = ANIMATION_TURN;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_TURN;
		}
	}
	
	if ((IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true) || (IntersectPipe(player->right, RIGHT, true) && player->inPipe == true)) { //土管に入るとき
		player->animation = ANIMATION_STAND; //これがないと、土管に入るときに当たり判定が消えているためジャンプのアニメーションになってしまう
	}

	player->frame++;

	//敵のアニメーションの処理
	//(これを先に行わないと、当たり判定によるアニメーションの変更が上書きされてしまうため、当たり判定よりも先に処理する)
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i]->animation = ENEMY_ANIMATION_STAND;
		enemy[i]->frame++;
	}

	//クリボーのアニメーションの処理
	for (int i = 0; i < KURIBOO_NUM; i++) {
		if (kuriboo[i]->stepped_on_flag == false) {
			kuriboo[i]->animation = KURIBOO_ANIMATION_STAND;
			kuriboo[i]->frame++;
			kuriboo[i]->frame_at_stepped_on = kuriboo[i]->frame; //踏まれた瞬間のフレームが保持される
		}
		else {
			if (kuriboo[i]->frame < kuriboo[i]->frame_at_stepped_on + 60) {
				kuriboo[i]->animation = KURIBOO_ANIMATION_CRUSHED;
				kuriboo[i]->frame++;
			}
			else {
				kuriboo[i]->display_flag = false;
			}
		}
	}

	//ノコノコのアニメーションの処理
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		if (nokonoko[i]->stepped_on_flag == false) {
			nokonoko[i]->animation = KURIBOO_ANIMATION_STAND;
			nokonoko[i]->frame++;
		}
		//踏まれたときに甲羅を描画する処理は、DrawSprite関数で行う
	}

	//ファイアフラワーのアニメーションの処理
	for (int i = 0; i < FIRE_FLOWER_NUM; i++) {
		if (fire_flower[i]->display_flag == true) {
			fire_flower[i]->animation = FIRE_FLOWER_ANIMATION;
			fire_flower[i]->frame++;
		}
	}

	//ファイアボールのアニメーションの処理
	//敵と当たった時のアニメーションの切り替えはintersect.cppで行う
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				fire_ball[i]->animation = FIRE_BALL_ANIMATION_BOUND;
				fire_ball[i]->frame++;
				fire_ball[i]->frame_at_attack = fire_ball[i]->frame; //敵と干渉した瞬間のフレームが保持される
			}
			else {
				if (fire_ball[i]->frame < fire_ball[i]->frame_at_attack + 60) {
					fire_ball[i]->animation = FIRE_BALL_ANIMATION_BURST;
					fire_ball[i]->frame++;
				}
				else {
					InitFireBallInstance(fire_ball[i]);
				}
			}
		}
	}
	
	//プレイヤーのエネミーとの当たり判定
	//DoIntersectDetection(PLAYER_INTERSECT,ENEMY_INTERSECT, 1, player, enemy, mushroom, fire_ball, block, question);

	//プレイヤーのクリボーとの当たり判定
	DoIntersectDetection(PLAYER_INTERSECT, KURIBOO_INTERSECT, 1, KURIBOO_NUM, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//プレイヤーのノコノコとの当たり判定
	DoIntersectDetection(PLAYER_INTERSECT, NOKONOKO_INTERSECT, 1, NOKONOKO_NUM, false, player, nokonoko, empty_enemy, mushroom, fire_ball, block, question);

	//プレイヤーのキノコとの当たり判定
	DoIntersectDetection(PLAYER_INTERSECT, MUSHROOM_INTERSECT, 1, MUSHROOM_NUM, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//プレイヤーの1UPキノコとの当たり判定
	DoIntersectDetection(PLAYER_INTERSECT, MUSHROOM_1UP_INTERSECT, 1, MUSHROOM_1UP_NUM, false, player, kuriboo, empty_enemy, mushroom_1up, fire_ball, block, question);

	//プレイヤーのファイアフラワーとの当たり判定
	DoIntersectDetection(PLAYER_INTERSECT, FIRE_FLOWER_INTERSECT, 1, FIRE_FLOWER_NUM, false, player, kuriboo, empty_enemy, fire_flower, fire_ball, block, question);

	//プレイヤーのスターとの当たり判定
	DoIntersectDetection(PLAYER_INTERSECT, STAR_INTERSECT, 1, STAR_NUM, false, player, empty_enemy, empty_enemy, star, fire_ball, block, question);

	//プレイヤーのゴールとの当たり判定
	DoIntersectDetection(PLAYER_INTERSECT, GOAL_INTERSECT, 1, 1, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	
	//-------------------------------------
	//エネミーの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i]->left = enemy[i]->position + glm::vec2(0, ENEMY_HEIGHT / 2.0f);
		enemy[i]->right = enemy[i]->position + glm::vec2(ENEMY_WIDTH, ENEMY_HEIGHT / 2.0f);
		enemy[i]->top = enemy[i]->position + glm::vec2(ENEMY_WIDTH / 2.0f, 0);
		enemy[i]->bottom_left = enemy[i]->position + glm::vec2(0 + 1, ENEMY_HEIGHT); //両端のブロックにひっかからないように、角の座標を小さめに指定
		enemy[i]->bottom_right = enemy[i]->position + glm::vec2(ENEMY_WIDTH - 1, ENEMY_HEIGHT);
	}
	
	//エネミー自身の周囲との当たり判定
	DoIntersectDetection(ENEMY_INTERSECT, PART_INTERSECT, ENEMY_NUM, 1,false, player, enemy, empty_enemy, mushroom, fire_ball, block, question);

	//エネミーの（壊せる）ブロックとの当たり判定
	//（要変更　かなり重くなる）干渉する可能性のある組合わせを個別に処理することで解決
	DoIntersectDetection(ENEMY_INTERSECT, BREAKABLE_BLOCK_INTERSECT, ENEMY_NUM, 1, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question);
	


	//-------------------------------------
	//クリボーの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < KURIBOO_NUM; i++) {
		kuriboo[i]->left = kuriboo[i]->position + glm::vec2(0, KURIBOO_HEIGHT / 2.0f);
		kuriboo[i]->right = kuriboo[i]->position + glm::vec2(KURIBOO_WIDTH, KURIBOO_HEIGHT / 2.0f);
		kuriboo[i]->top = kuriboo[i]->position + glm::vec2(KURIBOO_WIDTH / 2.0f, 0);
		kuriboo[i]->bottom_left = kuriboo[i]->position + glm::vec2(0 + 1, KURIBOO_HEIGHT);
		kuriboo[i]->bottom_right = kuriboo[i]->position + glm::vec2(KURIBOO_WIDTH - 1, KURIBOO_HEIGHT);
	}

	//クリボー自身の周囲との当たり判定
	DoIntersectDetection(ENEMY_INTERSECT, PART_INTERSECT, KURIBOO_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//クリボーの（壊せる）ブロックとの当たり判定
	DoIntersectDetection(KURIBOO_INTERSECT, BREAKABLE_BLOCK_INTERSECT, KURIBOO_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//クリボー同士の当たり判定
	DoIntersectDetection(KURIBOO_INTERSECT, KURIBOO_INTERSECT, KURIBOO_NUM, 0, false, player, kuriboo, kuriboo, mushroom, fire_ball, block, question);

	//ノコノコとの当たり判定
	DoIntersectDetection(KURIBOO_INTERSECT, NOKONOKO_INTERSECT, KURIBOO_NUM, 0, false, player, kuriboo, nokonoko, mushroom, fire_ball, block, question);


	//-------------------------------------
	//ノコノコの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		nokonoko[i]->left = nokonoko[i]->position + glm::vec2(0, NOKONOKO_HEIGHT / 2.0f);
		nokonoko[i]->right = nokonoko[i]->position + glm::vec2(NOKONOKO_WIDTH, NOKONOKO_HEIGHT / 2.0f);
		nokonoko[i]->top = nokonoko[i]->position + glm::vec2(NOKONOKO_WIDTH / 2.0f, 0);
		nokonoko[i]->bottom_left = nokonoko[i]->position + glm::vec2(0 + 1, NOKONOKO_HEIGHT);
		nokonoko[i]->bottom_right = nokonoko[i]->position + glm::vec2(NOKONOKO_WIDTH - 1, NOKONOKO_HEIGHT);
	}

	//ノコノコ自身の周囲との当たり判定
	DoIntersectDetection(ENEMY_INTERSECT, PART_INTERSECT, NOKONOKO_NUM, 0, false, player, nokonoko, empty_enemy, mushroom, fire_ball, block, question);

	//クリボーとの当たり判定
	DoIntersectDetection(NOKONOKO_INTERSECT, KURIBOO_INTERSECT, NOKONOKO_NUM, 0, false, player, nokonoko, kuriboo, mushroom, fire_ball, block, question);


	//-------------------------------------
	//スーパーキノコの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < MUSHROOM_NUM; i++) {
		mushroom[i]->left = mushroom[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		mushroom[i]->right = mushroom[i]->position + glm::vec2(PART_WIDTH, PART_HEIGHT / 2.0f);
		mushroom[i]->top = mushroom[i]->position + glm::vec2(PART_WIDTH / 2.0f, 0);
		mushroom[i]->bottom_left = mushroom[i]->position + glm::vec2(0 + 3, PART_HEIGHT); //両端のブロックにひっかからないように、角の座標を小さめに指定
		mushroom[i]->bottom_right = mushroom[i]->position + glm::vec2(PART_WIDTH - 3, PART_HEIGHT); //エネミーよりも速度が速いため、より小さめにしないと隙間を乗り越えてしまう
	}

	//スーパーキノコ自身の周囲との当たり判定
	DoIntersectDetection(MUSHROOM_INTERSECT, PART_INTERSECT, MUSHROOM_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//スーパーキノコの（壊せる）ブロックとの当たり判定
	DoIntersectDetection(MUSHROOM_INTERSECT, BREAKABLE_BLOCK_INTERSECT, MUSHROOM_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);


	//-------------------------------------
	//1UPキノコの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
		mushroom_1up[i]->left = mushroom_1up[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		mushroom_1up[i]->right = mushroom_1up[i]->position + glm::vec2(PART_WIDTH, PART_HEIGHT / 2.0f);
		mushroom_1up[i]->top = mushroom_1up[i]->position + glm::vec2(PART_WIDTH / 2.0f, 0);
		mushroom_1up[i]->bottom_left = mushroom_1up[i]->position + glm::vec2(0 + 3, PART_HEIGHT); //両端のブロックにひっかからないように、角の座標を小さめに指定
		mushroom_1up[i]->bottom_right = mushroom_1up[i]->position + glm::vec2(PART_WIDTH - 3, PART_HEIGHT); //エネミーよりも速度が速いため、より小さめにしないと隙間を乗り越えてしまう
	}

	//1UPキノコ自身の周囲との当たり判定
	DoIntersectDetection(MUSHROOM_1UP_INTERSECT, PART_INTERSECT, MUSHROOM_1UP_NUM, 0, false, player, kuriboo, empty_enemy, mushroom_1up, fire_ball, block, question);


	//-------------------------------------
	//スターの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < STAR_NUM; i++) {
		star[i]->left = star[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		star[i]->right = star[i]->position + glm::vec2(PART_WIDTH, PART_HEIGHT / 2.0f);
		star[i]->top = star[i]->position + glm::vec2(PART_WIDTH / 2.0f, 0);
		star[i]->bottom_left = star[i]->position + glm::vec2(0 + 3, PART_HEIGHT); 
		star[i]->bottom_right = star[i]->position + glm::vec2(PART_WIDTH - 3, PART_HEIGHT); 
	}

	//スター自身の周囲との当たり判定
	DoIntersectDetection(STAR_INTERSECT, PART_INTERSECT, STAR_NUM, 0, false, player, empty_enemy, empty_enemy, star, fire_ball, block, question);

	//スターの（壊せる）ブロックとの当たり判定
	DoIntersectDetection(STAR_INTERSECT, BREAKABLE_BLOCK_INTERSECT, STAR_NUM, 0, false, player, empty_enemy, empty_enemy, star, fire_ball, block, question);


	//-------------------------------------
	//ファイヤボールの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		fire_ball[i]->left = fire_ball[i]->position + glm::vec2(0, PART_HEIGHT / 4.0f);
		fire_ball[i]->right = fire_ball[i]->position + glm::vec2(PART_WIDTH / 2.0f, PART_HEIGHT / 4.0f);
		fire_ball[i]->top = fire_ball[i]->position + glm::vec2(PART_WIDTH / 4.0f, 0);
		fire_ball[i]->bottom = fire_ball[i]->position + glm::vec2(PART_WIDTH / 4.0f, PART_HEIGHT / 2.0f);
		fire_ball[i]->bottom_left = fire_ball[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		fire_ball[i]->bottom_right = fire_ball[i]->position + glm::vec2(PART_WIDTH / 2.0f, PART_HEIGHT / 2.0f);
		if (fire_ball[i]->display_flag == false) {
			fire_ball[i]->position = player->position + glm::vec2(22.5, PART_HEIGHT/2.0f);
		}
		
	}

	//ファイヤボール自身の周囲との当たり判定
	//要修正　ファイアボールが階段を上ってしまう
	DoIntersectDetection(FIRE_BALL_INTERSECT, PART_INTERSECT, FIRE_BALL_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//ファイアボールの（壊せる）ブロックとの当たり判定
	DoIntersectDetection(FIRE_BALL_INTERSECT, BREAKABLE_BLOCK_INTERSECT, FIRE_BALL_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//ファイアボールのクリボーとの当たり判定
	DoIntersectDetection(FIRE_BALL_INTERSECT, KURIBOO_INTERSECT, FIRE_BALL_NUM, KURIBOO_NUM, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//ファイアボールのノコノコとの当たり判定
	DoIntersectDetection(FIRE_BALL_INTERSECT, NOKONOKO_INTERSECT, FIRE_BALL_NUM, NOKONOKO_NUM, false, player, nokonoko, empty_enemy, mushroom, fire_ball, block, question);

	//----------------------------------------------------------
	//その他干渉時の処理（最終的にはintersect.cppにまとめたい）
	//----------------------------------------------------------

	//クエスチョンボックスをたたいた時の処理
	for (int i = 0; i < QUESTION_BOX_NUM; i++) {
		if (coin[i]->flag == false) {
			if (IsGetQuestionbox(player->top, i) == i) {
				coin[i]->flag = true; //コインを出す
				question[i]->flag = false;

				if (i != 1) { //コイン以外が出るクエスチョンボックスを除く
					if (i != 4) {
						if (i != 13) {
							if (i != 14) {
								audioPlay(SE_GET_COIN);
							}
						}
					}
				}
				//コインの位置を設定
				coin[i]->velocity.y += -7.0f;
				if (question[i]->sound_flag == true) {
					audioPlay(SE_BLOCK_BREAK);
					question[i]->sound_flag = false;
				}
			}
		}
	}
	//キノコが出てくるクエスチョンボックスを個別に処理
	//指定するインデックスは、course.hの数字 - 1
	if (IsGetQuestionbox(player->top, 1) == 1) {
		mushroom[0]->display_flag = true;
	}
	//ファイヤフラワーが出てくるクエスチョンボックスを個別に処理
	if (IsGetQuestionbox(player->top, 4) == 4) {
		fire_flower[0]->display_flag = true;
	}

	//1UPキノコが出てくるクエスチョンボックスを個別に処理
	if (IsGetQuestionbox(player->top, 13) == 13) {
		mushroom_1up[0]->display_flag = true;
	}
	
	//スターが出てくるクエスチョンボックスを個別に処理
	if (IsGetQuestionbox(player->top, 14) == 14) {
		star[0]->display_flag = true;
	}
	
	/*
	//クエスチョンボックスから出たコインと干渉した時の処理
	for (int i = 0; i < QUESTION_BOX_COIN_NUM; i++) { //for (int i = 1; i <= QUESTION_BOX_COIN_NUM; i++) {
		if (coin[i]->flag == true) {
			if (IntersectCoin(player->bottom, i) == i || IntersectCoin(player->left, i) == i || IntersectCoin(player->right, i) == i || IntersectCoin(player->top, i) == i) {
				coin[i]->flag = false;
				coin[i]->sub_flag = false; //一度とったコインが再表示されないようにする
				if (coin[i]->sound_flag == true) {
					audioPlay(SE_GET_COIN); //音
					coin_num += 1;
					coin[i]->sound_flag = false;
				}
			}
		}
	}
	*/

	//フィールド上のコインと干渉した時の処理
	if (player->isInGround == false) { //地上
		for (int i = 0; i < FIELD_COIN_NUM; i++) {
			if (field_coin[i]->flag == true) {
				if (IntersectFieldCoin(player->bottom, i, false) == i || IntersectFieldCoin(player->left, i, false) == i || IntersectFieldCoin(player->right, i, false) == i || IntersectFieldCoin(player->top, i, false) == i) {
					field_coin[i]->flag = false;
					field_coin[i]->sub_flag = false; //一度とったコインが再表示されないようにする
					if (field_coin[i]->sound_flag == true) {
						audioPlay(SE_GET_COIN); //音
						coin_num += 1;
						field_coin[i]->sound_flag = false;
					}
				}
			}
		}
	}
	if (player->isInGround == true) { //地下
		for (int i = 0; i < GROUND_FIELD_COIN_NUM; i++) {
			if (ground_field_coin[i]->flag == true) {
				if (IntersectFieldCoin(player->bottom, i, true) == i || IntersectFieldCoin(player->left, i, true) == i || IntersectFieldCoin(player->right, i, true) == i || IntersectFieldCoin(player->top, i, true) == i) {
					ground_field_coin[i]->flag = false;
					ground_field_coin[i]->sub_flag = false; //一度とったコインが再表示されないようにする
					if (ground_field_coin[i]->sound_flag == true) {
						audioPlay(SE_GET_COIN); //音
						coin_num += 1;
						ground_field_coin[i]->sound_flag = false;
					}
				}
			}
		}
	}

	//ブロックをたたいた時の処理
	for (int i = 0; i < BLOCK_NUM; i++) {
		if (IsHitBlock(player->top, i, block) == i) {
			if (player->isBig == 1) { //キャラクターが小さい状態でたたいた時の処理
				block[i]->animation_flag = true;
				if (block[i]->sound_flag == true) {
					audioPlay(SE_BLOCK_BREAK);
				}
			}
			else { //キャラクターが大きい状態でたたいた時にブロックが壊れる処理
				block[i]->flag = false; //ブロックの表示をやめる
				if (block[i]->sound_flag == true) {
					audioPlay(SE_BLOCK_BREAK);
					block[i]->sound_flag = false; //二回目の音がならないようにする
				}
				//DrawSprite(TEX_BREAKED_BLOCK, BREAKED_BLOCK_WIDTH, BREAKED_BLOCK_WIDTH, 0, { x * PART_WIDTH - scrollX, y * PART_HEIGHT });
			}
		}
		if (block[i]->animation_flag == true) {
			if (block[i]->reach_top_flag == false) {
				block[i]->delta_position.y -= 0.4;
			}
			else {
				block[i]->delta_position.y += 0.4;
			}

			if (block[i]->delta_position.y <= -12) {
				block[i]->reach_top_flag = true;
			}

			if (block[i]->delta_position.y >= 0) {
				block[i]->animation_flag = false;
				block[i]->reach_top_flag = false;
			}
		}
		if (block[i]->flag == false) {
			if (block[i]->fring_calculate_flag == true) {
				//ブロックの破片の位置を決める
				//ブロックが壊れた後は位置の更新を行う必要はないため、以下は行わなくていい
				block[i]->fring_delta_position_1.x += 0.9f;
				block[i]->fring_delta_position_1.y = 0.065 * (block[i]->fring_delta_position_1.x) * (block[i]->fring_delta_position_1.x - 100);

				block[i]->fring_delta_position_2.x -= 0.9f;
				block[i]->fring_delta_position_2.y = 0.065 * (block[i]->fring_delta_position_2.x) * (block[i]->fring_delta_position_2.x + 100);

				block[i]->fring_delta_position_3.x += 0.7f;
				block[i]->fring_delta_position_3.y = 0.125 * (block[i]->fring_delta_position_3.x) * (block[i]->fring_delta_position_3.x - 50);

				block[i]->fring_delta_position_4.x -= 0.7f;
				block[i]->fring_delta_position_4.y = 0.125 * (block[i]->fring_delta_position_4.x) * (block[i]->fring_delta_position_4.x + 50);
			}
			if (block[i]->fring_delta_position_1.y > WINDOW_HEIGHT) {
				block[i]->fring_calculate_flag = false;
			}
		}
	}

	
	//コインブロックをたたいた時の処理
	for (int i = 0; i < COIN_BLOCK_NUM; i++) {
		if (IsHitCoinBlock(player->top, i, coin_block) == i) {
			
			if (block_coin[i]->isMax == false) { //まだブロック内にコインが残っている場合、以下の処理を行う
				block_coin[i]->vector_flag[BLOCK_COIN_NUM - coin_block[i]->haveCoinNum] = true; //コインを出す//ブロックを一回たたくと、10-10=0番目のフラグが立つ
				block_coin[i]->vector_velocity[BLOCK_COIN_NUM - coin_block[i]->haveCoinNum].y += -7.0f; //コインの位置を設定
				coin_block[i]->haveCoinNum -= 1;
				audioPlay(SE_GET_COIN);
				if (coin_block[i]->haveCoinNum == 0) {
					block_coin[i]->isMax = true;
					coin_block[i]->flag = false;
				}
			}
			coin_block[i]->animation_flag = true; //ブロック自身のアニメーションをオンにする
			if (coin_block[i]->sound_flag == true) {
				audioPlay(SE_BLOCK_BREAK);
			}
			
		}
		//ブロックの動きの処理
		if (coin_block[i]->animation_flag == true) {
			if (coin_block[i]->reach_top_flag == false) {
				coin_block[i]->delta_position.y -= 0.6;
			}
			else {
				coin_block[i]->delta_position.y += 0.6;
			}

			if (coin_block[i]->delta_position.y <= -12) {
				coin_block[i]->reach_top_flag = true;
			}

			if (coin_block[i]->delta_position.y >= 0) {
				coin_block[i]->animation_flag = false;
				coin_block[i]->reach_top_flag = false;
			}
		}
	}


	//---------------------------------------------
	//敵やアイテムの位置の設定
	//---------------------------------------------

	/*
	//一匹目の敵の位置の設定 //横移動 //初期位置は(180,585)
	for (int i = 0; i < 1; i++) { //それぞれ個別に設定する必要がある
		if (enemy[i]->reach_right_flag == false) {
			enemy[i]->position.x += 0.1;
		}
		else {
			enemy[i]->position.x -= 0.1;
		}

		if (enemy[i]->position.x >= 270.0) { //ブロック一個の幅が45なので、270-18=90でブロック二つ分
			enemy[i]->reach_right_flag = true;
		}

		if (enemy[i]->position.x <= 180.0) {
			enemy[i]->reach_right_flag = false;
		}
	}
	

	//二匹目の敵の位置の設定
	if (player->position.x + WINDOW_WIDTH / 2.0f >= enemy[1]->position.x) {
		enemy[1]->animation_flag =  true;
	}
	if (enemy[1]->animation_flag == true) {
		enemy[1]->position.x -= 0.1;
	}
	/*
	//三匹目の敵の位置の設定、土管での跳ね返りを追加
	if (player->position.x + WINDOW_WIDTH / 2.0f >= enemy[2]->position.x) {
		enemy[2]->animation_flag = true;
	}
	if (enemy[2]->animation_flag == true) {
		if (enemy[2]->isMirror == false) {
			enemy[2]->position.x -= 0.1;
		}
		else {
			enemy[2]->position.x += 0.1;
		}
	}
	*/

	
	//クリボーの位置の設定
	for (int i = 0; i < KURIBOO_NUM; i++) {
		if (kuriboo[i]->display_flag == true && kuriboo[i]->isDeath == false) { //クリボーが踏まれて消えた後は、以下を行う必要はない
			//プレイヤーが近づいてきたらアニメーションを開始する
			if (player->position.x + WINDOW_WIDTH / 2.0f >= kuriboo[i]->position.x) {
				kuriboo[i]->animation_flag = true;
			}
			if (kuriboo[i]->stepped_on_flag == false) { //まだ踏まれてない間は、位置を更新する
				if (kuriboo[i]->animation_flag == true) {
					if (kuriboo[i]->isMirror == false) {
						kuriboo[i]->position.x -= 0.5;
					}
					else {
						kuriboo[i]->position.x += 0.5;
					}
				}
			}
		}
		if (kuriboo[i]->isDeath == true) { //ブロックの下から突き上げられたり、ファイアボールに当たって死んだとき
			if (kuriboo[i]->delta_psition_calculate_flag == true) {
				kuriboo[i]->delta_position.x += 0.9f;
				kuriboo[i]->delta_position.y = 0.065 * (kuriboo[i]->delta_position.x) * (kuriboo[i]->delta_position.x - 100);
				if (kuriboo[i]->delta_position.y > WINDOW_HEIGHT) {
					kuriboo[i]->delta_psition_calculate_flag = false;
				}
			}
		}
	}

	//ノコノコの位置の設定
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		if (nokonoko[i]->display_flag == true && nokonoko[i]->isDeath == false) { //ノコノコが踏まれて消えた後は、以下を行う必要はない
			//プレイヤーが近づいてきたらアニメーションを開始する
			if (player->position.x + WINDOW_WIDTH / 2.0f >= nokonoko[i]->position.x) {
				nokonoko[i]->animation_flag = true;
			}
			if (nokonoko[i]->animation_flag == true) { //まだ踏まれてない間は、位置を更新する
				if (nokonoko[i]->stepped_on_flag == false) {
					if (nokonoko[i]->isMirror == false) {
						nokonoko[i]->position.x -= 0.3;
					}
					else {
						nokonoko[i]->position.x += 0.3;
					}
				}
				else if (nokonoko[i]->stepped_on_flag == true) { //踏まれて甲羅状態になった時の処理
					if (nokonoko[i]->isShellMoving == true) {
						if (nokonoko[i]->touch_left_flag == true) { //甲羅状態で左側をたたかれた場合
							if (nokonoko[i]->isMirror == false) {
								nokonoko[i]->position.x -= 3.0f;
							}
							else {
								nokonoko[i]->position.x += 3.0f;
							}
						}
						else if (nokonoko[i]->touch_right_flag == true) { //甲羅状態で左側をたたかれた場合
							if (nokonoko[i]->isMirror == false) {
								nokonoko[i]->position.x -= 3.0f;
							}
							else {
								nokonoko[i]->position.x += 3.0f;
							}
						}
						else if (nokonoko[i]->touch_top_flag == true) { //甲羅状態で踏まれた場合
							if (nokonoko[i]->isMirror == false) {
								nokonoko[i]->position.x -= 3.0f;
							}
							else {
								nokonoko[i]->position.x += 3.0f;
							}
						}
					}
				}
			}
		}
		if (nokonoko[i]->isDeath == true) { //ブロックの下から突き上げられたり、ファイアボールに当たって死んだとき
			if (nokonoko[i]->delta_psition_calculate_flag == true) {
				nokonoko[i]->delta_position.x += 0.9f;
				nokonoko[i]->delta_position.y = 0.065 * (nokonoko[i]->delta_position.x) * (nokonoko[i]->delta_position.x - 100);
				if (nokonoko[i]->delta_position.y > WINDOW_HEIGHT) {
					nokonoko[i]->delta_psition_calculate_flag = false;
				}
			}
		}
	}
	
	//スーパーキノコの位置の設定
	for (int i = 0; i < MUSHROOM_NUM; i++) {
		if (mushroom[i]->display_flag == true) {
			if (mushroom[i]->isMirror == false) {
				mushroom[i]->position.x += 0.5;
			}
			else {
				mushroom[i]->position.x -= 0.5;
			}
		}
	}

	//1UPキノコの位置の設定
	for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
		if (mushroom_1up[i]->display_flag == true) {
			if (mushroom_1up[i]->isMirror == false) {
				mushroom_1up[i]->position.x += 0.5;
			}
			else {
				mushroom_1up[i]->position.x -= 0.5;
			}
		}
	}

	//スターの位置の設定
	for (int i = 0; i < STAR_NUM; i++) {
		if (star[i]->display_flag == true) {
			if (star[i]->isMirror == false) {
				star[i]->position.x += 1.0f;
			}
			else {
				star[i]->position.x -= 1.0f;
			}
		}
	}

	//ファイアボールの位置の設定
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				if (fire_ball[i]->isMirror == false) {
					fire_ball[i]->position.x += 3.0;
				}
				else {
					fire_ball[i]->position.x -= 3.0;
				}
			}
		}
		//ファイアボールが画面外に消えたら、ファイアボールのインスタンスを初期化する
		if (player->position.x + WINDOW_WIDTH  <= fire_ball[i]->position.x || player->position.x - WINDOW_WIDTH >= fire_ball[i]->position.x || fire_ball[i]->position.y > WINDOW_HEIGHT * 1.5) {
			InitFireBallInstance(fire_ball[i]);
		}
		//ファイアボールがコースパーツに当たった時の処理は、intersect.cpp の DoIntersectDetection 内で行う
		//ファイアボールが敵に当たった時や、コースパーツに当たった時も、適切にインスタンスの初期化を行う
	}

	//コインの位置の設定。重力を与え、速度を位置に追加する
	for (int i = 0; i < QUESTION_BOX_NUM; i++) {
		if (coin[i]->sub_flag == true) { //処理を一度だけ行う
			if (coin[i]->flag == true) {
				coin[i]->velocity.y += 0.2f;
				coin[i]->delta_pos += coin[i]->velocity;
			}
			if (coin[i]->delta_pos.y >= 1.0f) { //飛び跳ねたコインが元の位置に戻ったら描画をやめる
				coin[i]->sub_flag = false;
			}
		}
	}

	
	//レンガブロックから出てきたコインの位置の設定。重力を与え、速度を位置に追加する
	for (int i = 0; i < COIN_BLOCK_NUM; i++) {
		for (int j = 0; j < BLOCK_COIN_NUM; j++) {
			if (block_coin[i]->vector_sub_flag[j] == true) { //処理を一度だけ行う
				if (block_coin[i]->vector_flag[j] == true) {
					block_coin[i]->vector_velocity[j].y += 0.2f;
					block_coin[i]->vector_delta_pos[j] += block_coin[i]->vector_velocity[j];
				}
				if (block_coin[i]->vector_delta_pos[j].y >= 1.0f) { //飛び跳ねたコインが元の位置に戻ったら描画をやめる
					block_coin[i]->vector_sub_flag[j] = false;
				}
			}
		}
	}
	

	//ゴールの旗の位置の設定
	if (player->isGoal == true) {
		if (player->flag_pos.y <= 540) {
			player->flag_pos.y += 1.2;
		}
	}

	//ベクターのサイズが１３なので１３以上の要素へのアクセスはエラー
	//coinのサイズはなぜか１６
	//クエスチョンボックスから出たコインのアニメーション
	for (int i = 0; i < QUESTION_BOX_COIN_NUM; i++) {
		coin[i]->frame++;
	}

	//フィールド上のコインのアニメーション
	if (player->isInGround == false) { //地上
		for (int i = 0; i < FIELD_COIN_NUM; i++) {
			field_coin[i]->frame++;
		}
	}
	else if (player->isInGround == true) { //地下
		for (int i = 0; i < GROUND_FIELD_COIN_NUM; i++) {
			ground_field_coin[i]->frame++;
		}
	}

	//レンガブロックから出た	コインのアニメーション
	for (int i = 0; i < COIN_BLOCK_NUM; i++) {
		for (int j = 0; j < BLOCK_COIN_NUM; j++) {
			block_coin[i]->vector_frame[j]++;
		}
	}
	
	//クエスチョンボックスのアニメーション
	for (int i = 0; i < QUESTION_BOX_NUM; i++) {
		question[i]->frame++;
	}

	glutPostRedisplay();

	if (COMMAND_MODE == 2) {
		VoiceCommand();
	}
	
	//デスクトップパソコンでは、実際のフレームレートと処理速度が異なるため、IdleFuncではなくタイマーの再起呼び出しで処理する
	//第一引数は1000/60で、正しいフレームレートになるが、後から設定したため、適度な処理速度になるように、自分で設定した。
	glutTimerFunc(1000 / 180, Idle, 0);
}



//----------------------------
//キーボード入力を扱う関数
//----------------------------
//キーを押して、離した後もつづけたい処理をここに書く
//キーを押している間だけ続ける処理は、keyIsPressedで別の所で処理する
void Keyboard(unsigned char _key, int x, int y) {
	keysIsPressed[_key] = true;
	if (_key == 'j') {
		player->isMirror = true;
		for (int i = 0; i < FIRE_BALL_NUM; i++) {
			fire_ball[i]->isLookingLeft = true;
			if (fire_ball[i]->display_flag == false) {
				fire_ball[i]->isMirror = true;
			}
		}
	}
	if (_key == 'l') {
		player->isMirror = false;
		for (int i = 0; i < FIRE_BALL_NUM; i++) {
			fire_ball[i]->isLookingLeft = false;
			if (fire_ball[i]->display_flag == false) {
				fire_ball[i]->isMirror = false;
			}
		}

		//横向きの土管に入るときの処理(1-1)
		if (player->isInGround == true) {
			if (IntersectPipe(player->right, RIGHT, true)) {
				if (player->inPipe == false) { //lを連打したときに、音が何度もならないようにする
					audioPlay(SE_IN_PIPE);
				}
				player->inPipe = true;
			}
		}
		//横向きの土管に入るときの処理(1-2)
		if (player->course == START || player->course == MAIN) {
			if (IntersectPipe_1_2(player->right, RIGHT, player->course)) {
				if (player->inPipe == false) {
					audioPlay(SE_IN_PIPE);
				}
				player->inPipe = true;
			}
		}
	}
	if (_key == 'k') {
		if (player->isFire == true) {
			fire_ball[player->fire_ball_counter % FIRE_BALL_NUM]->display_flag = true;
			player->fire_ball_counter++;
			audioPlay(SE_FIRE_BALL);
		}
	}
	if (_key == 'c') {
		//player->isRunning = 2.0f;
	}
	if (_key == 'm') {
		player->isSquating = true;

		//縦向きの土管に入るときの処理(1-1)
		if (IntersectPipe(player->bottom, BOTTOM, false)) {
			if (player->inPipe == false) { //mを連打したときに、音が何度もならないようにする
				audioPlay(SE_IN_PIPE); 
			}
			player->inPipe = true;
		}
	}

	if (_key == 'f') {//////////////
		for (int i = 0; i < QUESTION_BOX_COIN_NUM; i++) {//////////////
			printf("coin[%d]->flag = %d\n", i, coin[i]->flag);//////////////
		}///////////////
	}

	switch (_key) {
	case 0x1b: exit(0); break;
	case 'p': audioPlay(MUSIC_STAGE); break;
	case 's': audioStop(MUSIC_STAGE); break;
		//case 'b': glutDestroyWindow("title"); //break;
		//case 'h': audioPlay(SE_BLOCK_HIT); break;
	default: break;
	}

	if ((_key == ' ') && (!player->isJumping)) {
		player->velocity.y -= 10.0f;
		audioPlay(VOICE_JUMP_1);
	}

}

/*
void SpecialKey(int _key, int _x, int _y) {
	keysIsPressed[_key] = true;
	float acceleration = 0.2f;

	switch (_key) {
	case GLUT_KEY_LEFT:
		player.isMirror = true;
		player.velocity.x -= acceleration;
		player.animation = ANIMATION_RUN;
	case GLUT_KEY_RIGHT:
		player.isMirror = false;
		player.velocity.x += acceleration;
		player.animation = ANIMATION_RUN;
	}
}
*/

void KeyboardUp(unsigned char _key, int _x, int _y) {
	keysIsPressed[_key] = false;
	if (_key == 'm') {
		player->isSquating = false;
	}
}



//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//これ以降は1-2に関するプログラム
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------
//1-2のデータを初期化する関数
//------------------------------------
void Init_1_2(void) {
	
	AllTexFromFile();

	player = MakePlayerInstance();
	coin_1_2 = MakeCoinInstance(QUESTION_BOX_NUM_1_2); //クエスチョンボックスから出るコイン
	block_1_2 = MakeBlockInstance(BLOCK_NUM_1_2);
	question_1_2 = MakeQuestionInstance(QUESTION_BOX_NUM_1_2);
	kuriboo_1_2 = MakeEnemyInstance(KURIBOO_WIDTH, KURIBOO_HEIGHT, KURIBOO_NUM_1_2);
	nokonoko_1_2 = MakeEnemyInstance(NOKONOKO_WIDTH, NOKONOKO_HEIGHT, NOKONOKO_NUM_1_2);
	mushroom_1_2 = MakeMushroomInstance(MUSHROOM_NUM_1_2);
	fire_flower_1_2 = MakeFireFlowerInstance(FIRE_FLOWER_NUM_1_2);
	
	fire_ball = MakeFireBallInstance(); //これがないとなぜか実行エラー
	lift = MakeLiftInstance();

	player->position = { 95, PART_HEIGHT * 8};
	//player->position = { 3000, PART_HEIGHT * 8 };
	player->flag_pos = { 945 + 22.5, 180 };
	player->course = START;
	player->isScroll = false;
	player->isBig = 1;
	player->isFire = false;
	player->isStar = false;

	//ファイアボールの初期位置を設定
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		fire_ball[i]->position = player->position + glm::vec2(22.5, 0);
	}

	//クリボーの位置設定
	kuriboo_1_2[0]->position = { 620, 585 };
	kuriboo_1_2[1]->position = { 665, 585 };
	kuriboo_1_2[2]->position = { 1350, 350 };
	kuriboo_1_2[3]->position = { 2880, 585 };
	kuriboo_1_2[4]->position = { 3015, 585 };
	kuriboo_1_2[5]->position = { 3290, 180 };
	kuriboo_1_2[6]->position = { 3460, 405 };
	kuriboo_1_2[7]->position = { 3550, 405 };
	kuriboo_1_2[8]->position = { 4410, 585 };
	kuriboo_1_2[9]->position = { 4500, 585 };
	kuriboo_1_2[10]->position = { 4590, 585 };
	kuriboo_1_2[11]->position = { 5130, 585 };
	kuriboo_1_2[12]->position = { 6075, 495 };
	kuriboo_1_2[13]->position = { 6165, 405 };

	//ノコノコの位置を設定
	nokonoko_1_2[0]->position = { 1885,  PART_HEIGHT * 13 };
	nokonoko_1_2[1]->position = { 1975,  PART_HEIGHT * 13 };//

	//キノコの初期位置を設定
	mushroom_1_2[0]->position = {450, 405};

	//ファイアフラワーの初期位置を設定
	fire_flower_1_2[0]->position = { 3105, 360 };

	//リフトの位置を設定
	lift[0]->position = { 6320, WINDOW_HEIGHT * ( 1 / 4.0f) };
	lift[1]->position = { 6320, WINDOW_HEIGHT * (3 / 4.0f) };
	lift[2]->position = { 6975, WINDOW_HEIGHT * (1 / 4.0f) };
	lift[3]->position = { 6975, WINDOW_HEIGHT * (3 / 4.0f) };

}


//-------------------------------------
//1-2のコースパーツを描画する関数
//-------------------------------------
void DrawPart_1_2(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, int _course) {

	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return;
	}
	
	if (_course == MAIN) { //メインコースの描画 //地下
		glClearColor(0.0, 0.0, 0.0, 1.0);

		if (_c == 'b') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_GROUND_BLOCK_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_GROUND_BLOCK_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2':
				//１－２はレンガブロックが377個もあり、重すぎるので、いったん区別せずに描画する
				/*
				for (int i = 0; i < BLOCK_NUM_1_2; i++) {
					DrawBlock(_position, _c, _sub_c, _sub2_c, i, true, COURSE_1_2);
				}
				*/
				DrawSprite(TEX_GROUND_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
				break;
			default: break;
			}
		}
		else if (_c == 'q') {
			for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
				DrawQuestionBox(_position, _c, _sub2_c, i, COURSE_1_2);
			}
		}
		else if (_c == 'p') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_GROUND_PIPE_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_GROUND_PIPE_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_GROUND_PIPE_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_GROUND_PIPE_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_GROUND_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '5': DrawSprite(TEX_GROUND_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '6': DrawSprite(TEX_GROUND_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '7': DrawSprite(TEX_GROUND_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '8': DrawSprite(TEX_JOINT_GROUND_PIPE_26, PART_WIDTH, PART_HEIGHT, 0, _position); break; //接合部
			case '9': DrawSprite(TEX_JOINT_GROUND_PIPE_27, PART_WIDTH, PART_HEIGHT, 0, _position); break; //接合部
			default: break;
			}
		}
		else if (_c == ' ') {

			glColor3ub(0x00, 0x00, 0x00);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			glPushMatrix();
			{
				glTranslatef(_position.x, _position.y, 0);
				glBegin(GL_QUADS);
				{
					glVertex2f(0, 0);
					glVertex2f(0, PART_HEIGHT);
					glVertex2f(PART_WIDTH, PART_HEIGHT);
					glVertex2f(PART_WIDTH, 0);
				}
				glEnd();
			}
			glPopMatrix();
		}
	}
	else if (_course == START || _course == GOAL) { //地上
		glClearColor(0.31372549, 0.50196078, 1.0, 1.0);  //(80,128, 255)背景色を空色にする

		if (_c == 'b') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_BLOCK_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_BLOCK_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': //破壊できるブロックはない
			default: break;
			}
		}
		else if (_c == 'q') { //クエスチョンボックスはない
		}
		else if (_c == 'p') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_PIPE_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_PIPE_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_PIPE_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_PIPE_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '5': DrawSprite(TEX_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '6': DrawSprite(TEX_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '7': DrawSprite(TEX_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, _position); break; //横向きの土管
			case '8': DrawSprite(TEX_JOINT_PIPE_26, PART_WIDTH, PART_HEIGHT, 0, _position); break; //接合部
			case '9': DrawSprite(TEX_JOINT_PIPE_27, PART_WIDTH, PART_HEIGHT, 0, _position); break; //接合部
			default: break;
			}
		}
		else if (_c == 'f') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_FORT_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_FORT_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_FORT_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_FORT_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_FORT_4, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '5': DrawSprite(TEX_FORT_5, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '6': DrawSprite(TEX_FORT_6, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			default: break;
			}
		}
		else if (_c == 'c') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_CLOUD_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_CLOUD_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_CLOUD_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_CLOUD_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_CLOUD_4, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '5': DrawSprite(TEX_CLOUD_5, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			default: break;
			}
		}
		else if (_c == 'g') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_GOAL_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_GOAL_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			default: break;
			}
		}
		else if (_c == ' ') {

			glColor3ub(0x50, 0x80, 0xff);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			glPushMatrix();
			{
				glTranslatef(_position.x, _position.y, 0);
				glBegin(GL_QUADS);
				{
					glVertex2f(0, 0);
					glVertex2f(0, PART_HEIGHT);
					glVertex2f(PART_WIDTH, PART_HEIGHT);
					glVertex2f(PART_WIDTH, 0);
				}
				glEnd();
			}
			glPopMatrix();
		}
	}

}



//-------------------------------
//1-2を描画する関数
//-------------------------------
void Display_1_2(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float scrollX = player->position.x - WINDOW_WIDTH / 2 + PLAYER_WIDTH / 2;

	//コースパーツを描画
	/*
	if (player->course == MAIN) { //マリオがメインコースにいるとき
		for (int y = 0; y < COURSE_HEIGHT; y++) {
			for (int x = 0; x < COURSE_WIDTH; x++) {
				DrawPart_1_2({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, course_1_2[y][x], sub_course_1_2[y][x], sub2_course_1_2[y][x], MAIN);
			}
		}
	}
	*/
	if (player->course == MAIN) { //マリオがメインコースにいるとき
		for (int y = 0 ; y < COURSE_HEIGHT; y++) {
			for (int x = int((player->position.x -  WINDOW_WIDTH / 2)/ 45); x < int((player->position.x + WINDOW_WIDTH / 2 + 90) / 45); x++) { //マップスクロールの軽減
				DrawPart_1_2({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, course_1_2[y][x], sub_course_1_2[y][x], sub2_course_1_2[y][x], MAIN);
			}
		}
	}
	else if (player->course == START) { //スタートコースにいるとき
		for (int y = 0; y < START_COURSE_HEIGHT_1_2; y++) {
			for (int x = 0; x < START_COURSE_WIDTH_1_2; x++) {
				DrawPart_1_2({ x * PART_WIDTH, y * PART_HEIGHT }, start_course_1_2[y][x], start_sub_course_1_2[y][x], start_sub2_course_1_2[y][x], START);
			}
		}
	}
	else if (player->course == GOAL) { //ゴールコースにいるとき
		for (int y = 0; y < GOAL_COURSE_HEIGHT_1_2; y++) {
			for (int x = 0; x < GOAL_COURSE_WIDTH_1_2; x++) {
				DrawPart_1_2({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, goal_course_1_2[y][x], goal_sub_course_1_2[y][x], goal_sub2_course_1_2[y][x], GOAL);
			}
		}
	}

	//クエスチョンボックスから出たコインを描画
	for (int i = 0; i < QUESTION_BOX_COIN_NUM_1_2; i++) {
		DrawCoin(i, COURSE_1_2);
	}
	

	//リフトを描画
	for (int i = 0; i < LIFT_NUM; i++) {
		DrawSprite(TEX_LIFT, PART_WIDTH * 5, PART_HEIGHT, 0,lift[i]->position + glm::vec2(-scrollX, 0));
	}

	//ゴールの旗を描画
	//これを先に描かないと、キャラクターがゴールの旗に隠されてしまう
	if (player->course == GOAL) {
		DrawSprite(TEX_FLAG, PART_WIDTH, PART_HEIGHT, 0,
			player->flag_pos + glm::vec2(-scrollX, 0));
	}

	//マリオを描画 //（スクロールするかしないかで分ける）つもりだったが、scrollXにplayer->isScrollを描けることで解決
	if (player->isBig == 1) {
		if (player->isStar == false) {
			DrawSprite(TEX_MARIO, PLAYER_WIDTH, PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame * player->isRunning / 31) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (player->isScroll), 0),
				player->isMirror
			);
		}
		else if (player->isStar == true) {
			DrawSprite(TEX_MARIO_STAR, PLAYER_WIDTH, PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame / 15) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (player->isScroll), 0),
				player->isMirror
			);
		}
	}
	else if (player->isBig == 2) {
		if (player->isFire == false && player->isStar == false) {
			DrawSprite(TEX_MARIO_BIG, PLAYER_WIDTH, 2 * PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame / 31) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (player->isScroll), 0),
				player->isMirror
			);
		}

		else if (player->isFire == true && player->isStar == false) {
			DrawSprite(TEX_MARIO_FIRE, PLAYER_WIDTH, 2 * PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame / 31) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (player->isScroll), 0),
				player->isMirror
			);
		}

		else if (player->isStar == true) {
			DrawSprite(TEX_MARIO_STAR_BIG, PLAYER_WIDTH, 2 * PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame / 15) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (player->isScroll), 0),
				player->isMirror
			);
		}
	}

	//ファイアボールを描画
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			DrawSprite(TEX_FIRE_BALL, PART_WIDTH, PART_HEIGHT,
				fire_ball_animations[fire_ball[i]->animation].frames[(fire_ball[i]->frame / 31) % animations[fire_ball[i]->animation].count],
				fire_ball[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 16.0f, PART_HEIGHT / 16.0f)
			);
		}
	}

	//クリボーを描画
	if (player->course == MAIN) {
		for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
			if (kuriboo_1_2[i]->display_flag == true && kuriboo_1_2[i]->isDeath == false) { //踏まれる、またはブロックの下からたたかれて死ぬまで描画する
				//printf("%d", i);
				DrawSprite(TEX_GROUND_KURIBOO, KURIBOO_WIDTH, KURIBOO_HEIGHT,
					kuriboo_animations[kuriboo_1_2[i]->animation].frames[(kuriboo_1_2[i]->frame / 63) % animations[kuriboo_1_2[i]->animation].count],
					kuriboo_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(KURIBOO_WIDTH / 16.0f, KURIBOO_HEIGHT / 16.0f),
					kuriboo_1_2[i]->isMirror
				);
			}
			//クリボーがひっくり返って死ぬアニメーションを描画
			if (kuriboo_1_2[i]->isDeath == true) {
				DrawSprite(TEX_GROUND_KURIBOO, KURIBOO_WIDTH, KURIBOO_HEIGHT,
					3, //Kuriboo.bmpファイルの４番目の（ひっくり返った）画像を表示する
					kuriboo_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(KURIBOO_WIDTH / 16.0f, KURIBOO_HEIGHT / 16.0f) + kuriboo_1_2[i]->delta_position,
					kuriboo_1_2[i]->isMirror
				);
			}
		}
	}


	//ノコノコを描画
	if (player->course == MAIN) {
		for (int i = 0; i < NOKONOKO_NUM_1_2; i++) {
			if (nokonoko_1_2[i]->display_flag == true && nokonoko_1_2[i]->isDeath == false) { //踏まれる、または死ぬまで描画する
				if (nokonoko_1_2[i]->stepped_on_flag == false) {
					DrawSprite(TEX_GROUND_NOKONOKO, NOKONOKO_WIDTH, NOKONOKO_HEIGHT,
						nokonoko_animations[nokonoko_1_2[i]->animation].frames[(nokonoko_1_2[i]->frame / 31) % animations[nokonoko_1_2[i]->animation].count],
						nokonoko_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_WIDTH / 15.0f, NOKONOKO_HEIGHT / 15.0f),
						nokonoko_1_2[i]->isMirror
					);
				}
				else { //甲羅状態のノコノコ
					DrawSprite(TEX_NOKONOKO_SHELL, NOKONOKO_SHELL_WIDTH, NOKONOKO_SHELL_HEIGHT,
						0,
						nokonoko_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(0/*NOKONOKO_SHELL_WIDTH / 18.0f*/, 0),
						nokonoko_1_2[i]->isMirror
					);
				}
			}
			//ノコノコがひっくり返って死ぬアニメーションを描画
			if (nokonoko_1_2[i]->isDeath == true) {
				if (nokonoko_1_2[i]->stepped_on_flag == false) { //ノコノコが歩いて入りるときにファイアボールに殺された場合
					DrawSprite(TEX_NOKONOKO, NOKONOKO_WIDTH, NOKONOKO_HEIGHT,
						2, //ファイルの2番目の（ひっくり返った）画像を表示する
						nokonoko_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_WIDTH / 16.0f, NOKONOKO_HEIGHT / 16.0f) + nokonoko_1_2[i]->delta_position,
						nokonoko_1_2[i]->isMirror
					);
				}
				else { //ノコノコが甲羅の時にファイアボールに殺された場合
					DrawSprite(TEX_NOKONOKO_SHELL, NOKONOKO_SHELL_WIDTH, NOKONOKO_SHELL_HEIGHT,
						2, //ファイルの2番目の（ひっくり返った）画像を表示する
						nokonoko_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_SHELL_WIDTH / 16.0f, NOKONOKO_SHELL_HEIGHT / 16.0f) + nokonoko_1_2[i]->delta_position,
						nokonoko_1_2[i]->isMirror
					);
				}
			}
		}
	}


	//スーパーキノコを描画
	for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
		if (mushroom_1_2[i]->display_flag == true) {
			if (mushroom_1_2[i]->get_flag == false) {
				DrawSprite(TEX_MUSHROOM, PART_WIDTH, PART_HEIGHT, 0,
					mushroom_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)
				);
			}
		}
	}

	//ファイアフラワーを描画
	for (int i = 0; i < FIRE_FLOWER_NUM_1_2; i++) {
		if (fire_flower_1_2[i]->display_flag == true) {
			if (fire_flower_1_2[i]->get_flag == false) {
				DrawSprite(TEX_FIRE_FLOWER, PART_WIDTH, PART_HEIGHT,
					fire_flower_animations[fire_flower_1_2[i]->animation].frames[(fire_flower_1_2[i]->frame / 15) % animations[fire_flower_1_2[i]->animation].count],
					fire_flower_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)
				);
			}
		}
	}

	//入れる土管だけ上書き
	if (player->course == START) {
		DrawSprite(TEX_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, { 450, 540 });
		DrawSprite(TEX_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, { 450, 585 });
		DrawSprite(TEX_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, { 495, 540 });
		DrawSprite(TEX_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, { 495, 585 });
	}
	if (player->course == MAIN) {
		DrawSprite(TEX_GROUND_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, { 7515 - scrollX, 405 });
		DrawSprite(TEX_GROUND_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, { 7515 - scrollX, 450 });
		DrawSprite(TEX_GROUND_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, { 7560 - scrollX, 405 });
		DrawSprite(TEX_GROUND_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, { 7560 - scrollX, 450 });
	}





	fontBegin();
	fontSetWeight(2.5); //最小0.5、最大10.0
	fontSetColor(0xff - 0x10, 0xff - 0x10, 0xff - 0x10); //微妙に暗めの白色
	fontFont(FONT_FONT_MONO_ROMAN);
	fontSetSize(FONT_DEFAULT_SIZE / 5.5);

	fontSetSize(FONT_DEFAULT_SIZE / 5);////deback
	fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 0.5);////deback
	fontDraw("player->x  %lf", player->position.x);////deback
	fontSetPosition(WINDOW_WIDTH * 0.3, fontGetsize() * 2);////deback
	fontDraw("player->y  %lf", player->position.y);////deback

	glutSwapBuffers();
}


//-----------------------------------------------
//アイドル時の処理（1-2）
//-----------------------------------------------
void Idle_1_2(int value) {
	float acceleration = 0.2f;

	if (keysIsPressed['j']) {
		player->velocity.x -= acceleration;
	}

	if (keysIsPressed['l']) {
		player->velocity.x += acceleration;
	}

	//キャラクターに物理法則を適用（抵抗と重力）
	player->velocity.x *= 0.93f;
	player->velocity.y += 0.2f;
	player->position += player->velocity;
	player->frame++;

	
	//マリオのスターの秒数を制御する
	if (player->isStar == false) {
		player->tmp_star_frame = player->frame;
	}
	else if (player->isStar == true) {
		if (player->frame > player->tmp_star_frame + 180 * 8) { //いま、180fpsで描画しているため、スターを８秒間続けるために180*8
			player->isStar = false;
			audioStop(MUSIC_STAR); //スターのBGMを止め、
			//audioPlay(MUSIC_STAGE); //ステージのBGMを流す
		}
	}
	

	//キャラクターがコース外に行った時のリセット処理
	if ((player->position.y >= WINDOW_HEIGHT + PART_HEIGHT * 100)) {
		player->position = { WINDOW_WIDTH / 2.0f,  PART_HEIGHT * (13 - (player->isBig - 1)) };
		player->isBig = 1;
		player->isFire = false;
	}

	/*
	//縦の土管に入るときの処理
	if (!(IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true)) {
		player->tmp1_pos = player->position;
	}
	else if (IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.y += 0.1f;
		if (player->position.y > player->tmp1_pos.y + 135) { //マリオが土管に入りきったらワープ
			player->isInGround = true;
			player->position = { 45 * 2, 0 };
			player->inPipe = false; //ワープしたら土管に入り中であるというフラグをおろす
			audioStop(MUSIC_STAGE); //地上の音楽を止めて
			audioPlay(MUSIC_UNDER_GROUND_STAGE); //地下の音楽を流す
		}
	}
	*/
	//スタートコースの土管に入るときの処理
	if (!(IntersectPipe_1_2(player->right, RIGHT, START) && player->inPipe == true)) {
		player->tmp1_pos = player->position;
	}
	else if (IntersectPipe_1_2(player->right, RIGHT, START) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.x += 0.15f;
		if (player->position.x > player->tmp1_pos.x + 50) { //マリオが土管に入りきったらワープ
			player->position = { 160, PART_HEIGHT * 1 };
			player->course = MAIN;
			player->isScroll = true;
			
			player->inPipe = false; //ワープしたら土管に入り中であるというフラグをおろす
			//audioStop(MUSIC_UNDER_GROUND_STAGE); //地下の音楽を止め
			//audioPlay(MUSIC_STAGE); //地上の音楽を流す
		}
	}
	//メインコースの横向き土管（ゴールコースに続く）に入るときの処理
	if (!(IntersectPipe_1_2(player->right, RIGHT, MAIN) && player->inPipe == true)) {
		player->tmp2_pos = player->position;
	}
	else if (IntersectPipe_1_2(player->right, RIGHT, MAIN) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.x += 0.15f;
		if (player->position.x > player->tmp2_pos.x + 50) { //マリオが土管に入りきったらワープ
			player->course = GOAL;
			player->isScroll = true;
			player->position = { 160, PART_HEIGHT * (11 - player->isBig + 1) };
			player->inPipe = false;
			//audioStop(MUSIC_UNDER_GROUND_STAGE); //地下の音楽を止め
			//audioPlay(MUSIC_STAGE); //地上の音楽を流す
		}
	}

	
	//クリボーに重力を与える
	//if (keysIsPressed['g']) {
	if (player->course == MAIN) {
		for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
			if (kuriboo_1_2[i]->display_flag == true) {
				kuriboo_1_2[i]->velocity.y += 0.2f;
				kuriboo_1_2[i]->position += kuriboo_1_2[i]->velocity;
			}
		}
	}
	//}
	
	
	//スーパーキノコに重力を与える
	for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
		if (mushroom_1_2[i]->display_flag == true) {
			mushroom_1_2[i]->velocity.y += 0.2f;
			mushroom_1_2[i]->position += mushroom_1_2[i]->velocity;
		}
	}
	
	
	//ファイアボールに重力を与える
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				fire_ball[i]->velocity.y += 0.2f;
				fire_ball[i]->position += fire_ball[i]->velocity;
			}
		}
	}
	

	//マリオのアニメーションの処理
	if (player->isJumping) {
		player->animation = ANIMATION_JUMP;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_JUMP;
		}
	}
	else if (keysIsPressed['j'] || keysIsPressed['l']) {
		player->animation = ANIMATION_RUN;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_RUN;
		}
	}
	else if (keysIsPressed['m']) {
		player->animation = ANIMATION_SQUAT;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_SQUAT;
		}
	}
	else {
		player->animation = ANIMATION_STAND;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_STAND;
		}
	}

	if (keysIsPressed['j'] && keysIsPressed['l'] && keysIsPressed['c']) { //マリオがダッシュ中に切り返すとき
		player->animation = ANIMATION_TURN;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_TURN;
		}
	}

	if ((IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true) || (IntersectPipe(player->right, RIGHT, true) && player->inPipe == true)) { //土管に入るとき
		player->animation = ANIMATION_STAND; //これがないと、土管に入るときに当たり判定が消えているためジャンプのアニメーションになってしまう
	}

	//コインのアニメーション
	for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
		coin_1_2[i]->frame++;
	}

	//クエスチョンボックスのアニメーション
	for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
		question_1_2[i]->frame++;
	}

	//ファイアフラワーのアニメーション
	for (int i = 0; i < FIRE_FLOWER_NUM_1_2; i++) {
		fire_flower_1_2[i]->frame++;
	}


	//ファイアボールのアニメーションの処理
	//敵と当たった時のアニメーションの切り替えはintersect.cppで行う
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				fire_ball[i]->animation = FIRE_BALL_ANIMATION_BOUND;
				fire_ball[i]->frame++;
				fire_ball[i]->frame_at_attack = fire_ball[i]->frame; //敵と干渉した瞬間のフレームが保持される
			}
			else {
				if (fire_ball[i]->frame < fire_ball[i]->frame_at_attack + 60) {
					fire_ball[i]->animation = FIRE_BALL_ANIMATION_BURST;
					fire_ball[i]->frame++;
				}
				else {
					InitFireBallInstance(fire_ball[i]);
				}
			}
		}
	}

	//クリボーのアニメーションの処理
	for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
		if (kuriboo_1_2[i]->stepped_on_flag == false) {
			kuriboo_1_2[i]->animation = KURIBOO_ANIMATION_STAND;
			kuriboo_1_2[i]->frame++;
			kuriboo_1_2[i]->frame_at_stepped_on = kuriboo_1_2[i]->frame; //踏まれた瞬間のフレームが保持される
		}
		else {
			if (kuriboo_1_2[i]->frame < kuriboo_1_2[i]->frame_at_stepped_on + 60) {
				kuriboo_1_2[i]->animation = KURIBOO_ANIMATION_CRUSHED;
				kuriboo_1_2[i]->frame++;
			}
			else {
				kuriboo_1_2[i]->display_flag = false;
			}
		}
	}

	
	//-------------------------------------
	//キャラクターの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	player->left = player->position + glm::vec2(0, player->isBig * PLAYER_HEIGHT / 2.0f);
	player->right = player->position + glm::vec2(PLAYER_WIDTH, player->isBig * PLAYER_HEIGHT / 2.0f);
	player->top = player->position + glm::vec2(PLAYER_WIDTH / 2.0f, 0);
	player->bottom = player->position + glm::vec2(PLAYER_WIDTH / 2.0f, player->isBig * PLAYER_HEIGHT);

	//プレイヤーのコースパーツとの当たり判定
	DoIntersectDetection_1_2(PLAYER_INTERSECT, PART_INTERSECT, 1, 1, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question, lift);

	//プレイヤーのリフトとの当たり判定
	DoIntersectDetection_1_2(PLAYER_INTERSECT, LIFT_INTERSECT, 1, LIFT_NUM, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question, lift);

	//プレイヤーのゴールとの当たり判定
	DoIntersectDetection_1_2(PLAYER_INTERSECT, GOAL_INTERSECT, 1, 1, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question, lift);

	//プレイヤーのキノコとの当たり判定
	DoIntersectDetection_1_2(PLAYER_INTERSECT, MUSHROOM_INTERSECT, 1, MUSHROOM_NUM_1_2, false, player, kuriboo, empty_enemy, mushroom_1_2, fire_ball, block, question, lift);

	//プレイヤーのファイアフラワーとの当たり判定
	DoIntersectDetection_1_2(PLAYER_INTERSECT, FIRE_FLOWER_INTERSECT, 1, FIRE_FLOWER_NUM_1_2, false, player, kuriboo, empty_enemy, fire_flower_1_2, fire_ball, block, question, lift);

	//プレイヤーのクリボーとの当たり判定
	DoIntersectDetection_1_2(PLAYER_INTERSECT, KURIBOO_INTERSECT, 1, KURIBOO_NUM_1_2, false, player, kuriboo_1_2, empty_enemy, mushroom, fire_ball, block, question, lift);


	//-------------------------------------
	//スーパーキノコの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
		mushroom_1_2[i]->left = mushroom_1_2[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		mushroom_1_2[i]->right = mushroom_1_2[i]->position + glm::vec2(PART_WIDTH, PART_HEIGHT / 2.0f);
		mushroom_1_2[i]->top = mushroom_1_2[i]->position + glm::vec2(PART_WIDTH / 2.0f, 0);
		mushroom_1_2[i]->bottom_left = mushroom_1_2[i]->position + glm::vec2(0 + 3, PART_HEIGHT); //両端のブロックにひっかからないように、角の座標を小さめに指定
		mushroom_1_2[i]->bottom_right = mushroom_1_2[i]->position + glm::vec2(PART_WIDTH - 3, PART_HEIGHT); //エネミーよりも速度が速いため、より小さめにしないと隙間を乗り越えてしまう
	}

	//スーパーキノコ自身の周囲との当たり判定
	DoIntersectDetection_1_2(MUSHROOM_INTERSECT, PART_INTERSECT, MUSHROOM_NUM_1_2, 0, false, player, kuriboo, empty_enemy, mushroom_1_2, fire_ball, block, question, lift);

	//スーパーキノコの（壊せる）ブロックとの当たり判定
	//DoIntersectDetection(MUSHROOM_INTERSECT, BREAKABLE_BLOCK_INTERSECT, MUSHROOM_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);


	//-------------------------------------
	//ファイヤボールの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		fire_ball[i]->left = fire_ball[i]->position + glm::vec2(0, PART_HEIGHT / 4.0f);
		fire_ball[i]->right = fire_ball[i]->position + glm::vec2(PART_WIDTH / 2.0f, PART_HEIGHT / 4.0f);
		fire_ball[i]->top = fire_ball[i]->position + glm::vec2(PART_WIDTH / 4.0f, 0);
		fire_ball[i]->bottom = fire_ball[i]->position + glm::vec2(PART_WIDTH / 4.0f, PART_HEIGHT / 2.0f);
		fire_ball[i]->bottom_left = fire_ball[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		fire_ball[i]->bottom_right = fire_ball[i]->position + glm::vec2(PART_WIDTH / 2.0f, PART_HEIGHT / 2.0f);
		if (fire_ball[i]->display_flag == false) {
			fire_ball[i]->position = player->position + glm::vec2(22.5, PART_HEIGHT / 2.0f);
		}

	}

	//ファイヤボール自身の周囲との当たり判定
	//要修正　ファイアボールが階段を上ってしまう
	DoIntersectDetection_1_2(FIRE_BALL_INTERSECT, PART_INTERSECT, FIRE_BALL_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question, lift);

	//ファイアボールの（壊せる）ブロックとの当たり判定
	//DoIntersectDetection(FIRE_BALL_INTERSECT, BREAKABLE_BLOCK_INTERSECT, FIRE_BALL_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//ファイアボールのクリボーとの当たり判定
	DoIntersectDetection_1_2(FIRE_BALL_INTERSECT, KURIBOO_INTERSECT, FIRE_BALL_NUM, KURIBOO_NUM_1_2, false, player, kuriboo_1_2, empty_enemy, mushroom, fire_ball, block, question, lift);


	//-------------------------------------
	//クリボーの当たり判定を調べる
	//-------------------------------------
	//位置を更新
	for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
		kuriboo_1_2[i]->left = kuriboo_1_2[i]->position + glm::vec2(0, KURIBOO_HEIGHT / 2.0f);
		kuriboo_1_2[i]->right = kuriboo_1_2[i]->position + glm::vec2(KURIBOO_WIDTH, KURIBOO_HEIGHT / 2.0f);
		kuriboo_1_2[i]->top = kuriboo_1_2[i]->position + glm::vec2(KURIBOO_WIDTH / 2.0f, 0);
		kuriboo_1_2[i]->bottom_left = kuriboo_1_2[i]->position + glm::vec2(0 + 1, KURIBOO_HEIGHT);
		kuriboo_1_2[i]->bottom_right = kuriboo_1_2[i]->position + glm::vec2(KURIBOO_WIDTH - 1, KURIBOO_HEIGHT);
	}

	//クリボー自身の周囲との当たり判定
	DoIntersectDetection_1_2(KURIBOO_INTERSECT, PART_INTERSECT, KURIBOO_NUM_1_2, 0, false, player, kuriboo_1_2, empty_enemy, mushroom, fire_ball, block, question, lift);

	//----------------------------------------------------------
	//その他干渉時の処理（最終的にはintersect.cppにまとめたい）
	//----------------------------------------------------------

	
	//クエスチョンボックスをたたいた時の処理
	for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
		if (coin_1_2[i]->flag == false) {
			if (IsGetQuestionbox_1_2(player->top, i) == i) {
				coin_1_2[i]->flag = true; //コインを出す
				question_1_2[i]->flag = false;

				if (i != 0) { //コイン以外が出るクエスチョンボックスを除く
					if (i != 5) {
						//if (i != 13) {
							//if (i != 14) {
								audioPlay(SE_GET_COIN);
							//}
						//}
					}
				}
				//コインの位置を設定
				coin_1_2[i]->velocity.y += -7.0f;
				if (question_1_2[i]->sound_flag == true) {
					audioPlay(SE_BLOCK_BREAK);
					question_1_2[i]->sound_flag = false;
				}
			}
		}
	}
	//キノコが出てくるクエスチョンボックスを個別に処理
	//指定するインデックスは、course.hの数字 - 1
	if (IsGetQuestionbox_1_2(player->top, 0) == 0) {
		mushroom_1_2[0]->display_flag = true;
	}
	//ファイヤフラワーが出てくるクエスチョンボックスを個別に処理
	if (IsGetQuestionbox_1_2(player->top, 5) == 5) {
		fire_flower_1_2[0]->display_flag = true;
	}
	/*
	//1UPキノコが出てくるクエスチョンボックスを個別に処理
	if (IsGetQuestionbox_1_2(player->top, 13) == 13) {
		mushroom_1up[0]->display_flag = true;
	}

	//スターが出てくるクエスチョンボックスを個別に処理
	if (IsGetQuestionbox_1_2(player->top, 14) == 14) {
		star[0]->display_flag = true;
	}
	*/
	

	//---------------------------------------------
	//敵やアイテムの位置の設定
	//---------------------------------------------


	//スーパーキノコの位置の設定
	for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
		if (mushroom_1_2[i]->display_flag == true) {
			if (mushroom_1_2[i]->isMirror == false) {
				mushroom_1_2[i]->position.x += 0.5;
			}
			else {
				mushroom_1_2[i]->position.x -= 0.5;
			}
		}
	}


	//ファイアボールの位置の設定
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				if (fire_ball[i]->isMirror == false) {
					fire_ball[i]->position.x += 3.0;
				}
				else {
					fire_ball[i]->position.x -= 3.0;
				}
			}
		}
		//ファイアボールが画面外に消えたら、ファイアボールのインスタンスを初期化する
		if (player->position.x + WINDOW_WIDTH <= fire_ball[i]->position.x || player->position.x - WINDOW_WIDTH >= fire_ball[i]->position.x || fire_ball[i]->position.y > WINDOW_HEIGHT * 1.5) {
			InitFireBallInstance(fire_ball[i]);
		}


		//ファイアボールがコースパーツに当たった時の処理は、intersect.cpp の DoIntersectDetection 内で行う
		//ファイアボールが敵に当たった時や、コースパーツに当たった時も、適切にインスタンスの初期化を行う
	}

	
	//コインの位置の設定。重力を与え、速度を位置に追加する
	for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
		if (coin_1_2[i]->sub_flag == true) { //処理を一度だけ行う
			if (coin_1_2[i]->flag == true) {
				coin_1_2[i]->velocity.y += 0.2f;
				coin_1_2[i]->delta_pos += coin_1_2[i]->velocity;
			}
			if (coin_1_2[i]->delta_pos.y >= 1.0f) { //飛び跳ねたコインが元の位置に戻ったら描画をやめる
				coin_1_2[i]->sub_flag = false;
			}
		}
	}

	
	//クリボーの位置の設定
	for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
		if (kuriboo_1_2[i]->display_flag == true && kuriboo_1_2[i]->isDeath == false && player->course== MAIN) { //クリボーが踏まれて消えた後は、以下を行う必要はない
			//プレイヤーが近づいてきたらアニメーションを開始する
			if (player->position.x + WINDOW_WIDTH / 2.0f >= kuriboo_1_2[i]->position.x) {
				kuriboo_1_2[i]->animation_flag = true;
			}
			if (kuriboo_1_2[i]->stepped_on_flag == false) { //まだ踏まれてない間は、位置を更新する
				if (kuriboo_1_2[i]->animation_flag == true) {
					if (kuriboo_1_2[i]->isMirror == false) {
						kuriboo_1_2[i]->position.x -= 0.5;
					}
					else {
						kuriboo_1_2[i]->position.x += 0.5;
					}
				}
			}
		}
		if (kuriboo_1_2[i]->isDeath == true) { //ブロックの下から突き上げられたり、ファイアボールに当たって死んだとき
			if (kuriboo_1_2[i]->delta_psition_calculate_flag == true) {
				kuriboo_1_2[i]->delta_position.x += 0.9f;
				kuriboo_1_2[i]->delta_position.y = 0.065 * (kuriboo_1_2[i]->delta_position.x) * (kuriboo_1_2[i]->delta_position.x - 100);
				if (kuriboo_1_2[i]->delta_position.y > WINDOW_HEIGHT) {
					kuriboo_1_2[i]->delta_psition_calculate_flag = false;
				}
			}
		}
	}
	


	//リフトの位置の更新
	for (int i = 0; i < DOWN_LIFT_NUM; i++) { //下向きに動くリフト
		lift[i]->position.y += 1.0f;
		if (lift[i]->position.y > 740) {
			lift[i]->position.y = -20;
		}
	}
	for (int i = 2; i < 2 + UP_LIFT_NUM; i++) { //上向きに動くリフト
		lift[i]->position.y -= 1.0f;
		if (lift[i]->position.y < -20) {
			lift[i]->position.y = 740;
		}
	}

	//ゴールの旗の位置の更新
	if (player->isGoal == true) {
		if (player->flag_pos.y <= 540) {
			player->flag_pos.y += 1.2f;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(1000 / 180, Idle_1_2, 0);


}


void SwitchDisplay() {

	if (GetWhitchCourse() == 11) { //intersect.cpp内で大域変数として定義した値を参照する
		Display();
	}
	if (GetWhitchCourse() == 12) { //intersect.cpp内で大域変数として定義した値を参照する

	}
}


void SwitchIdle() {

}


int main(int argc, char* argv[]) {


	if (audioInit() != 0) {
		return 1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("title");
	

	//１－１の表示
	//if (COURSE == COURSE_1_1) {
	//if(player.whitch_course == 11){ //playerに持たせたメンバ変数を使うと、playerがnullポインタでエラーがでてしまう。
	//if(GetWhitchCourse() == 11) { //intersect.cpp内で大域変数として定義した値を参照する
		//glutDisplayFunc(Display);
		//glutTimerFunc(0,Idle,0);
		//Init();

	//}

	//glutDisplayFuncで指定するコールバック関数の中で、さらに描画関数を選ぶ感じにすれば、画面遷移ができるのでは？

	//１－２の表示
	//if (COURSE == COURSE_1_2) {
	//if (GetWhitchCourse() == 12) { //main文はループしていないため、これだとうまくいかない

		glutDisplayFunc(Display_1_2);
		glutTimerFunc(0, Idle_1_2, 0);
		Init_1_2();

	//}


	glutKeyboardFunc(Keyboard);
	//glutSpecialFunc(SpecialKey);
	glutKeyboardUpFunc(KeyboardUp);
	
	
	
	if (player->isInGround == false) {
		glClearColor(0.31372549, 0.50196078, 1.0, 1.0);  //(80,128, 255)空色
	}


	if (COMMAND_MODE == 2) {
		std::thread voiceRecognitionThread(voice_recognition); //追加---------------------------------------------------------------
		//voiceRecognitionThread.join();
	}


	glutMainLoop();
	
}
