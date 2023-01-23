#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <d3dx12.h>

using namespace DirectX;

//3D���f��
class Model
{
public:
	//�}�e���A��
	struct Material
	{
		std::string name;//�}�e���A����
		XMFLOAT3 ambient;//�A���r�G���g�e���x
		XMFLOAT3 diffuse;//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;//�X�y�L�����[�e���x
		float alpha;//�A���t�@
		std::string textureFilename;//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;//�A���r�G���g�W��
		float pad1;      //�p�f�B���O
		XMFLOAT3 diffuse;//�f�B�t���[�Y�W��
		float pad2;      //�p�f�B���O
		XMFLOAT3 specular;//�X�y�L�����[�W��
		float alpha;     //�A���t�@
	};

public://�ÓI�����o�֐�
	//OBJ�t�@�C������3D���f����ǂݍ���
	static Model* LoadFromOBJ();

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParamIndexMaterial);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// �e��o�b�t�@����
	/// </summary>
	void CreateBuffers();

private://�����o�ϐ�
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	//�}�e���A��
	Material material;

	//���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;

	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;

	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

	CD3DX12_RESOURCE_DESC resourceDesc1 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);
	
private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;

private://����J�̃����o�֐�
	//OBJ�t�@�C������3D���f����ǂݍ���(����J)
	void LoadFromOBJInternal();
public:
	//setter
	static void SetDevice(ID3D12Device* device) { Model::device = device; }
};