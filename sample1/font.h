#pragma once

#define FONT_DEFAULT_SIZE 100.f

enum {
	FONT_FONT_ROMAN,
	FONT_FONT_MONO_ROMAN,
	FONT_FONT_MAX
};

void fontBegin();
void fontEnd();

void fontFont(int _font);
void fontSetPosition(float _x, float _y);
void fontSetSize(float _size);
float fontGetsize(void);

float fontGetWeightMin();
float fontGetWeightMax();
void fontSetWeight(float _weight);

void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue);
void fontDraw(const char *_format, ...);