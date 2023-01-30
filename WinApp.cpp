#include "WinApp.h"
#include "imgui/imgui_impl_win32.h"

#pragma comment(lib,"winmm.lib") 

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    //ImGui�p�E�B���h�E�v���V�[�W���Ăяo��
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;
    //���b�Z�[�W�ŕ���
    switch (msg) {
    case WM_DESTROY:  //�E�B���h�E���j�����ꂽ
        PostQuitMessage(0);  //OS�ɑ΂��āA �A�v���̏I����`����
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);  //�W���̏������s��
}
void WinApp::Initialize()
{

    // �E�B���h�E�N���X�̐ݒ�
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
    w.lpszClassName = L"DirectXGame"; // �E�B���h�E�N���X��
    w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

    // �E�B���h�E�N���X��OS�ɓo�^����
    RegisterClassEx(&w);
    // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
    RECT wrc = { 0, 0, window_width, window_height };
    // �����ŃT�C�Y��␳����
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // �E�B���h�E�I�u�W�F�N�g�̐���
    hwnd = CreateWindow(w.lpszClassName, // �N���X��
        L"DirectXGame",         // �^�C�g���o�[�̕���
        WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
        CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
        CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
        wrc.right - wrc.left,       // �E�B���h�E����
        wrc.bottom - wrc.top,   // �E�B���h�E�c��
        nullptr,                // �e�E�B���h�E�n���h��
        nullptr,                // ���j���[�n���h��
        w.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);               // �I�v�V����

    // �E�B���h�E��\����Ԃɂ���
    ShowWindow(hwnd, SW_SHOW);
    //�V�X�e���^�C�}�[�̕���x���グ��
    timeBeginPeriod(1);
}




void WinApp::Finalize()
{
    // �E�B���h�E�N���X��o�^����
    UnregisterClass(w.lpszClassName, w.hInstance);
}

bool WinApp::ProcessMessage()
{
    MSG msg{};  // ���b�Z�[�W
    // ���b�Z�[�W������H
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
        DispatchMessage(&msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
    }
    // ?�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
    if (msg.message == WM_QUIT) {
        return true;
    }
    return false;
}