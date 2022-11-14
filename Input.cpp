#include "Input.h"
#include<cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//using namespace Microsoft::WRL;

void Input::Initialize(WinApp * winApp) {

    HRESULT result;
    
    //借りてきたWinAppのインスタンスを記録
    this->winApp = winApp;

    // DirectInputの初期化
   // ComPtr<IDirectInput8> directInput;
    result = DirectInput8Create(
        this->winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    // 入力データ形式のセット
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));
    // 排他制御レベルのセット
    result = keyboard->SetCooperativeLevel(
        this->winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));

   
}

void Input::Update() {

    //前回のキー入力を保存
    memcpy(keyPre, key, sizeof(key));

    // キーボード情報の取得開始
     keyboard->Acquire();
    // 全キーの入力状態を取得する
     keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::Pushkey(BYTE keyNumber) {
    //指定キーを押していればtrueを返す
    if (key[keyNumber])
    {
        return true;
    }
    //そうでなければfalseを返す
    return false;
}


bool Input::Triggerkey(BYTE keyNumber) {

    //指定キーを押していればtrueを返す
    if (key[keyNumber]&& !keyPre[keyNumber])
    {
        return true;
    }
    //そうでなければfalseを返す
    return false;
}