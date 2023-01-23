#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <d3dx12.h>

class Model
{
public: //�ÓI�����o�֐�
	//OBJ�t�@�C������3D���f����ǂݍ���
	static Model* LoadFromOBJ();

	//setter
	static void SetDevice(ID3D12Device* device) { Model::device = device; }

public: //�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv;  // uv���W
	};

	//�}�e���A��
	struct Material
	{
		std::string name;   //�}�e���A����
		DirectX::XMFLOAT3 ambient;   //�A���r�G���g�e���x
		DirectX::XMFLOAT3 diffuse;   //�f�B�t���[�Y�e���x
		DirectX::XMFLOAT3 specular;  //�X�y�L�����[�e���x
		float alpha;        //�A���t�@
		std::string textureFilename;  //�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private: //�����o�ϐ�
	//���_�C���f�b�N�X
	std::vector<unsigned short>indices;

	//�e�N�X�`���ǂݍ���
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	

private: //����J�̃����o�֐�
	//OBJ�t�@�C������3D���f����ǂݍ���(����J)
	void LoadFromInternal();

private:  //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;

};

