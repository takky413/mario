#include "header.h"

TEX tex[TEX_MAX];

//---------------------------------------
//基本となる関数の定義
//---------------------------------------

//---------------------------------------
//文字による進数表示を数字に変換する
//---------------------------------------
int LetterToNum(char _c) {
	switch (_c) {
	case '1': return 1; break;
	case '2': return 2; break;
	case '3': return 3; break;
	case '4': return 4; break;
	case '5': return 5; break;
	case '6': return 6; break;
	case '7': return 7; break;
	case '8': return 8; break;
	case '9': return 9; break;
	case 'a': return 10; break;
	case 'b': return 11; break;
	case 'c': return 12; break;
	case 'd': return 13; break;
	case 'e': return 14; break;
	case 'f': return 15; break;
	case 'g': return 16; break;
	case 'h': return 17; break;
	case 'i': return 18; break;
	case 'j': return 19; break;
	case 'k': return 20; break;
	case 'l': return 21; break;
	case 'm': return 22; break;
	case 'n': return 23; break;
	case 'o': return 24; break;
	case 'p': return 25; break;
	case 'q': return 26; break;
	case 'r': return 27; break;
	case 's': return 28; break;
	case 't': return 29; break;
	case 'u': return 30; break;
	case 'v': return 31; break;
	case 'w': return 32; break;
	case 'x': return 33; break;
	case 'y': return 34; break;
	case 'z': return 35; break;
	case 'A': return 36; break;
	case 'B': return 37; break;
	case 'C': return 38; break;
	case 'D': return 39; break;
	default: break;
	}
}


//--------------------------------------
//ファイルからテクスチャを読み込む関数
//--------------------------------------
void TexFromFile(const char* _FileName, int _tex, glm::ivec2 _rect) {
	tex[_tex].rect = _rect;
	FILE* stream;

	//画像ファイル名の先頭に、.bmpがまとめてあるファイルのパスを追加する
	//追加
	char filePathChar[256] = "texture\\";
	sprintf_s(filePathChar, "%s%s", filePathChar, _FileName);
	_FileName = filePathChar;
	//終わり

	fopen_s(&stream, _FileName, "rb");
	if (!stream) {
		printf("%s open failed!\n", _FileName);
		return;
	}
	printf("%s is open.\n", _FileName);

	fread(&tex[_tex].bf, sizeof BITMAPFILEHEADER, 1, stream);
	printf("bfSize:%d\n", tex[_tex].bf.bfSize);
	fread(&tex[_tex].bi, sizeof BITMAPINFOHEADER, 1, stream);
	printf("%d * %d\n", tex[_tex].bi.biWidth, tex[_tex].bi.biHeight);

	typedef struct {
		unsigned char b, g, r;
	}BGR;

	BGR* bits = (BGR*)malloc(sizeof(BGR) * tex[_tex].bi.biWidth * tex[_tex].bi.biHeight);
	fread(bits, sizeof(BGR), tex[_tex].bi.biWidth * tex[_tex].bi.biHeight, stream);

	typedef struct {
		unsigned char r, g, b, a;
	}RGBA;

	RGBA* pixels = (RGBA*)malloc(sizeof(RGBA) * tex[_tex].bi.biWidth * tex[_tex].bi.biHeight);
	for (int i = 0; i < tex[_tex].bi.biWidth * tex[_tex].bi.biHeight; i++) {
		pixels[i].r = bits[i].r;
		pixels[i].g = bits[i].g;
		pixels[i].b = bits[i].b;
		pixels[i].a = ((bits[i].r == 0x00) && (bits[i].g == 0xff) && (bits[i].b == 0x00)) ? 0x00 : 0xff;
	}
	free(bits);
	for (int y = 0; y < tex[_tex].bi.biHeight / 2; y++) {
		for (int x = 0; x < tex[_tex].bi.biWidth; x++) {
			RGBA temp = pixels[y * tex[_tex].bi.biWidth + x];
			pixels[y * tex[_tex].bi.biWidth + x]
				= pixels[(tex[_tex].bi.biHeight - 1 - y) * tex[_tex].bi.biWidth + x];
			pixels[(tex[_tex].bi.biHeight - 1 - y) * tex[_tex].bi.biWidth + x] = temp;
		}
	}

	glGenTextures(1, &tex[_tex].texture);
	glBindTexture(GL_TEXTURE_2D, tex[_tex].texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		tex[_tex].bi.biWidth,
		tex[_tex].bi.biHeight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels);

	free(pixels);
	fclose(stream);
	printf("%s is closed.\n", _FileName);

}



//-------------------------------
//長方形を描画する関数
//-------------------------------
void DrawSprite(int _tex, float _width, float _height, int _index, glm::vec2 _position, bool _isMirror) {
	glBindTexture(GL_TEXTURE_2D, tex[_tex].texture);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4ub(0xff, 0xff, 0xff, 0xff);

	glPushMatrix();
	{
		glTranslatef(_position.x + _width * _isMirror, _position.y, 0);

		if (_isMirror) {
			glScalef(-1, 1, 0);
		}

		glBegin(GL_QUADS);
		{
			int w = tex[_tex].bi.biWidth;
			int h = tex[_tex].bi.biHeight;
			int tw = tex[_tex].rect.x;
			int th = tex[_tex].rect.y;
			int columnMax = w / tw;
			int rowMax = h / th;
			int tx = tw * (_index % columnMax);
			int ty = th * (_index / columnMax);
			glTexCoord2f(tx / (float)w, ty / (float)h);
			glVertex2f(0, 0);
			glTexCoord2f(tx / (float)w, (ty + th) / (float)h);
			glVertex2f(0, _height);
			glTexCoord2f((tx + tw) / (float)w, (ty + th) / (float)h);
			glVertex2f(_width, _height);
			glTexCoord2f((tx + tw) / (float)w, ty / (float)h);
			glVertex2f(_width, 0);
		}
		glEnd();
	}
	glPopMatrix();
}