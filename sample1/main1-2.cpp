#include <stdio.h>
#include <vector>
#include <GL/glut.h>
#include <AL\alut.h>
#include "glm\glm.hpp"
#include "font.h"
#include "audio.h"
#include "course.h"
#include "header.h"
#include "instance.h"
#include "intersect.h"


//メモ
//ここで定義した関数をmain関数で使うため、基本的にはすべて関数の引数の中に入れる
// 

//----------------------------------------
//(破壊できる)ブロックを描画する関数
//----------------------------------------
void DrawBlock_1_2(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, std::vector<BLOCK*> _block, int _index) {

	if (_c == 'b') { //ブロッかどうか
		if (_sub_c == '2') { //破壊できる種類か
			if (LetterToNum(_sub2_c) == _index + 1) {
				if (_block[_index]->flag == true) {
					DrawSprite(TEX_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position + _block[_index]->delta_position);
				}
			}
		}
	}
}


//----------------------------------------
//クエスチョンボックスを描画する関数
//----------------------------------------
void DrawQuestionBox_1_2(glm::vec2 _position, char _c, char _sub2_c, std::vector<QUESTION*> _question, ANIMATION _question_box_animations, int _index) {

	if (_c == 'q') {
		if (LetterToNum(_sub2_c) == _index + 1) {
			if (_question[_index]->flag == true) {
				DrawSprite(TEX_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, _question_box_animations.frames[(_question[_index]->frame / 45) % _question_box_animations.count], _position);
			}
			else {
				DrawSprite(TEX_EMPTY_QUESTION_BOX, PART_WIDTH, PART_HEIGHT, 0, _position);
			}
		}
	}
}


//--------------------------------
//コースパーツを描画する関数
//--------------------------------
void DrawPart_1_2(glm::vec2 _position, char _c, char _sub_c, char _sub2_c,
	std::vector<BLOCK*> _block, std::vector<QUESTION*> _question, ANIMATION _question_box_animations) {

	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	
	glClearColor(0.0, 0.0, 0.0, 1.0);

	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return;
	}

	if (_c == 'b') {
		switch (_sub_c) {
		case '0': DrawSprite(TEX_BLOCK_0, PART_WIDTH, PART_HEIGHT, 0, _position); break;
		case '1': DrawSprite(TEX_BLOCK_1, PART_WIDTH, PART_HEIGHT, 0, _position); break;
		case '2':
			//１－２はレンガブロックが377個もあり、重すぎるので、いったん区別せずに描画する
			/*
			for (int i = 0; i < BLOCK_NUM_1_2; i++) {
				DrawBlock_1_2(_position, _c, _sub_c, _sub2_c, _block, i);
			}
			*/
			DrawSprite(TEX_BLOCK_2, PART_WIDTH, PART_HEIGHT, 0, _position); break;
			break;
		default: break;
		}
	}
	else if (_c == 'q') {
		for (int i = 0; i < QUESTION_BOX_NUM_1_2; i++) {
			DrawQuestionBox_1_2(_position, _c, _sub2_c, _question, _question_box_animations, i);
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
