#include <vector>
#include "glm\glm.hpp"
#include "course.h"
#include "header.h"
#include "audio.h"
#include "instance.h"

//main.cpp��������C���N���[�h���邽�߁A�����瑤�ɑ��ϐ��Ƃ��Ē�`���Ă���
//�S�[���|�[���Ƃ̓����蔻����s�����ۂɕύX����
int whitch_course = 11; //�ǂ̃R�[�X���v���C���Ă��邩�����߂�ϐ�

void SetWhitchCourse(int _whitch_course) { //�Z�b�^�[
	whitch_course = _whitch_course;
}

int GetWhitchCourse() { //�Q�b�^�[
	return whitch_course;
}


//------------------------------------------------------
//�t�B�[���h�̃I�u�W�F�N�g�Ɗ����Ă��邩���ׂ�֐�
//------------------------------------------------------
bool IntersectField(glm::vec2 _position, PLAYER* _player) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_player->isInGround == false) { //�n��
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return false;
		}
		switch (course[y][x]) {
		case 'b':
			if (sub_course[y][x] == '0' || sub_course[y][x] == '1') {
				//_player->inPipe = false; //�y�ǂɓ�������ɒn�ʂɂ�����A�y�ǂɓ��蒆�ł��邱�ƕ\���t���O�����낷
				return true;
			}
			break;
		case 'p':
			if (_player->inPipe == true) { //�y�ǂ̏�ł��Ⴊ�񂾎��̓����蔻�������
				//if (sub_course[y][x] == '0' || sub_course[y][x] == '1') {
				if (sub2_course[y][x] == 'i') { //�v�C���@���̂܂܂��ƁA�}���I���y�ǂɓ����Ă���Œ��́A�G���y�ǂ����蔲���Ă��܂�
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
	
	else if (_player->isInGround == true) { //�n��
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
			if (_player->inPipe == true) { //�y�ǂ̏�ł��Ⴊ�񂾎��̓����蔻�������
				if (ground_sub2_course[y][x] == 'i') { //�v�C���@���̂܂܂��ƁA�}���I���y�ǂɓ����Ă���Œ��́A�G���y�ǂ����蔲���Ă��܂�
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
//�����y�ǂƊ����Ă��邩���ׂ�֐�
//�������ɂ͓����蔻����s���ӏ��ileft,right,top,botttom�̂ǂꂩ������j
//--------------------------------------------------------------------------
bool IntersectPipe(glm::vec2 _position, int _intersect_poit, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) {
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return false;
		}
		if (course[y][x] == 'p') { //�y�ǂ�
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
		if (ground_course[y][x] == 'p') { //�y�ǂ�
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
//�ǂ́i�j��ł���j�u���b�N�Ɗ����Ă��邩���ׂ�֐��A�����Ă��Ȃ�������-1��Ԃ�
//---------------------------------------------------------------------------------
int IntersectBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) { //�n��
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return -1;
		}
		if (_block[_index]->flag == true) {
			if (course[y][x] == 'b') {
				if (sub_course[y][x] == '2') {  //'3'�̓R�C���u���b�N
					if (LetterToNum(sub2_course[y][x]) == _index + 1) {
						return _index;
					}
				}
			}
		}
		return -1;
	}
	
	else if (_isInGround == true) { //�n��
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
//�ǂ̃R�C���u���b�N�Ɗ����Ă��邩���ׂ�֐��A�����Ă��Ȃ�������-1��Ԃ�
//---------------------------------------------------------------------------------
int IntersectCoinBlock(glm::vec2 _position, int _index, std::vector<BLOCK*> _block, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) { //�n��
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return -1;
		}
		//if (_block[_index]->flag == true) { �R�C���u���b�N�͔j�󂳂�邱�Ƃ��Ȃ��A�����蔻�肪�����邱�Ƃ͂Ȃ�
			if (course[y][x] == 'b') {
				if (sub_course[y][x] == '3') {  //'3'�̓R�C���u���b�N
					if (LetterToNum(sub2_course[y][x]) == _index + 1) {
						return _index;
					}
				}
			}
		//}
		return -1;
	}
	else if (_isInGround == true) { //�n��
	}

	return -1;
}



//-------------------------------------------------------------------------------------
//�w�肵����́i�j��ł���j�u���b�N�Ɗ����Ă��邩���ׂ�֐�(�A�C�e���ƃG�l�~�[�p�j
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
//�w�肵���G�l�~�[���A�w�肵���i�j��ł���j�u���b�N�Ɗ����Ă��邩���A�I�u�W�F�N�g�̑g���킹���w�肵�Ē��ׂāA�����蔻����s���֐�
//�u���b�N�̔ԍ����w�肷�邽�߂̈���_block_index���K�v������
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
//�w�肵���A�C�e�����A�w�肵���i�j��ł���j�u���b�N�Ɗ����Ă��邩���A�I�u�W�F�N�g�̑g���킹���w�肵�Ē��ׂāA�����蔻����s���֐�
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
//�w�肵���t�@�C�A�{�[�����A�w�肵���i�j��ł���j�u���b�N�Ɗ����Ă��邩���A�I�u�W�F�N�g�̑g���킹���w�肵�Ē��ׂāA�����蔻����s���֐�
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
//�N�G�X�`�����{�b�N�X����o���R�C���Ɗ����Ă��邩�A�܂������Ă���Ȃ�ǂ̃R�C�������ׂ�֐�
//--------------------------------------------------------------------------------------------------
int IntersectCoin(glm::vec2 _position, int _index) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;
	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return -1;
	}
	if (sub_course[y][x] == 'C') {
		if (LetterToNum(sub_course[y - 1][x]) == _index + 1) {
			return _index; //�v�C�� return _index+1 �ł����������ɂȂ���Ăǂ��������ƁH�H
		}
	}
	return -1;
}



//-----------------------------------------------------------------------------------
//�t�B�[���h��̃R�C���Ɗ����Ă��邩�A�܂������Ă���Ȃ�ǂ̃R�C�������ׂ�֐�
//-----------------------------------------------------------------------------------
int IntersectFieldCoin(glm::vec2 _position, int _index, bool _isInGround) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_isInGround == false) { //�n��
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
	else if (_isInGround == true) { //�n��
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
//�ǂ̃N�G�X�`�����{�b�N�X�������������ǂ������ׂ�֐��A�������ĂȂ�������-1��Ԃ�
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
//�ǂ̃u���b�N�������������ǂ������ׂ�֐��A�������ĂȂ�������-1��Ԃ�
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
//�ǂ̃R�C���u���b�N�������������ǂ������ׂ�֐�
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
//�G�Ƃ̓����蔻��ɂ����āA�G�̍��E�㉺�̂ǂ��������������𒲂ׂ�֐�
//���ۂ͓G�̍��E�㉺�ǂ̈ʒu�ɂ��邩��Ԃ��Ă���
//�N���{�[�Ȃǂ̑��̓G�ɂ��K���\�ɂ��Ă���
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
	else if (_position.y + (_player.isBig + 1) * PLAYER_HEIGHT <= _enemy[_index]->position.y) { //_player.isBig��+1���Ă����Ȃ��ƁA�傫����Ԃŗ����Ă��邾���œG�𓥂�ł��܂�
		_enemy[_index]->intersect_left_flag = false;
		_enemy[_index]->intersect_right_flag = false;
		_enemy[_index]->intersect_top_flag = true;
		_enemy[_index]->intersect_bottom_flag = false;
	}
	//�u�G�̉��Ɉʒu���Ă���v�Ɣ��肷��͈͂����߂Ă����Ȃ��ƁA�u���b�N����i����Ƃ��ɁA���̒i�̃u���b�N�������������ɂ��A��̒i�ɂ���G������ł��܂�
	else if (_position.y >= _enemy[_index]->position.y + _player.isBig * PLAYER_HEIGHT && _position.y <= _enemy[_index]->position.y + _player.isBig * PLAYER_HEIGHT + PART_HEIGHT*2) {
		_enemy[_index]->intersect_left_flag = false;
		_enemy[_index]->intersect_right_flag = false;
		_enemy[_index]->intersect_top_flag = false;
		_enemy[_index]->intersect_bottom_flag = true;
	}
}


//----------------------------------------------------------------------------------------------
//�v���C���[�̓G�Ƃ̓����蔻��𒲂ׂ�֐��A�������Ă���Ȃ�ǂ̓G����Ԃ�
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
//�G���m�̓����蔻��ɂ����āA�G�̍��E�㉺�̂ǂ̈ʒu�ɂ��邩�𒲂ׂ�֐�
//_enemy1����Ƃ��āA_enemy2�ɑ΂��Ăǂ̈ʒu�ɂ��邩��Ԃ�
//-----------------------------------------------------------------------------------
int SearchIntersectEnemyPointToEnemy(ENEMY* _enemy1, ENEMY* _enemy2) {
	//�ʒu�֌W�ύX�̋��E���A�G�̎��͂��ƁA�G���d�Ȃ��Ă���Ƃ��ɂ��܂���������Ȃ����߁A���S���W�̊֌W��Ԃ����Ƃɂ���
	if (_enemy1->position.x + _enemy1->width / 2.0f <= _enemy2->position.x) { //_enemy1��_enemy2�������ɂ��鎞
		return LEFT;
	}
	else if (_enemy1->position.x >= _enemy2->position.x + _enemy2->width / 2.0f) { //_enemy1��_enemy2�����E�ɂ��鎞
		return RIGHT;
	}
	else if (_enemy1->position.y + _enemy1->height / 2.0f <= _enemy2->position.y) { //_enemy1��_enemy2������ɂ��鎞
		return TOP;
	}
	else if (_enemy1->position.y >= _enemy2->position.y + _enemy1->height / 2.0f) { //_enemy1��_enemy2�������ɂ��鎞
		return BOTTOM;
	}
}


//----------------------------------------------------------------------------------------------
//�G���m�̓����蔻��𒲂ׂ�֐��A�������Ă���Ȃ�ǂ̓G����Ԃ�
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
//�G���m�̓����蔻�菈�����s���֐�
//----------------------------------------------------------------------------------------------
void  DoIntersectEnemyToEnemy(ENEMY* _enemy1, ENEMY* _enemy2, int _obj1, int _obj2) {
	switch (_obj1) {

	case KURIBOO_INTERSECT :

		//�N���{�[���m�̊���
		if (_obj2 == KURIBOO_INTERSECT) {
			if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == LEFT) { //_enemy2�̍���_enmey1������ꍇ
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

		//�N���{�[�ƃm�R�m�R�̊���
		if (_obj2 == NOKONOKO_INTERSECT) {
			//�ʏ�̏�Ԃ̃m�R�m�R�A�܂��́A�~�܂��Ă���b���Ɗ������Ƃ�
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
			else if (_enemy2->stepped_on_flag == true && _enemy2->isShellMoving == true) { //�����Ă���b����Ԃ̃m�R�m�R�Ɗ�������
				if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == LEFT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						if (_enemy1->isDeath == false) {
							audioPlay(SE_ENEMY_DEATH);
						}
						_enemy1->isDeath = true; //�N���{�[������
					}
				}
				else if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == RIGHT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						if (_enemy1->isDeath == false) {
							audioPlay(SE_ENEMY_DEATH);
						}
						_enemy1->isDeath = true; //�N���{�[������
					}
				}
			}
		}

		break;

	case NOKONOKO_INTERSECT :

		//�m�R�m�R�ƃN���{�[�̊���
		if (_obj2 == KURIBOO_INTERSECT) {
			//�ʏ�̏�Ԃ̃m�R�m�R�A�܂��́A�~�܂��Ă���b���Ɗ������Ƃ�
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

			else if (_enemy1->stepped_on_flag == true && _enemy1->isShellMoving == true) { //�����Ă���b����Ԃ̃m�R�m�R�Ɗ�������
				if (SearchIntersectEnemyPointToEnemy(_enemy1, _enemy2) == LEFT) {
					if (IntersectEnemyToEnemy(_enemy1, _enemy2) == true) {
						//�m�R�m�R���͓����Ă���b���Ȃ̂ŁA���ɉ������Ȃ�
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
//�t�@�C�A�{�[���̓G�Ƃ̓����蔻��𒲂ׂ�֐��A�������Ă���Ȃ�ǂ̓G����Ԃ�
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
//�A�C�e���Ƃ̓����蔻��𒲂ׂ�֐��A�������Ă���Ȃ�ǂ̃A�C�e������Ԃ�
//�L�m�R�A�t�@�C���[�t�����[�ȂǁA�����ɃA�C�e���̎�ނ����邱�Ƃ��ł���悤�ɂ���
//�A�C�e���̏ꍇ�́A���������ʒu�ɂ���ď�����ς���K�v�͂Ȃ��H
//(�v���C���[�̍��E�㉺�̂ǂ��������������ɂ���ď�����ς���\�������邽�߁A_positon��n��)
//----------------------------------------------------------------------------------------------
int IntersectItem(int _index, PLAYER _player, std::vector<ITEM*> _item) {
	if ((_player.position.x + ITEM_WIDTH >= _item[_index]->position.x)
		&& (_player.position.x <= _item[_index]->position.x + ITEM_WIDTH)
		&& (_player.position.y + _player.isBig * PLAYER_HEIGHT >= _item[_index]->position.y)
		&& (_player.position.y <= _item[_index]->position.y + PART_HEIGHT)) //�Ƃ肠�����A�u���b�N�̉�����A�C�e�������Ȃ��悤�ɏC���i�K�؂��ǂ��������j
	{
		return _index;
	}
	else {
		return -1;
	}
}


//--------------------------------------------------
//�S�[���|�[���Ƃ̓����蔻��𒲂ׂ�֐� 
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
//�����P����Ƃ��āA�����Q�Ɋ��������̏������L�q����֐�
//�I�u�W�F�N�g�̃C���X�^���X�͂��ׂĈ����Ƃ��ēn��
//loop�ł́Afor���[�v�̉񐔂�n��
//�����Ƃ���player��n���āA���̃����o�ϐ���ύX�������Ƃ��̓|�C���^�œn���K�v������
//���̊֐��ŎQ�Ɠn���ł��܂������Ă���̂́A�����o�ϐ��̒l���擾���邾���ŁA�ύX�������悤�Ƃ��Ă���킯�ł͂Ȃ�����
//-------------------------------------------------------------------------------------------------------------------------
void DoIntersectDetection(int _obj1, int _obj2, int _loop1, int _loop2, bool _isInGround, 
	PLAYER* _player, std::vector<ENEMY*> _enemy1, std::vector<ENEMY*> _enemy2,
	std::vector<ITEM*> _item, std::vector<FIRE_BALL*> _fire_ball, std::vector<BLOCK*> _block, std::vector<QUESTION*> _question) {
	switch (_obj1) {
		//�v���C���[�Ɋւ��铖���蔻��
	case PLAYER_INTERSECT:
		//�v���C���[�̃R�[�X�p�[�c�Ƃ̓����蔻��
		if (_obj2 == PART_INTERSECT) {
			if (IntersectField(_player->bottom, _player)) {
				_player->velocity.y = 0;
				_player->position.y = (float)((int)_player->position.y / PART_HEIGHT * PART_HEIGHT); //�L�������u���b�N�̏�ɗ��Ă�悤�ɂ���
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
		//�v���C���[�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			for (int i = 0; i < _loop2; i++) {
				if (_block[i]->flag == true) {
					if (IntersectBlock(_player->bottom, i, _block, _isInGround) == i) {
						_player->velocity.y = 0;
						_player->position.y = (float)((int)_player->position.y / PART_HEIGHT * PART_HEIGHT); //�L�������u���b�N�̏�ɗ��Ă�悤�ɂ���
						_player->isJumping = false;
					}
					else {
						//_player->isJumping = true; //�v�C���@�󂹂�u���b�N�̏�ɂ����A�A�j���[�V�������W�����v�A�j���[�V�����ɂȂ��Ă��܂�
						//_player->animation = ANIMATION_STAND; //�����ƓK�؂ȏC���Ă�����͂��E�E�E
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
		//�v���C���[�̃R�C���u���b�N�Ƃ̓����蔻��
			if (_obj2 == COIN_BLOCK_INTERSECT) {
				for (int i = 0; i < _loop2; i++) {
					//if (_block[i]->flag == true) { �R�C���u���b�N�͔j�󂳂�邱�Ƃ��Ȃ��A�����蔻�肪�����邱�Ƃ͂Ȃ�
						if (IntersectCoinBlock(_player->bottom, i, _block, _isInGround) == i) {
							_player->velocity.y = 0;
							_player->position.y = (float)((int)_player->position.y / PART_HEIGHT * PART_HEIGHT); //�L�������u���b�N�̏�ɗ��Ă�悤�ɂ���
							_player->isJumping = false;
						}
						else {
							//_player->isJumping = true; //�v�C���@�󂹂�u���b�N�̏�ɂ����A�A�j���[�V�������W�����v�A�j���[�V�����ɂȂ��Ă��܂�
							//_player->animation = ANIMATION_STAND; //�����ƓK�؂ȏC���Ă�����͂��E�E�E
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
		//�v���C���[�̃G�l�~�[�Ƃ̓����蔻��
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
		//�v���C���[�̃N���{�[�Ƃ̓����蔻��
		if (_obj2 == KURIBOO_INTERSECT) {
			if (_player->isInGround == false) {
				for (int i = 0; i < KURIBOO_NUM; i++) {
					if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //���܂�ď�������ƁA���񂾌�͈ȉ��̓����蔻��������Ȃ��K�v�͂Ȃ�
						SearchIntersectEnemyPoint(_player->position, i, *_player, _enemy1);

						if (IntersectEnemy(i, *_player, _enemy1) == i) {
							if (_enemy1[i]->intersect_left_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->position.x = _enemy1[i]->position.x - KURIBOO_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_right_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->position.x = _enemy1[i]->position.x + KURIBOO_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_top_flag == true) { //�G�𓥂ނƂ������Ƃ́A�G�̓��Ɗ����邱�Ƃɒ��ӂ���
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									if (_enemy1[i]->stepped_on_flag == false) {
										_player->velocity.y = -8;
										audioPlay(SE_STEP_ON);
										_enemy1[i]->stepped_on_flag = true; //���܂ꂽ�t���O�𗧂Ă�
									}
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_bottom_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->velocity.y = 0;
									_player->position.y = _enemy1[i]->position.y + _player->isBig * PLAYER_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
						}
						if (_enemy1[i]->intersect_bottom_flag == true) { //�u���b�N���͂���ŉ�����˂��グ�����̏���
							for (int j = 0; j < BLOCK_NUM; j++) {
								if (IsHitBlock(_player->top, j, _block) == j) {
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH); //�G�����񂾂Ƃ��̉���炷
								}
							}
							for (int j = 0; j < QUESTION_BOX_NUM; j++) { //�v�C���@�A�C�e�����o����̋�̃N�G�X�`�����{�b�N�X�ł��G��|���Ă��܂�
								if (_question[j]->flag == true) {
									if (IsGetQuestionbox(_player->top, j) == j) {
										_enemy1[i]->isDeath = true;
										audioPlay(SE_ENEMY_DEATH); //�G�����񂾂Ƃ��̉���炷
									}
								}
							}
						}
					}
				}
			}
		}
		//�v���C���[�̃m�R�m�R�Ƃ̓����蔻��
		if (_obj2 == NOKONOKO_INTERSECT) {
			for (int i = 0; i < NOKONOKO_NUM; i++) {
				if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //���܂�ď�������ƁA���񂾌�͈ȉ��̓����蔻��������Ȃ��K�v�͂Ȃ�
					SearchIntersectEnemyPoint(_player->position, i, *_player, _enemy1);

					if (IntersectEnemy(i, *_player, _enemy1) == i) {
						if (_enemy1[i]->stepped_on_flag == false) { //�ʏ��Ԃ̃m�R�m�R�Ƃ̊���
							if (_enemy1[i]->intersect_left_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->position.x = _enemy1[i]->position.x - NOKONOKO_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_right_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->position.x = _enemy1[i]->position.x + NOKONOKO_WIDTH;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_top_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->velocity.y = -8.0f;
									_player->position.y = _enemy1[i]->position.y - _player->isBig * PLAYER_HEIGHT;
									audioPlay(SE_STEP_ON);
									_enemy1[i]->stepped_on_flag = true; //���܂ꂽ�t���O�𗧂Ă�
									_enemy1[i]->width = NOKONOKO_SHELL_WIDTH; //�����b���̕��ɍX�V
									_enemy1[i]->height = NOKONOKO_SHELL_HEIGHT; //�������b���̍����ɍX�V
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}

							}
							else if (_enemy1[i]->intersect_bottom_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->velocity.y = 0;
									_player->position.y = _enemy1[i]->position.y + _player->isBig * PLAYER_HEIGHT;
									_player->animation = ANIMATION_DAMAGE;
									_player->isBig = 1;
									_player->isFire = false;
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
						}

						else if (_enemy1[i]->stepped_on_flag == true) { //���܂ꂽ��Ԃ̃m�R�m�R�Ƃ̊���
							if (_enemy1[i]->intersect_left_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->position.x = _enemy1[i]->position.x - NOKONOKO_WIDTH;

									if (_enemy1[i]->isShellMoving == false) { //�b�����~�܂��Ă��鎞�Ɋ������ꍇ�̏���
										_enemy1[i]->touch_left_flag = true;
										_enemy1[i]->touch_right_flag = false; //�����ɗ����̃t���O�������Ȃ��悤�ɂ���
										_enemy1[i]->isMirror = true; //�G�L�����͍����������Ȃ̂ŁA���Ε����ɐi�ނƂ���Mirror�t���O�𗧂Ă�i�������Ȃ��Ɣ��˂����܂������Ȃ��j
										_enemy1[i]->isShellMoving = true;
									}
									else if (_enemy1[i]->isShellMoving == true) { //�b���������Ă��鎞�Ɋ������ꍇ�̏���
										_player->animation = ANIMATION_DAMAGE;
										_player->isBig = 1;
										_player->isFire = false;
									}
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_right_flag == true) { //�b�����~�܂��Ă��鎞�Ɋ������ꍇ�̏���
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->position.x = _enemy1[i]->position.x + NOKONOKO_WIDTH;

									if (_enemy1[i]->isShellMoving == false) {
										_enemy1[i]->touch_right_flag = true;
										_enemy1[i]->touch_left_flag = false;
										_enemy1[i]->isMirror = false;
										_enemy1[i]->isShellMoving = true;
									}
									else if (_enemy1[i]->isShellMoving == true) { //�b���������Ă��鎞�Ɋ������ꍇ�̏���
										_player->animation = ANIMATION_DAMAGE;
										_player->isBig = 1;
										_player->isFire = false;
									}
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
							else if (_enemy1[i]->intersect_top_flag == true) { //�v�C���@�G�̍���Ȃ̂��E��Ȃ̂��ōb�����͂���������ς�����
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->position.y = _enemy1[i]->position.y - _player->isBig * PLAYER_HEIGHT;

									if (_enemy1[i]->isShellMoving == false) { //�b�����~�܂��Ă��鎞�Ɋ������ꍇ�̏���
										_enemy1[i]->touch_top_flag = true;
										_enemy1[i]->isMirror = true;
										_enemy1[i]->isShellMoving = true;
									}
									else if (_enemy1[i]->isShellMoving == true && _enemy1[i]->isMirror == false) { //�����Ă���b�����ォ�瓥�񂾎��̏���
										//_enemy1[i]->isMirror == false���Ȃ��ƁA�~�܂��Ă���b���𓥂񂾏u�Ԃɂ������̃��[�v�������s���Ă��܂��A���܂������Ȃ��B
										//�����Ă���b���𓥂ނƂ��́A�ǂ����Œ��˕Ԃ��Ă�����Ԃœ��ނ��Ƃɒ��ڂ��A���̏������������B
										_player->velocity.y = -8.0f;
										//_player->position.y = _enemy1[i]->position.y - _player->isBig * PLAYER_HEIGHT;
										audioPlay(SE_STEP_ON);
										_enemy1[i]->isShellMoving = false; //�b���̓������~�߂�
										double tmp = _enemy1[i]->position.x; //���܂ꂽ�u�Ԃ̍b���̈ʒu��ێ�
										_enemy1[i]->position.x = tmp; //�b���̈ʒu���A���܂ꂽ�ʒu�Ŏ~�߂�
									}
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}

							}
							else if (_enemy1[i]->intersect_bottom_flag == true) {
								if (_player->isStar == false) { //�X�^�[��ԂłȂ��Ƃ�
									_player->velocity.y = 0;
									_player->position.y = _enemy1[i]->position.y + _player->isBig * PLAYER_HEIGHT;
								}
								else if (_player->isStar == true) { //�X�^�[��Ԃ̎�
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH);
								}
							}
						}
					}
				}
			}
		}
		//�v���C���[�̃X�[�p�[�L�m�R�Ƃ̓����蔻��
		if (_obj2 == MUSHROOM_INTERSECT) {
			for (int i = 0; i < MUSHROOM_NUM; i++) {
				if (_item[i]->display_flag == true) { //�������͓����蔻�������
					if (_item[i]->get_flag == false) {
						if (IntersectItem(i, *_player, _item) == i) {
							_player->position.y = _player->position.y - PART_HEIGHT / 100.0f; //�L�m�R�������ɏ�����Ȃ���Ȃ����A�L�m�R�̏�𒵂˂�悤�ȕ`��ɂȂ��Ă��܂�
																				//�v���C���[�̃|�W�V�����������������Ēu���Ȃ��ƁA�傫���Ȃ������ɒn�ʂɖ��܂��Ă��܂�
							_player->isBig = 2;
							_item[i]->get_flag = true;
							audioPlay(SE_GET_MUSHROOM);
						}
					}
				}
			}
		}
		//�v���C���[��1UP�L�m�R�Ƃ̓����蔻��
		if (_obj2 == MUSHROOM_1UP_INTERSECT) {
			for (int i = 0; i < MUSHROOM_1UP_NUM; i++) {
				if (_item[i]->display_flag == true) { //�������͓����蔻�������
					if (_item[i]->get_flag == false) {
						if (IntersectItem(i, *_player, _item) == i) {
							_item[i]->get_flag = true;
							audioPlay(SE_1UP);
						}
					}
				}
			}
		}
		//�v���C���[�̃t�@�C�A�t�����[�Ƃ̓����蔻��
		if (_obj2 == FIRE_FLOWER_INTERSECT) {
			for (int i = 0; i < FIRE_FLOWER_NUM; i++) {
				if (_item[i]->display_flag == true) { //�������͓����蔻�������
					if (_item[i]->get_flag == false) {
						if (IntersectItem(i, *_player, _item) == i) {
							_player->position.y = _player->position.y - PART_HEIGHT / 100.0f;
							_player->isBig = 2;
							_player->isFire = true;
							_item[i]->get_flag = true;
							audioPlay(SE_GET_MUSHROOM); //�X�[�p�[�L�m�R����������ƃt�@�C�A�t�����[����������͓�����
						}
					}
				}
			}
		}
		//�v���C���[�̃X�^�[�Ƃ̓����蔻��
		if (_obj2 == STAR_INTERSECT) {
			for (int i = 0; i < STAR_NUM; i++) {
				if (_item[i]->display_flag == true) { //�������͓����蔻�������
					if (_item[i]->get_flag == false) {
						if (IntersectItem(i, *_player, _item) == i) {
							//�X�^�[����������̏����������ɏ���
							_player->isStar = true;
							_item[i]->get_flag = true;
							audioStop(MUSIC_STAGE); //�X�e�[�W��BGM���~�߁A
							audioPlay(MUSIC_STAR); //�X�^�[��BGM�𗬂�
						}
					}
				}
			}
		}
		//�v���C���[�̃S�[���Ƃ̓����蔻��
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
				if (_player->flag_pos.y <= 540) { //�S�[���̊������ɂ���܂ł̊Ԃ̏���
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
						_player->tmp_goal_frame = _player->frame; //�S�[���̊������ɂ����O�̃t���[����ێ�
					}
				}
				else { //�S�[���̊������ɂ�����̏���
					//���̓v���C���[�͎��R�ɓ�����
					//�L�����N�^�[�̓�������������Ȃ炱���ɏ���������
					if (_player->frame < _player->tmp_goal_frame + 60) { //60�t���[���̊Ԃ����ȉ��̏������s��
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

		//�G�l�~�[�Ɋւ��铖���蔻��
	case ENEMY_INTERSECT:
		//�G�l�~�[���g�̎��͂Ƃ̓����蔻��A�N���{�[���g�̎��͂Ƃ̓����蔻��
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
		//�G�l�~�[�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
		//�����̏����͑S��������Əd���Ȃ邽�߁A������g�ݍ��킹�̂ݎw�肷��
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			DoIntersectEnemyToBlock(_enemy1[1], _block[0], 0);
			DoIntersectEnemyToBlock(_enemy1[1], _block[1], 1);
			DoIntersectEnemyToBlock(_enemy1[1], _block[2], 2);
		}
		break;

	//�N���{�[�Ɋւ��铖���蔻��
	case KURIBOO_INTERSECT:
		//�N���{�[���g�̎��͂Ƃ̓����蔻��́A�G�l�~�[�̕����Ɠ����ł��邽�߁A�������H�v���ď�������

		//�N���{�[�́i�󂹂�j�u���b�N�Ƃ̓����蔻��
		//�v�ύX�@�d������A�ʂɏ������邱�Ƃŉ���
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

		//�N���{�[���m�̓����蔻��
		if (_obj2 == KURIBOO_INTERSECT) {
			DoIntersectEnemyToEnemy(_enemy1[2], _enemy1[3], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[3], _enemy1[2], _obj1, _obj2);
		}

		//�m�R�m�R�Ƃ̓����蔻��
		if (_obj2 == NOKONOKO_INTERSECT) {
			DoIntersectEnemyToEnemy(_enemy1[8], _enemy2[0], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[9], _enemy2[0], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[10], _enemy2[0], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[11], _enemy2[0], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[12], _enemy2[0], _obj1, _obj2);

			//DoIntersectEnemyToEnemy(_enemy1[15], _enemy2[2], _obj1, _obj2);
		}
		break;

		//�m�R�m�R�Ɋւ��铖���蔻��
	case NOKONOKO_INTERSECT:
		//�m�R�m�R���g�̎��͂Ƃ̓����蔻��́A�G�l�~�[�̕����Ɠ����ł��邽�߁A�������H�v���ď�������

		//�N���{�[�Ƃ̓����蔻��
		if (_obj2 == KURIBOO_INTERSECT) {
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[8], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[9], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[10], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[11], _obj1, _obj2);
			DoIntersectEnemyToEnemy(_enemy1[0], _enemy2[12], _obj1, _obj2);

			//DoIntersectEnemyToEnemy(_enemy1[2], _enemy2[15], _obj1, _obj2);
		}

		break;

	//�X�[�p�[�L�m�R�Ɋւ��铖���蔻��
	case MUSHROOM_INTERSECT:
		//�X�[�p�[�L�m�R���g�̎��͂Ƃ̓����蔻��
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
		//�X�[�p�[�L�m�R�Ɣj��ł���u���b�N�̓����蔻��
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			DoIntersectItemToBlock(_item[0], _block[0], 0);
			DoIntersectItemToBlock(_item[0], _block[1], 1);
			DoIntersectItemToBlock(_item[0], _block[2], 2);
		}
		break;

	//1UP�L�m�R�Ɋւ��铖���蔻��
	case MUSHROOM_1UP_INTERSECT:
		//1UP�L�m�R���g�̎��͂Ƃ̓����蔻��
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

	//�X�^�[�Ɋւ��铖���蔻��
	case STAR_INTERSECT:
		///�X�^�[���g�̎��͂Ƃ̓����蔻��
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
		///�X�^�[�Ɣj��ł���u���b�N�̓����蔻��
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			//DoIntersectItemToBlock(_item[0], _block[31], 31);
		}
		break;

	//�t�@�C���{�[���Ɋւ��铖���蔻��
	case FIRE_BALL_INTERSECT :
		//�t�@�C���{�[�����g�̎��͂Ƃ̓����蔻��
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
					//�t�@�C�A�{�[���͑��̃p�[�c�������������߁A�����蔻��̎��̏����𒲐߂��邽�߂�+ PART_HEIGHT / 2.0f�������Ă���
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
		//�t�@�C�A�{�[���́i�󂹂�j�u���b�N�Ƃ̓����蔻��
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			for (int i = 0; i < FIRE_BALL_NUM; i++) {
				if (_fire_ball[i]->display_flag == true) {
					for (int j = 0; j < BLOCK_NUM; j++) {
						DoIntersectFireBallToBlock(_fire_ball[i], _block[j], j);
					}
				}
			}
		}
		//�t�@�C�A�{�[���̃N���{�[�i�m�R�m�R�j�Ƃ̓����蔻��
		if (_obj2 == KURIBOO_INTERSECT || _obj2 == NOKONOKO_INTERSECT) {
			for (int j = 0; j < FIRE_BALL_NUM /*_loop1*/; j++) {
				if (_fire_ball[j]->display_flag == true) {
					for (int i = 0; i < _loop2; i++) {
						if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //���܂�ď�������ƁA���񂾌�͈ȉ��̓����蔻��������Ȃ��K�v�͂Ȃ�
							SearchIntersectEnemyPoint(_fire_ball[j]->position, i, *_player, _enemy1); //������*_player�́A���ۂɂ͂����ł͕K�v�Ȃ�

							if (IntersectEnemyFireBall(i, _fire_ball[j], _enemy1) == i) {
								_fire_ball[j]->animation = FIRE_BALL_ANIMATION_BURST;
								_fire_ball[j]->isAttackEnemy = true; //�����true�ɂ��邱�ƂŁA�t�@�C�A�{�[���֏d�͂������邱�Ƃ���߂�
								_enemy1[i]->isDeath = true; //�G������
								audioPlay(SE_ENEMY_DEATH); //�G�����񂾂Ƃ��̉���炷

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
