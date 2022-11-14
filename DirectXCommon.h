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
	///初期化
	/// </summary>
	void Initialize(WinApp*winApp);
	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();
	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	//getter
	//デバイス取得
	ID3D12Device* GetDevice()const { return device.Get(); }

	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); }

private:
	///<summary>
	///デバイス初期化
	/// </summary>
	void InitializeDevice();
	/// <summary>
	/// コマンド関連の初期化
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

	//DirettX12のデバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//DirctX
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	//コマンド関連
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	//スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;
};

