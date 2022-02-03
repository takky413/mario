//WindowsのSAPI5またはMicrosoft Speech PlatformとC++で音声認識を使用するサンプル。
//
//■サンプルの動作内容は以下の通り。
//1.音声入力に標準入力デバイスを設定。
//2.設定されている認識エンジン名・プロファイル名・入力デバイス名を表示。
//3.認識文を表示するコールバック関数を設定。
//4.音声入力をメッセージ・ループで待ち受け、コールバックで認識文を表示し続ける。
//5.「終了」という文字を含む文字列が来ればループを打ち切り終了。
//
//■その他雑記。
//・開発環境は「Win7x64」と「VC++2010」。
//・「Win32 コンソール アプリケーション」。
//・Unicode文字セット(SAPI5の関数群はUnicodeを使用するためマルチバイト文字セットを使う場合は変換が必要になる)。
//・「sapi.h」をプロジェクトに含める必要がある。
//・サンプルであるためSAPI5関数群の戻り値hResultのチェックとエラー処理は省略している。
//
//http://denspe.blog84.fc2.com/blog-entry-191.html

//古い形式の関数の使用警告を表示しないプラグマ。
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h> //_wtoi()関数を使うためにインクルード
#include <windows.h>
#include <tchar.h>
#include <locale.h>

#include "sapi.h"


WCHAR* Grobal_pResultText = NULL; //MovwCommand関数で使うためにグローバル変数として入力音声を格納する変数を定義する。
//bool voice_command_flag = true; //音声認識による速度の加算を一回だけ行うためのフラグ
bool NewHorizontalVoiceInput = false; //音声認識による水平方向の速度の加算を一回だけ行うためのフラグ
bool NewVerticalVoiceInput = false; //音声認識による垂直方向の速度の加算を一回だけ行うためのフラグ


//開始時にCoInitialize、終了時にCoUninitializeするだけのクラス。
//COMコンポーネントを使用するアプリケーションの最初で実体を確保しておくことで、終了時にCoUninitializeを不要にする。
//CComPtr等の自動解放系システムの使用時には、システムが各COMコンポーネントを自動解放した後に
//CoUninitializeが実行されるようにしなければならないため、スコープを分ける必要がある。
class CAutoCoInitialize
{
public:
	HRESULT hResult;
	//コンストラクタ。
	CAutoCoInitialize() :hResult(S_FALSE) { hResult = CoInitialize(NULL); }
	//デストラクタ。
	~CAutoCoInitialize() { CoUninitialize(); hResult = S_FALSE; }
} AutoCoInitialize;

//COMオブジェクトの自動解放をカプセル化したもの。
//atlbase.hにあるCComQIPtrの劣化版。atlbase.hを使用しないため似たものを自作。
template <class T> class AutoCom
{
public:
	//実装していない機能は実体をそのまま使う。
	T* pCom;

	//標準のコンストラクタ。
	AutoCom(void) :pCom(NULL) {}
	//初期化用コンストラクタ(コピー・コンストラクタ呼び出し)。
	AutoCom(T* pSrcCom) :pCom(NULL) { operator=(pSrcCom); }
	//デストラクタ。
	~AutoCom(void) { if (pCom) { pCom->Release(); pCom = NULL; } }

	//////////////////////
	//有効・無効の確認。//
	//////////////////////
	bool IsAvailable(void) { return pCom ? true : false; }
	bool IsNull(void) { return pCom ? false : true; }

	///////////////////////////////////////////////////
	//COMポインタと同等に扱えるようにするための定義。//
	///////////////////////////////////////////////////
	operator T* (void) { return pCom; }
	operator T** (void) { return &pCom; }
	operator T& (void) { return *pCom; }
	T& operator*(void) { return *pCom; }
	T** operator&(void) { if (pCom) { pCom->Release(); pCom = NULL; }return &pCom; }
	T* operator->(void) { return pCom; }
	T* operator=(T* pSrcCom) { if (pSrcCom == pCom)return pCom; if (pCom)pCom->Release(); pCom = pSrcCom; return pCom; }

	////////////////////////////
	//その他作業用関数の定義。//
	////////////////////////////
	//COMオブジェクト生成。
	HRESULT Create(GUID ClsID, GUID IID = __uuidof(T))
	{
		if (pCom) { pCom->Release(); pCom = NULL; }
		HRESULT hResult = CoCreateInstance(ClsID, NULL, CLSCTX_ALL, IID, (LPVOID*)&pCom);
		if (FAILED(hResult))pCom = NULL;
		return hResult;
	}
};

//認識イベント発生時に呼び出されるコールバック関数。
//コールバック設定時に第一引数wParamとして音声認識管理用オブジェクトISpRecoContextのアドレスが渡されるようにしておく。
void __stdcall NotifyCallback(WPARAM wParam, LPARAM lParam)
{
	//音声認識管理用オブジェクトISpRecoContextの実体はメイン関数にあるため使用後に解放する必要はない。
	ISpRecoContext* pRecoContext = (ISpRecoContext*)wParam;
	//メッセージループ終了の目印。
	bool IsQuit = false;

	//イベントキューに蓄積されたイベントの処理。
	SPEVENT SpEvent; memset(&SpEvent, 0, sizeof(SPEVENT));
	ULONG Fetched = 0;
	//イベント発生毎に必ずコールバックされるのであれば、ここでは一つのイベントのみ処理すれば間に合うはずだが、
	//確認が面倒だったため複数のイベントが存在する可能性があることを前提に取得不能になるまでループで処理している。
	while (SUCCEEDED(pRecoContext->GetEvents(1, &SpEvent, &Fetched)) && Fetched) //試しにコメントアウトしてみる->だめそう
	{
		switch (SpEvent.eEventId)
		{
		case SPEI_RECOGNITION:
			//「SpEvent.elParamType==SPET_LPARAM_IS_OBJECT」のとき「SpEvent.lParam」に結果(ISpRecoResult)が入っている。
			if (SpEvent.elParamType == SPET_LPARAM_IS_OBJECT)
			{
				//「SpEvent.lParam」を結果オブジェクトISpRecoResultとして扱う。
				ISpRecoResult* pResult = (ISpRecoResult*)SpEvent.lParam;
				//ISpRecoResultのGetText関数で認識結果を文字列で取得できる。
				WCHAR* pResultText = NULL;
				//Grobal_pResultText = NULL;
				if (SUCCEEDED(pResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &pResultText, NULL)))
				{
					Grobal_pResultText = pResultText; //自分で定義した大域変数にも格納
					
					wprintf(L"%s\n", pResultText);
					//「終了」の文字が含まれればイベント処理ループ終了後に認識ループも終了。
					if (wcsstr(pResultText, L"終了"))IsQuit = true;
					//取得した文字列は解放する必要がある。
					CoTaskMemFree(pResultText); pResultText = NULL;
				}
			}
			break;
		default:
			break;
		}

		//イベント通知のためにSAPI5側が内部で動的確保した各種情報は受け取り側の責任で解放する必要がある。
		//これは「sphelper.h」内で定義されているヘルパー関数SpClearEventで行えるが
		//「sphelper.h」はATLがない環境では使用できないため、必要な処理を手作業で移植する。
		//以下「sphelper.h」のSpClearEvent関数の内容を移植。
		if (SpEvent.elParamType != SPEI_UNDEFINED)
		{
			if (SpEvent.elParamType == SPET_LPARAM_IS_POINTER || SpEvent.elParamType == SPET_LPARAM_IS_STRING)
				CoTaskMemFree((void*)SpEvent.lParam);
			else if (SpEvent.elParamType == SPET_LPARAM_IS_TOKEN || SpEvent.elParamType == SPET_LPARAM_IS_OBJECT)
				((IUnknown*)SpEvent.lParam)->Release();
		}
		memset(&SpEvent, 0, sizeof(SPEVENT));
		//以上「sphelper.h」のSpClearEvent関数の内容を移植。
	}

	//認識結果に「終了」の文字が含まれていれば認識ループ終了。
	//スレッドの隠しウインドウにWM_QUITを送ることでメイン関数にあるメッセージ・ループを打ち切る。
	if (IsQuit)PostMessage(NULL, WM_QUIT, 0, 0);
}


int voice_recognition() 
{
	//日本語の表示に必要な処理。
	_tsetlocale(LC_ALL, _T(""));
	//SAPI5関連の戻り値を受ける。
	//今回のサンプルではただ受けるだけで内容は無視しているが本来はエラー処理等を行う必要がある。
	HRESULT hResult = S_OK;

	/////////////////////////
	//ISpRecognizerの生成。//
	/////////////////////////
	//ISpRecognizerは認識エンジンを管理するオブジェクト。
	AutoCom<ISpRecognizer> acRecognizer;
	acRecognizer.Create(CLSID_SpInprocRecognizer);

	////////////////////
	//音声入力の設定。//
	////////////////////
	//InProc(非共有)タイプの音声認識システムは標準では入力デバイスが未設定であるため、必ず何かを設定する必要がある。

	//入力デバイス一覧を管理するオブジェクトの生成。
	AutoCom<ISpObjectTokenCategory> acAudioInputCategory;
	acAudioInputCategory.Create(CLSID_SpObjectTokenCategory);
	//入力デバイス一覧の生成に成功すればそこから標準入力デバイスIDが取得できる。
	if (acAudioInputCategory.IsAvailable())
	{
		//入力デバイス一覧記録位置の設定(上はMSSP11で下がSAPI5)。
		//hResult=acAudioInputCategory->SetId(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech Server\\v11.0\\AudioInput",TRUE);
		hResult = acAudioInputCategory->SetId(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech\\AudioInput", TRUE);
		//標準入力デバイスIDの取得。
		WCHAR* pDefaultAudioInputId = NULL;
		if (FAILED(acAudioInputCategory->GetDefaultTokenId(&pDefaultAudioInputId)))pDefaultAudioInputId = NULL;

		//標準入力デバイスIDから標準入力デバイスを生成し設定。
		if (pDefaultAudioInputId)
		{
			//標準入力デバイスを生成。
			AutoCom<ISpObjectToken> acAudioInputToken;
			acAudioInputToken.Create(CLSID_SpObjectToken);
			acAudioInputToken->SetId(NULL, pDefaultAudioInputId, FALSE);
			//生成した標準入力デバイスを入力デバイスとして設定。
			hResult = acRecognizer->SetInput(acAudioInputToken, TRUE);
			//取得した文字列は解放する必要がある。
			CoTaskMemFree(pDefaultAudioInputId); pDefaultAudioInputId = NULL;
		}
	}

	////////////////////
	//各種情報の表示。//
	////////////////////
	AutoCom<ISpObjectToken> acObjectToken;
	WCHAR* pObjectTokenName = NULL;

	//使用中の認識エンジンを取得。
	if (SUCCEEDED(acRecognizer->GetRecognizer(&acObjectToken)))
	{
		//認識エンジンの取得に成功すれば認識エンジン名を取得。
		//GetStringValue関数を必要な情報名を与えずに呼び出すと、標準値(ここでは認識エンジン名)が返る。
		if (SUCCEEDED(acObjectToken->GetStringValue(NULL, &pObjectTokenName)) && pObjectTokenName)
		{
			//認識エンジンの取得に成功すれば表示。
			wprintf(L"認識エンジン:%s\n", pObjectTokenName);
			//取得した文字列は解放する必要がある。
			CoTaskMemFree(pObjectTokenName); pObjectTokenName = NULL;
		}
	}

	//使用中のプロファイルを取得。
	if (SUCCEEDED(acRecognizer->GetRecoProfile(&acObjectToken)))
	{
		//プロファイルの取得に成功すればプロファイル名を取得。
		//GetStringValue関数を必要な情報名を与えずに呼び出すと、標準値(ここではプロファイル名)が返る。
		if (SUCCEEDED(acObjectToken->GetStringValue(NULL, &pObjectTokenName)) && pObjectTokenName)
		{
			//プロファイルの取得に成功すれば表示。
			wprintf(L"プロファイル:%s\n", pObjectTokenName);
			//取得した文字列は解放する必要がある。
			CoTaskMemFree(pObjectTokenName); pObjectTokenName = NULL;
		}
	}

	//使用中の入力デバイスを取得。
	if (SUCCEEDED(acRecognizer->GetInputObjectToken(&acObjectToken)))
	{
		//入力デバイスの取得に成功すれば入力デバイス名を取得。
		//GetStringValue関数を必要な情報名を与えずに呼び出すと、標準値(ここでは入力デバイス名)が返る。
		if (SUCCEEDED(acObjectToken->GetStringValue(NULL, &pObjectTokenName)) && pObjectTokenName)
		{
			//入力デバイスの取得に成功すれば表示。
			wprintf(L"入力デバイス:%s\n", pObjectTokenName);
			//取得した文字列は解放する必要がある。
			CoTaskMemFree(pObjectTokenName); pObjectTokenName = NULL;
		}
	}

	////////////////////////////////
	//ISpRecoContextの生成と設定。//
	////////////////////////////////
	//ISpRecoContextは音声認識管理用オブジェクト。
	//認識エンジン管理オブジェクトISpRecognizeや後述の文法管理オブジェクトISpRecoGrammarを統括し
	//音声認識利用者はこのISpRecoContextオブジェクトを通して認識結果を取得する。
	//認識エンジン管理オブジェクトISpRecognizeと密接に結びついており、このオブジェクトが単体で存在することはない。
	//このオブジェクトを直接生成した場合には内部で標準の認識エンジンを管理するISpRecognizeが生成される。
	//また、一度このオブジェクトが生成されると、
	//それが持つISpRecognizeが管理する認識エンジンやISpRecognize自体を差し替えることはできなくなるため、
	//認識言語(エンジン)を変更する際にはこのオブジェクトも作り直す必要がある。
	AutoCom<ISpRecoContext> acRecoContext;
	acRecognizer->CreateRecoContext(&acRecoContext);
	//コールバック関数の設定。
	//呼び出された先で使えるようWPARAMとして自分自身のアドレスを渡すように設定。
	hResult = acRecoContext->SetNotifyCallbackFunction(NotifyCallback, (WPARAM)acRecoContext.pCom, 0);
	//どのイベントが発生した場合に通知させるかを設定。「SPEI_RECOGNITION」は認識完了時。
	hResult = acRecoContext->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));

	////////////////////////////////
	//ISpRecoGrammarの生成と設定。//
	////////////////////////////////
	//ISpRecoGrammarは音声認識文法管理用オブジェクト。
	//あらかじめ登録した単語(文章)のどれが一番近いか？を認識する「コマンド認識」モードの場合は
	//ここでこのオブジェクトに対してコマンドの登録などを行う。
	//自由に話した内容を文字列化する「口述筆記」モード使用時には特にやることはない。
	AutoCom<ISpRecoGrammar> acRecoGrammar;
	acRecoContext->CreateGrammar(NULL, &acRecoGrammar);
	//辞書の読み込み。読み込み対象無指定(NULL)で一般的な辞書(「口述筆記」モード用の辞書)が読み込まれる。
	hResult = acRecoGrammar->LoadDictation(NULL, SPLO_STATIC);
	//読み込んだ辞書の活動を開始させる。これは実質的には認識処理開始の合図となる。
	hResult = acRecoGrammar->SetDictationState(SPRS_ACTIVE);

	//////////////////////
	//メッセージの処理。//
	//////////////////////
	//SAPI5のコールバックは内部に生成した隠しウインドウによって実現されているため
	//ウインドウ・メッセージを消化する必要がある(スリープ関数などで待ち受けていても呼び出されることがない)。
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) //描画が止まるのはここのwhile文が原因っぽい
	{
		if (bRet == -1)break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		
		if (Grobal_pResultText != NULL) {
			printf("Grobal_pResultText %d\n", _wtoi(Grobal_pResultText)); //エラー出る
			NewHorizontalVoiceInput = true;
			NewVerticalVoiceInput = true;
		}
		
	}


	//////////
	//終了。//
	//////////
	//認識結果に「終了」の文字が含まれていればメッセージ・ループが打ち切られてここに来る。
	return EXIT_SUCCESS;
}

int SetVoiceCommand() {
	if (Grobal_pResultText != NULL) { //このif文がないとnullポインタでエラーが起きる
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