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


//����
//�X�^�[���N�G�X�`�����{�b�N�X����ł͂Ȃ��A�����K�u���b�N����o�Ă���悤�ɂ���A
//�n��̃����K�u���b�N�ƒn���̃����K�u���b�N�������C���X�^���X�ɂȂ��Ă��܂��Ă���i�n��̃u���b�N��j�󂵂���A�n�����j�󂳂ꂽ�B�j
// 

//���P�������������Ǝv������
//�������ł��邾���y���Ȃ�悤�ɁAfor���[�v�̏��Ԃ��������āA�ł��邾�����������Ȃ��悤�ɂ���Ƃ���

//�v�C��
//�B���N�G�X�`�����{�b�N�X���̓����蔻�肪�A�����Ƃ���
//�v���C���[�̉E���������Ɗ����Ă���Ǝ��ɁA�t�@�C�A�{�[�����o�Ȃ�
//�N���{�[���m�̓����蔻�肪�A�N���{�[�����񂾌�ɂ��c���Ă��܂�
//�����K�u���b�N�������炽���������ɁA�R�[�X�̍ŏ��̕��̃u���b�N(index���P�̃u���b�N)�����A�}���I�̕\���Ɉ�a��������
//�n���̃����K�u���b�N�̃C���f�b�N�X�̔��肪�ABLOCK_NUM�Ɉˑ����Ă��܂��Ă���
//�t�@�C�A�{�[�����o�Ă��鍂��������Ⴄ�C������


//---------------------------------------------
//���s���ɃI�v�V�����Ƃ��Č��߂Ă����l
//---------------------------------------------
#define COURSE 1 //�u�P�v�Ȃ�1-1���v���C�A�u�Q�v�Ȃ�2-2���v���C //�Ƃ肠��������header.h�ɋL�q
#define COMMAND_MODE 1 //�u�P�v�Ȃ�L�[�{�[�h���́A�u�Q�v�Ȃ特������


//--------------------------------------
//�O���[�o���ϐ��̒�`
//--------------------------------------
int coin_flag = 0;
int coin_num = 0; //���v�ŉ����̃R�C������������ێ�����

bool voice_command_flag; //�����F���ɂ�鑬�x�̉��Z����񂾂��s�����߂̃t���O

/*
int whitch_course = 11; //�ǂ̃R�[�X���v���C���Ă��邩�����߂�ϐ�

void SetWhitchCourse(int _whitch_course) { //�Z�b�^�[
	whitch_course = _whitch_course;
}
*/

bool keysIsPressed[256];


//----------------------------------------------
//�e�\���̂̃C���X�^���X�̐���
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
	4, {0, 1, 2, 3}  // �R�C���̉�]
};

ANIMATION question_box_animations = {
	4, {0, 0, 1, 2} // �N�G�X�`�����{�b�N�X�̓_��
};

ANIMATION enemy_animations[ENEMY_ANIMATION_MAX] = {
	4, {0, 0, 0, 1}, // ENEMY_ANIMATION_STAND
	3, {2, 2, 2} // ENEMY_ANIMATION_ATTACK
};

ANIMATION kuriboo_animations[KURIBOO_ANIMATION_MAX] = {
	4, {0, 1, 0, 1}, // KURIBOO_ANIMATION_STAND
	3, {2, 2, 2}, // KURIBOO_ANIMATION_CRUSHED
};
//���ӁI�IANIMATION�̗v�f��������ƂƂȂ����G���[���N����

ANIMATION nokonoko_animations[NOKONOKO_ANIMATION_MAX] = {
	4, {0, 1, 0, 1} // NOKONOKO_ANIMATION_STAND
};

ANIMATION fire_flower_animations[FIRE_FLOWER_ANIMATION_MAX] = {
	6, {0, 1, 2, 0, 1, 2} // �t�@�C�A�t�����[�̓_�� //���ӁI�I�v�f�����R���ƂƂȂ����G���[���N����
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

std::vector<ENEMY*> empty_enemy; //���ۂɂ͎g��Ȃ������ɓ����


//�P�|�Q�p�̃C���X�^���X
std::vector<COIN*> coin_1_2;
std::vector<BLOCK*> block_1_2;
std::vector<QUESTION*> question_1_2;
std::vector<ENEMY*> kuriboo_1_2;
std::vector<ENEMY*> nokonoko_1_2;
std::vector<ITEM*> mushroom_1_2;
std::vector<ITEM*> fire_flower_1_2;


//-------------------------------------------
//�����F���ɂ��w�����o���֐�
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
			if (GetNewVerticalVoiceInput() == true) { //�����������x�w��
				player->velocity.y -= 10.0f;
				audioPlay(VOICE_JUMP_1);
				SetNewVerticalVoiceInput(false);
			}
			if (player->position.x - player->voice_old_pos.x < 45 * VOICE_COMMAND_AMOUNT) { //�����������x�w��
				player->velocity.x += acceleration * player->isRunning;
				player->isMirror = false;
			}
			else {
				SetNewHorizontalVoiceInput(false);
			}
		}
		if (SetVoiceCommand() == 3) {
			if (player->position.x - player->voice_old_pos.x < 45 * VOICE_COMMAND_AMOUNT) { //�P�܂��������i�܂���
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
			if (GetNewVerticalVoiceInput() == true) { //�����������x�w��
				player->velocity.y -= 10.0f;
				audioPlay(VOICE_JUMP_1);
				SetNewVerticalVoiceInput(false);
			}
			if (player->position.x - player->voice_old_pos.x > -45 * VOICE_COMMAND_AMOUNT) { //�����������x�w��
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
//���ׂẴe�N�X�`����ǂݍ��ފ֐�
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
//���������s���֐�
//---------------------
void Init(void) {
	
	AllTexFromFile();

	//�C���X�^���X�̐����Ə�����
	player = MakePlayerInstance();
	coin = MakeCoinInstance(QUESTION_BOX_NUM); //�N�G�X�`�����{�b�N�X����o��R�C��
	field_coin = MakeFieldCoinInstance(FIELD_COIN_NUM); //�t�B�[���h�ɏ��߂���u���Ă���R�C��
	ground_field_coin = MakeFieldCoinInstance(GROUND_FIELD_COIN_NUM); //�n���ɏ��߂���u���Ă���R�C��
	block_coin = MakeBlockCoinInstance(); //�����K�u���b�N����o�Ă���R�C��
	question = MakeQuestionInstance(QUESTION_BOX_NUM);
	block = MakeBlockInstance(BLOCK_NUM);
	coin_block = MakeCoinBlockInstance();
	ground_block = MakeBlockInstance(GROUND_BLOCK_NUM);
	enemy = MakeEnemyInstance(28 * ENEMY_WIDTH / 32.0f, 28 * ENEMY_HEIGHT / 32.0f, ENEMY_NUM); //�����ł́A�摜�̃T�C�Y�ł͂Ȃ��A�����蔻��Ŏg���T�C�Y��n���B
	kuriboo = MakeEnemyInstance(KURIBOO_WIDTH, KURIBOO_HEIGHT, KURIBOO_NUM);
	nokonoko = MakeEnemyInstance(NOKONOKO_WIDTH, NOKONOKO_HEIGHT, NOKONOKO_NUM);
	mushroom = MakeMushroomInstance(MUSHROOM_NUM);
	mushroom_1up = MakeMushroomInstance(MUSHROOM_1UP_NUM);
	fire_flower = MakeFireFlowerInstance(FIRE_FLOWER_NUM);
	fire_ball = MakeFireBallInstance();
	star = MakeStarInstance();

	//�G�̃A�j���[�V�����̏����ݒ�
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i]->animation = ENEMY_ANIMATION_STAND;
	}

	//�N���{�[�̃A�j���[�V�����̏����ݒ�
	for (int i = 0; i < KURIBOO_NUM; i++) {
		kuriboo[i]->animation = KURIBOO_ANIMATION_STAND;
	}

	//�m�R�m�R�̃A�j���[�V�����̏����ݒ�
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		nokonoko[i]->animation = NOKONOKO_ANIMATION_STAND;
	}

	//�t�@�C�A�t�����[�̃A�j���[�V�����̏����ݒ�
	for (int i = 0; i < FIRE_FLOWER_NUM; i++) {
		fire_flower[i]->animation = FIRE_FLOWER_ANIMATION;
	}

	//�t�@�C�A�{�[���̃A�j���[�V�����̏����ݒ�
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		//fire_ball[i]->animation = FIRE_BALL_ANIMATION_BOUND;
	}

	/*
	//�G�̈ʒu���ʂɐݒ� �T���l�p
	enemy[0]->position = { 945,  PART_HEIGHT * 13 }; //(180, 585)
	enemy[1]->position = { 1125,  PART_HEIGHT * 11 };
	enemy[2]->position = { 1035,  PART_HEIGHT * 9 };
	*/

	//�N���{�[�̈ʒu��ݒ�
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

	//�m�R�m�R�̈ʒu��ݒ�
	nokonoko[0]->position = { 4770,  PART_HEIGHT * 13 };
	nokonoko[1]->position = { 1430 /* + 2250*/,  PART_HEIGHT * 13};//

	//nokonoko[2]->position = { 1430,  PART_HEIGHT * 13 };//

	//�L�m�R�̏����ʒu��ݒ�
	mushroom[0]->position = { 985,  PART_HEIGHT * 9 };

	//1UP�L�m�R�̏����ʒu��ݒ�
	mushroom_1up[0]->position = { 2925,  PART_HEIGHT * 8 };

	//�t�@�C�A�t�����[�̏����ʒu��ݒ�
	fire_flower[0]->position = { 3510,  PART_HEIGHT * 9 };
	
	//�t�@�C�A�{�[���̏����ʒu��ݒ�
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		fire_ball[i]->position = player->position + glm::vec2(22.5, 0);
	}

	//�X�^�[�̏����ʒu��ݒ�
	star[0]->position = { 4590,  PART_HEIGHT * 9 };
}


//----------------------------------------
//�N�G�X�`�����{�b�N�X��`�悷��֐�
//----------------------------------------
void DrawQuestionBox(glm::vec2 _position, char _c, char _sub2_c, int _index, int _course) {

	if (_course == COURSE_1_1) { //1-1
		if (_c == 'q') {
			if (LetterToNum(_sub2_c) == _index + 1 && _index != 13) { //�B���N�G�X�`�����{�b�N�X������
				if (question[_index]->flag == true) {
					DrawSprite(TEX_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, question_box_animations.frames[(question[_index]->frame / 45) % question_box_animations.count], _position);
				}
				else {
					DrawSprite(TEX_EMPTY_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, 0, _position);
				}
			}
			//�B���N�G�X�`�����{�b�N�X���ʂɏ���
			if (LetterToNum(_sub2_c) == 13 + 1) {
				if (question[13]->flag == true) {
					//��������Ă��Ȃ����͉��������Ȃ�
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
			//�����K�u���b�N�̌����ڂ������N�G�X�`�����{�b�N�X�i�t�@�C�A�[���o��j���ʂɏ���
			if (LetterToNum(_sub2_c) == 6) { //�B���N�G�X�`�����{�b�N�X������
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
//(�j��ł���)�u���b�N��`�悷��֐�
//----------------------------------------
void DrawBlock(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, int _index, bool _isInGround, int _course) {

	if (_course == COURSE_1_1) { //1-1
		if (_c == 'b') { //�u���b���ǂ���
			if (_sub_c == '2') { //�j��ł����ނ�
				if (LetterToNum(_sub2_c) == _index + 1) {
					if (_isInGround == false) { //�n��
						if (block[_index]->flag == true) {
							DrawSprite(TEX_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position + block[_index]->delta_position);
						}
					}
					else if (_isInGround == true) { //�n��
						if (ground_block[_index]->flag == true) {
							DrawSprite(TEX_GROUND_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position + ground_block[_index]->delta_position);
						}
					}
				}
			}
		}
	}
	else if (_course == COURSE_1_2) { //1-2
		if (_c == 'b') { //�u���b���ǂ���
			if (_sub_c == '2') { //�j��ł����ނ�
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
//�R�C�����o�郌���K�u���b�N��`�悷��֐�
//---------------------------------------------
void DrawCoinBlock(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, int _index) {

	if (_c == 'b') { //�u���b���ǂ���
		if (_sub_c == '3') { //�R�C�����o����̂�
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
//�j�󂳂ꂽ�u���b�N��`�悷��֐�
//----------------------------------------
void DrawBreakedBlock(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, int _index) {

	if (_c == 'b') { //�u���b���ǂ���
		if (_sub_c == '2') { //�j��ł����ނ�
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
//�R�[�X�p�[�c��`�悷��֐�
//-------------------------------------
void DrawPart(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) { //�n��ɂ���ꍇ
		glClearColor(0.31372549, 0.50196078, 1.0, 1.0);  //(80,128, 255)�w�i�F����F�ɂ���

		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return;
		}

		if (_c == 'b') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_BLOCK_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_BLOCK_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2':  //�u_c == 'b'�v�Ɓu_sub_c == '2'�v��DrawBlock���œ�d�m�F���Ă��܂��Ă��邽�߁A������������������
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
		else if (_c == 'q') { //�u_c == 'q'�v��DrawQuestionBox���œ�d�m�F���Ă��܂��Ă��邽�߁A������������������
			//(���ӁI���\�d���Ȃ�)���P�ς݁B�R�[�X�̑������d�Ń��[�v���Ă������߁A�������C��
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
			case '4': DrawSprite(TEX_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '5': DrawSprite(TEX_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '6': DrawSprite(TEX_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '7': DrawSprite(TEX_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
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
			//DrawSprite(TEX_EMPTY, PART_WIDTH, PART_HEIGHT, 0, _position); //�����Ȃ��Ƃ���͋�i�S�����ߐF�j�̉摜��\������j
			//��̕`���w�i�F�̎w��ōs���ƁA�󂵂��u���b�N�̔j�Ђ���̗��ɉB��Ă��܂�

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

	else  { //�n���ɂ���ꍇ
	glClearColor(0.0, 0.0, 0.0, 1.0); //�w�i�F�����ɂ���

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
			case '4': DrawSprite(TEX_GROUND_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '5': DrawSprite(TEX_GROUND_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '6': DrawSprite(TEX_GROUND_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '7': DrawSprite(TEX_GROUND_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '8': DrawSprite(TEX_JOINT_GROUND_PIPE_26, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�ڍ���
			case '9': DrawSprite(TEX_JOINT_GROUND_PIPE_27, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�ڍ���
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
			glColor3ub(0x00, 0x00, 0x00); //�n���ɂ���Ƃ��A�w�i�͍�
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
//�N�G�X�`�����{�b�N�X����o���R�C����`�悷��֐�
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
//�t�B�[���h��̃R�C����`�悷��֐�
//----------------------------------------------------
void DrawFieldCoin(int _index, bool _isInGround) {
	float scrollX = player->position.x - WINDOW_WIDTH / 2 + PLAYER_WIDTH / 2;

	if (_isInGround == false) { //�n��)
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
	else if (_isInGround == true) { //�n��
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
//�����K�u���b�N����o���R�C����`�悷��֐�
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
//�`��֐��{��
//-------------------
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	float scrollX = player->position.x - WINDOW_WIDTH / 2 + PLAYER_WIDTH / 2;
	
	
	//�R�[�X�p�[�c��`��
	if (player->isInGround == false) { //�n��
		for (int y = 0; y < COURSE_HEIGHT; y++) {
			for (int x = 0; x < COURSE_WIDTH; x++) {
				DrawPart({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, course[y][x], sub_course[y][x], sub2_course[y][x], player->isInGround);
			}
		}
	}
	if (player->isInGround == true) { //�n��
		for (int y = 0; y < GROUND_COURSE_HEIGHT; y++) {
			for (int x = 0; x < GROUND_COURSE_WIDTH; x++) {
				DrawPart({ x * PART_WIDTH, y * PART_HEIGHT }, ground_course[y][x], ground_sub_course[y][x], ground_sub2_course[y][x], player->isInGround);
			}
		}
	}
	

	//�N�G�X�`�����{�b�N�X����o���R�C����`��
	for (int i = 0; i < QUESTION_BOX_COIN_NUM; i++) {
		DrawCoin(i, COURSE_1_1);
	}

	//�t�B�[���h��̃R�C����`��
	if (player->isInGround == false) {
		for (int i = 0; i < FIELD_COIN_NUM; i++) {
			DrawFieldCoin(i, false); //�n��
		}
	}
	else if (player->isInGround == true) {//�n���̃R�C����`��
		for (int i = 0; i < GROUND_FIELD_COIN_NUM; i++) {
			DrawFieldCoin(i, true); //�n��
		}
	}

	//�����K�u���b�N����o��̃R�C����`��
	for (int i = 0; i < COIN_BLOCK_NUM; i++) { //�R�C�����o�Ă��郌���K�u���b�N�ƁA��������o�Ă����R�C���̃C���X�^���X�̐��͓����ł��邱�Ƃɒ��ӂ���
		DrawBlockCoin(i);
	}
	
	//�S�[���̊���`��
	//������ɕ`���Ȃ��ƁA�L�����N�^�[���S�[���̊��ɉB����Ă��܂�
	DrawSprite(TEX_FLAG, PART_WIDTH, PART_HEIGHT, 0, player->flag_pos + glm::vec2(-scrollX, 0));

	//�}���I��`��
	if (player->isBig == 1) {
		if (player->isStar == false) {
			DrawSprite(TEX_MARIO, PLAYER_WIDTH, PLAYER_HEIGHT,
				animations[player->animation].frames[(player->frame * player->isRunning / 31) % animations[player->animation].count],
				player->position + glm::vec2(-scrollX * (!player->isInGround), 0), //�n���ŃX�N���[�����Ȃ��悤�ɂ��邽�߂ɁAplayer->isInGround�̔ے��������
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

	//�����y�ǂ����㏑��
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
	//.�T���l�p�̃N�b�p�ƃr�b�N���}�[�N��`��
	DrawSprite(TEX_KUPPA, 64, 64, 0, enemy[0]->position + glm::vec2(-scrollX, -20), true);
	DrawSprite(TEX_KUPPA, 64, 64, 1, enemy[1]->position + glm::vec2(-scrollX - 20, 0), false);
	DrawSprite(TEX_KUPPA, 64, 64, 0, enemy[2]->position + glm::vec2(-scrollX, -20), false);
	DrawSprite(TEX_SURPRISE, 45, 45, 0, glm::vec2(1050, PART_HEIGHT*12) + glm::vec2(-scrollX, 0), false);
	*/

	//�N���{�[��`��
	if (player->isInGround == false) {
		for (int i = 0; i < KURIBOO_NUM; i++) {
			if (kuriboo[i]->display_flag == true && kuriboo[i]->isDeath == false) { //���܂��A�܂��̓u���b�N�̉����炽������Ď��ʂ܂ŕ`�悷��
				DrawSprite(TEX_KURIBOO, KURIBOO_WIDTH, KURIBOO_HEIGHT,
					kuriboo_animations[kuriboo[i]->animation].frames[(kuriboo[i]->frame / 63) % animations[kuriboo[i]->animation].count],
					kuriboo[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(KURIBOO_WIDTH / 16.0f, KURIBOO_HEIGHT / 16.0f),
					kuriboo[i]->isMirror
				);
			}
			//�N���{�[���Ђ�����Ԃ��Ď��ʃA�j���[�V������`��
			if (kuriboo[i]->isDeath == true) {
				DrawSprite(TEX_KURIBOO, KURIBOO_WIDTH, KURIBOO_HEIGHT,
					3, //Kuriboo.bmp�t�@�C���̂S�Ԗڂ́i�Ђ�����Ԃ����j�摜��\������
					kuriboo[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(KURIBOO_WIDTH / 16.0f, KURIBOO_HEIGHT / 16.0f) + kuriboo[i]->delta_position,
					kuriboo[i]->isMirror
				);
			}
		}
	}

	//�m�R�m�R��`��
	if (player->isInGround == false) {
		for (int i = 0; i < NOKONOKO_NUM; i++) {
			if (nokonoko[i]->display_flag == true && nokonoko[i]->isDeath == false) { //���܂��A�܂��͎��ʂ܂ŕ`�悷��
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
			//�m�R�m�R���Ђ�����Ԃ��Ď��ʃA�j���[�V������`��
			if (nokonoko[i]->isDeath == true) {
				if (nokonoko[i]->stepped_on_flag == false) { //�m�R�m�R�������ē����Ƃ��Ƀt�@�C�A�{�[���ɎE���ꂽ�ꍇ
					DrawSprite(TEX_NOKONOKO, NOKONOKO_WIDTH, NOKONOKO_HEIGHT,
						2, //�t�@�C����2�Ԗڂ́i�Ђ�����Ԃ����j�摜��\������
						nokonoko[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_WIDTH / 16.0f, NOKONOKO_HEIGHT / 16.0f) + nokonoko[i]->delta_position,
						nokonoko[i]->isMirror
					);
				}
				else { //�m�R�m�R���b���̎��Ƀt�@�C�A�{�[���ɎE���ꂽ�ꍇ
					DrawSprite(TEX_NOKONOKO_SHELL, NOKONOKO_SHELL_WIDTH, NOKONOKO_SHELL_HEIGHT,
						2, //�t�@�C����2�Ԗڂ́i�Ђ�����Ԃ����j�摜��\������
						nokonoko[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_SHELL_WIDTH / 16.0f, NOKONOKO_SHELL_HEIGHT / 16.0f) + nokonoko[i]->delta_position,
						nokonoko[i]->isMirror
					);
				}
			}
		}
	}

	//�X�[�p�[�L�m�R��`��
	for (int i = 0; i < MUSHROOM_NUM; i++) {
		if (mushroom[i]->display_flag == true) {
			if (mushroom[i]->get_flag == false) {
				DrawSprite(TEX_MUSHROOM, PART_WIDTH, PART_HEIGHT, 0,
					mushroom[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)
				);
			}
		}
	}

	//1UP�L�m�R��`��
	for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
		if (mushroom_1up[i]->display_flag == true) {
			if (mushroom_1up[i]->get_flag == false) {
				DrawSprite(TEX_MUSHROOM_1UP, PART_WIDTH, PART_HEIGHT, 0,
					mushroom_1up[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)
				);
			}
		}
	}
	
	//�t�@�C�A�t�����[��`��
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

	//�t�@�C�A�{�[����`��
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			DrawSprite(TEX_FIRE_BALL, PART_WIDTH , PART_HEIGHT ,
				//0,
				fire_ball_animations[fire_ball[i]->animation].frames[(fire_ball[i]->frame / 31) % animations[fire_ball[i]->animation].count],
				fire_ball[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 16.0f, PART_HEIGHT / 16.0f)
			);
		}
	}

	//�X�^�[��`��
	for (int i = 0; i < STAR_NUM; i++) {
		if (star[i]->display_flag == true) {
			if (star[i]->get_flag == false) {
				DrawSprite(TEX_STAR, PART_WIDTH, PART_HEIGHT, 0,
					star[i]->position + glm::vec2(-scrollX, 0) /* - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)*/
				);
			}
		}
	}
	
	//��ꂽ�u���b�N�̔j�Ђ�`��
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
	//�t�H���g�̕`��
	//--------------------------------
	fontBegin();
	fontSetWeight(2.5); //�ŏ�0.5�A�ő�10.0
	fontSetColor(0xff - 0x10, 0xff - 0x10, 0xff - 0x10); //�����ɈÂ߂̔��F
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
//�A�C�h�����̏���
//------------------------
//void Idle(void) {
void Idle(int value) {

	float acceleration = 0.2f; //0.155f;���ƕ������x�Ƃ��Ă��傤�ǂ悢

	//�L�[��������Ă���ԁA�s������
	if (!keysIsPressed['c']) {
		player->isRunning = 1;
	}
	else if (keysIsPressed['c']) { //�_�b�V�����Ă���ꍇ
		player->isRunning = 2;
	}

	if (keysIsPressed['j']) {
		player->velocity.x -= acceleration * player->isRunning;
	}

	if (keysIsPressed['l']) {
		player->velocity.x += acceleration * player->isRunning;
	}

	//�c�̓y�ǂɓ���Ƃ��̏���
	if (!(IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true)) {
		player->tmp1_pos = player->position;
	}
	else if (IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.y += 0.1f;
		if (player->position.y > player->tmp1_pos.y + 135) { //�}���I���y�ǂɓ��肫�����烏�[�v
			player->isInGround = true;
			player->position = {45*2, 0}; 
			player->inPipe = false; //���[�v������y�ǂɓ��蒆�ł���Ƃ����t���O�����낷
			audioStop(MUSIC_STAGE); //�n��̉��y���~�߂�
			audioPlay(MUSIC_UNDER_GROUND_STAGE); //�n���̉��y�𗬂�
		}
	}
	//���̓y�ǂɓ���Ƃ��̏���
	if (!(IntersectPipe(player->right, RIGHT, true) && player->inPipe == true)) {
		player->tmp2_pos = player->position;
	}
	else if (IntersectPipe(player->right, RIGHT, true) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.x += 0.15f;
		if (player->position.x > player->tmp2_pos.x + 50) { //�}���I���y�ǂɓ��肫�����烏�[�v
			player->isInGround = false;
			player->position = { 7355, PART_HEIGHT * 11 };
			player->inPipe = false; //���[�v������y�ǂɓ��蒆�ł���Ƃ����t���O�����낷
			audioStop(MUSIC_UNDER_GROUND_STAGE); //�n���̉��y���~��
			audioPlay(MUSIC_STAGE); //�n��̉��y�𗬂�
		}
	}

	//glutSpecialFunc(SpecialKey); //���L�[�ł������悤�ɂ���

	//�L�����N�^�[�ɕ����@����K�p�i��R�Əd�́j
	player->velocity.x *= 0.93f;
	player->velocity.y += 0.2f;
	player->position += player->velocity;

	//�}���I�̃X�^�[�̕b���𐧌䂷��
	if(player->isStar == false){
		player->tmp_star_frame = player->frame;
	}
	else if (player->isStar == true) {
		if (player->frame > player->tmp_star_frame + 180 * 8) { //���܁A180fps�ŕ`�悵�Ă��邽�߁A�X�^�[���W�b�ԑ����邽�߂�180*8
			player->isStar = false;
			audioStop(MUSIC_STAR); //�X�^�[��BGM���~�߁A
			audioPlay(MUSIC_STAGE); //�X�e�[�W��BGM�𗬂�
		}
	}
	
	/*
	//�G�ɏd�͂�^����
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i]->velocity.y += 0.2f;
		enemy[i]->position += enemy[i]->velocity;
	}
	*/

	//�N���{�[�ɏd�͂�^����
	for (int i = 0; i < KURIBOO_NUM; i++) {
		kuriboo[i]->velocity.y += 0.2f;
		kuriboo[i]->position += kuriboo[i]->velocity;
	}

	//�m�R�m�R�ɏd�͂�^����
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		nokonoko[i]->velocity.y += 0.2f;
		nokonoko[i]->position += nokonoko[i]->velocity;
	}

	//�X�[�p�[�L�m�R�ɏd�͂�^����
	for (int i = 0; i < MUSHROOM_NUM; i++) {
		mushroom[i]->velocity.y += 0.2f;
		mushroom[i]->position += mushroom[i]->velocity;
	}

	//1UP�L�m�R�ɏd�͂�^����
	for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
		mushroom_1up[i]->velocity.y += 0.2f;
		mushroom_1up[i]->position += mushroom_1up[i]->velocity;
	}

	//�X�^�[�ɏd�͂�^����
	for (int i = 0; i < STAR_NUM; i++) {
		if (star[i]->display_flag == true) {
			star[i]->velocity.y += 0.2f;
			star[i]->position += star[i]->velocity;
		}
	}

	//�t�@�C�A�{�[���ɏd�͂�^����
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				fire_ball[i]->velocity.y += 0.2f;
				fire_ball[i]->position += fire_ball[i]->velocity;
			}
		}
	}

	//�L�����N�^�[���R�[�X�O�ɍs�������̃��Z�b�g����
	//if ((player.position.x < 0) || (player.position.x >= COURSE_WIDTH) || (player.position.y < 0) || (player.position.y >= COURSE_HEIGHT)) {
	if ((player->position.y >= WINDOW_HEIGHT + PART_HEIGHT * 100)) {
		player->position = { WINDOW_WIDTH / 2.0f,  PART_HEIGHT * (13 - (player->isBig - 1)) };
		player->isBig = 1;
		player->isFire = false;
	}

	//-------------------------------------
	//�L�����N�^�[�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	player->left = player->position + glm::vec2(0, player->isBig * PLAYER_HEIGHT / 2.0f);
	player->right = player->position + glm::vec2(PLAYER_WIDTH, player->isBig * PLAYER_HEIGHT / 2.0f);
	player->top = player->position + glm::vec2(PLAYER_WIDTH / 2.0f, 0);
	player->bottom = player->position + glm::vec2(PLAYER_WIDTH / 2.0f, player->isBig * PLAYER_HEIGHT);

	//�v���C���[�̃R�[�X�p�[�c�Ƃ̓����蔻��
	//�Ȃ������܂������Ȃ����ߗv�C��
	//�����Ƃ���player��n���āA���̃����o�ϐ���ύX�������Ƃ��̓|�C���^�œn���K�v������
	DoIntersectDetection(PLAYER_INTERSECT, PART_INTERSECT, 1, 1, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question); //�Ō�̈����͎g��Ȃ����ǁA�Ƃ肠�����^���Ă���PART_INTERSECT

	//�v���C���[�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
	if (player->isInGround == false) {
		DoIntersectDetection(PLAYER_INTERSECT, BREAKABLE_BLOCK_INTERSECT, 1, BLOCK_NUM, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question);
	}
	//�v���C���[�̒n���̃����K�u���b�N�Ƃ̓����蔻��
	if (player->isInGround == true) {
		DoIntersectDetection(PLAYER_INTERSECT, BREAKABLE_BLOCK_INTERSECT, 1, GROUND_BLOCK_NUM, true, player, enemy, empty_enemy, mushroom, fire_ball, ground_block, question);
	}

	//�v���C���[�̃R�C���u���b�N�Ƃ̓����蔻��
	DoIntersectDetection(PLAYER_INTERSECT, COIN_BLOCK_INTERSECT, 1, COIN_BLOCK_NUM, false, player, enemy, empty_enemy, mushroom, fire_ball, coin_block, question);

	//�}���I�̃A�j���[�V�����̏���
	//(������ɍs��Ȃ��ƁA�����蔻��ɂ��A�j���[�V�����̕ύX���㏑������Ă��܂����߁A�����蔻�������ɏ�������)
	//(����ɁA�u���b�N�Ƃ̓����蔻��������Ƃɍs��Ȃ��ƁA�u���b�N��ŃW�����v�A�j���[�V�����ɂȂ��Ă��܂�)
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

	if (keysIsPressed['j'] && keysIsPressed['l'] && keysIsPressed['c']) { //�}���I���_�b�V�����ɐ؂�Ԃ��Ƃ�
		player->animation = ANIMATION_TURN;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_TURN;
		}
	}
	
	if ((IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true) || (IntersectPipe(player->right, RIGHT, true) && player->inPipe == true)) { //�y�ǂɓ���Ƃ�
		player->animation = ANIMATION_STAND; //���ꂪ�Ȃ��ƁA�y�ǂɓ���Ƃ��ɓ����蔻�肪�����Ă��邽�߃W�����v�̃A�j���[�V�����ɂȂ��Ă��܂�
	}

	player->frame++;

	//�G�̃A�j���[�V�����̏���
	//(������ɍs��Ȃ��ƁA�����蔻��ɂ��A�j���[�V�����̕ύX���㏑������Ă��܂����߁A�����蔻�������ɏ�������)
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i]->animation = ENEMY_ANIMATION_STAND;
		enemy[i]->frame++;
	}

	//�N���{�[�̃A�j���[�V�����̏���
	for (int i = 0; i < KURIBOO_NUM; i++) {
		if (kuriboo[i]->stepped_on_flag == false) {
			kuriboo[i]->animation = KURIBOO_ANIMATION_STAND;
			kuriboo[i]->frame++;
			kuriboo[i]->frame_at_stepped_on = kuriboo[i]->frame; //���܂ꂽ�u�Ԃ̃t���[�����ێ������
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

	//�m�R�m�R�̃A�j���[�V�����̏���
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		if (nokonoko[i]->stepped_on_flag == false) {
			nokonoko[i]->animation = KURIBOO_ANIMATION_STAND;
			nokonoko[i]->frame++;
		}
		//���܂ꂽ�Ƃ��ɍb����`�悷�鏈���́ADrawSprite�֐��ōs��
	}

	//�t�@�C�A�t�����[�̃A�j���[�V�����̏���
	for (int i = 0; i < FIRE_FLOWER_NUM; i++) {
		if (fire_flower[i]->display_flag == true) {
			fire_flower[i]->animation = FIRE_FLOWER_ANIMATION;
			fire_flower[i]->frame++;
		}
	}

	//�t�@�C�A�{�[���̃A�j���[�V�����̏���
	//�G�Ɠ����������̃A�j���[�V�����̐؂�ւ���intersect.cpp�ōs��
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				fire_ball[i]->animation = FIRE_BALL_ANIMATION_BOUND;
				fire_ball[i]->frame++;
				fire_ball[i]->frame_at_attack = fire_ball[i]->frame; //�G�Ɗ������u�Ԃ̃t���[�����ێ������
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
	
	//�v���C���[�̃G�l�~�[�Ƃ̓����蔻��
	//DoIntersectDetection(PLAYER_INTERSECT,ENEMY_INTERSECT, 1, player, enemy, mushroom, fire_ball, block, question);

	//�v���C���[�̃N���{�[�Ƃ̓����蔻��
	DoIntersectDetection(PLAYER_INTERSECT, KURIBOO_INTERSECT, 1, KURIBOO_NUM, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�v���C���[�̃m�R�m�R�Ƃ̓����蔻��
	DoIntersectDetection(PLAYER_INTERSECT, NOKONOKO_INTERSECT, 1, NOKONOKO_NUM, false, player, nokonoko, empty_enemy, mushroom, fire_ball, block, question);

	//�v���C���[�̃L�m�R�Ƃ̓����蔻��
	DoIntersectDetection(PLAYER_INTERSECT, MUSHROOM_INTERSECT, 1, MUSHROOM_NUM, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�v���C���[��1UP�L�m�R�Ƃ̓����蔻��
	DoIntersectDetection(PLAYER_INTERSECT, MUSHROOM_1UP_INTERSECT, 1, MUSHROOM_1UP_NUM, false, player, kuriboo, empty_enemy, mushroom_1up, fire_ball, block, question);

	//�v���C���[�̃t�@�C�A�t�����[�Ƃ̓����蔻��
	DoIntersectDetection(PLAYER_INTERSECT, FIRE_FLOWER_INTERSECT, 1, FIRE_FLOWER_NUM, false, player, kuriboo, empty_enemy, fire_flower, fire_ball, block, question);

	//�v���C���[�̃X�^�[�Ƃ̓����蔻��
	DoIntersectDetection(PLAYER_INTERSECT, STAR_INTERSECT, 1, STAR_NUM, false, player, empty_enemy, empty_enemy, star, fire_ball, block, question);

	//�v���C���[�̃S�[���Ƃ̓����蔻��
	DoIntersectDetection(PLAYER_INTERSECT, GOAL_INTERSECT, 1, 1, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	
	//-------------------------------------
	//�G�l�~�[�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i]->left = enemy[i]->position + glm::vec2(0, ENEMY_HEIGHT / 2.0f);
		enemy[i]->right = enemy[i]->position + glm::vec2(ENEMY_WIDTH, ENEMY_HEIGHT / 2.0f);
		enemy[i]->top = enemy[i]->position + glm::vec2(ENEMY_WIDTH / 2.0f, 0);
		enemy[i]->bottom_left = enemy[i]->position + glm::vec2(0 + 1, ENEMY_HEIGHT); //���[�̃u���b�N�ɂЂ�������Ȃ��悤�ɁA�p�̍��W�������߂Ɏw��
		enemy[i]->bottom_right = enemy[i]->position + glm::vec2(ENEMY_WIDTH - 1, ENEMY_HEIGHT);
	}
	
	//�G�l�~�[���g�̎��͂Ƃ̓����蔻��
	DoIntersectDetection(ENEMY_INTERSECT, PART_INTERSECT, ENEMY_NUM, 1,false, player, enemy, empty_enemy, mushroom, fire_ball, block, question);

	//�G�l�~�[�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
	//�i�v�ύX�@���Ȃ�d���Ȃ�j������\���̂���g���킹���ʂɏ������邱�Ƃŉ���
	DoIntersectDetection(ENEMY_INTERSECT, BREAKABLE_BLOCK_INTERSECT, ENEMY_NUM, 1, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question);
	


	//-------------------------------------
	//�N���{�[�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	for (int i = 0; i < KURIBOO_NUM; i++) {
		kuriboo[i]->left = kuriboo[i]->position + glm::vec2(0, KURIBOO_HEIGHT / 2.0f);
		kuriboo[i]->right = kuriboo[i]->position + glm::vec2(KURIBOO_WIDTH, KURIBOO_HEIGHT / 2.0f);
		kuriboo[i]->top = kuriboo[i]->position + glm::vec2(KURIBOO_WIDTH / 2.0f, 0);
		kuriboo[i]->bottom_left = kuriboo[i]->position + glm::vec2(0 + 1, KURIBOO_HEIGHT);
		kuriboo[i]->bottom_right = kuriboo[i]->position + glm::vec2(KURIBOO_WIDTH - 1, KURIBOO_HEIGHT);
	}

	//�N���{�[���g�̎��͂Ƃ̓����蔻��
	DoIntersectDetection(ENEMY_INTERSECT, PART_INTERSECT, KURIBOO_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�N���{�[�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
	DoIntersectDetection(KURIBOO_INTERSECT, BREAKABLE_BLOCK_INTERSECT, KURIBOO_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�N���{�[���m�̓����蔻��
	DoIntersectDetection(KURIBOO_INTERSECT, KURIBOO_INTERSECT, KURIBOO_NUM, 0, false, player, kuriboo, kuriboo, mushroom, fire_ball, block, question);

	//�m�R�m�R�Ƃ̓����蔻��
	DoIntersectDetection(KURIBOO_INTERSECT, NOKONOKO_INTERSECT, KURIBOO_NUM, 0, false, player, kuriboo, nokonoko, mushroom, fire_ball, block, question);


	//-------------------------------------
	//�m�R�m�R�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		nokonoko[i]->left = nokonoko[i]->position + glm::vec2(0, NOKONOKO_HEIGHT / 2.0f);
		nokonoko[i]->right = nokonoko[i]->position + glm::vec2(NOKONOKO_WIDTH, NOKONOKO_HEIGHT / 2.0f);
		nokonoko[i]->top = nokonoko[i]->position + glm::vec2(NOKONOKO_WIDTH / 2.0f, 0);
		nokonoko[i]->bottom_left = nokonoko[i]->position + glm::vec2(0 + 1, NOKONOKO_HEIGHT);
		nokonoko[i]->bottom_right = nokonoko[i]->position + glm::vec2(NOKONOKO_WIDTH - 1, NOKONOKO_HEIGHT);
	}

	//�m�R�m�R���g�̎��͂Ƃ̓����蔻��
	DoIntersectDetection(ENEMY_INTERSECT, PART_INTERSECT, NOKONOKO_NUM, 0, false, player, nokonoko, empty_enemy, mushroom, fire_ball, block, question);

	//�N���{�[�Ƃ̓����蔻��
	DoIntersectDetection(NOKONOKO_INTERSECT, KURIBOO_INTERSECT, NOKONOKO_NUM, 0, false, player, nokonoko, kuriboo, mushroom, fire_ball, block, question);


	//-------------------------------------
	//�X�[�p�[�L�m�R�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	for (int i = 0; i < MUSHROOM_NUM; i++) {
		mushroom[i]->left = mushroom[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		mushroom[i]->right = mushroom[i]->position + glm::vec2(PART_WIDTH, PART_HEIGHT / 2.0f);
		mushroom[i]->top = mushroom[i]->position + glm::vec2(PART_WIDTH / 2.0f, 0);
		mushroom[i]->bottom_left = mushroom[i]->position + glm::vec2(0 + 3, PART_HEIGHT); //���[�̃u���b�N�ɂЂ�������Ȃ��悤�ɁA�p�̍��W�������߂Ɏw��
		mushroom[i]->bottom_right = mushroom[i]->position + glm::vec2(PART_WIDTH - 3, PART_HEIGHT); //�G�l�~�[�������x���������߁A��菬���߂ɂ��Ȃ��ƌ��Ԃ����z���Ă��܂�
	}

	//�X�[�p�[�L�m�R���g�̎��͂Ƃ̓����蔻��
	DoIntersectDetection(MUSHROOM_INTERSECT, PART_INTERSECT, MUSHROOM_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�X�[�p�[�L�m�R�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
	DoIntersectDetection(MUSHROOM_INTERSECT, BREAKABLE_BLOCK_INTERSECT, MUSHROOM_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);


	//-------------------------------------
	//1UP�L�m�R�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
		mushroom_1up[i]->left = mushroom_1up[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		mushroom_1up[i]->right = mushroom_1up[i]->position + glm::vec2(PART_WIDTH, PART_HEIGHT / 2.0f);
		mushroom_1up[i]->top = mushroom_1up[i]->position + glm::vec2(PART_WIDTH / 2.0f, 0);
		mushroom_1up[i]->bottom_left = mushroom_1up[i]->position + glm::vec2(0 + 3, PART_HEIGHT); //���[�̃u���b�N�ɂЂ�������Ȃ��悤�ɁA�p�̍��W�������߂Ɏw��
		mushroom_1up[i]->bottom_right = mushroom_1up[i]->position + glm::vec2(PART_WIDTH - 3, PART_HEIGHT); //�G�l�~�[�������x���������߁A��菬���߂ɂ��Ȃ��ƌ��Ԃ����z���Ă��܂�
	}

	//1UP�L�m�R���g�̎��͂Ƃ̓����蔻��
	DoIntersectDetection(MUSHROOM_1UP_INTERSECT, PART_INTERSECT, MUSHROOM_1UP_NUM, 0, false, player, kuriboo, empty_enemy, mushroom_1up, fire_ball, block, question);


	//-------------------------------------
	//�X�^�[�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	for (int i = 0; i < STAR_NUM; i++) {
		star[i]->left = star[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		star[i]->right = star[i]->position + glm::vec2(PART_WIDTH, PART_HEIGHT / 2.0f);
		star[i]->top = star[i]->position + glm::vec2(PART_WIDTH / 2.0f, 0);
		star[i]->bottom_left = star[i]->position + glm::vec2(0 + 3, PART_HEIGHT); 
		star[i]->bottom_right = star[i]->position + glm::vec2(PART_WIDTH - 3, PART_HEIGHT); 
	}

	//�X�^�[���g�̎��͂Ƃ̓����蔻��
	DoIntersectDetection(STAR_INTERSECT, PART_INTERSECT, STAR_NUM, 0, false, player, empty_enemy, empty_enemy, star, fire_ball, block, question);

	//�X�^�[�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
	DoIntersectDetection(STAR_INTERSECT, BREAKABLE_BLOCK_INTERSECT, STAR_NUM, 0, false, player, empty_enemy, empty_enemy, star, fire_ball, block, question);


	//-------------------------------------
	//�t�@�C���{�[���̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
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

	//�t�@�C���{�[�����g�̎��͂Ƃ̓����蔻��
	//�v�C���@�t�@�C�A�{�[�����K�i������Ă��܂�
	DoIntersectDetection(FIRE_BALL_INTERSECT, PART_INTERSECT, FIRE_BALL_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�t�@�C�A�{�[���́i�󂹂�j�u���b�N�Ƃ̓����蔻��
	DoIntersectDetection(FIRE_BALL_INTERSECT, BREAKABLE_BLOCK_INTERSECT, FIRE_BALL_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�t�@�C�A�{�[���̃N���{�[�Ƃ̓����蔻��
	DoIntersectDetection(FIRE_BALL_INTERSECT, KURIBOO_INTERSECT, FIRE_BALL_NUM, KURIBOO_NUM, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�t�@�C�A�{�[���̃m�R�m�R�Ƃ̓����蔻��
	DoIntersectDetection(FIRE_BALL_INTERSECT, NOKONOKO_INTERSECT, FIRE_BALL_NUM, NOKONOKO_NUM, false, player, nokonoko, empty_enemy, mushroom, fire_ball, block, question);

	//----------------------------------------------------------
	//���̑������̏����i�ŏI�I�ɂ�intersect.cpp�ɂ܂Ƃ߂����j
	//----------------------------------------------------------

	//�N�G�X�`�����{�b�N�X�������������̏���
	for (int i = 0; i < QUESTION_BOX_NUM; i++) {
		if (coin[i]->flag == false) {
			if (IsGetQuestionbox(player->top, i) == i) {
				coin[i]->flag = true; //�R�C�����o��
				question[i]->flag = false;

				if (i != 1) { //�R�C���ȊO���o��N�G�X�`�����{�b�N�X������
					if (i != 4) {
						if (i != 13) {
							if (i != 14) {
								audioPlay(SE_GET_COIN);
							}
						}
					}
				}
				//�R�C���̈ʒu��ݒ�
				coin[i]->velocity.y += -7.0f;
				if (question[i]->sound_flag == true) {
					audioPlay(SE_BLOCK_BREAK);
					question[i]->sound_flag = false;
				}
			}
		}
	}
	//�L�m�R���o�Ă���N�G�X�`�����{�b�N�X���ʂɏ���
	//�w�肷��C���f�b�N�X�́Acourse.h�̐��� - 1
	if (IsGetQuestionbox(player->top, 1) == 1) {
		mushroom[0]->display_flag = true;
	}
	//�t�@�C���t�����[���o�Ă���N�G�X�`�����{�b�N�X���ʂɏ���
	if (IsGetQuestionbox(player->top, 4) == 4) {
		fire_flower[0]->display_flag = true;
	}

	//1UP�L�m�R���o�Ă���N�G�X�`�����{�b�N�X���ʂɏ���
	if (IsGetQuestionbox(player->top, 13) == 13) {
		mushroom_1up[0]->display_flag = true;
	}
	
	//�X�^�[���o�Ă���N�G�X�`�����{�b�N�X���ʂɏ���
	if (IsGetQuestionbox(player->top, 14) == 14) {
		star[0]->display_flag = true;
	}
	
	/*
	//�N�G�X�`�����{�b�N�X����o���R�C���Ɗ��������̏���
	for (int i = 0; i < QUESTION_BOX_COIN_NUM; i++) { //for (int i = 1; i <= QUESTION_BOX_COIN_NUM; i++) {
		if (coin[i]->flag == true) {
			if (IntersectCoin(player->bottom, i) == i || IntersectCoin(player->left, i) == i || IntersectCoin(player->right, i) == i || IntersectCoin(player->top, i) == i) {
				coin[i]->flag = false;
				coin[i]->sub_flag = false; //��x�Ƃ����R�C�����ĕ\������Ȃ��悤�ɂ���
				if (coin[i]->sound_flag == true) {
					audioPlay(SE_GET_COIN); //��
					coin_num += 1;
					coin[i]->sound_flag = false;
				}
			}
		}
	}
	*/

	//�t�B�[���h��̃R�C���Ɗ��������̏���
	if (player->isInGround == false) { //�n��
		for (int i = 0; i < FIELD_COIN_NUM; i++) {
			if (field_coin[i]->flag == true) {
				if (IntersectFieldCoin(player->bottom, i, false) == i || IntersectFieldCoin(player->left, i, false) == i || IntersectFieldCoin(player->right, i, false) == i || IntersectFieldCoin(player->top, i, false) == i) {
					field_coin[i]->flag = false;
					field_coin[i]->sub_flag = false; //��x�Ƃ����R�C�����ĕ\������Ȃ��悤�ɂ���
					if (field_coin[i]->sound_flag == true) {
						audioPlay(SE_GET_COIN); //��
						coin_num += 1;
						field_coin[i]->sound_flag = false;
					}
				}
			}
		}
	}
	if (player->isInGround == true) { //�n��
		for (int i = 0; i < GROUND_FIELD_COIN_NUM; i++) {
			if (ground_field_coin[i]->flag == true) {
				if (IntersectFieldCoin(player->bottom, i, true) == i || IntersectFieldCoin(player->left, i, true) == i || IntersectFieldCoin(player->right, i, true) == i || IntersectFieldCoin(player->top, i, true) == i) {
					ground_field_coin[i]->flag = false;
					ground_field_coin[i]->sub_flag = false; //��x�Ƃ����R�C�����ĕ\������Ȃ��悤�ɂ���
					if (ground_field_coin[i]->sound_flag == true) {
						audioPlay(SE_GET_COIN); //��
						coin_num += 1;
						ground_field_coin[i]->sound_flag = false;
					}
				}
			}
		}
	}

	//�u���b�N�������������̏���
	for (int i = 0; i < BLOCK_NUM; i++) {
		if (IsHitBlock(player->top, i, block) == i) {
			if (player->isBig == 1) { //�L�����N�^�[����������Ԃł����������̏���
				block[i]->animation_flag = true;
				if (block[i]->sound_flag == true) {
					audioPlay(SE_BLOCK_BREAK);
				}
			}
			else { //�L�����N�^�[���傫����Ԃł����������Ƀu���b�N�����鏈��
				block[i]->flag = false; //�u���b�N�̕\������߂�
				if (block[i]->sound_flag == true) {
					audioPlay(SE_BLOCK_BREAK);
					block[i]->sound_flag = false; //���ڂ̉����Ȃ�Ȃ��悤�ɂ���
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
				//�u���b�N�̔j�Ђ̈ʒu�����߂�
				//�u���b�N����ꂽ��͈ʒu�̍X�V���s���K�v�͂Ȃ����߁A�ȉ��͍s��Ȃ��Ă���
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

	
	//�R�C���u���b�N�������������̏���
	for (int i = 0; i < COIN_BLOCK_NUM; i++) {
		if (IsHitCoinBlock(player->top, i, coin_block) == i) {
			
			if (block_coin[i]->isMax == false) { //�܂��u���b�N���ɃR�C�����c���Ă���ꍇ�A�ȉ��̏������s��
				block_coin[i]->vector_flag[BLOCK_COIN_NUM - coin_block[i]->haveCoinNum] = true; //�R�C�����o��//�u���b�N����񂽂����ƁA10-10=0�Ԗڂ̃t���O������
				block_coin[i]->vector_velocity[BLOCK_COIN_NUM - coin_block[i]->haveCoinNum].y += -7.0f; //�R�C���̈ʒu��ݒ�
				coin_block[i]->haveCoinNum -= 1;
				audioPlay(SE_GET_COIN);
				if (coin_block[i]->haveCoinNum == 0) {
					block_coin[i]->isMax = true;
					coin_block[i]->flag = false;
				}
			}
			coin_block[i]->animation_flag = true; //�u���b�N���g�̃A�j���[�V�������I���ɂ���
			if (coin_block[i]->sound_flag == true) {
				audioPlay(SE_BLOCK_BREAK);
			}
			
		}
		//�u���b�N�̓����̏���
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
	//�G��A�C�e���̈ʒu�̐ݒ�
	//---------------------------------------------

	/*
	//��C�ڂ̓G�̈ʒu�̐ݒ� //���ړ� //�����ʒu��(180,585)
	for (int i = 0; i < 1; i++) { //���ꂼ��ʂɐݒ肷��K�v������
		if (enemy[i]->reach_right_flag == false) {
			enemy[i]->position.x += 0.1;
		}
		else {
			enemy[i]->position.x -= 0.1;
		}

		if (enemy[i]->position.x >= 270.0) { //�u���b�N��̕���45�Ȃ̂ŁA270-18=90�Ńu���b�N���
			enemy[i]->reach_right_flag = true;
		}

		if (enemy[i]->position.x <= 180.0) {
			enemy[i]->reach_right_flag = false;
		}
	}
	

	//��C�ڂ̓G�̈ʒu�̐ݒ�
	if (player->position.x + WINDOW_WIDTH / 2.0f >= enemy[1]->position.x) {
		enemy[1]->animation_flag =  true;
	}
	if (enemy[1]->animation_flag == true) {
		enemy[1]->position.x -= 0.1;
	}
	/*
	//�O�C�ڂ̓G�̈ʒu�̐ݒ�A�y�ǂł̒��˕Ԃ��ǉ�
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

	
	//�N���{�[�̈ʒu�̐ݒ�
	for (int i = 0; i < KURIBOO_NUM; i++) {
		if (kuriboo[i]->display_flag == true && kuriboo[i]->isDeath == false) { //�N���{�[�����܂�ď�������́A�ȉ����s���K�v�͂Ȃ�
			//�v���C���[���߂Â��Ă�����A�j���[�V�������J�n����
			if (player->position.x + WINDOW_WIDTH / 2.0f >= kuriboo[i]->position.x) {
				kuriboo[i]->animation_flag = true;
			}
			if (kuriboo[i]->stepped_on_flag == false) { //�܂����܂�ĂȂ��Ԃ́A�ʒu���X�V����
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
		if (kuriboo[i]->isDeath == true) { //�u���b�N�̉�����˂��グ��ꂽ��A�t�@�C�A�{�[���ɓ������Ď��񂾂Ƃ�
			if (kuriboo[i]->delta_psition_calculate_flag == true) {
				kuriboo[i]->delta_position.x += 0.9f;
				kuriboo[i]->delta_position.y = 0.065 * (kuriboo[i]->delta_position.x) * (kuriboo[i]->delta_position.x - 100);
				if (kuriboo[i]->delta_position.y > WINDOW_HEIGHT) {
					kuriboo[i]->delta_psition_calculate_flag = false;
				}
			}
		}
	}

	//�m�R�m�R�̈ʒu�̐ݒ�
	for (int i = 0; i < NOKONOKO_NUM; i++) {
		if (nokonoko[i]->display_flag == true && nokonoko[i]->isDeath == false) { //�m�R�m�R�����܂�ď�������́A�ȉ����s���K�v�͂Ȃ�
			//�v���C���[���߂Â��Ă�����A�j���[�V�������J�n����
			if (player->position.x + WINDOW_WIDTH / 2.0f >= nokonoko[i]->position.x) {
				nokonoko[i]->animation_flag = true;
			}
			if (nokonoko[i]->animation_flag == true) { //�܂����܂�ĂȂ��Ԃ́A�ʒu���X�V����
				if (nokonoko[i]->stepped_on_flag == false) {
					if (nokonoko[i]->isMirror == false) {
						nokonoko[i]->position.x -= 0.3;
					}
					else {
						nokonoko[i]->position.x += 0.3;
					}
				}
				else if (nokonoko[i]->stepped_on_flag == true) { //���܂�čb����ԂɂȂ������̏���
					if (nokonoko[i]->isShellMoving == true) {
						if (nokonoko[i]->touch_left_flag == true) { //�b����Ԃō������������ꂽ�ꍇ
							if (nokonoko[i]->isMirror == false) {
								nokonoko[i]->position.x -= 3.0f;
							}
							else {
								nokonoko[i]->position.x += 3.0f;
							}
						}
						else if (nokonoko[i]->touch_right_flag == true) { //�b����Ԃō������������ꂽ�ꍇ
							if (nokonoko[i]->isMirror == false) {
								nokonoko[i]->position.x -= 3.0f;
							}
							else {
								nokonoko[i]->position.x += 3.0f;
							}
						}
						else if (nokonoko[i]->touch_top_flag == true) { //�b����Ԃœ��܂ꂽ�ꍇ
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
		if (nokonoko[i]->isDeath == true) { //�u���b�N�̉�����˂��グ��ꂽ��A�t�@�C�A�{�[���ɓ������Ď��񂾂Ƃ�
			if (nokonoko[i]->delta_psition_calculate_flag == true) {
				nokonoko[i]->delta_position.x += 0.9f;
				nokonoko[i]->delta_position.y = 0.065 * (nokonoko[i]->delta_position.x) * (nokonoko[i]->delta_position.x - 100);
				if (nokonoko[i]->delta_position.y > WINDOW_HEIGHT) {
					nokonoko[i]->delta_psition_calculate_flag = false;
				}
			}
		}
	}
	
	//�X�[�p�[�L�m�R�̈ʒu�̐ݒ�
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

	//1UP�L�m�R�̈ʒu�̐ݒ�
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

	//�X�^�[�̈ʒu�̐ݒ�
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

	//�t�@�C�A�{�[���̈ʒu�̐ݒ�
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
		//�t�@�C�A�{�[������ʊO�ɏ�������A�t�@�C�A�{�[���̃C���X�^���X������������
		if (player->position.x + WINDOW_WIDTH  <= fire_ball[i]->position.x || player->position.x - WINDOW_WIDTH >= fire_ball[i]->position.x || fire_ball[i]->position.y > WINDOW_HEIGHT * 1.5) {
			InitFireBallInstance(fire_ball[i]);
		}
		//�t�@�C�A�{�[�����R�[�X�p�[�c�ɓ����������̏����́Aintersect.cpp �� DoIntersectDetection ���ōs��
		//�t�@�C�A�{�[�����G�ɓ�����������A�R�[�X�p�[�c�ɓ������������A�K�؂ɃC���X�^���X�̏��������s��
	}

	//�R�C���̈ʒu�̐ݒ�B�d�͂�^���A���x���ʒu�ɒǉ�����
	for (int i = 0; i < QUESTION_BOX_NUM; i++) {
		if (coin[i]->sub_flag == true) { //��������x�����s��
			if (coin[i]->flag == true) {
				coin[i]->velocity.y += 0.2f;
				coin[i]->delta_pos += coin[i]->velocity;
			}
			if (coin[i]->delta_pos.y >= 1.0f) { //��ђ��˂��R�C�������̈ʒu�ɖ߂�����`�����߂�
				coin[i]->sub_flag = false;
			}
		}
	}

	
	//�����K�u���b�N����o�Ă����R�C���̈ʒu�̐ݒ�B�d�͂�^���A���x���ʒu�ɒǉ�����
	for (int i = 0; i < COIN_BLOCK_NUM; i++) {
		for (int j = 0; j < BLOCK_COIN_NUM; j++) {
			if (block_coin[i]->vector_sub_flag[j] == true) { //��������x�����s��
				if (block_coin[i]->vector_flag[j] == true) {
					block_coin[i]->vector_velocity[j].y += 0.2f;
					block_coin[i]->vector_delta_pos[j] += block_coin[i]->vector_velocity[j];
				}
				if (block_coin[i]->vector_delta_pos[j].y >= 1.0f) { //��ђ��˂��R�C�������̈ʒu�ɖ߂�����`�����߂�
					block_coin[i]->vector_sub_flag[j] = false;
				}
			}
		}
	}
	

	//�S�[���̊��̈ʒu�̐ݒ�
	if (player->isGoal == true) {
		if (player->flag_pos.y <= 540) {
			player->flag_pos.y += 1.2;
		}
	}

	//�x�N�^�[�̃T�C�Y���P�R�Ȃ̂łP�R�ȏ�̗v�f�ւ̃A�N�Z�X�̓G���[
	//coin�̃T�C�Y�͂Ȃ����P�U
	//�N�G�X�`�����{�b�N�X����o���R�C���̃A�j���[�V����
	for (int i = 0; i < QUESTION_BOX_COIN_NUM; i++) {
		coin[i]->frame++;
	}

	//�t�B�[���h��̃R�C���̃A�j���[�V����
	if (player->isInGround == false) { //�n��
		for (int i = 0; i < FIELD_COIN_NUM; i++) {
			field_coin[i]->frame++;
		}
	}
	else if (player->isInGround == true) { //�n��
		for (int i = 0; i < GROUND_FIELD_COIN_NUM; i++) {
			ground_field_coin[i]->frame++;
		}
	}

	//�����K�u���b�N����o��	�R�C���̃A�j���[�V����
	for (int i = 0; i < COIN_BLOCK_NUM; i++) {
		for (int j = 0; j < BLOCK_COIN_NUM; j++) {
			block_coin[i]->vector_frame[j]++;
		}
	}
	
	//�N�G�X�`�����{�b�N�X�̃A�j���[�V����
	for (int i = 0; i < QUESTION_BOX_NUM; i++) {
		question[i]->frame++;
	}

	glutPostRedisplay();

	if (COMMAND_MODE == 2) {
		VoiceCommand();
	}
	
	//�f�X�N�g�b�v�p�\�R���ł́A���ۂ̃t���[�����[�g�Ə������x���قȂ邽�߁AIdleFunc�ł͂Ȃ��^�C�}�[�̍ċN�Ăяo���ŏ�������
	//��������1000/60�ŁA�������t���[�����[�g�ɂȂ邪�A�ォ��ݒ肵�����߁A�K�x�ȏ������x�ɂȂ�悤�ɁA�����Őݒ肵���B
	glutTimerFunc(1000 / 180, Idle, 0);
}



//----------------------------
//�L�[�{�[�h���͂������֐�
//----------------------------
//�L�[�������āA����������Â����������������ɏ���
//�L�[�������Ă���Ԃ��������鏈���́AkeyIsPressed�ŕʂ̏��ŏ�������
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

		//�������̓y�ǂɓ���Ƃ��̏���(1-1)
		if (player->isInGround == true) {
			if (IntersectPipe(player->right, RIGHT, true)) {
				if (player->inPipe == false) { //l��A�ł����Ƃ��ɁA�������x���Ȃ�Ȃ��悤�ɂ���
					audioPlay(SE_IN_PIPE);
				}
				player->inPipe = true;
			}
		}
		//�������̓y�ǂɓ���Ƃ��̏���(1-2)
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

		//�c�����̓y�ǂɓ���Ƃ��̏���(1-1)
		if (IntersectPipe(player->bottom, BOTTOM, false)) {
			if (player->inPipe == false) { //m��A�ł����Ƃ��ɁA�������x���Ȃ�Ȃ��悤�ɂ���
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
//����ȍ~��1-2�Ɋւ���v���O����
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------
//1-2�̃f�[�^������������֐�
//------------------------------------
void Init_1_2(void) {
	
	AllTexFromFile();

	player = MakePlayerInstance();
	coin_1_2 = MakeCoinInstance(QUESTION_BOX_NUM_1_2); //�N�G�X�`�����{�b�N�X����o��R�C��
	block_1_2 = MakeBlockInstance(BLOCK_NUM_1_2);
	question_1_2 = MakeQuestionInstance(QUESTION_BOX_NUM_1_2);
	kuriboo_1_2 = MakeEnemyInstance(KURIBOO_WIDTH, KURIBOO_HEIGHT, KURIBOO_NUM_1_2);
	nokonoko_1_2 = MakeEnemyInstance(NOKONOKO_WIDTH, NOKONOKO_HEIGHT, NOKONOKO_NUM_1_2);
	mushroom_1_2 = MakeMushroomInstance(MUSHROOM_NUM_1_2);
	fire_flower_1_2 = MakeFireFlowerInstance(FIRE_FLOWER_NUM_1_2);
	
	fire_ball = MakeFireBallInstance(); //���ꂪ�Ȃ��ƂȂ������s�G���[
	lift = MakeLiftInstance();

	player->position = { 95, PART_HEIGHT * 8};
	//player->position = { 3000, PART_HEIGHT * 8 };
	player->flag_pos = { 945 + 22.5, 180 };
	player->course = START;
	player->isScroll = false;
	player->isBig = 1;
	player->isFire = false;
	player->isStar = false;

	//�t�@�C�A�{�[���̏����ʒu��ݒ�
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		fire_ball[i]->position = player->position + glm::vec2(22.5, 0);
	}

	//�N���{�[�̈ʒu�ݒ�
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

	//�m�R�m�R�̈ʒu��ݒ�
	nokonoko_1_2[0]->position = { 1885,  PART_HEIGHT * 13 };
	nokonoko_1_2[1]->position = { 1975,  PART_HEIGHT * 13 };//

	//�L�m�R�̏����ʒu��ݒ�
	mushroom_1_2[0]->position = {450, 405};

	//�t�@�C�A�t�����[�̏����ʒu��ݒ�
	fire_flower_1_2[0]->position = { 3105, 360 };

	//���t�g�̈ʒu��ݒ�
	lift[0]->position = { 6320, WINDOW_HEIGHT * ( 1 / 4.0f) };
	lift[1]->position = { 6320, WINDOW_HEIGHT * (3 / 4.0f) };
	lift[2]->position = { 6975, WINDOW_HEIGHT * (1 / 4.0f) };
	lift[3]->position = { 6975, WINDOW_HEIGHT * (3 / 4.0f) };

}


//-------------------------------------
//1-2�̃R�[�X�p�[�c��`�悷��֐�
//-------------------------------------
void DrawPart_1_2(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, int _course) {

	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return;
	}
	
	if (_course == MAIN) { //���C���R�[�X�̕`�� //�n��
		glClearColor(0.0, 0.0, 0.0, 1.0);

		if (_c == 'b') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_GROUND_BLOCK_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_GROUND_BLOCK_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2':
				//�P�|�Q�̓����K�u���b�N��377������A�d������̂ŁA���������ʂ����ɕ`�悷��
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
			case '4': DrawSprite(TEX_GROUND_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '5': DrawSprite(TEX_GROUND_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '6': DrawSprite(TEX_GROUND_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '7': DrawSprite(TEX_GROUND_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '8': DrawSprite(TEX_JOINT_GROUND_PIPE_26, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�ڍ���
			case '9': DrawSprite(TEX_JOINT_GROUND_PIPE_27, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�ڍ���
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
	else if (_course == START || _course == GOAL) { //�n��
		glClearColor(0.31372549, 0.50196078, 1.0, 1.0);  //(80,128, 255)�w�i�F����F�ɂ���

		if (_c == 'b') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_BLOCK_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_BLOCK_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': //�j��ł���u���b�N�͂Ȃ�
			default: break;
			}
		}
		else if (_c == 'q') { //�N�G�X�`�����{�b�N�X�͂Ȃ�
		}
		else if (_c == 'p') {
			switch (_sub_c) {
			case '0': DrawSprite(TEX_PIPE_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '1': DrawSprite(TEX_PIPE_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '2': DrawSprite(TEX_PIPE_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '3': DrawSprite(TEX_PIPE_3, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			case '4': DrawSprite(TEX_PIPE_4, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '5': DrawSprite(TEX_PIPE_5, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '6': DrawSprite(TEX_PIPE_6, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '7': DrawSprite(TEX_PIPE_7, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�������̓y��
			case '8': DrawSprite(TEX_JOINT_PIPE_26, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�ڍ���
			case '9': DrawSprite(TEX_JOINT_PIPE_27, PART_WIDTH, PART_HEIGHT, 0, _position); break; //�ڍ���
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
//1-2��`�悷��֐�
//-------------------------------
void Display_1_2(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float scrollX = player->position.x - WINDOW_WIDTH / 2 + PLAYER_WIDTH / 2;

	//�R�[�X�p�[�c��`��
	/*
	if (player->course == MAIN) { //�}���I�����C���R�[�X�ɂ���Ƃ�
		for (int y = 0; y < COURSE_HEIGHT; y++) {
			for (int x = 0; x < COURSE_WIDTH; x++) {
				DrawPart_1_2({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, course_1_2[y][x], sub_course_1_2[y][x], sub2_course_1_2[y][x], MAIN);
			}
		}
	}
	*/
	if (player->course == MAIN) { //�}���I�����C���R�[�X�ɂ���Ƃ�
		for (int y = 0 ; y < COURSE_HEIGHT; y++) {
			for (int x = int((player->position.x -  WINDOW_WIDTH / 2)/ 45); x < int((player->position.x + WINDOW_WIDTH / 2 + 90) / 45); x++) { //�}�b�v�X�N���[���̌y��
				DrawPart_1_2({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, course_1_2[y][x], sub_course_1_2[y][x], sub2_course_1_2[y][x], MAIN);
			}
		}
	}
	else if (player->course == START) { //�X�^�[�g�R�[�X�ɂ���Ƃ�
		for (int y = 0; y < START_COURSE_HEIGHT_1_2; y++) {
			for (int x = 0; x < START_COURSE_WIDTH_1_2; x++) {
				DrawPart_1_2({ x * PART_WIDTH, y * PART_HEIGHT }, start_course_1_2[y][x], start_sub_course_1_2[y][x], start_sub2_course_1_2[y][x], START);
			}
		}
	}
	else if (player->course == GOAL) { //�S�[���R�[�X�ɂ���Ƃ�
		for (int y = 0; y < GOAL_COURSE_HEIGHT_1_2; y++) {
			for (int x = 0; x < GOAL_COURSE_WIDTH_1_2; x++) {
				DrawPart_1_2({ x * PART_WIDTH - scrollX, y * PART_HEIGHT }, goal_course_1_2[y][x], goal_sub_course_1_2[y][x], goal_sub2_course_1_2[y][x], GOAL);
			}
		}
	}

	//�N�G�X�`�����{�b�N�X����o���R�C����`��
	for (int i = 0; i < QUESTION_BOX_COIN_NUM_1_2; i++) {
		DrawCoin(i, COURSE_1_2);
	}
	

	//���t�g��`��
	for (int i = 0; i < LIFT_NUM; i++) {
		DrawSprite(TEX_LIFT, PART_WIDTH * 5, PART_HEIGHT, 0,lift[i]->position + glm::vec2(-scrollX, 0));
	}

	//�S�[���̊���`��
	//������ɕ`���Ȃ��ƁA�L�����N�^�[���S�[���̊��ɉB����Ă��܂�
	if (player->course == GOAL) {
		DrawSprite(TEX_FLAG, PART_WIDTH, PART_HEIGHT, 0,
			player->flag_pos + glm::vec2(-scrollX, 0));
	}

	//�}���I��`�� //�i�X�N���[�����邩���Ȃ����ŕ�����j���肾�������AscrollX��player->isScroll��`���邱�Ƃŉ���
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

	//�t�@�C�A�{�[����`��
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			DrawSprite(TEX_FIRE_BALL, PART_WIDTH, PART_HEIGHT,
				fire_ball_animations[fire_ball[i]->animation].frames[(fire_ball[i]->frame / 31) % animations[fire_ball[i]->animation].count],
				fire_ball[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 16.0f, PART_HEIGHT / 16.0f)
			);
		}
	}

	//�N���{�[��`��
	if (player->course == MAIN) {
		for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
			if (kuriboo_1_2[i]->display_flag == true && kuriboo_1_2[i]->isDeath == false) { //���܂��A�܂��̓u���b�N�̉����炽������Ď��ʂ܂ŕ`�悷��
				//printf("%d", i);
				DrawSprite(TEX_GROUND_KURIBOO, KURIBOO_WIDTH, KURIBOO_HEIGHT,
					kuriboo_animations[kuriboo_1_2[i]->animation].frames[(kuriboo_1_2[i]->frame / 63) % animations[kuriboo_1_2[i]->animation].count],
					kuriboo_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(KURIBOO_WIDTH / 16.0f, KURIBOO_HEIGHT / 16.0f),
					kuriboo_1_2[i]->isMirror
				);
			}
			//�N���{�[���Ђ�����Ԃ��Ď��ʃA�j���[�V������`��
			if (kuriboo_1_2[i]->isDeath == true) {
				DrawSprite(TEX_GROUND_KURIBOO, KURIBOO_WIDTH, KURIBOO_HEIGHT,
					3, //Kuriboo.bmp�t�@�C���̂S�Ԗڂ́i�Ђ�����Ԃ����j�摜��\������
					kuriboo_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(KURIBOO_WIDTH / 16.0f, KURIBOO_HEIGHT / 16.0f) + kuriboo_1_2[i]->delta_position,
					kuriboo_1_2[i]->isMirror
				);
			}
		}
	}


	//�m�R�m�R��`��
	if (player->course == MAIN) {
		for (int i = 0; i < NOKONOKO_NUM_1_2; i++) {
			if (nokonoko_1_2[i]->display_flag == true && nokonoko_1_2[i]->isDeath == false) { //���܂��A�܂��͎��ʂ܂ŕ`�悷��
				if (nokonoko_1_2[i]->stepped_on_flag == false) {
					DrawSprite(TEX_GROUND_NOKONOKO, NOKONOKO_WIDTH, NOKONOKO_HEIGHT,
						nokonoko_animations[nokonoko_1_2[i]->animation].frames[(nokonoko_1_2[i]->frame / 31) % animations[nokonoko_1_2[i]->animation].count],
						nokonoko_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_WIDTH / 15.0f, NOKONOKO_HEIGHT / 15.0f),
						nokonoko_1_2[i]->isMirror
					);
				}
				else { //�b����Ԃ̃m�R�m�R
					DrawSprite(TEX_NOKONOKO_SHELL, NOKONOKO_SHELL_WIDTH, NOKONOKO_SHELL_HEIGHT,
						0,
						nokonoko_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(0/*NOKONOKO_SHELL_WIDTH / 18.0f*/, 0),
						nokonoko_1_2[i]->isMirror
					);
				}
			}
			//�m�R�m�R���Ђ�����Ԃ��Ď��ʃA�j���[�V������`��
			if (nokonoko_1_2[i]->isDeath == true) {
				if (nokonoko_1_2[i]->stepped_on_flag == false) { //�m�R�m�R�������ē����Ƃ��Ƀt�@�C�A�{�[���ɎE���ꂽ�ꍇ
					DrawSprite(TEX_NOKONOKO, NOKONOKO_WIDTH, NOKONOKO_HEIGHT,
						2, //�t�@�C����2�Ԗڂ́i�Ђ�����Ԃ����j�摜��\������
						nokonoko_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_WIDTH / 16.0f, NOKONOKO_HEIGHT / 16.0f) + nokonoko_1_2[i]->delta_position,
						nokonoko_1_2[i]->isMirror
					);
				}
				else { //�m�R�m�R���b���̎��Ƀt�@�C�A�{�[���ɎE���ꂽ�ꍇ
					DrawSprite(TEX_NOKONOKO_SHELL, NOKONOKO_SHELL_WIDTH, NOKONOKO_SHELL_HEIGHT,
						2, //�t�@�C����2�Ԗڂ́i�Ђ�����Ԃ����j�摜��\������
						nokonoko_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(NOKONOKO_SHELL_WIDTH / 16.0f, NOKONOKO_SHELL_HEIGHT / 16.0f) + nokonoko_1_2[i]->delta_position,
						nokonoko_1_2[i]->isMirror
					);
				}
			}
		}
	}


	//�X�[�p�[�L�m�R��`��
	for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
		if (mushroom_1_2[i]->display_flag == true) {
			if (mushroom_1_2[i]->get_flag == false) {
				DrawSprite(TEX_MUSHROOM, PART_WIDTH, PART_HEIGHT, 0,
					mushroom_1_2[i]->position + glm::vec2(-scrollX, 0) - glm::vec2(PART_WIDTH / 30.0f, PART_HEIGHT / 30.0f)
				);
			}
		}
	}

	//�t�@�C�A�t�����[��`��
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

	//�����y�ǂ����㏑��
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
	fontSetWeight(2.5); //�ŏ�0.5�A�ő�10.0
	fontSetColor(0xff - 0x10, 0xff - 0x10, 0xff - 0x10); //�����ɈÂ߂̔��F
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
//�A�C�h�����̏����i1-2�j
//-----------------------------------------------
void Idle_1_2(int value) {
	float acceleration = 0.2f;

	if (keysIsPressed['j']) {
		player->velocity.x -= acceleration;
	}

	if (keysIsPressed['l']) {
		player->velocity.x += acceleration;
	}

	//�L�����N�^�[�ɕ����@����K�p�i��R�Əd�́j
	player->velocity.x *= 0.93f;
	player->velocity.y += 0.2f;
	player->position += player->velocity;
	player->frame++;

	
	//�}���I�̃X�^�[�̕b���𐧌䂷��
	if (player->isStar == false) {
		player->tmp_star_frame = player->frame;
	}
	else if (player->isStar == true) {
		if (player->frame > player->tmp_star_frame + 180 * 8) { //���܁A180fps�ŕ`�悵�Ă��邽�߁A�X�^�[���W�b�ԑ����邽�߂�180*8
			player->isStar = false;
			audioStop(MUSIC_STAR); //�X�^�[��BGM���~�߁A
			//audioPlay(MUSIC_STAGE); //�X�e�[�W��BGM�𗬂�
		}
	}
	

	//�L�����N�^�[���R�[�X�O�ɍs�������̃��Z�b�g����
	if ((player->position.y >= WINDOW_HEIGHT + PART_HEIGHT * 100)) {
		player->position = { WINDOW_WIDTH / 2.0f,  PART_HEIGHT * (13 - (player->isBig - 1)) };
		player->isBig = 1;
		player->isFire = false;
	}

	/*
	//�c�̓y�ǂɓ���Ƃ��̏���
	if (!(IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true)) {
		player->tmp1_pos = player->position;
	}
	else if (IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.y += 0.1f;
		if (player->position.y > player->tmp1_pos.y + 135) { //�}���I���y�ǂɓ��肫�����烏�[�v
			player->isInGround = true;
			player->position = { 45 * 2, 0 };
			player->inPipe = false; //���[�v������y�ǂɓ��蒆�ł���Ƃ����t���O�����낷
			audioStop(MUSIC_STAGE); //�n��̉��y���~�߂�
			audioPlay(MUSIC_UNDER_GROUND_STAGE); //�n���̉��y�𗬂�
		}
	}
	*/
	//�X�^�[�g�R�[�X�̓y�ǂɓ���Ƃ��̏���
	if (!(IntersectPipe_1_2(player->right, RIGHT, START) && player->inPipe == true)) {
		player->tmp1_pos = player->position;
	}
	else if (IntersectPipe_1_2(player->right, RIGHT, START) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.x += 0.15f;
		if (player->position.x > player->tmp1_pos.x + 50) { //�}���I���y�ǂɓ��肫�����烏�[�v
			player->position = { 160, PART_HEIGHT * 1 };
			player->course = MAIN;
			player->isScroll = true;
			
			player->inPipe = false; //���[�v������y�ǂɓ��蒆�ł���Ƃ����t���O�����낷
			//audioStop(MUSIC_UNDER_GROUND_STAGE); //�n���̉��y���~��
			//audioPlay(MUSIC_STAGE); //�n��̉��y�𗬂�
		}
	}
	//���C���R�[�X�̉������y�ǁi�S�[���R�[�X�ɑ����j�ɓ���Ƃ��̏���
	if (!(IntersectPipe_1_2(player->right, RIGHT, MAIN) && player->inPipe == true)) {
		player->tmp2_pos = player->position;
	}
	else if (IntersectPipe_1_2(player->right, RIGHT, MAIN) && player->inPipe == true) {
		player->velocity.x = 0;
		player->velocity.y = 0;
		player->position.x += 0.15f;
		if (player->position.x > player->tmp2_pos.x + 50) { //�}���I���y�ǂɓ��肫�����烏�[�v
			player->course = GOAL;
			player->isScroll = true;
			player->position = { 160, PART_HEIGHT * (11 - player->isBig + 1) };
			player->inPipe = false;
			//audioStop(MUSIC_UNDER_GROUND_STAGE); //�n���̉��y���~��
			//audioPlay(MUSIC_STAGE); //�n��̉��y�𗬂�
		}
	}

	
	//�N���{�[�ɏd�͂�^����
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
	
	
	//�X�[�p�[�L�m�R�ɏd�͂�^����
	for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
		if (mushroom_1_2[i]->display_flag == true) {
			mushroom_1_2[i]->velocity.y += 0.2f;
			mushroom_1_2[i]->position += mushroom_1_2[i]->velocity;
		}
	}
	
	
	//�t�@�C�A�{�[���ɏd�͂�^����
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				fire_ball[i]->velocity.y += 0.2f;
				fire_ball[i]->position += fire_ball[i]->velocity;
			}
		}
	}
	

	//�}���I�̃A�j���[�V�����̏���
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

	if (keysIsPressed['j'] && keysIsPressed['l'] && keysIsPressed['c']) { //�}���I���_�b�V�����ɐ؂�Ԃ��Ƃ�
		player->animation = ANIMATION_TURN;
		if (player->isStar == true) {
			player->animation = ANIMATION_STAR_TURN;
		}
	}

	if ((IntersectPipe(player->bottom, BOTTOM, false) && player->inPipe == true) || (IntersectPipe(player->right, RIGHT, true) && player->inPipe == true)) { //�y�ǂɓ���Ƃ�
		player->animation = ANIMATION_STAND; //���ꂪ�Ȃ��ƁA�y�ǂɓ���Ƃ��ɓ����蔻�肪�����Ă��邽�߃W�����v�̃A�j���[�V�����ɂȂ��Ă��܂�
	}

	//�R�C���̃A�j���[�V����
	for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
		coin_1_2[i]->frame++;
	}

	//�N�G�X�`�����{�b�N�X�̃A�j���[�V����
	for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
		question_1_2[i]->frame++;
	}

	//�t�@�C�A�t�����[�̃A�j���[�V����
	for (int i = 0; i < FIRE_FLOWER_NUM_1_2; i++) {
		fire_flower_1_2[i]->frame++;
	}


	//�t�@�C�A�{�[���̃A�j���[�V�����̏���
	//�G�Ɠ����������̃A�j���[�V�����̐؂�ւ���intersect.cpp�ōs��
	for (int i = 0; i < FIRE_BALL_NUM; i++) {
		if (fire_ball[i]->display_flag == true) {
			if (fire_ball[i]->isAttackEnemy == false) {
				fire_ball[i]->animation = FIRE_BALL_ANIMATION_BOUND;
				fire_ball[i]->frame++;
				fire_ball[i]->frame_at_attack = fire_ball[i]->frame; //�G�Ɗ������u�Ԃ̃t���[�����ێ������
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

	//�N���{�[�̃A�j���[�V�����̏���
	for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
		if (kuriboo_1_2[i]->stepped_on_flag == false) {
			kuriboo_1_2[i]->animation = KURIBOO_ANIMATION_STAND;
			kuriboo_1_2[i]->frame++;
			kuriboo_1_2[i]->frame_at_stepped_on = kuriboo_1_2[i]->frame; //���܂ꂽ�u�Ԃ̃t���[�����ێ������
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
	//�L�����N�^�[�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	player->left = player->position + glm::vec2(0, player->isBig * PLAYER_HEIGHT / 2.0f);
	player->right = player->position + glm::vec2(PLAYER_WIDTH, player->isBig * PLAYER_HEIGHT / 2.0f);
	player->top = player->position + glm::vec2(PLAYER_WIDTH / 2.0f, 0);
	player->bottom = player->position + glm::vec2(PLAYER_WIDTH / 2.0f, player->isBig * PLAYER_HEIGHT);

	//�v���C���[�̃R�[�X�p�[�c�Ƃ̓����蔻��
	DoIntersectDetection_1_2(PLAYER_INTERSECT, PART_INTERSECT, 1, 1, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question, lift);

	//�v���C���[�̃��t�g�Ƃ̓����蔻��
	DoIntersectDetection_1_2(PLAYER_INTERSECT, LIFT_INTERSECT, 1, LIFT_NUM, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question, lift);

	//�v���C���[�̃S�[���Ƃ̓����蔻��
	DoIntersectDetection_1_2(PLAYER_INTERSECT, GOAL_INTERSECT, 1, 1, false, player, enemy, empty_enemy, mushroom, fire_ball, block, question, lift);

	//�v���C���[�̃L�m�R�Ƃ̓����蔻��
	DoIntersectDetection_1_2(PLAYER_INTERSECT, MUSHROOM_INTERSECT, 1, MUSHROOM_NUM_1_2, false, player, kuriboo, empty_enemy, mushroom_1_2, fire_ball, block, question, lift);

	//�v���C���[�̃t�@�C�A�t�����[�Ƃ̓����蔻��
	DoIntersectDetection_1_2(PLAYER_INTERSECT, FIRE_FLOWER_INTERSECT, 1, FIRE_FLOWER_NUM_1_2, false, player, kuriboo, empty_enemy, fire_flower_1_2, fire_ball, block, question, lift);

	//�v���C���[�̃N���{�[�Ƃ̓����蔻��
	DoIntersectDetection_1_2(PLAYER_INTERSECT, KURIBOO_INTERSECT, 1, KURIBOO_NUM_1_2, false, player, kuriboo_1_2, empty_enemy, mushroom, fire_ball, block, question, lift);


	//-------------------------------------
	//�X�[�p�[�L�m�R�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
		mushroom_1_2[i]->left = mushroom_1_2[i]->position + glm::vec2(0, PART_HEIGHT / 2.0f);
		mushroom_1_2[i]->right = mushroom_1_2[i]->position + glm::vec2(PART_WIDTH, PART_HEIGHT / 2.0f);
		mushroom_1_2[i]->top = mushroom_1_2[i]->position + glm::vec2(PART_WIDTH / 2.0f, 0);
		mushroom_1_2[i]->bottom_left = mushroom_1_2[i]->position + glm::vec2(0 + 3, PART_HEIGHT); //���[�̃u���b�N�ɂЂ�������Ȃ��悤�ɁA�p�̍��W�������߂Ɏw��
		mushroom_1_2[i]->bottom_right = mushroom_1_2[i]->position + glm::vec2(PART_WIDTH - 3, PART_HEIGHT); //�G�l�~�[�������x���������߁A��菬���߂ɂ��Ȃ��ƌ��Ԃ����z���Ă��܂�
	}

	//�X�[�p�[�L�m�R���g�̎��͂Ƃ̓����蔻��
	DoIntersectDetection_1_2(MUSHROOM_INTERSECT, PART_INTERSECT, MUSHROOM_NUM_1_2, 0, false, player, kuriboo, empty_enemy, mushroom_1_2, fire_ball, block, question, lift);

	//�X�[�p�[�L�m�R�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
	//DoIntersectDetection(MUSHROOM_INTERSECT, BREAKABLE_BLOCK_INTERSECT, MUSHROOM_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);


	//-------------------------------------
	//�t�@�C���{�[���̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
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

	//�t�@�C���{�[�����g�̎��͂Ƃ̓����蔻��
	//�v�C���@�t�@�C�A�{�[�����K�i������Ă��܂�
	DoIntersectDetection_1_2(FIRE_BALL_INTERSECT, PART_INTERSECT, FIRE_BALL_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question, lift);

	//�t�@�C�A�{�[���́i�󂹂�j�u���b�N�Ƃ̓����蔻��
	//DoIntersectDetection(FIRE_BALL_INTERSECT, BREAKABLE_BLOCK_INTERSECT, FIRE_BALL_NUM, 0, false, player, kuriboo, empty_enemy, mushroom, fire_ball, block, question);

	//�t�@�C�A�{�[���̃N���{�[�Ƃ̓����蔻��
	DoIntersectDetection_1_2(FIRE_BALL_INTERSECT, KURIBOO_INTERSECT, FIRE_BALL_NUM, KURIBOO_NUM_1_2, false, player, kuriboo_1_2, empty_enemy, mushroom, fire_ball, block, question, lift);


	//-------------------------------------
	//�N���{�[�̓����蔻��𒲂ׂ�
	//-------------------------------------
	//�ʒu���X�V
	for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
		kuriboo_1_2[i]->left = kuriboo_1_2[i]->position + glm::vec2(0, KURIBOO_HEIGHT / 2.0f);
		kuriboo_1_2[i]->right = kuriboo_1_2[i]->position + glm::vec2(KURIBOO_WIDTH, KURIBOO_HEIGHT / 2.0f);
		kuriboo_1_2[i]->top = kuriboo_1_2[i]->position + glm::vec2(KURIBOO_WIDTH / 2.0f, 0);
		kuriboo_1_2[i]->bottom_left = kuriboo_1_2[i]->position + glm::vec2(0 + 1, KURIBOO_HEIGHT);
		kuriboo_1_2[i]->bottom_right = kuriboo_1_2[i]->position + glm::vec2(KURIBOO_WIDTH - 1, KURIBOO_HEIGHT);
	}

	//�N���{�[���g�̎��͂Ƃ̓����蔻��
	DoIntersectDetection_1_2(KURIBOO_INTERSECT, PART_INTERSECT, KURIBOO_NUM_1_2, 0, false, player, kuriboo_1_2, empty_enemy, mushroom, fire_ball, block, question, lift);

	//----------------------------------------------------------
	//���̑������̏����i�ŏI�I�ɂ�intersect.cpp�ɂ܂Ƃ߂����j
	//----------------------------------------------------------

	
	//�N�G�X�`�����{�b�N�X�������������̏���
	for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
		if (coin_1_2[i]->flag == false) {
			if (IsGetQuestionbox_1_2(player->top, i) == i) {
				coin_1_2[i]->flag = true; //�R�C�����o��
				question_1_2[i]->flag = false;

				if (i != 0) { //�R�C���ȊO���o��N�G�X�`�����{�b�N�X������
					if (i != 5) {
						//if (i != 13) {
							//if (i != 14) {
								audioPlay(SE_GET_COIN);
							//}
						//}
					}
				}
				//�R�C���̈ʒu��ݒ�
				coin_1_2[i]->velocity.y += -7.0f;
				if (question_1_2[i]->sound_flag == true) {
					audioPlay(SE_BLOCK_BREAK);
					question_1_2[i]->sound_flag = false;
				}
			}
		}
	}
	//�L�m�R���o�Ă���N�G�X�`�����{�b�N�X���ʂɏ���
	//�w�肷��C���f�b�N�X�́Acourse.h�̐��� - 1
	if (IsGetQuestionbox_1_2(player->top, 0) == 0) {
		mushroom_1_2[0]->display_flag = true;
	}
	//�t�@�C���t�����[���o�Ă���N�G�X�`�����{�b�N�X���ʂɏ���
	if (IsGetQuestionbox_1_2(player->top, 5) == 5) {
		fire_flower_1_2[0]->display_flag = true;
	}
	/*
	//1UP�L�m�R���o�Ă���N�G�X�`�����{�b�N�X���ʂɏ���
	if (IsGetQuestionbox_1_2(player->top, 13) == 13) {
		mushroom_1up[0]->display_flag = true;
	}

	//�X�^�[���o�Ă���N�G�X�`�����{�b�N�X���ʂɏ���
	if (IsGetQuestionbox_1_2(player->top, 14) == 14) {
		star[0]->display_flag = true;
	}
	*/
	

	//---------------------------------------------
	//�G��A�C�e���̈ʒu�̐ݒ�
	//---------------------------------------------


	//�X�[�p�[�L�m�R�̈ʒu�̐ݒ�
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


	//�t�@�C�A�{�[���̈ʒu�̐ݒ�
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
		//�t�@�C�A�{�[������ʊO�ɏ�������A�t�@�C�A�{�[���̃C���X�^���X������������
		if (player->position.x + WINDOW_WIDTH <= fire_ball[i]->position.x || player->position.x - WINDOW_WIDTH >= fire_ball[i]->position.x || fire_ball[i]->position.y > WINDOW_HEIGHT * 1.5) {
			InitFireBallInstance(fire_ball[i]);
		}


		//�t�@�C�A�{�[�����R�[�X�p�[�c�ɓ����������̏����́Aintersect.cpp �� DoIntersectDetection ���ōs��
		//�t�@�C�A�{�[�����G�ɓ�����������A�R�[�X�p�[�c�ɓ������������A�K�؂ɃC���X�^���X�̏��������s��
	}

	
	//�R�C���̈ʒu�̐ݒ�B�d�͂�^���A���x���ʒu�ɒǉ�����
	for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
		if (coin_1_2[i]->sub_flag == true) { //��������x�����s��
			if (coin_1_2[i]->flag == true) {
				coin_1_2[i]->velocity.y += 0.2f;
				coin_1_2[i]->delta_pos += coin_1_2[i]->velocity;
			}
			if (coin_1_2[i]->delta_pos.y >= 1.0f) { //��ђ��˂��R�C�������̈ʒu�ɖ߂�����`�����߂�
				coin_1_2[i]->sub_flag = false;
			}
		}
	}

	
	//�N���{�[�̈ʒu�̐ݒ�
	for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
		if (kuriboo_1_2[i]->display_flag == true && kuriboo_1_2[i]->isDeath == false && player->course== MAIN) { //�N���{�[�����܂�ď�������́A�ȉ����s���K�v�͂Ȃ�
			//�v���C���[���߂Â��Ă�����A�j���[�V�������J�n����
			if (player->position.x + WINDOW_WIDTH / 2.0f >= kuriboo_1_2[i]->position.x) {
				kuriboo_1_2[i]->animation_flag = true;
			}
			if (kuriboo_1_2[i]->stepped_on_flag == false) { //�܂����܂�ĂȂ��Ԃ́A�ʒu���X�V����
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
		if (kuriboo_1_2[i]->isDeath == true) { //�u���b�N�̉�����˂��グ��ꂽ��A�t�@�C�A�{�[���ɓ������Ď��񂾂Ƃ�
			if (kuriboo_1_2[i]->delta_psition_calculate_flag == true) {
				kuriboo_1_2[i]->delta_position.x += 0.9f;
				kuriboo_1_2[i]->delta_position.y = 0.065 * (kuriboo_1_2[i]->delta_position.x) * (kuriboo_1_2[i]->delta_position.x - 100);
				if (kuriboo_1_2[i]->delta_position.y > WINDOW_HEIGHT) {
					kuriboo_1_2[i]->delta_psition_calculate_flag = false;
				}
			}
		}
	}
	


	//���t�g�̈ʒu�̍X�V
	for (int i = 0; i < DOWN_LIFT_NUM; i++) { //�������ɓ������t�g
		lift[i]->position.y += 1.0f;
		if (lift[i]->position.y > 740) {
			lift[i]->position.y = -20;
		}
	}
	for (int i = 2; i < 2 + UP_LIFT_NUM; i++) { //������ɓ������t�g
		lift[i]->position.y -= 1.0f;
		if (lift[i]->position.y < -20) {
			lift[i]->position.y = 740;
		}
	}

	//�S�[���̊��̈ʒu�̍X�V
	if (player->isGoal == true) {
		if (player->flag_pos.y <= 540) {
			player->flag_pos.y += 1.2f;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(1000 / 180, Idle_1_2, 0);


}


void SwitchDisplay() {

	if (GetWhitchCourse() == 11) { //intersect.cpp���ő��ϐ��Ƃ��Ē�`�����l���Q�Ƃ���
		Display();
	}
	if (GetWhitchCourse() == 12) { //intersect.cpp���ő��ϐ��Ƃ��Ē�`�����l���Q�Ƃ���

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
	

	//�P�|�P�̕\��
	//if (COURSE == COURSE_1_1) {
	//if(player.whitch_course == 11){ //player�Ɏ������������o�ϐ����g���ƁAplayer��null�|�C���^�ŃG���[���łĂ��܂��B
	//if(GetWhitchCourse() == 11) { //intersect.cpp���ő��ϐ��Ƃ��Ē�`�����l���Q�Ƃ���
		//glutDisplayFunc(Display);
		//glutTimerFunc(0,Idle,0);
		//Init();

	//}

	//glutDisplayFunc�Ŏw�肷��R�[���o�b�N�֐��̒��ŁA����ɕ`��֐���I�Ԋ����ɂ���΁A��ʑJ�ڂ��ł���̂ł́H

	//�P�|�Q�̕\��
	//if (COURSE == COURSE_1_2) {
	//if (GetWhitchCourse() == 12) { //main���̓��[�v���Ă��Ȃ����߁A���ꂾ�Ƃ��܂������Ȃ�

		glutDisplayFunc(Display_1_2);
		glutTimerFunc(0, Idle_1_2, 0);
		Init_1_2();

	//}


	glutKeyboardFunc(Keyboard);
	//glutSpecialFunc(SpecialKey);
	glutKeyboardUpFunc(KeyboardUp);
	
	
	
	if (player->isInGround == false) {
		glClearColor(0.31372549, 0.50196078, 1.0, 1.0);  //(80,128, 255)��F
	}


	if (COMMAND_MODE == 2) {
		std::thread voiceRecognitionThread(voice_recognition); //�ǉ�---------------------------------------------------------------
		//voiceRecognitionThread.join();
	}


	glutMainLoop();
	
}
