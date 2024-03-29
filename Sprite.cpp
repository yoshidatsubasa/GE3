#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* _spriteCommon, uint32_t textureIndex) {

	HRESULT result{};
	assert(_spriteCommon);
	spriteCommon = _spriteCommon;

	//テクスチャサイズをイメージにあわせる
	if (textureIndex_ != UINT32_MAX)
	{
		this->textureIndex_ = textureIndex_;
		AdjustTextureSize();
		//テクスチャサイズをスプライトのサイズに適用
		size_ = textureSize_;
	}
	//UV
	{
		ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex_);

		if (textureBuffer) {

			//テクスチャ情報取得
			D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

			float tex_left = textureLeftTop_.x / resDesc.Width;
			float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
			float tex_top = textureLeftTop_.y / resDesc.Height;
			float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;


			vertices[LB].uv = { tex_left, tex_bottom };
			vertices[LT].uv = { tex_left, tex_top };
			vertices[RB].uv = { tex_right,tex_bottom };
			vertices[RT].uv = { tex_right,tex_top };
		}
	}
	//座標
	{
		float left = (0.0f - anchorPoint_.x) * size_.x;
		float right = (1.0f - anchorPoint_.x) * size_.x;
		float top = (0.0f - anchorPoint_.y) * size_.y;
		float bottom = (1.0f - anchorPoint_.y) * size_.y;

		if (isFlipX_)
		{
			left = -left;
			right = -right;
		}

		if (isFlipY_)
		{
			top = -top;
			bottom = -bottom;
		}

		// 頂点データ
		vertices[LB].pos ={left,bottom,  0.0f}; // 左下
		vertices[LT].pos = {left,top,     0.0f}; // 左上
		vertices[RB].pos = {right,bottom, 0.0f}; // 右下
		vertices[RT].pos = {right,top,    0.0f }; // 右下
	}
	

	// 頂点データ
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));


	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 頂点バッファの生成
	result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 転送
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];   // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);



	//頂点バッファビュー
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);
	//マテリアル
	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;   //GPUへの転送用
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;   // 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
		assert(SUCCEEDED(result));

		// 定数バッファのマッピング
		result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
		assert(SUCCEEDED(result));

		constMapMaterial->color = color_;              // RGBAで半透明の赤

	}



	//行列
	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;   //GPUへの転送用
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;   // 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));

		// 定数バッファのマッピング
		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
		assert(SUCCEEDED(result));
		//ワールド
		XMMATRIX matWorld;
		matWorld= XMMatrixIdentity();

		rotationZ_ = 0.0f;
		position_ = { 0.0f,0.0f};

		//回転
		XMMATRIX matRot;//回転行列
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ_));//Z軸まわりに45度回転してから
		matWorld *= matRot;

		//平行移動
		XMMATRIX matTrans;
		matTrans = XMMatrixTranslation(position_.x, position_.y,0.0f);
		matWorld *= matTrans;

		//射影変換行列(透視投影)
		XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
			0.f,WinApp::window_width,
			WinApp::window_height,0.f,
			0.0f, 1.0f);

		constMapTransform->mat = matWorld*matProjection;

	}

}
void Sprite::Update()
{
	//UV
	{
		ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex_);

		if (textureBuffer) {

			//テクスチャ情報取得
			D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

			float tex_left = textureLeftTop_.x / resDesc.Width;
			float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
			float tex_top = textureLeftTop_.y / resDesc.Height;
			float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;


			vertices[LB].uv = { tex_left, tex_bottom };
			vertices[LT].uv = { tex_left, tex_top };
			vertices[RB].uv = { tex_right,tex_bottom };
			vertices[RT].uv = { tex_right,tex_top };
		}
	}
	//座標
	{
		float left = (0.0f - anchorPoint_.x) * size_.x;
		float right = (1.0f - anchorPoint_.x) * size_.x;
		float top = (0.0f - anchorPoint_.y) * size_.y;
		float bottom = (1.0f - anchorPoint_.y) * size_.y;

		if (isFlipX_)
		{
			left = -left;
			right = -right;
		}

		if (isFlipY_)
		{
			top = -top;
			bottom = -bottom;
		}

		// 頂点データ
		vertices[LB].pos = { left,bottom,  0.0f }; // 左下
		vertices[LT].pos = { left,top,     0.0f }; // 左上
		vertices[RB].pos = { right,bottom, 0.0f }; // 右下
		vertices[RT].pos = { right,top,    0.0f }; // 右下
	}

	

	// 転送
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];   // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	constMapMaterial->color = color_;

	//ワールド
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();


	//回転
	XMMATRIX matRot;//回転行列
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ_));//Z軸まわりに45度回転してから
	matWorld *= matRot;

	//平行移動
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position_.x, position_.y,0.0f);
	matWorld *= matTrans;

	//射影変換行列(透視投影)
	XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
		0.f, WinApp::window_width,
		WinApp::window_height, 0.f,
		0.0f, 1.0f);

	constMapTransform->mat = matWorld * matProjection;
}


void Sprite::Draw() {

	//非表示
	if (isInvisible_) {
		return;
	}

	//テクスチャコマンド
	spriteCommon->SetTextureCommand(textureIndex_);

	// 頂点バッファビューの設定コマンド
	spriteCommon->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//定数
	spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// 描画コマンド
	spriteCommon->GetDirectXCommon()->GetCommandList()->DrawInstanced(4,1, 0, 0);// 全ての頂点を使って描画


}
void Sprite::AdjustTextureSize()
{
	ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex_);
	assert(textureBuffer);

	//テクスチャ情報
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);

}