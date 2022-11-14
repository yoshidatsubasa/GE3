#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#include"WinApp.h"

class Input
{
public:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize(WinApp*winApp);

	///<summary>
	///更新
	/// </summary>
	void Update();

	///<summary>
	///キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool Pushkey(BYTE keyNumber);
	///<summary>
	///キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool Triggerkey(BYTE keyNumber);


private:

	//WindowsAPI
	WinApp* winApp = nullptr;

	// キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboard;

	//DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;

	//全キーの状態
	BYTE key[256] = {};

	//前回の全キーの状態
	BYTE keyPre[256] = {};

	
};

