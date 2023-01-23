#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* _spriteCommon, uint32_t textureIndex) {

	HRESULT result{};
	assert(_spriteCommon);
	spriteCommon = _spriteCommon;

	//�e�N�X�`���T�C�Y���C���[�W�ɂ��킹��
	if (textureIndex_ != UINT32_MAX)
	{
		this->textureIndex_ = textureIndex_;
		AdjustTextureSize();
		//�e�N�X�`���T�C�Y���X�v���C�g�̃T�C�Y�ɓK�p
		size_ = textureSize_;
	}
	//UV
	{
		ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex_);

		if (textureBuffer) {

			//�e�N�X�`�����擾
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
	//���W
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

		// ���_�f�[�^
		vertices[LB].pos ={left,bottom,  0.0f}; // ����
		vertices[LT].pos = {left,top,     0.0f}; // ����
		vertices[RB].pos = {right,bottom, 0.0f}; // �E��
		vertices[RT].pos = {right,top,    0.0f }; // �E��
	}
	

	// ���_�f�[�^
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));


	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};   // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���_�o�b�t�@�̐���
	result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// �]��
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];   // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);



	//���_�o�b�t�@�r���[
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);
	//�}�e���A��
	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;   //GPU�ւ̓]���p
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
		assert(SUCCEEDED(result));

		// �萔�o�b�t�@�̃}�b�s���O
		result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
		assert(SUCCEEDED(result));

		constMapMaterial->color = color_;              // RGBA�Ŕ������̐�

	}



	//�s��
	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;   //GPU�ւ̓]���p
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));

		// �萔�o�b�t�@�̃}�b�s���O
		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // �}�b�s���O
		assert(SUCCEEDED(result));
		//���[���h
		XMMATRIX matWorld;
		matWorld= XMMatrixIdentity();

		rotationZ_ = 0.0f;
		position_ = { 0.0f,0.0f};

		//��]
		XMMATRIX matRot;//��]�s��
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ_));//Z���܂���45�x��]���Ă���
		matWorld *= matRot;

		//���s�ړ�
		XMMATRIX matTrans;
		matTrans = XMMatrixTranslation(position_.x, position_.y,0.0f);
		matWorld *= matTrans;

		//�ˉe�ϊ��s��(�������e)
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

			//�e�N�X�`�����擾
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
	//���W
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

		// ���_�f�[�^
		vertices[LB].pos = { left,bottom,  0.0f }; // ����
		vertices[LT].pos = { left,top,     0.0f }; // ����
		vertices[RB].pos = { right,bottom, 0.0f }; // �E��
		vertices[RT].pos = { right,top,    0.0f }; // �E��
	}

	

	// �]��
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];   // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	constMapMaterial->color = color_;

	//���[���h
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();


	//��]
	XMMATRIX matRot;//��]�s��
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ_));//Z���܂���45�x��]���Ă���
	matWorld *= matRot;

	//���s�ړ�
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position_.x, position_.y,0.0f);
	matWorld *= matTrans;

	//�ˉe�ϊ��s��(�������e)
	XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
		0.f, WinApp::window_width,
		WinApp::window_height, 0.f,
		0.0f, 1.0f);

	constMapTransform->mat = matWorld * matProjection;
}


void Sprite::Draw() {

	//��\��
	if (isInvisible_) {
		return;
	}

	//�e�N�X�`���R�}���h
	spriteCommon->SetTextureCommand(textureIndex_);

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�萔
	spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// �`��R�}���h
	spriteCommon->GetDirectXCommon()->GetCommandList()->DrawInstanced(4,1, 0, 0);// �S�Ă̒��_���g���ĕ`��


}
void Sprite::AdjustTextureSize()
{
	ID3D12Resource* textureBuffer = spriteCommon->GetTextureBuffer(textureIndex_);
	assert(textureBuffer);

	//�e�N�X�`�����
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);

}