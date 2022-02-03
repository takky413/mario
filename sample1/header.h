#pragma once

#include "glm\glm.hpp"
#include <vector>
#include <GL/glut.h>

//------------------------------------------
//�}�N����`
//------------------------------------------

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

#define COURSE_WIDTH 512 //256
#define COURSE_HEIGHT 16

#define GROUND_COURSE_WIDTH 17
#define GROUND_COURSE_HEIGHT 17

#define START_COURSE_WIDTH_1_2 17
#define START_COURSE_HEIGHT_1_2 17

#define PART_WIDTH (WINDOW_WIDTH / 16) //45
#define PART_HEIGHT (WINDOW_HEIGHT / 16) //45

#define BREAKED_BLOCK_WIDTH (4 * PART_WIDTH / 5)
#define BREAKED_BLOCK_HEIGHT (4 *PART_HEIGHT / 5)

#define PLAYER_WIDTH PART_WIDTH
#define PLAYER_HEIGHT PART_HEIGHT

#define ENEMY_WIDTH PART_WIDTH
#define ENEMY_HEIGHT PART_HEIGHT

#define KURIBOO_WIDTH PART_WIDTH
#define KURIBOO_HEIGHT PART_HEIGHT

#define NOKONOKO_WIDTH PART_WIDTH
#define NOKONOKO_HEIGHT PART_HEIGHT * 1.5
#define NOKONOKO_SHELL_WIDTH PART_WIDTH
#define NOKONOKO_SHELL_HEIGHT PART_HEIGHT //���܂�āA�b����ԂɂȂ������̍���

#define ITEM_WIDTH PART_WIDTH
#define ITEM_HEIGHT PART_HEIGHT

#define LIFT_WIDTH 66 * (45.0f/16.0f)
#define LIFT_HEIGHT 10 * (45.0f/16.0f)

#define MUSIC_STAGE 1
#define SE_GET_COIN 2
#define SE_BLOCK_BREAK 3
#define SE_BLOCK_HIT 4
#define SE_STEP_ON 5
#define SE_GET_MUSHROOM 6
#define VOICE_JUMP_1 7
#define VOICE_JUMP_2 8
#define SE_CLEAR 9
#define SE_FIRE_BALL 10
#define SE_ENEMY_DEATH 11
#define SE_IN_PIPE 12
#define SE_1UP 13
#define MUSIC_STAR 14
#define MUSIC_UNDER_GROUND_STAGE 15

#define FIELD_COIN_NUM 11
#define GROUND_FIELD_COIN_NUM 19
#define QUESTION_BOX_COIN_NUM 14 //+ 1
#define QUESTION_BOX_NUM 14 + 1 //+ 1
#define BLOCK_NUM 30 - 1 - 1//+ 2
#define COIN_BLOCK_NUM 1 //1 //���s�G���[�͂����������I�I
#define BLOCK_COIN_NUM 10 * COIN_BLOCK_NUM //����́A��̃u���b�N����o��R�C���̖�����\��
#define ENEMY_NUM 3
#define KURIBOO_NUM 15 //+ 1
#define NOKONOKO_NUM 2 //+ 1
#define MUSHROOM_NUM 1
#define MUSHROOM_1UP_NUM 1
#define FIRE_FLOWER_NUM 1
#define FIRE_BALL_NUM 10
#define STAR_NUM 1
#define LIFT_NUM 4
#define UP_LIFT_NUM 2
#define DOWN_LIFT_NUM 2

#define GROUND_BLOCK_NUM 39// 89

#define LEFT 1
#define RIGHT 2
#define TOP 3
#define BOTTOM 4

//�����蔻��̏������s���Ƃ��Ɉ����Ƃ��ėp����
#define GOAL_INTERSECT 0
#define PLAYER_INTERSECT 1
#define PART_INTERSECT 2 //�I�u�W�F�N�g�����������̑��̃p�[�c
#define BREAKABLE_BLOCK_INTERSECT 3
#define ENEMY_INTERSECT 4
#define KURIBOO_INTERSECT 5
#define MUSHROOM_INTERSECT 6
#define MUSHROOM_1UP_INTERSECT 7
#define FIRE_FLOWER_INTERSECT 8
#define FIRE_BALL_INTERSECT 9
#define NOKONOKO_INTERSECT 10
#define STAR_INTERSECT 11
#define COIN_BLOCK_INTERSECT 12
#define LIFT_INTERSECT 13


//�R�[�X�ԍ�
#define COURSE_1_1 1
#define COURSE_1_2 2

//�P�|�Q�ŁA�X�^�[�g�̒n�ォ�A���C���R�[�X���A�y�ǂ̉����A�S�[���̒n�ォ�𔻒f����
#define START 1
#define MAIN 2
#define MIDDLE 3
#define GOAL 4

 
//------------------------
//1-2�̃f�[�^
//------------------------
#define BLOCK_NUM_1_2 0 //377
#define QUESTION_BOX_NUM_1_2 6
#define QUESTION_BOX_COIN_NUM_1_2 5
#define KURIBOO_NUM_1_2 14
#define NOKONOKO_NUM_1_2 2 
#define MUSHROOM_NUM_1_2 1
#define FIRE_FLOWER_NUM_1_2 1


#define VOICE_COMMAND_AMOUNT 3 //���̉����w���ŁA���}�X���i�ނ������߂�

//-------------------------------------------
//�񋓌^�A����э\���̂̒�`
// ------------------------------------------


//�e�N�X�`���̎�ނ�\��
enum {
	TEX_MARIO,
	TEX_MARIO_BIG,
	TEX_MARIO_FIRE,
	TEX_MARIO_STAR,
	TEX_MARIO_STAR_BIG,
	TEX_UNITY_CHAN,
	TEX_UNITY_CHAN_FIRE,
	TEX_LUIGI,
	TEX_KINOPIO,
	TEX_MARIO_CAR,
	TEX_EMPTY,
	TEX_BLOCK,
	TEX_BLOCK_0,
	TEX_BLOCK_1,
	TEX_BLOCK_2,
	TEX_GROUND_BLOCK_0,
	TEX_GROUND_BLOCK_1,
	TEX_GROUND_BLOCK_2,
	TEX_BREAKED_BLOCK,
	TEX_PIPE_0,//�����
	TEX_PIPE_1,//�����
	TEX_PIPE_2,//�����
	TEX_PIPE_3,//�����
	TEX_PIPE_4,//������
	TEX_PIPE_5,//������
	TEX_PIPE_6,//������
	TEX_PIPE_7,//������
	TEX_JOINT_PIPE_26,//�Ȃ����Ă���y�ǂ̐ڍ���
	TEX_JOINT_PIPE_27,//�Ȃ����Ă���y�ǂ̐ڍ���
	TEX_GROUND_PIPE_0,
	TEX_GROUND_PIPE_1,
	TEX_GROUND_PIPE_2,
	TEX_GROUND_PIPE_3,
	TEX_GROUND_PIPE_4,
	TEX_GROUND_PIPE_5,
	TEX_GROUND_PIPE_6,
	TEX_GROUND_PIPE_7,
	TEX_JOINT_GROUND_PIPE_26,//�Ȃ����Ă���y�ǂ̐ڍ���
	TEX_JOINT_GROUND_PIPE_27,//�Ȃ����Ă���y�ǂ̐ڍ���
	TEX_GOAL_0,
	TEX_GOAL_1,
	TEX_FORT_0,
	TEX_FORT_1,
	TEX_FORT_2,
	TEX_FORT_3,
	TEX_FORT_4,
	TEX_FORT_5,
	TEX_FORT_6,
	TEX_TREE_0,
	TEX_TREE_1,
	TEX_TREE_2,
	TEX_CLOUD_0,
	TEX_CLOUD_1,
	TEX_CLOUD_2,
	TEX_CLOUD_3,
	TEX_CLOUD_4,
	TEX_CLOUD_5,
	TEX_MOUNTAIN_0,
	TEX_MOUNTAIN_1,
	TEX_MOUNTAIN_2,
	TEX_MOUNTAIN_3,
	TEX_MOUNTAIN_4,
	TEX_MOUNTAIN_5,
	TEX_COIN,
	TEX_QUESTION_BOX,
	TEX_EMPTY_QUESTION_BOX,
	TEX_EMPTY_GROUND_QUESTION_BOX,
	TEX_ENEMY,
	TEX_MUSHROOM,
	TEX_MUSHROOM_1UP,
	TEX_FIRE_FLOWER,
	TEX_FIRE_BALL,
	TEX_STAR,
	TEX_KURIBOO,
	TEX_GROUND_KURIBOO,
	TEX_NOKONOKO,
	TEX_GROUND_NOKONOKO,
	TEX_NOKONOKO_SHELL,
	TEX_KUPPA,
	TEX_FLAG,
	TEX_SURPRISE,
	TEX_LIFT,
	TEX_MAX
};


//���j�e�B�����̃A�j���[�V�����̎�ނ�\��
enum {
	ANIMATION_STAND,
	ANIMATION_DAMAGE,
	ANIMATION_RUN,
	ANIMATION_JUMP,
	ANIMATION_TURN,
	ANIMATION_SQUAT,
	ANIMATION_GOAL,
	ANIMATION_STAR_STAND,
	ANIMATION_STAR_DAMAGE,
	ANIMATION_STAR_RUN,
	ANIMATION_STAR_JUMP,
	ANIMATION_STAR_TURN,
	ANIMATION_STAR_SQUAT,
	ANIMATION_MAX
};

//�G�̃A�j���[�V�����̎�ނ�\��
enum {
	ENEMY_ANIMATION_STAND,
	ENEMY_ANIMATION_ATTACK,
	ENEMY_ANIMATION_MAX
};

//�N���{�[�̃A�j���[�V�����̎�ނ�\��
enum {
	KURIBOO_ANIMATION_STAND,
	KURIBOO_ANIMATION_CRUSHED,
	KURIBOO_ANIMATION_MAX
};

//�m�R�m�R�̃A�j���[�V�����̎�ނ�\��
enum {
	NOKONOKO_ANIMATION_STAND,
	NOKONOKO_ANIMATION_MAX
};

//�t�@�C���t�����[�̃A�j���[�V�����̎�ނ�\��
enum {
	FIRE_FLOWER_ANIMATION,
	FIRE_FLOWER_ANIMATION_MAX
};

//�t�@�C���{�[���̃A�j���[�V�����̎�ނ�\��
enum {
	FIRE_BALL_ANIMATION_BOUND,
	FIRE_BALL_ANIMATION_BURST,
	FIRE_BALL_ANIMATION_MAX
};


//�e�N�X�`���Ɋւ���\����
typedef struct {
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	GLuint texture;
	glm::ivec2 rect;
}TEX;


//�A�j���[�V�����Ɋւ���\����
typedef struct {
	int count;
	std::vector<int> frames;
}ANIMATION;

//�v���C���[�Ɋւ���\����
typedef struct {
	int animation;
	int frame;
	int tmp_star_frame;
	int tmp_goal_frame;
	glm::vec2 position;
	glm::vec2 tmp1_pos; //���݂̈ʒu��ۑ�����(�y�Ǘp)
	glm::vec2 tmp2_pos; //���݂̈ʒu��ۑ�����(�y�Ǘp)
	glm::vec2 voice_old_pos; //�����F���ő��삷��ۂɎg���A�ʒu��ۑ�����
	glm::vec2 velocity;
	glm::vec2 left;
	glm::vec2 right;
	glm::vec2 top;
	glm::vec2 bottom;
	float height_at_goal; //�v���C���[���S�[����������y���W�A�S�[�����̃v���C���[�̍����ݒ�ɗp����
	int current_fire_ball; //���A���^�C���ł̕`�撆�̃t�@�C�A�{�[���̐�
	bool isMirror;
	bool isJumping;
	bool isSquating; //���Ⴊ��ł��邩�ǂ���
	int isRunning; //�_�b�V�����Ă��邩�ǂ����B�_�b�V�����Ă��Ȃ��Ƃ���1�A�_�b�V�����Ă���Ƃ���2;
	int isBig;
	bool isFire;
	bool isStar;
	bool inPipe; //�y�ǂ̏�ł��Ⴊ�݃{�^�������������ǂ����B��x�������炸���ƃt���O�͗������܂܂ɂ���
	bool isInGround; //�n���ɂ��邩�ǂ���
	bool isScroll; //�X�N���[�����邩�ǂ����B�}���I�̕`��̂Ƃ���Ŏg��
	int course; //�P�|�Q�ŁA�X�^�[�g�̒n�ォ�A���C���R�[�X���A�y�ǂ̉����A�S�[���̒n�ォ�𔻒f����
	int fire_ball_counter; //�t�@�C�A�{�[�����o�������𐧌䂷�邽�߂̃J�E���^
	bool isGoal; //�S�[���|�[���ɐG�ꂽ���ǂ���
	glm::vec2 flag_pos; //�S�[���̊��̈ʒu�i�����̃t�@�C���ŎQ�Ƃ������̂ŁA�v���C���[�̃C���X�^���X�Ɏ�������j
	//int whitch_course; //���ǂ̃R�[�X���v���C���Ă��邩��\��
}PLAYER;

//�R�C���Ɋւ���\����
typedef struct {
	int frame;
	int number;
	bool flag; //�\�����邩���Ȃ����̂̃t���O
	bool sub_flag; //��x�Ƃ����R�C���̃t���O���Œ肷�邽�߂̃T�u�t���O
	bool sound_flag; //��x�Ƃ����R�C���̃T�E���h��炳�Ȃ����߂̃T�u�t���O
	glm::vec2 delta_pos;
	glm::vec2 velocity;
	//�����K�u���b�N����o��R�C���Ɋւ���p�����[�^�B��̃u���b�N���畡���̃R�C�����o�邽�߁A�x�N�^�[�^�ōĒ�`���Ă���
	bool isMax;
	std::vector<int> vector_frame;
	std::vector<bool> vector_flag; 
	std::vector<bool> vector_sub_flag; 
	std::vector<bool> vector_sound_flag; 
	std::vector <glm::vec2> vector_delta_pos;
	std::vector <glm::vec2> vector_velocity;
}COIN;

//�N�G�X�`�����{�b�N�X�Ɋւ���\����
typedef struct {
	int frame;
	int number;
	bool flag; //�����������ǂ����̃t���O
	bool sound_flag;
}QUESTION;

//�u���b�N�Ɋւ���\����
typedef struct {
	int frame;
	int number;
	glm::vec2 delta_position; //�u���b�N�̈ʒu�̕ω���
	glm::vec2 fring_delta_position_1; //�j�Ђ̈ʒu�̕ω�
	glm::vec2 fring_delta_position_2; //�j�Ђ̈ʒu�̕ω�
	glm::vec2 fring_delta_position_3; //�j�Ђ̈ʒu�̕ω�
	glm::vec2 fring_delta_position_4; //�j�Ђ̈ʒu�̕ω�
	bool flag; //���݂��邩�ǂ����̃t���O
	bool sound_flag;
	bool animation_flag;
	bool reach_top_flag;
	bool fring_calculate_flag; //�u���b�N��j�󂵂���̔j�Ђ̍��W���v�Z���邩���Ȃ����̃t���O
	int haveCoinNum; //�������ƃR�C�����o��p�^�[���̃����K�u���b�N�ɗp����
}BLOCK;

//�G�Ɋւ���\����
typedef struct {
	int animation;
	int frame;
	double frame_at_stepped_on; //�v���C���[�ɓ��܂ꂽ���̃t���[�����L�^����
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 left;
	glm::vec2 right;
	glm::vec2 top;
	glm::vec2 bottom;
	glm::vec2 bottom_left;
	glm::vec2 bottom_right;
	glm::vec2 delta_position; //�G�����񂾂Ƃ��̂Ђ�����Ԃ��ė����Ă����A�j���[�V�������s�����߂́A�ʒu�̕ω���
	double width;
	double height;
	bool isMirror;
	bool touch_left_flag; //�b����Ԃ̃m�R�m�R�ɑ΂��āA�ǂ̕����ɐG�ꂽ����\��
	bool touch_right_flag;
	bool touch_top_flag;
	bool isShellMoving; //�b����Ԃ̃m�R�m�R�������Ă��邩�ǂ�����\��
	bool intersect_left_flag; //�G�ɑ΂��ăv���C���[���ǂ̈ʒu�ɂ��邩��\���t���O
	bool intersect_right_flag;
	bool intersect_top_flag;
	bool intersect_bottom_flag;
	bool stepped_on_flag; //�L�����N�^�[�ɓ��܂ꂽ����\���t���O
	bool isDeath;
	bool animation_flag;
	bool display_flag; //�\�����邩�ǂ����̃t���O
	bool delta_psition_calculate_flag; //�Ђ�����Ԃ��ė����Ă����Ƃ��̍��W���v�Z���邩���Ȃ����̃t���O
}ENEMY;

//�A�C�e���Ɋւ���\����
typedef struct {
	int animation;
	int frame;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 left;
	glm::vec2 right;
	glm::vec2 top;
	glm::vec2 bottom;
	glm::vec2 bottom_left;
	glm::vec2 bottom_right;
	bool intersect_left_flag;
	bool intersect_right_flag;
	bool intersect_top_flag;
	bool intersect_bottom_flag;
	bool isMirror;
	bool get_flag; //�A�C�e������������ǂ����̃t���O
	bool display_flag; //�\�����邩�ǂ����̃t���O
}ITEM;

//�t�@�C�A�{�[���Ɋւ���\����
typedef struct {
	int animation;
	int frame;
	int frame_at_attack; //�G�Ɗ��������̃t���[����ێ�����
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 left;
	glm::vec2 right;
	glm::vec2 top;
	glm::vec2 bottom;
	glm::vec2 bottom_left;
	glm::vec2 bottom_right;
	bool intersect_left_flag;
	bool intersect_right_flag;
	bool intersect_top_flag;
	bool intersect_bottom_flag;
	bool isMirror;
	bool isLookingLeft; //�v���C���[�����������Ă��邩�ǂ����iisMirrir�Ŕ��肷��ƁA�����������܂������Ȃ����߁j
	bool isAttackEnemy; //�G�Ɗ��������ǂ�����\���B�A�j���[�V������ʒu�̐���ŗp����
	bool display_flag; //�\�����邩�ǂ����̃t���O
}FIRE_BALL;



//���t�g�Ɋւ���\����
typedef struct {
	int animation;
	int frame;
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 top;
	bool intersect_top_flag;
}LIFT;






int LetterToNum(char _c);

void TexFromFile(const char* _FileName, int _tex, glm::ivec2 _rect);

void DrawSprite(int _tex, float _width, float _height, int _index, glm::vec2 _position, bool _isMirror = false);
