#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <d3dx12.h>

class Model
{
public: //静的メンバ関数
	//OBJファイルから3Dモデルを読み込む
	static Model* LoadFromOBJ();

	//setter
	static void SetDevice(ID3D12Device* device) { Model::device = device; }

public: //サブクラス
	//頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	//マテリアル
	struct Material
	{
		std::string name;   //マテリアル名
		DirectX::XMFLOAT3 ambient;   //アンビエント影響度
		DirectX::XMFLOAT3 diffuse;   //ディフューズ影響度
		DirectX::XMFLOAT3 specular;  //スペキュラー影響度
		float alpha;        //アルファ
		std::string textureFilename;  //テクスチャファイル名
		//コンストラクタ
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private: //メンバ変数
	//頂点インデックス
	std::vector<unsigned short>indices;

	//テクスチャ読み込み
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	

private: //非公開のメンバ関数
	//OBJファイルから3Dモデルを読み込む(非公開)
	void LoadFromInternal();

private:  //静的メンバ変数
	//デバイス
	static ID3D12Device* device;

};

