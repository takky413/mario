#include <vector>
#include "glm\glm.hpp"
#include "course.h"
#include "header.h"
#include "audio.h"
#include "instance.h"


//---------------------------------------------------------------------------------
//�ǂ̃N�G�X�`�����{�b�N�X�������������ǂ������ׂ�֐��A�������ĂȂ�������-1��Ԃ�
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
//�ǂ̃u���b�N�������������ǂ������ׂ�֐��A�������ĂȂ�������-1��Ԃ�
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
//�A�C�e���Ƃ̓����蔻��𒲂ׂ�֐��A�������Ă���Ȃ�ǂ̃A�C�e������Ԃ�
//----------------------------------------------------------------------------------------------
int IntersectItem_1_2(int _index, PLAYER _player, std::vector<ITEM*> _item) {
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


//-----------------------------------------------------------------------------------
//�G�Ƃ̓����蔻��ɂ����āA�G�̍��E�㉺�̂ǂ��������������𒲂ׂ�֐�
//���ۂ͓G�̍��E�㉺�ǂ̈ʒu�ɂ��邩��Ԃ��Ă���
//�N���{�[�Ȃǂ̑��̓G�ɂ��K���\�ɂ��Ă���
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
	else if (_position.y + (_player.isBig + 1) * PLAYER_HEIGHT <= _enemy[_index]->position.y) { //_player.isBig��+1���Ă����Ȃ��ƁA�傫����Ԃŗ����Ă��邾���œG�𓥂�ł��܂�
		_enemy[_index]->intersect_left_flag = false;
		_enemy[_index]->intersect_right_flag = false;
		_enemy[_index]->intersect_top_flag = true;
		_enemy[_index]->intersect_bottom_flag = false;
	}
	//�u�G�̉��Ɉʒu���Ă���v�Ɣ��肷��͈͂����߂Ă����Ȃ��ƁA�u���b�N����i����Ƃ��ɁA���̒i�̃u���b�N�������������ɂ��A��̒i�ɂ���G������ł��܂�
	else if (_position.y >= _enemy[_index]->position.y + _player.isBig * PLAYER_HEIGHT && _position.y <= _enemy[_index]->position.y + _player.isBig * PLAYER_HEIGHT + PART_HEIGHT * 2) {
		_enemy[_index]->intersect_left_flag = false;
		_enemy[_index]->intersect_right_flag = false;
		_enemy[_index]->intersect_top_flag = false;
		_enemy[_index]->intersect_bottom_flag = true;
	}
}


//----------------------------------------------------------------------------------------------
//�v���C���[�̓G�Ƃ̓����蔻��𒲂ׂ�֐��A�������Ă���Ȃ�ǂ̓G����Ԃ�
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
//�����y�ǂƊ����Ă��邩���ׂ�֐�
//�������ɂ͓����蔻����s���ӏ��ileft,right,top,botttom�̂ǂꂩ������j
//--------------------------------------------------------------------------
bool IntersectPipe_1_2(glm::vec2 _position, int _intersect_poit, int _course) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if (_course == MAIN) {
		if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
			return false;
		}
		if (course_1_2[y][x] == 'p') { //�y�ǂ�
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
		if (start_course_1_2[y][x] == 'p') { //�y�ǂ�
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
//�t�@�C�A�{�[���̓G�Ƃ̓����蔻��𒲂ׂ�֐��A�������Ă���Ȃ�ǂ̓G����Ԃ�
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
//���t�g�Ƃ̓����蔻��ɂ����āA���t�g�̍��E�㉺�̂ǂ��������������𒲂ׂ�֐�
//���ۂ̓��t�g�̏�ɂ��邩�ǂ����𒲂ׂĂ���B
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
	else if (_position.y + (_player.isBig + 1) * PLAYER_HEIGHT <= _lift[_index]->position.y) { //_player.isBig��+1���Ă����Ȃ��ƁA�傫����Ԃŗ����Ă��邾���œG�𓥂�ł��܂�
		_lift[_index]->intersect_top_flag = true;
	}
	else if (_position.y >= _lift[_index]->position.y + LIFT_HEIGHT) {
		_lift[_index]->intersect_top_flag = false;
	}
}



//----------------------------------------
//���t�g�Ɗ����Ă��邩���ׂ�֐�
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
//�S�[���|�[���Ƃ̓����蔻��𒲂ׂ�֐� 
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
//�t�B�[���h�̃I�u�W�F�N�g�Ɗ����Ă��邩���ׂ�֐�
//------------------------------------------------------
bool IntersectField_1_2(glm::vec2 _position, PLAYER* _player) {
	int x = (int)_position.x / PART_WIDTH;
	int y = (int)_position.y / PART_HEIGHT;

	if ((x < 0) || (x >= COURSE_WIDTH) || (y < 0) || (y >= COURSE_HEIGHT)) {
		return false;
	}

	if (_player->course == MAIN) { //���C���R�[�X�Ƃ̓����蔻��
		switch (course_1_2[y][x]) {
		case 'b':
			//if (sub_course_1_2[y][x] == '0' || sub_course_1_2[y][x] == '1') {
			return true;
			//}
			break;
		case 'p':
			if (_player->inPipe == true) { //�y�ǂ̏�ł��Ⴊ�񂾎��̓����蔻�������
				if (sub2_course_1_2[y][x] == 'i') { //�v�C���@���̂܂܂��ƁA�}���I���y�ǂɓ����Ă���Œ��́A�G���y�ǂ����蔲���Ă��܂�
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
	else if (_player->course == START) { //�X�^�[�g�R�[�X�Ƃ̓����蔻��
		switch (start_course_1_2[y][x]) {
		case 'p':
			if (_player->inPipe == true) { //�y�ǂ̏�ł��Ⴊ�񂾎��̓����蔻�������
				if (start_sub2_course_1_2[y][x] == 'i') { //�v�C���@���̂܂܂��ƁA�}���I���y�ǂɓ����Ă���Œ��́A�G���y�ǂ����蔲���Ă��܂�
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
	else if (_player->course == GOAL) { //�S�[���R�[�X�Ƃ̓����蔻��
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
//�����蔻������ۂɍs���֐�
//-------------------------------------
void DoIntersectDetection_1_2(int _obj1, int _obj2, int _loop1, int _loop2, bool _isInGround,
	PLAYER* _player, std::vector<ENEMY*> _enemy1, std::vector<ENEMY*> _enemy2,
	std::vector<ITEM*> _item, std::vector<FIRE_BALL*> _fire_ball, std::vector<BLOCK*> _block, std::vector<QUESTION*> _question,
	std::vector<LIFT*> _lift) {
	switch (_obj1) {
		//�v���C���[�Ɋւ��铖���蔻��
	case PLAYER_INTERSECT:
		//�v���C���[�̃R�[�X�p�[�c�Ƃ̓����蔻��
		if (_obj2 == PART_INTERSECT) {
			if (IntersectField_1_2(_player->bottom, _player)) {
				_player->velocity.y = 0;
				_player->position.y = (float)((int)_player->position.y / PART_HEIGHT * PART_HEIGHT); //�L�������u���b�N�̏�ɗ��Ă�悤�ɂ���
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
		//�v���C���[�̃N���{�[�Ƃ̓����蔻��
		if (_obj2 == KURIBOO_INTERSECT) {
			if (_player->course == MAIN) {
				for (int i = 0; i < KURIBOO_NUM_1_2; i++) {
					if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //���܂�ď�������ƁA���񂾌�͈ȉ��̓����蔻��������Ȃ��K�v�͂Ȃ�
						SearchIntersectEnemyPoint_1_2(_player->position, i, *_player, _enemy1);

						if (IntersectEnemy_1_2(i, *_player, _enemy1) == i) {
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
						/*
						if (_enemy1[i]->intersect_bottom_flag == true) { //�u���b�N���͂���ŉ�����˂��グ�����̏���
							for (int j = 0; j < BLOCK_NUM; j++) {
								if (IsHitBlock_1_2(_player->top, j, _block) == j) {
									_enemy1[i]->isDeath = true;
									audioPlay(SE_ENEMY_DEATH); //�G�����񂾂Ƃ��̉���炷
								}
							}
							for (int j = 0; j < QUESTION_BOX_NUM; j++) { //�v�C���@�A�C�e�����o����̋�̃N�G�X�`�����{�b�N�X�ł��G��|���Ă��܂�
								if (_question[j]->flag == true) {
									if (IsGetQuestionbox_1_2(_player->top, j) == j) {
										_enemy1[i]->isDeath = true;
										audioPlay(SE_ENEMY_DEATH); //�G�����񂾂Ƃ��̉���炷
									}
								}
							}
						}*/
					}
				}
			}
		}
		//�v���C���[�̃X�[�p�[�L�m�R�Ƃ̓����蔻��
		if (_obj2 == MUSHROOM_INTERSECT) {
			for (int i = 0; i < MUSHROOM_NUM_1_2; i++) {
				if (_item[i]->display_flag == true) { //�������͓����蔻�������
					if (_item[i]->get_flag == false) {
						if (IntersectItem_1_2(i, *_player, _item) == i) {
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
		//�v���C���[�̃t�@�C�A�t�����[�Ƃ̓����蔻��
		if (_obj2 == FIRE_FLOWER_INTERSECT) {
			for (int i = 0; i < FIRE_FLOWER_NUM_1_2; i++) {
				if (_item[i]->display_flag == true) { //�������͓����蔻�������
					if (_item[i]->get_flag == false) {
						if (IntersectItem_1_2(i, *_player, _item) == i) {
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
		//�v���C���[�̃��t�g�Ƃ̓����蔻��
		if (_obj2 == LIFT_INTERSECT) {
			for (int i = 0; i < LIFT_NUM; i++) {
				SearchIntersectLiftPoint(_player->position, i, *_player, _lift);

				if (IntersectLift(i, *_player, _lift) == i) {
					if (_lift[i]->intersect_top_flag == true) {
						_player->velocity.y = 0.8f; //���t�g�ɏ���Ă��鎞�����W�����v�͂��Ⴍ�Ȃ��Ă��܂��B
						_player->position.y = _lift[i]->position.y - _player->isBig * PLAYER_HEIGHT;
						_player->isJumping = false;
					}
				}
			}
		}
		//�v���C���[�̃S�[���Ƃ̓����蔻��
		if (_obj2 == GOAL_INTERSECT) {
			if (_player->course == GOAL) {
				if (IntersectGoal_1_2(_player->right) == 1) {
					if (_player->isGoal == false) {
						_player->isGoal = true;
						audioStop(MUSIC_STAGE);
					}
				}
				if (_player->isGoal == true) {
					if (_player->flag_pos.y <= 540) { //�S�[���̊������ɂ���܂ł̊Ԃ̏���
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
							_player->tmp_goal_frame = _player->frame; //�S�[���̊������ɂ����O�̃t���[����ێ�
						}
					}
					else { //�S�[���̊������ɂ�����̏���
						//���̓v���C���[�͎��R�ɓ�����
						//�L�����N�^�[�̓�������������Ȃ炱���ɏ���������
						if (_player->frame < _player->tmp_goal_frame + 60) { //60�t���[���̊Ԃ����ȉ��̏������s��
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
		//�X�[�p�[�L�m�R�Ɋւ��铖���蔻��
	case MUSHROOM_INTERSECT:
		//�X�[�p�[�L�m�R���g�̎��͂Ƃ̓����蔻��
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
		//�X�[�p�[�L�m�R�Ɣj��ł���u���b�N�̓����蔻��
		/*
		if (_obj2 == BREAKABLE_BLOCK_INTERSECT) {
			DoIntersectItemToBlock(_item[0], _block[0], 0);
			DoIntersectItemToBlock(_item[0], _block[1], 1);
			DoIntersectItemToBlock(_item[0], _block[2], 2);
		}
		*/
		break;
		//�t�@�C���{�[���Ɋւ��铖���蔻��
	case FIRE_BALL_INTERSECT:
		//�t�@�C���{�[�����g�̎��͂Ƃ̓����蔻��
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
					//�t�@�C�A�{�[���͑��̃p�[�c�������������߁A�����蔻��̎��̏����𒲐߂��邽�߂�+ PART_HEIGHT / 2.0f�������Ă���
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
		//�t�@�C�A�{�[���̃N���{�[�i�m�R�m�R�j�Ƃ̓����蔻��
		if (_obj2 == KURIBOO_INTERSECT || _obj2 == NOKONOKO_INTERSECT) {
			for (int j = 0; j < FIRE_BALL_NUM /*_loop1*/; j++) {
				if (_fire_ball[j]->display_flag == true) {
					for (int i = 0; i < _loop2; i++) {
						if (_enemy1[i]->display_flag == true && _enemy1[i]->isDeath == false) { //���܂�ď�������ƁA���񂾌�͈ȉ��̓����蔻��������Ȃ��K�v�͂Ȃ�
							SearchIntersectEnemyPoint_1_2(_fire_ball[j]->position, i, *_player, _enemy1); //������*_player�́A���ۂɂ͂����ł͕K�v�Ȃ�

							if (IntersectEnemyFireBall_1_2(i, _fire_ball[j], _enemy1) == i) {
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
	//�N���{�[�̊ւ��铖���蔻��
	case KURIBOO_INTERSECT:
		//�N���{�[���g�̎��͂Ƃ̓����蔻��
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


