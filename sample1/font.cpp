#include <stdio.h>
#include <stdarg.h>
#include "font.h"
#include <GL/glut.h>
#include "glm\glm.hpp"

static void* font = GLUT_STROKE_ROMAN;

static glm::vec2 position;
static float size = FONT_DEFAULT_SIZE;
static unsigned char color[3];
static float weight = 1;

void fontBegin() {
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluOrtho2D(0, viewport[2], viewport[3], 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void fontEnd() {
	glPopMatrix();
	glPopAttrib();
}

void fontFont(int _font) {
	switch (_font) {
	case FONT_FONT_ROMAN: font = GLUT_STROKE_ROMAN; break;
	case FONT_FONT_MONO_ROMAN: font = GLUT_STROKE_MONO_ROMAN; break;
	}
}

void fontSetPosition(float _x, float _y) {
	position = glm::vec2(_x, _y);
}

void fontSetSize(float _size) {
	size = _size;
}

float fontGetsize() {
	return size;
}

float fontGetWeightMin() {
	GLfloat weight[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, weight);
	return weight[0];
}

float fontGetWeightMax() {
	GLfloat weight[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, weight);
	return weight[1];
}

void fontSetWeight(float _weight) {
	weight = _weight;
}

void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue) {
	color[0] = _red;
	color[1] = _green;
	color[2] = _blue;
}

void fontDraw(const char *_format, ...) {
	va_list argList;
	va_start(argList, _format);
	char str[256];
	vsprintf_s(str, _format, argList);
	va_end(argList);

	glLineWidth(weight);
	glColor3ub(color[0], color[1], color[2]);
	glPushMatrix();
	{
		glTranslatef(position.x, position.y + size, 0);
		float s = size / FONT_DEFAULT_SIZE;
		glScalef(s, -s, s);
		for (char* p = str; *p != '\0'; p++) {
			glutStrokeCharacter(font, *p);
		}
	}
	glPopMatrix();
}