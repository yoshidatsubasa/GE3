#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include"WinApp.h"
#include <vector>
#include<chrono>

class DirectXCommon
{
public:
	///<summary>
	///������
	/// </summary>
	void Initialize(WinApp*winApp);
	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();
	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDraw();

	//getter
	//�f�o�C�X�擾
	ID3D12Device* GetDevice()const { return device.Get(); }

	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); }

private:
	///<summary>
	///�f�o�C�X������
	/// </summary>
	void InitializeDevice();
	/// <summary>
	/// �R�}���h�֘A�̏�����
	/// </summary>
	void InitilizeCommand();

    /// <summary>
    /// 
    /// </summary>
	void InitilizeSwapchain();
	
	void InitilizeRenderTargetView();
	
	void InitilizeDepthBuffer();

	void InitilizeFence();

	void InitilizeFixFPS();

	void UpdateFixFPS();

private:

	//WindowsAPI
	WinApp* winApp = nullptr;

	//DirettX12�̃f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//DirctX
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	//�R�}���h�֘A
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	//�X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//�����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//�t�F���X
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	//�L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;
};

