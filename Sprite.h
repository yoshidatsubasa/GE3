#pragma once
#include<DirectXMath.h>
#include"SpriteCommon.h"
class Sprite
{

private:
	

	struct ConstBufferDataMaterial
	{
		DirectX::XMFLOAT4 color;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	struct ConstBufferDataTransform
	{
		DirectX::XMMATRIX mat;
		
	};

	//頂点番号
	enum VertexNumber {
		LB,  //左下
		LT,  //左上
		RB,  //右下
		RT,  //右上
	};

public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize(SpriteCommon*_spriteCommon,uint32_t textureIndex);

	///<summary>
	///更新
	/// </summary>
	void Update();

	///<summary>
	///描画
	/// </summary>
	void Draw();

	//Getter
	const DirectX::XMFLOAT4 GetColor() const { return color_; }

	const uint32_t GetTextureIndex()const { return textureIndex_; }

	const DirectX::XMFLOAT2& GetPosition()const { return position_; }

	const float& GetRotationZ() const { return rotationZ_; }

	const DirectX::XMFLOAT2& GetSize()const { return size_; }

	const DirectX::XMFLOAT2& GetAnchorPoint()const { return anchorPoint_; }

	const DirectX::XMFLOAT2& GettextureLeftTop()const { return textureLeftTop_; }

	const DirectX::XMFLOAT2& GettextureSize()const { return textureSize_; }

	const bool& GetFlipX()const { return isFlipX_; }
	const bool& GetFlipY()const { return isFlipY_; }

	const bool& GetInvisible()const { return isInvisible_; }


	//Setter
	void SetColor(DirectX::XMFLOAT4 color) { this->color_ = color; }

	void SetTextureIndex(uint32_t index) { this->textureIndex_ = index; }

	void SetPosition(const DirectX::XMFLOAT2& position) { this->position_ = position; }

	void SetRotationZ(const float& rotationZ) { this->rotationZ_ = rotationZ; }

	void SetSize(const DirectX::XMFLOAT2& size) { this->size_ = size; }

	void SetAnchorPoint(const DirectX::XMFLOAT2& anchorPoint) { this->anchorPoint_ = anchorPoint; }

	void SettextureLeftTop(const DirectX::XMFLOAT2& textureLeftTop) { this->textureLeftTop_ = textureLeftTop; }

	void SettextureSize(const DirectX::XMFLOAT2& textureSize) { this->textureSize_ = textureSize; }

	void SetFlipX(const bool& isFlipX) { this->isFlipX_ = isFlipX; }
	void SetFlipY(const bool& isFlipY) { this->isFlipY_ = isFlipY; }

	void SetInvisible(const bool& invisible) { this->isInvisible_= invisible; }
private:
	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

private:
	//スプライト共通部分
	SpriteCommon* spriteCommon = nullptr;

	DirectX::XMFLOAT4 color_= {1,1,1,1};

	//テクスチャ番号
	uint32_t textureIndex_ = 1;
	
	DirectX::XMFLOAT2 position_={0.0f,0.0f};
	float rotationZ_ = 0.0f;

	//表示サイズ
	DirectX::XMFLOAT2 size_ = { 100.0f,100.0f };

	//アンカーポイント
	DirectX::XMFLOAT2 anchorPoint_ = { 0.0f,0.0f };

	//テクスチャ情報
	DirectX::XMFLOAT2 textureLeftTop_ = { 0.0f,0.0f };
	DirectX::XMFLOAT2 textureSize_ = { 100.0f,100.0f };


	//左右フリップ
	bool isFlipX_ = false;
	bool isFlipY_ = false;

	//非表示フラグ
	bool isInvisible_ = false;

	//頂点データ
	Vertex vertices[4];

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

