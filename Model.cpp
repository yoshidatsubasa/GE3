#include "Model.h"
#include<cassert>
#include<fstream>
#include<sstream>
#include<Windows.h>
#include <DirectXTex.h>


using namespace std;
using namespace DirectX;

//静的メンバ変数の実体
ID3D12Device* Model::device = nullptr;

Model* Model::LoadFromOBJ()
{
	//新たなModel型のインスタンスをnewする
	Model* model = new Model();


	

	model->LoadFromInternal();

	//バッファ生成


	return model;

}

void Model::LoadFromInternal()
{
	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	//file.open("Resources/triangle_tex/triangle_tex.obj");
	const string modelname = "triangle_mat";
	const string filename = modelname + ".obj";//"triangle_mat.obj"
	const string directoryPath = "Resources/" + modelname + "/";//"Resources/triangle_mat/"
	file.open(directoryPath + filename);//"Resources/triangle_mat/triangle_mat.obj"
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}
	vector<XMFLOAT3>positions;//頂点座標
	vector<XMFLOAT3>normals;//法線ベクトル
	vector<XMFLOAT2>texcoords;//テクスチャUV
	//1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		//1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字がmtllibならマテリアル
		if (key == "mtllib")
		{
			//マテリアルファイルの読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		//先頭文字がvなら頂点座標
		if (key == "v") {
			// v X Y Z 
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
			//頂点データに追加
			//VertexPosNormalUv vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);
		}
		//先頭文字がvtならテクスチャ
		if (key == "vt") {
			//U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		//先頭文字がvnなら法線ベクトル
		if (key == "vn") {
			//X,Y,Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		//先頭文字がfならポリゴン(三角形)
		if (key == "f") {
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);//スラッシュを飛ばす
				index_stream >> indexNormal;
				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				//頂点インデックスに追加
				indices.emplace_back((unsigned short)indices.size());
			}
		}

	}
	//ファイルを閉じる
	file.close();

}