//Windows��SAPI5�܂���Microsoft Speech Platform��C++�ŉ����F�����g�p����T���v���B
//
//���T���v���̓�����e�͈ȉ��̒ʂ�B
//1.�������͂ɕW�����̓f�o�C�X��ݒ�B
//2.�ݒ肳��Ă���F���G���W�����E�v���t�@�C�����E���̓f�o�C�X����\���B
//3.�F������\������R�[���o�b�N�֐���ݒ�B
//4.�������͂����b�Z�[�W�E���[�v�ő҂��󂯁A�R�[���o�b�N�ŔF������\����������B
//5.�u�I���v�Ƃ����������܂ޕ����񂪗���΃��[�v��ł��؂�I���B
//
//�����̑��G�L�B
//�E�J�����́uWin7x64�v�ƁuVC++2010�v�B
//�E�uWin32 �R���\�[�� �A�v���P�[�V�����v�B
//�EUnicode�����Z�b�g(SAPI5�̊֐��Q��Unicode���g�p���邽�߃}���`�o�C�g�����Z�b�g���g���ꍇ�͕ϊ����K�v�ɂȂ�)�B
//�E�usapi.h�v���v���W�F�N�g�Ɋ܂߂�K�v������B
//�E�T���v���ł��邽��SAPI5�֐��Q�̖߂�lhResult�̃`�F�b�N�ƃG���[�����͏ȗ����Ă���B
//
//http://denspe.blog84.fc2.com/blog-entry-191.html

//�Â��`���̊֐��̎g�p�x����\�����Ȃ��v���O�}�B
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h> //_wtoi()�֐����g�����߂ɃC���N���[�h
#include <windows.h>
#include <tchar.h>
#include <locale.h>

#include "sapi.h"


WCHAR* Grobal_pResultText = NULL; //MovwCommand�֐��Ŏg�����߂ɃO���[�o���ϐ��Ƃ��ē��͉������i�[����ϐ����`����B
//bool voice_command_flag = true; //�����F���ɂ�鑬�x�̉��Z����񂾂��s�����߂̃t���O
bool NewHorizontalVoiceInput = false; //�����F���ɂ�鐅�������̑��x�̉��Z����񂾂��s�����߂̃t���O
bool NewVerticalVoiceInput = false; //�����F���ɂ�鐂�������̑��x�̉��Z����񂾂��s�����߂̃t���O


//�J�n����CoInitialize�A�I������CoUninitialize���邾���̃N���X�B
//COM�R���|�[�l���g���g�p����A�v���P�[�V�����̍ŏ��Ŏ��̂��m�ۂ��Ă������ƂŁA�I������CoUninitialize��s�v�ɂ���B
//CComPtr���̎�������n�V�X�e���̎g�p���ɂ́A�V�X�e�����eCOM�R���|�[�l���g����������������
//CoUninitialize�����s�����悤�ɂ��Ȃ���΂Ȃ�Ȃ����߁A�X�R�[�v�𕪂���K�v������B
class CAutoCoInitialize
{
public:
	HRESULT hResult;
	//�R���X�g���N�^�B
	CAutoCoInitialize() :hResult(S_FALSE) { hResult = CoInitialize(NULL); }
	//�f�X�g���N�^�B
	~CAutoCoInitialize() { CoUninitialize(); hResult = S_FALSE; }
} AutoCoInitialize;

//COM�I�u�W�F�N�g�̎���������J�v�Z�����������́B
//atlbase.h�ɂ���CComQIPtr�̗򉻔ŁBatlbase.h���g�p���Ȃ����ߎ������̂�����B
template <class T> class AutoCom
{
public:
	//�������Ă��Ȃ��@�\�͎��̂����̂܂܎g���B
	T* pCom;

	//�W���̃R���X�g���N�^�B
	AutoCom(void) :pCom(NULL) {}
	//�������p�R���X�g���N�^(�R�s�[�E�R���X�g���N�^�Ăяo��)�B
	AutoCom(T* pSrcCom) :pCom(NULL) { operator=(pSrcCom); }
	//�f�X�g���N�^�B
	~AutoCom(void) { if (pCom) { pCom->Release(); pCom = NULL; } }

	//////////////////////
	//�L���E�����̊m�F�B//
	//////////////////////
	bool IsAvailable(void) { return pCom ? true : false; }
	bool IsNull(void) { return pCom ? false : true; }

	///////////////////////////////////////////////////
	//COM�|�C���^�Ɠ����Ɉ�����悤�ɂ��邽�߂̒�`�B//
	///////////////////////////////////////////////////
	operator T* (void) { return pCom; }
	operator T** (void) { return &pCom; }
	operator T& (void) { return *pCom; }
	T& operator*(void) { return *pCom; }
	T** operator&(void) { if (pCom) { pCom->Release(); pCom = NULL; }return &pCom; }
	T* operator->(void) { return pCom; }
	T* operator=(T* pSrcCom) { if (pSrcCom == pCom)return pCom; if (pCom)pCom->Release(); pCom = pSrcCom; return pCom; }

	////////////////////////////
	//���̑���Ɨp�֐��̒�`�B//
	////////////////////////////
	//COM�I�u�W�F�N�g�����B
	HRESULT Create(GUID ClsID, GUID IID = __uuidof(T))
	{
		if (pCom) { pCom->Release(); pCom = NULL; }
		HRESULT hResult = CoCreateInstance(ClsID, NULL, CLSCTX_ALL, IID, (LPVOID*)&pCom);
		if (FAILED(hResult))pCom = NULL;
		return hResult;
	}
};

//�F���C�x���g�������ɌĂяo�����R�[���o�b�N�֐��B
//�R�[���o�b�N�ݒ莞�ɑ�����wParam�Ƃ��ĉ����F���Ǘ��p�I�u�W�F�N�gISpRecoContext�̃A�h���X���n�����悤�ɂ��Ă����B
void __stdcall NotifyCallback(WPARAM wParam, LPARAM lParam)
{
	//�����F���Ǘ��p�I�u�W�F�N�gISpRecoContext�̎��̂̓��C���֐��ɂ��邽�ߎg�p��ɉ������K�v�͂Ȃ��B
	ISpRecoContext* pRecoContext = (ISpRecoContext*)wParam;
	//���b�Z�[�W���[�v�I���̖ڈ�B
	bool IsQuit = false;

	//�C�x���g�L���[�ɒ~�ς��ꂽ�C�x���g�̏����B
	SPEVENT SpEvent; memset(&SpEvent, 0, sizeof(SPEVENT));
	ULONG Fetched = 0;
	//�C�x���g�������ɕK���R�[���o�b�N�����̂ł���΁A�����ł͈�̃C�x���g�̂ݏ�������ΊԂɍ����͂������A
	//�m�F���ʓ|���������ߕ����̃C�x���g�����݂���\�������邱�Ƃ�O��Ɏ擾�s�\�ɂȂ�܂Ń��[�v�ŏ������Ă���B
	while (SUCCEEDED(pRecoContext->GetEvents(1, &SpEvent, &Fetched)) && Fetched) //�����ɃR�����g�A�E�g���Ă݂�->���߂���
	{
		switch (SpEvent.eEventId)
		{
		case SPEI_RECOGNITION:
			//�uSpEvent.elParamType==SPET_LPARAM_IS_OBJECT�v�̂Ƃ��uSpEvent.lParam�v�Ɍ���(ISpRecoResult)�������Ă���B
			if (SpEvent.elParamType == SPET_LPARAM_IS_OBJECT)
			{
				//�uSpEvent.lParam�v�����ʃI�u�W�F�N�gISpRecoResult�Ƃ��Ĉ����B
				ISpRecoResult* pResult = (ISpRecoResult*)SpEvent.lParam;
				//ISpRecoResult��GetText�֐��ŔF�����ʂ𕶎���Ŏ擾�ł���B
				WCHAR* pResultText = NULL;
				//Grobal_pResultText = NULL;
				if (SUCCEEDED(pResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &pResultText, NULL)))
				{
					Grobal_pResultText = pResultText; //�����Œ�`�������ϐ��ɂ��i�[
					
					wprintf(L"%s\n", pResultText);
					//�u�I���v�̕������܂܂��΃C�x���g�������[�v�I����ɔF�����[�v���I���B
					if (wcsstr(pResultText, L"�I��"))IsQuit = true;
					//�擾����������͉������K�v������B
					CoTaskMemFree(pResultText); pResultText = NULL;
				}
			}
			break;
		default:
			break;
		}

		//�C�x���g�ʒm�̂��߂�SAPI5���������œ��I�m�ۂ����e����͎󂯎�葤�̐ӔC�ŉ������K�v������B
		//����́usphelper.h�v���Œ�`����Ă���w���p�[�֐�SpClearEvent�ōs���邪
		//�usphelper.h�v��ATL���Ȃ����ł͎g�p�ł��Ȃ����߁A�K�v�ȏ��������ƂňڐA����B
		//�ȉ��usphelper.h�v��SpClearEvent�֐��̓��e���ڐA�B
		if (SpEvent.elParamType != SPEI_UNDEFINED)
		{
			if (SpEvent.elParamType == SPET_LPARAM_IS_POINTER || SpEvent.elParamType == SPET_LPARAM_IS_STRING)
				CoTaskMemFree((void*)SpEvent.lParam);
			else if (SpEvent.elParamType == SPET_LPARAM_IS_TOKEN || SpEvent.elParamType == SPET_LPARAM_IS_OBJECT)
				((IUnknown*)SpEvent.lParam)->Release();
		}
		memset(&SpEvent, 0, sizeof(SPEVENT));
		//�ȏ�usphelper.h�v��SpClearEvent�֐��̓��e���ڐA�B
	}

	//�F�����ʂɁu�I���v�̕������܂܂�Ă���ΔF�����[�v�I���B
	//�X���b�h�̉B���E�C���h�E��WM_QUIT�𑗂邱�ƂŃ��C���֐��ɂ��郁�b�Z�[�W�E���[�v��ł��؂�B
	if (IsQuit)PostMessage(NULL, WM_QUIT, 0, 0);
}


int voice_recognition() 
{
	//���{��̕\���ɕK�v�ȏ����B
	_tsetlocale(LC_ALL, _T(""));
	//SAPI5�֘A�̖߂�l���󂯂�B
	//����̃T���v���ł͂����󂯂邾���œ��e�͖������Ă��邪�{���̓G���[���������s���K�v������B
	HRESULT hResult = S_OK;

	/////////////////////////
	//ISpRecognizer�̐����B//
	/////////////////////////
	//ISpRecognizer�͔F���G���W�����Ǘ�����I�u�W�F�N�g�B
	AutoCom<ISpRecognizer> acRecognizer;
	acRecognizer.Create(CLSID_SpInprocRecognizer);

	////////////////////
	//�������͂̐ݒ�B//
	////////////////////
	//InProc(�񋤗L)�^�C�v�̉����F���V�X�e���͕W���ł͓��̓f�o�C�X�����ݒ�ł��邽�߁A�K��������ݒ肷��K�v������B

	//���̓f�o�C�X�ꗗ���Ǘ�����I�u�W�F�N�g�̐����B
	AutoCom<ISpObjectTokenCategory> acAudioInputCategory;
	acAudioInputCategory.Create(CLSID_SpObjectTokenCategory);
	//���̓f�o�C�X�ꗗ�̐����ɐ�������΂�������W�����̓f�o�C�XID���擾�ł���B
	if (acAudioInputCategory.IsAvailable())
	{
		//���̓f�o�C�X�ꗗ�L�^�ʒu�̐ݒ�(���MSSP11�ŉ���SAPI5)�B
		//hResult=acAudioInputCategory->SetId(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech Server\\v11.0\\AudioInput",TRUE);
		hResult = acAudioInputCategory->SetId(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech\\AudioInput", TRUE);
		//�W�����̓f�o�C�XID�̎擾�B
		WCHAR* pDefaultAudioInputId = NULL;
		if (FAILED(acAudioInputCategory->GetDefaultTokenId(&pDefaultAudioInputId)))pDefaultAudioInputId = NULL;

		//�W�����̓f�o�C�XID����W�����̓f�o�C�X�𐶐����ݒ�B
		if (pDefaultAudioInputId)
		{
			//�W�����̓f�o�C�X�𐶐��B
			AutoCom<ISpObjectToken> acAudioInputToken;
			acAudioInputToken.Create(CLSID_SpObjectToken);
			acAudioInputToken->SetId(NULL, pDefaultAudioInputId, FALSE);
			//���������W�����̓f�o�C�X����̓f�o�C�X�Ƃ��Đݒ�B
			hResult = acRecognizer->SetInput(acAudioInputToken, TRUE);
			//�擾����������͉������K�v������B
			CoTaskMemFree(pDefaultAudioInputId); pDefaultAudioInputId = NULL;
		}
	}

	////////////////////
	//�e����̕\���B//
	////////////////////
	AutoCom<ISpObjectToken> acObjectToken;
	WCHAR* pObjectTokenName = NULL;

	//�g�p���̔F���G���W�����擾�B
	if (SUCCEEDED(acRecognizer->GetRecognizer(&acObjectToken)))
	{
		//�F���G���W���̎擾�ɐ�������ΔF���G���W�������擾�B
		//GetStringValue�֐���K�v�ȏ�񖼂�^�����ɌĂяo���ƁA�W���l(�����ł͔F���G���W����)���Ԃ�B
		if (SUCCEEDED(acObjectToken->GetStringValue(NULL, &pObjectTokenName)) && pObjectTokenName)
		{
			//�F���G���W���̎擾�ɐ�������Ε\���B
			wprintf(L"�F���G���W��:%s\n", pObjectTokenName);
			//�擾����������͉������K�v������B
			CoTaskMemFree(pObjectTokenName); pObjectTokenName = NULL;
		}
	}

	//�g�p���̃v���t�@�C�����擾�B
	if (SUCCEEDED(acRecognizer->GetRecoProfile(&acObjectToken)))
	{
		//�v���t�@�C���̎擾�ɐ�������΃v���t�@�C�������擾�B
		//GetStringValue�֐���K�v�ȏ�񖼂�^�����ɌĂяo���ƁA�W���l(�����ł̓v���t�@�C����)���Ԃ�B
		if (SUCCEEDED(acObjectToken->GetStringValue(NULL, &pObjectTokenName)) && pObjectTokenName)
		{
			//�v���t�@�C���̎擾�ɐ�������Ε\���B
			wprintf(L"�v���t�@�C��:%s\n", pObjectTokenName);
			//�擾����������͉������K�v������B
			CoTaskMemFree(pObjectTokenName); pObjectTokenName = NULL;
		}
	}

	//�g�p���̓��̓f�o�C�X���擾�B
	if (SUCCEEDED(acRecognizer->GetInputObjectToken(&acObjectToken)))
	{
		//���̓f�o�C�X�̎擾�ɐ�������Γ��̓f�o�C�X�����擾�B
		//GetStringValue�֐���K�v�ȏ�񖼂�^�����ɌĂяo���ƁA�W���l(�����ł͓��̓f�o�C�X��)���Ԃ�B
		if (SUCCEEDED(acObjectToken->GetStringValue(NULL, &pObjectTokenName)) && pObjectTokenName)
		{
			//���̓f�o�C�X�̎擾�ɐ�������Ε\���B
			wprintf(L"���̓f�o�C�X:%s\n", pObjectTokenName);
			//�擾����������͉������K�v������B
			CoTaskMemFree(pObjectTokenName); pObjectTokenName = NULL;
		}
	}

	////////////////////////////////
	//ISpRecoContext�̐����Ɛݒ�B//
	////////////////////////////////
	//ISpRecoContext�͉����F���Ǘ��p�I�u�W�F�N�g�B
	//�F���G���W���Ǘ��I�u�W�F�N�gISpRecognize���q�̕��@�Ǘ��I�u�W�F�N�gISpRecoGrammar�𓝊���
	//�����F�����p�҂͂���ISpRecoContext�I�u�W�F�N�g��ʂ��ĔF�����ʂ��擾����B
	//�F���G���W���Ǘ��I�u�W�F�N�gISpRecognize�Ɩ��ڂɌ��т��Ă���A���̃I�u�W�F�N�g���P�̂ő��݂��邱�Ƃ͂Ȃ��B
	//���̃I�u�W�F�N�g�𒼐ڐ��������ꍇ�ɂ͓����ŕW���̔F���G���W�����Ǘ�����ISpRecognize�����������B
	//�܂��A��x���̃I�u�W�F�N�g�����������ƁA
	//���ꂪ����ISpRecognize���Ǘ�����F���G���W����ISpRecognize���̂������ւ��邱�Ƃ͂ł��Ȃ��Ȃ邽�߁A
	//�F������(�G���W��)��ύX����ۂɂ͂��̃I�u�W�F�N�g����蒼���K�v������B
	AutoCom<ISpRecoContext> acRecoContext;
	acRecognizer->CreateRecoContext(&acRecoContext);
	//�R�[���o�b�N�֐��̐ݒ�B
	//�Ăяo���ꂽ��Ŏg����悤WPARAM�Ƃ��Ď������g�̃A�h���X��n���悤�ɐݒ�B
	hResult = acRecoContext->SetNotifyCallbackFunction(NotifyCallback, (WPARAM)acRecoContext.pCom, 0);
	//�ǂ̃C�x���g�����������ꍇ�ɒʒm�����邩��ݒ�B�uSPEI_RECOGNITION�v�͔F���������B
	hResult = acRecoContext->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));

	////////////////////////////////
	//ISpRecoGrammar�̐����Ɛݒ�B//
	////////////////////////////////
	//ISpRecoGrammar�͉����F�����@�Ǘ��p�I�u�W�F�N�g�B
	//���炩���ߓo�^�����P��(����)�̂ǂꂪ��ԋ߂����H��F������u�R�}���h�F���v���[�h�̏ꍇ��
	//�����ł��̃I�u�W�F�N�g�ɑ΂��ăR�}���h�̓o�^�Ȃǂ��s���B
	//���R�ɘb�������e�𕶎��񉻂���u���q�M�L�v���[�h�g�p���ɂ͓��ɂ�邱�Ƃ͂Ȃ��B
	AutoCom<ISpRecoGrammar> acRecoGrammar;
	acRecoContext->CreateGrammar(NULL, &acRecoGrammar);
	//�����̓ǂݍ��݁B�ǂݍ��ݑΏۖ��w��(NULL)�ň�ʓI�Ȏ���(�u���q�M�L�v���[�h�p�̎���)���ǂݍ��܂��B
	hResult = acRecoGrammar->LoadDictation(NULL, SPLO_STATIC);
	//�ǂݍ��񂾎����̊������J�n������B����͎����I�ɂ͔F�������J�n�̍��}�ƂȂ�B
	hResult = acRecoGrammar->SetDictationState(SPRS_ACTIVE);

	//////////////////////
	//���b�Z�[�W�̏����B//
	//////////////////////
	//SAPI5�̃R�[���o�b�N�͓����ɐ��������B���E�C���h�E�ɂ���Ď�������Ă��邽��
	//�E�C���h�E�E���b�Z�[�W����������K�v������(�X���[�v�֐��Ȃǂő҂��󂯂Ă��Ă��Ăяo����邱�Ƃ��Ȃ�)�B
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) //�`�悪�~�܂�̂͂�����while�����������ۂ�
	{
		if (bRet == -1)break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		
		if (Grobal_pResultText != NULL) {
			printf("Grobal_pResultText %d\n", _wtoi(Grobal_pResultText)); //�G���[�o��
			NewHorizontalVoiceInput = true;
			NewVerticalVoiceInput = true;
		}
		
	}


	//////////
	//�I���B//
	//////////
	//�F�����ʂɁu�I���v�̕������܂܂�Ă���΃��b�Z�[�W�E���[�v���ł��؂��Ă����ɗ���B
	return EXIT_SUCCESS;
}

int SetVoiceCommand() {
	if (Grobal_pResultText != NULL) { //����if�����Ȃ���null�|�C���^�ŃG���[���N����
			return _wtoi(Grobal_pResultText);
	}
}

bool GetNewHorizontalVoiceInput() {
	return NewHorizontalVoiceInput;
}

void SetNewHorizontalVoiceInput(bool _NewHorizontalVoiceInput) {
	NewHorizontalVoiceInput = _NewHorizontalVoiceInput;
}

bool GetNewVerticalVoiceInput() {
	return NewVerticalVoiceInput;
}

void SetNewVerticalVoiceInput(bool _NewVerticalVoiceInput) {
	NewVerticalVoiceInput = _NewVerticalVoiceInput;
}