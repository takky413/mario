#include <al.h>
#include <stdio.h>
#include <stdlib.h>
#include <alc.h>
#include "audio.h"
#include <cstdlib>

#pragma comment(lib, "OpenAL32.lib")
#pragma warning(disable : 4996)

static ALuint buffer, source;

enum {
	MUSIC_STAGE,
	MUSIC_UNDER_GROUND_STAGE,
	MUSIC_TITLE,
	MUSIC_START,
	MUSIC_STAR,
	MUSIC_MAX
};

enum {
	SE_BLOCK_BREAK,
	SE_BLOCK_HIT,
	SE_GET_COIN,
	SE_GET_MUSHROOM,
	SE_1UP,
	SE_START,
	SE_STEP_ON,
	SE_FIRE_BALL,
	SE_ENEMY_DEATH,
	SE_IN_PIPE,
	SE_CLEAR,
	SE_MAX
};


enum {
	VOICE_JUMP_1,
	VOICE_JUMP_2,
	VOICE_UNITY_0006,
	VOICE_UNITY_0010,
	VOICE_UNITY_1019,
	VOICE_MAX
};



typedef struct {
	ALuint source;
	ALuint buffer;
	FILE* fp;
	int wavChannel;
	int wavBit;
	int wavSize;
	int wavFreq;
	unsigned char* data;
}AUDIO;


AUDIO music[MUSIC_MAX];
AUDIO se[SE_MAX];
AUDIO voice[VOICE_MAX];


//-----------------------------------
//wav�t�@�C����ǂݍ��ގ���֐�
//-----------------------------------
int ReadHeaderWav(FILE* fp, int* channel, int* bit, int* size, int* freq) {
	short res16;
	int res32;
	int dataSize, chunkSize;
	short channelCnt, bitParSample, blockSize;
	int samplingRate, byteParSec;

	int dataPos;
	int flag = 0;
	printf("1\n");
	fread(&res32, 4, 1, fp);
	if (res32 != 0x46464952) {	//"RIFF"
		return 1;	//error 1
	}
	printf("2\n");
	//�f�[�^�T�C�Y = �t�@�C���T�C�Y - 8 byte �̎擾
	fread(&dataSize, 4, 1, fp);
	printf("3\n");
	//WAVE�w�b�_�[�̓ǂ�
	fread(&res32, 4, 1, fp);
	if (res32 != 0x45564157) {	//"WAVE"
		return 2;	//error 2
	}
	printf("4\n");
	while (flag != 3) {
		//�`�����N�̓ǂ�
		printf("5\n");
		fread(&res32, 4, 1, fp);
		printf("6\n");
		fread(&chunkSize, 4, 1, fp);
		printf("7\n");
		printf("res32 = %d\n",res32);
		switch (res32) {
		case 0x20746d66:	//"fmt "
			//format �ǂݍ���
			//PCM��ނ̎擾
			printf("8\n");
			fread(&res16, 2, 1, fp);
			printf("9\n");
			if (res16 != 1) {
				//��Ή��t�H�[�}�b�g
				return 4;
			}
			printf("10\n");
			//���m����(1)or�X�e���I(2)
			fread(&channelCnt, 2, 1, fp);
			printf("11\n");
			if (res16 > 2) {
				//�`�����l�����ԈႢ
				return 5;
			}
			printf("12\n");
			//�T���v�����O���[�g
			fread(&samplingRate, 4, 1, fp);
			printf("13\n");
			//�f�[�^���x(byte/sec)=�T���v�����O���[�g*�u���b�N�T�C�Y
			fread(&byteParSec, 4, 1, fp);
			printf("14\n");
			//�u���b�N�T�C�Y(byte/sample)=�`�����l����*�T���v��������̃o�C�g��
			fread(&blockSize, 2, 1, fp);
			printf("15\n");
			//�T���v���������bit��(bit/sample)�F8 or 16
			fread(&bitParSample, 2, 1, fp);
			printf("16\n");

			*channel = (int)channelCnt;
			*bit = (int)bitParSample;
			*freq = samplingRate;

			flag += 1;
			printf("17\n");
			printf("flag = %d\n",flag);

			break;
		case  0x61746164:	//"data"
			printf("18\n");
			*size = chunkSize;
			printf("19\n");
			dataPos = ftell(fp);
			printf("20\n");
			flag += 2;
			break;
		case  0x61640000:	//1633943552,voice�f�[�^����data�Ɋ��蓖�Ă��鐔�����قȂ��Ă���
			printf("18\n");
			*size = chunkSize +10;/////
			printf("19\n");
			dataPos = ftell(fp);
			printf("20\n");
			flag += 2;
			break;
		default : break;//�ǉ�
		}

	}

	//���o��("fmt "��"data"����ɂ������ꍇ�̂ݓ���)
	if (dataPos != ftell(fp)) {
		fseek(fp, dataPos, SEEK_SET);
	}

	return 0;
}


//---------------------------------------------------------
//�O���t�@�C����ǂݍ��݁A�����̐ݒ���s���֐�(music�p)
//---------------------------------------------------------
void SetAudioMusicFromFile(const char* _FileName, int _music) {

	//�����t�@�C�����̐擪�ɁA.wav���܂Ƃ߂Ă���t�@�C���̃p�X��ǉ�����
	//�ǉ�
	char filePathChar[256] = "sound\\";
	sprintf_s(filePathChar, "%s%s", filePathChar, _FileName);
	_FileName = filePathChar;
	//�I���

	//wav�̎��샍�[�h
	music[_music].fp = fopen(_FileName, "rb");

	if (ReadHeaderWav(music[_music].fp, &music[_music].wavChannel, &music[_music].wavBit, &music[_music].wavSize, &music[_music].wavFreq)) {
		printf("format_error!\n");
		return;
	}

	//�o�b�t�@�̍쐬
	alGenBuffers(1, &music[_music].buffer);

	//�f�[�^�ǂݍ���
	music[_music].data = new unsigned char[music[_music].wavSize];
	fread(music[_music].data, music[_music].wavSize, 1, music[_music].fp);

	//�o�b�t�@�ւ̃f�[�^�o�^
	if (music[_music].wavChannel == 1) {
		if (music[_music].wavBit == 8) {
			alBufferData(music[_music].buffer, AL_FORMAT_MONO8, music[_music].data, music[_music].wavSize, music[_music].wavFreq);
		}
		else {
			alBufferData(music[_music].buffer, AL_FORMAT_MONO16, music[_music].data, music[_music].wavSize, music[_music].wavFreq);
		}
	}
	else {
		if (music[_music].wavBit == 8) {
			alBufferData(music[_music].buffer, AL_FORMAT_STEREO8, music[_music].data, music[_music].wavSize, music[_music].wavFreq);
		}
		else {
			alBufferData(music[_music].buffer, AL_FORMAT_STEREO16, music[_music].data, music[_music].wavSize, music[_music].wavFreq);
		}
	}

	alGenSources(1, &music[_music].source);
	alSourcei(music[_music].source, AL_BUFFER, music[_music].buffer);
	alSourcei(music[_music].source, AL_LOOPING, AL_TRUE);
	alSourcef(music[_music].source, AL_GAIN, 0.1f);

	fclose(music[_music].fp);
}

//------------------------------------------------------
//�O���t�@�C����ǂݍ��݁A�����̐ݒ���s���֐�(se�p)
//------------------------------------------------------
void SetAudioSeFromFile(const char* _FileName, int _se) {

	//�����t�@�C�����̐擪�ɁA.wav���܂Ƃ߂Ă���t�@�C���̃p�X��ǉ�����
	//�ǉ�
	char filePathChar[256] = "sound\\";
	sprintf_s(filePathChar, "%s%s", filePathChar, _FileName);
	_FileName = filePathChar;
	//�I���
	
	//wav�̎��샍�[�h
	se[_se].fp = fopen(_FileName, "rb");

	if (ReadHeaderWav(se[_se].fp, &se[_se].wavChannel, &se[_se].wavBit, &se[_se].wavSize, &se[_se].wavFreq)) {
		printf("format_error!\n");
		return;
	}

	//�o�b�t�@�̍쐬
	alGenBuffers(1, &se[_se].buffer);

	//�f�[�^�ǂݍ���
	se[_se].data = new unsigned char[se[_se].wavSize];
	fread(se[_se].data, se[_se].wavSize, 1, se[_se].fp);

	//�o�b�t�@�ւ̃f�[�^�o�^
	if (se[_se].wavChannel == 1) {
		if (se[_se].wavBit == 8) {
			alBufferData(se[_se].buffer, AL_FORMAT_MONO8, se[_se].data, se[_se].wavSize, se[_se].wavFreq);
		}
		else {
			alBufferData(se[_se].buffer, AL_FORMAT_MONO16, se[_se].data, se[_se].wavSize, se[_se].wavFreq);
		}
	}
	else {
		if (se[_se].wavBit == 8) {
			alBufferData(se[_se].buffer, AL_FORMAT_STEREO8, se[_se].data, se[_se].wavSize, se[_se].wavFreq);
		}
		else {
			alBufferData(se[_se].buffer, AL_FORMAT_STEREO16, se[_se].data, se[_se].wavSize, se[_se].wavFreq);
		}
	}

	alGenSources(1, &se[_se].source);
	alSourcei(se[_se].source, AL_BUFFER, se[_se].buffer);
	alSourcef(se[_se].source, AL_GAIN, 0.07f);

	fclose(se[_se].fp);
}


//------------------------------------------------------
//�O���t�@�C����ǂݍ��݁A�����̐ݒ���s���֐�(voice�p)
//------------------------------------------------------
void SetAudioVoiceFromFile(const char* _FileName, int _voice) {

	//�����t�@�C�����̐擪�ɁA.wav���܂Ƃ߂Ă���t�@�C���̃p�X��ǉ�����
	//�ǉ�
	char filePathChar[256] = "sound\\";
	sprintf_s(filePathChar, "%s%s", filePathChar, _FileName);
	_FileName = filePathChar;
	//�I���

	voice[_voice].fp = fopen(_FileName, "rb");

	if (ReadHeaderWav(voice[_voice].fp, &voice[_voice].wavChannel, &voice[_voice].wavBit, &voice[_voice].wavSize, &voice[_voice].wavFreq)) {
		printf("format_error!\n");
		return;
	}

	//�o�b�t�@�̍쐬
	alGenBuffers(1, &voice[_voice].buffer);
	
	//�f�[�^�ǂݍ���
	voice[_voice].data = new unsigned char[voice[_voice].wavSize];
	
	fread(voice[_voice].data, voice[_voice].wavSize, 1, voice[_voice].fp);
	
	//�o�b�t�@�ւ̃f�[�^�o�^
	if (voice[_voice].wavChannel == 1) {
		if (voice[_voice].wavBit == 8) {
			alBufferData(voice[_voice].buffer, AL_FORMAT_MONO8, voice[_voice].data, voice[_voice].wavSize, voice[_voice].wavFreq);
		}
		else {
			alBufferData(voice[_voice].buffer, AL_FORMAT_MONO16, voice[_voice].data, voice[_voice].wavSize, voice[_voice].wavFreq);
		}
	}
	else {
		if (voice[_voice].wavBit == 8) {
			alBufferData(voice[_voice].buffer, AL_FORMAT_STEREO8, voice[_voice].data, voice[_voice].wavSize, voice[_voice].wavFreq);
		}
		else {
			alBufferData(voice[_voice].buffer, AL_FORMAT_STEREO16, voice[_voice].data, voice[_voice].wavSize, voice[_voice].wavFreq);
		}
	}

	alGenSources(1, &voice[_voice].source);
	alSourcei(voice[_voice].source, AL_BUFFER, voice[_voice].buffer);
	alSourcef(voice[_voice].source, AL_GAIN, 0.07f);

	fclose(voice[_voice].fp);
}


//------------------------
//����������
//------------------------
int audioInit() {
	ALCdevice* device = alcOpenDevice(NULL);
	if (device == NULL) {
		return 1;
	}

	ALCcontext* context = alcCreateContext(device, NULL);
	if (context == NULL) {
		return 1;
	}

	alcMakeContextCurrent(context);

	SetAudioMusicFromFile("stage2.wav", MUSIC_STAGE);
	SetAudioMusicFromFile("Star.wav", MUSIC_STAR);
	SetAudioMusicFromFile("UnderGroundStage.wav", MUSIC_UNDER_GROUND_STAGE);

	SetAudioSeFromFile("GetCoin.wav", SE_GET_COIN);
	SetAudioSeFromFile("GetMushroom.wav", SE_GET_MUSHROOM);
	SetAudioSeFromFile("1Up.wav", SE_1UP);
	SetAudioSeFromFile("block_hit.wav", SE_BLOCK_HIT);
	SetAudioSeFromFile("block_break.wav", SE_BLOCK_BREAK);
	SetAudioSeFromFile("StepOn.wav", SE_STEP_ON);
	SetAudioSeFromFile("FireBall.wav", SE_FIRE_BALL);
	SetAudioSeFromFile("EnemyDeath.wav", SE_ENEMY_DEATH);
	SetAudioSeFromFile("InPipe.wav", SE_IN_PIPE);
	SetAudioSeFromFile("Clear.wav", SE_CLEAR);

	SetAudioVoiceFromFile("Jump1.wav", VOICE_JUMP_1);

	return 0;

}


void audioPlay(int _index) {
	switch (_index) {
	case 1: alSourcePlay(music[MUSIC_STAGE].source); break;
	case 2: alSourcePlay(se[SE_GET_COIN].source); break;
	case 3: alSourcePlay(se[SE_BLOCK_BREAK].source); break;
	case 4: alSourcePlay(se[SE_BLOCK_HIT].source); break;
	case 5: alSourcePlay(se[SE_STEP_ON].source); break;
	case 6: alSourcePlay(se[SE_GET_MUSHROOM].source); break;
	case 7: alSourcePlay(voice[VOICE_JUMP_1].source); break;
	case 8: alSourcePlay(voice[VOICE_JUMP_2].source); break;
	case 9: alSourcePlay(se[SE_CLEAR].source); break;
	case 10: alSourcePlay(se[SE_FIRE_BALL].source); break;
	case 11: alSourcePlay(se[SE_ENEMY_DEATH].source); break;
	case 12: alSourcePlay(se[SE_IN_PIPE].source); break;
	case 13: alSourcePlay(se[SE_1UP].source); break;
	case 14: alSourcePlay(music[MUSIC_STAR].source); break;
	case 15: alSourcePlay(music[MUSIC_UNDER_GROUND_STAGE].source); break;
	default: break;
	}
	
}

void audioStop(int _index) {
	switch(_index) {
	case 1: alSourceStop(music[MUSIC_STAGE].source); break;
	case 2: alSourceStop(se[SE_GET_COIN].source); break;
	case 3: alSourceStop(se[SE_BLOCK_BREAK].source); break;
	case 4: alSourceStop(se[SE_BLOCK_HIT].source); break;
	case 5: alSourceStop(se[SE_STEP_ON].source); break;
	case 6: alSourceStop(se[SE_GET_MUSHROOM].source); break;
	case 7: alSourceStop(voice[VOICE_JUMP_1].source); break;
	case 8: alSourceStop(voice[VOICE_JUMP_2].source); break;
	case 9: alSourceStop(se[SE_CLEAR].source); break;
	case 10: alSourceStop(se[SE_FIRE_BALL].source); break;
	case 11: alSourceStop(se[SE_ENEMY_DEATH].source); break;
	case 12: alSourceStop(se[SE_IN_PIPE].source); break;
	case 13: alSourceStop(se[SE_1UP].source); break;
	case 14: alSourceStop(music[MUSIC_STAR].source); break;
	case 15: alSourceStop(music[MUSIC_UNDER_GROUND_STAGE].source); break;
	default: break;
	}
	
}


