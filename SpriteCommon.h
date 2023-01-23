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
	//// �������s�N�Z����
	//const size_t textureWidth = 256;
	//// �c�����s�N�Z����
	//const size_t textureHeight = 256;
	//// �z��̗v�f��
	//const size_t imageDataCount = textureWidth * textureHeight;
public:
	///<summary>
	///������
	/// </summary>
	void Initialize(DirectXCommon*_dxCommon);


	///<summary>
	///�`��
	/// </summary>
	void PreDraw();

	void PostDraw();

	///<summary>
	///�e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture(uint32_t index, const std::string& filename);
	///<summary>
	///�`��p�e�N�X�`���R�}���h�̔��s
	/// </summary>
	void SetTextureCommand(uint32_t index);

	//Getter
	DirectXCommon* GetDirectXCommon() {return dxCommon;}

	ID3D12Resource* GetTextureBuffer(uint32_t index) { return texBuff[index].Get(); }
private://�ÓI�����o�ϐ�
	 // SRV�̍ő��
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefalutTextureDirectoryPath;

private://�����o�ϐ�
	DirectXCommon* dxCommon;

	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	//���[�g�V�O�l�N�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

	//�摜
	//DirectX::XMFLOAT4* imageData;
	//�e�N�X�`���o�b�t�@
	std::array< Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount>texBuff;

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>srvHeap;
};

