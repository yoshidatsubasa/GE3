#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include"WinApp.h"

class Input
{
public:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	///<summary>
	///������
	/// </summary>
	void Initialize(WinApp*winApp);

	///<summary>
	///�X�V
	/// </summary>
	void Update();

	///<summary>
	///�L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool Pushkey(BYTE keyNumber);
	///<summary>
	///�L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool Triggerkey(BYTE keyNumber);


private:

	//WindowsAPI
	WinApp* winApp = nullptr;

	// �L�[�{�[�h�f�o�C�X�̐���
	ComPtr<IDirectInputDevice8> keyboard;

	//DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> directInput;

	//�S�L�[�̏��
	BYTE key[256] = {};

	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	
};

