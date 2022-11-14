#pragma once
#include <Windows.h>
class WinApp
{
 public:  //�ÓI�����o�ϐ�
	 static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

 public:
	///<summary>
	///������
	/// </summary>
	void Initialize();

	

	//getter
	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance()const { return w.hInstance; }

	//�I��
	void Finalize();

	//���b�Z�[�W�̏���
	bool ProcessMessage();


public:  //�萔
	//�E�B���h�E����
	static const int window_width = 1280;
	//�E�B���h�E�c��
	static const int window_height = 720;


private:  //�����o�ϐ�
	//�E�B���h�E�n���h��
	HWND hwnd = nullptr;

	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
};

