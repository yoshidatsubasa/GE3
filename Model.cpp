#include "Model.h"
#include<cassert>
#include<fstream>
#include<sstream>
#include<Windows.h>
#include <DirectXTex.h>


using namespace std;
using namespace DirectX;

//�ÓI�����o�ϐ��̎���
ID3D12Device* Model::device = nullptr;

Model* Model::LoadFromOBJ()
{
	//�V����Model�^�̃C���X�^���X��new����
	Model* model = new Model();


	

	model->LoadFromInternal();

	//�o�b�t�@����


	return model;

}

void Model::LoadFromInternal()
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	//file.open("Resources/triangle_tex/triangle_tex.obj");
	const string modelname = "triangle_mat";
	const string filename = modelname + ".obj";//"triangle_mat.obj"
	const string directoryPath = "Resources/" + modelname + "/";//"Resources/triangle_mat/"
	file.open(directoryPath + filename);//"Resources/triangle_mat/triangle_mat.obj"
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}
	vector<XMFLOAT3>positions;//���_���W
	vector<XMFLOAT3>normals;//�@���x�N�g��
	vector<XMFLOAT2>texcoords;//�e�N�X�`��UV
	//1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷���Ȃ�
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			//�}�e���A���t�@�C���̓ǂݍ���
			string filename;
			line_stream >> filename;
			//�}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}

		//�擪������v�Ȃ璸�_���W
		if (key == "v") {
			// v X Y Z 
			//X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
			//���_�f�[�^�ɒǉ�
			//VertexPosNormalUv vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);
		}
		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt") {
			//U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn") {
			//X,Y,Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}
		//�擪������f�Ȃ�|���S��(�O�p�`)
		if (key == "f") {
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//�X���b�V�����΂�
				index_stream >> indexNormal;
				//���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				//���_�C���f�b�N�X�ɒǉ�
				indices.emplace_back((unsigned short)indices.size());
			}
		}

	}
	//�t�@�C�������
	file.close();

}