#pragma once
#include"DirectXCommon.h"

#include<d3d12.h>
#include<wrl.h>
#include<array>
#include<DirectXMath.h>
#include<string>
class SpriteCommon
{
public:
	//// 横方向ピクセル数
	//const size_t textureWidth = 256;
	//// 縦方向ピクセル数
	//const size_t textureHeight = 256;
	//// 配列の要素数
	//const size_t imageDataCount = textureWidth * textureHeight;
public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize(DirectXCommon*_dxCommon);


	///<summary>
	///描画
	/// </summary>
	void PreDraw();

	void PostDraw();

	///<summary>
	///テクスチャ読み込み
	/// </summary>
	void LoadTexture(uint32_t index, const std::string& filename);
	///<summary>
	///描画用テクスチャコマンドの発行
	/// </summary>
	void SetTextureCommand(uint32_t index);

	//Getter
	DirectXCommon* GetDirectXCommon() {return dxCommon;}

	ID3D12Resource* GetTextureBuffer(uint32_t index) { return texBuff[index].Get(); }
private://静的メンバ変数
	 // SRVの最大個数
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefalutTextureDirectoryPath;

private://メンバ変数
	DirectXCommon* dxCommon;

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//ルートシグネクチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

	//画像
	//DirectX::XMFLOAT4* imageData;
	//テクスチャバッファ
	std::array< Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount>texBuff;

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>srvHeap;
};

