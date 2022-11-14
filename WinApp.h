#pragma once
#include <Windows.h>
class WinApp
{
 public:  //静的メンバ変数
	 static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

 public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize();

	

	//getter
	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance()const { return w.hInstance; }

	//終了
	void Finalize();

	//メッセージの処理
	bool ProcessMessage();


public:  //定数
	//ウィンドウ横幅
	static const int window_width = 1280;
	//ウィンドウ縦幅
	static const int window_height = 720;


private:  //メンバ変数
	//ウィンドウハンドル
	HWND hwnd = nullptr;

	//ウィンドウクラスの設定
	WNDCLASSEX w{};
};

