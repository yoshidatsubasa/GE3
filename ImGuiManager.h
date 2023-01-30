#pragma once
#include "DirectXCommon.h"

class ImGuiManager
{
public:
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	void Finalize();

	void Begin();

	void End();

	void Draw();
private:
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	DirectXCommon* dxCommon_ = nullptr;
};
