#pragma once

int ReadHeaderWav(FILE* fp, int* channel, int* bit, int* size, int* freq);
void SetAudioMusicFromFile(const char* _FileName, int _music);
void SetAudioSeFromFile(const char* _FileName, int _se);
void SetAudioVoiceFromFile(const char* _FileName, int _voice);

int audioInit();
void audioPlay(int);
void audioStop(int);
