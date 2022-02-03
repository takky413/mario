#pragma once

#include "glm\glm.hpp"
#include <vector>
#include <GL/glut.h>

//------------------------------------------
//マクロ定義
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
#define NOKONOKO_SHELL_HEIGHT PART_HEIGHT //踏まれて、甲羅状態になった時の高さ

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
#define COIN_BLOCK_NUM 1 //1 //実行エラーはここが原因！！
#define BLOCK_COIN_NUM 10 * COIN_BLOCK_NUM //これは、一つのブロックから出るコインの枚数を表す
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

//当たり判定の処理を行うときに引数として用いる
#define GOAL_INTERSECT 0
#define PLAYER_INTERSECT 1
#define PART_INTERSECT 2 //オブジェクトを除いたその他のパーツ
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


//コース番号
#define COURSE_1_1 1
#define COURSE_1_2 2

//１－２で、スタートの地上か、メインコースか、土管の下か、ゴールの地上かを判断する
#define START 1
#define MAIN 2
#define MIDDLE 3
#define GOAL 4

 
//------------------------
//1-2のデータ
//------------------------
#define BLOCK_NUM_1_2 0 //377
#define QUESTION_BOX_NUM_1_2 6
#define QUESTION_BOX_COIN_NUM_1_2 5
#define KURIBOO_NUM_1_2 14
#define NOKONOKO_NUM_1_2 2 
#define MUSHROOM_NUM_1_2 1
#define FIRE_FLOWER_NUM_1_2 1


#define VOICE_COMMAND_AMOUNT 3 //一回の音声指示で、何マス分進むかを決める

//-------------------------------------------
//列挙型、および構造体の定義
// ------------------------------------------


//テクスチャの種類を表す
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
	TEX_PIPE_0,//上向き
	TEX_PIPE_1,//上向き
	TEX_PIPE_2,//上向き
	TEX_PIPE_3,//上向き
	TEX_PIPE_4,//横向き
	TEX_PIPE_5,//横向き
	TEX_PIPE_6,//横向き
	TEX_PIPE_7,//横向き
	TEX_JOINT_PIPE_26,//曲がっている土管の接合部
	TEX_JOINT_PIPE_27,//曲がっている土管の接合部
	TEX_GROUND_PIPE_0,
	TEX_GROUND_PIPE_1,
	TEX_GROUND_PIPE_2,
	TEX_GROUND_PIPE_3,
	TEX_GROUND_PIPE_4,
	TEX_GROUND_PIPE_5,
	TEX_GROUND_PIPE_6,
	TEX_GROUND_PIPE_7,
	TEX_JOINT_GROUND_PIPE_26,//曲がっている土管の接合部
	TEX_JOINT_GROUND_PIPE_27,//曲がっている土管の接合部
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


//ユニティちゃんのアニメーションの種類を表す
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

//敵のアニメーションの種類を表す
enum {
	ENEMY_ANIMATION_STAND,
	ENEMY_ANIMATION_ATTACK,
	ENEMY_ANIMATION_MAX
};

//クリボーのアニメーションの種類を表す
enum {
	KURIBOO_ANIMATION_STAND,
	KURIBOO_ANIMATION_CRUSHED,
	KURIBOO_ANIMATION_MAX
};

//ノコノコのアニメーションの種類を表す
enum {
	NOKONOKO_ANIMATION_STAND,
	NOKONOKO_ANIMATION_MAX
};

//ファイヤフラワーのアニメーションの種類を表す
enum {
	FIRE_FLOWER_ANIMATION,
	FIRE_FLOWER_ANIMATION_MAX
};

//ファイヤボールのアニメーションの種類を表す
enum {
	FIRE_BALL_ANIMATION_BOUND,
	FIRE_BALL_ANIMATION_BURST,
	FIRE_BALL_ANIMATION_MAX
};


//テクスチャに関する構造体
typedef struct {
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	GLuint texture;
	glm::ivec2 rect;
}TEX;


//アニメーションに関する構造体
typedef struct {
	int count;
	std::vector<int> frames;
}ANIMATION;

//プレイヤーに関する構造体
typedef struct {
	int animation;
	int frame;
	int tmp_star_frame;
	int tmp_goal_frame;
	glm::vec2 position;
	glm::vec2 tmp1_pos; //現在の位置を保存する(土管用)
	glm::vec2 tmp2_pos; //現在の位置を保存する(土管用)
	glm::vec2 voice_old_pos; //音声認識で操作する際に使う、位置を保存する
	glm::vec2 velocity;
	glm::vec2 left;
	glm::vec2 right;
	glm::vec2 top;
	glm::vec2 bottom;
	float height_at_goal; //プレイヤーがゴールした時のy座標、ゴール時のプレイヤーの高さ設定に用いる
	int current_fire_ball; //リアルタイムでの描画中のファイアボールの数
	bool isMirror;
	bool isJumping;
	bool isSquating; //しゃがんでいるかどうか
	int isRunning; //ダッシュしているかどうか。ダッシュしていないときは1、ダッシュしているときは2;
	int isBig;
	bool isFire;
	bool isStar;
	bool inPipe; //土管の上でしゃがみボタンを押したかどうか。一度押したらずっとフラグは立ったままにする
	bool isInGround; //地下にいるかどうか
	bool isScroll; //スクロールするかどうか。マリオの描画のところで使う
	int course; //１－２で、スタートの地上か、メインコースか、土管の下か、ゴールの地上かを判断する
	int fire_ball_counter; //ファイアボールを出した個数を制御するためのカウンタ
	bool isGoal; //ゴールポールに触れたかどうか
	glm::vec2 flag_pos; //ゴールの旗の位置（複数のファイルで参照したいので、プレイヤーのインスタンスに持たせる）
	//int whitch_course; //今どのコースをプレイしているかを表す
}PLAYER;

//コインに関する構造体
typedef struct {
	int frame;
	int number;
	bool flag; //表示するかしないかののフラグ
	bool sub_flag; //一度とったコインのフラグを固定するためのサブフラグ
	bool sound_flag; //一度とったコインのサウンドを鳴らさないためのサブフラグ
	glm::vec2 delta_pos;
	glm::vec2 velocity;
	//レンガブロックから出るコインに関するパラメータ。一つのブロックから複数のコインが出るため、ベクター型で再定義しておく
	bool isMax;
	std::vector<int> vector_frame;
	std::vector<bool> vector_flag; 
	std::vector<bool> vector_sub_flag; 
	std::vector<bool> vector_sound_flag; 
	std::vector <glm::vec2> vector_delta_pos;
	std::vector <glm::vec2> vector_velocity;
}COIN;

//クエスチョンボックスに関する構造体
typedef struct {
	int frame;
	int number;
	bool flag; //たたいたかどうかのフラグ
	bool sound_flag;
}QUESTION;

//ブロックに関する構造体
typedef struct {
	int frame;
	int number;
	glm::vec2 delta_position; //ブロックの位置の変化量
	glm::vec2 fring_delta_position_1; //破片の位置の変化
	glm::vec2 fring_delta_position_2; //破片の位置の変化
	glm::vec2 fring_delta_position_3; //破片の位置の変化
	glm::vec2 fring_delta_position_4; //破片の位置の変化
	bool flag; //存在するかどうかのフラグ
	bool sound_flag;
	bool animation_flag;
	bool reach_top_flag;
	bool fring_calculate_flag; //ブロックを破壊した後の破片の座標を計算するかしないかのフラグ
	int haveCoinNum; //たたくとコインが出るパターンのレンガブロックに用いる
}BLOCK;

//敵に関する構造体
typedef struct {
	int animation;
	int frame;
	double frame_at_stepped_on; //プレイヤーに踏まれた時のフレームを記録する
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 left;
	glm::vec2 right;
	glm::vec2 top;
	glm::vec2 bottom;
	glm::vec2 bottom_left;
	glm::vec2 bottom_right;
	glm::vec2 delta_position; //敵が死んだときのひっくり返って落ちていくアニメーションを行うための、位置の変化量
	double width;
	double height;
	bool isMirror;
	bool touch_left_flag; //甲羅状態のノコノコに対して、どの部分に触れたかを表す
	bool touch_right_flag;
	bool touch_top_flag;
	bool isShellMoving; //甲羅状態のノコノコが動いているかどうかを表す
	bool intersect_left_flag; //敵に対してプレイヤーがどの位置にいるかを表すフラグ
	bool intersect_right_flag;
	bool intersect_top_flag;
	bool intersect_bottom_flag;
	bool stepped_on_flag; //キャラクターに踏まれたかを表すフラグ
	bool isDeath;
	bool animation_flag;
	bool display_flag; //表示するかどうかのフラグ
	bool delta_psition_calculate_flag; //ひっくり返って落ちていくときの座標を計算するかしないかのフラグ
}ENEMY;

//アイテムに関する構造体
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
	bool get_flag; //アイテムを取ったかどうかのフラグ
	bool display_flag; //表示するかどうかのフラグ
}ITEM;

//ファイアボールに関する構造体
typedef struct {
	int animation;
	int frame;
	int frame_at_attack; //敵と干渉した時のフレームを保持する
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
	bool isLookingLeft; //プレイヤーが左を向いているかどうか（isMirrirで判定すると、初期化がうまくいかないため）
	bool isAttackEnemy; //敵と干渉したかどうかを表す。アニメーションや位置の制御で用いる
	bool display_flag; //表示するかどうかのフラグ
}FIRE_BALL;



//リフトに関する構造体
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
