#pragma once

#include <stdio.h>
#include "glm\glm.hpp"
#include <vector>
#include "glm\glm.hpp"
#include "header.h"

void DrawBlock_1_2(glm::vec2 _position, char _c, char _sub_c, char _sub2_c, std::vector<BLOCK*> _block, int _index);

void DrawQuestionBox_1_2(glm::vec2 _position, char _c, char _sub2_c, std::vector<QUESTION*> _question, ANIMATION _question_box_animations, int _index);

//void DrawPart_1_2(glm::vec2 _position, char _c, char _sub_c, char _sub2_c,
//	std::vector<BLOCK*> _block, std::vector<QUESTION*> _question, ANIMATION _question_box_animations);