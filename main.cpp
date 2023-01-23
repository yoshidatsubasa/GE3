#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"

#include"SpriteCommon.h"
#include"Sprite.h"

#include"Object3d.h"
#include"Model.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region 基盤システムの初期化
    WinApp* winApp = nullptr;
    //windowsAPIの初期化
    winApp = new WinApp();
    winApp->Initialize();

    //ポインタ
    DirectXCommon* dxCommon = nullptr;
    dxCommon = new DirectXCommon();
    dxCommon->Initialize(winApp);

    //Input
    Input* input = nullptr;
    input = new Input();
    input->Initialize(winApp);


    //SpriteCommon
    SpriteCommon* spriteCommon = nullptr;
    spriteCommon = new SpriteCommon;
    spriteCommon->Initialize(dxCommon);
    spriteCommon->LoadTexture(0, "texture.png");
    spriteCommon->LoadTexture(1, "reimu.png");


    //3Dオブジェクト静的初期化
    Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

#pragma endregion 基盤システムの初期化

#pragma region 最初のシーン初期化 

    //sprite
    Sprite* sprite = nullptr;
    sprite = new Sprite;
    sprite->Initialize(spriteCommon,1);
    //sprite->SetColor({ 0,1,1,1 });

    //OBJからモデルデータを読み込む
    Model* model_1 = Model::LoadFromOBJ("ground");
    Model* model_2 = Model::LoadFromOBJ("triangle_mat");
    //3Dオブジェクト生成
    Object3d* object3d_1 = Object3d::Create();
    Object3d* object3d_2 = Object3d::Create();
    Object3d* object3d_3 = Object3d::Create();
    //オブジェクトにモデルをひも付ける
    object3d_1->SetModel(model_1);
    object3d_2->SetModel(model_2);
    object3d_3->SetModel(model_2);
    //3Dオブジェクトの位置を指定
    object3d_1->SetPosition({ 0,-50,0 });
    object3d_2->SetPosition({ -5,0,-5 });
    object3d_3->SetPosition({ +5,0,+5 });
    //3Dオブジェクトの大きさを指定
    object3d_1->SetScale({ 10.0f,10.0f,10.0f });
    object3d_2->SetScale({ 10.0f,10.0f,10.0f });
    object3d_3->SetScale({ 10.0f,10.0f,10.0f });
    

#pragma endregion 最初のシーン初期化 
    // ゲームループ
    while (true) {


        //Windowsのメッセージ処理
        if (winApp->ProcessMessage()==true)
        {
            //ゲームループを抜ける
            break;
        }

        input->Update();
       

        DirectX::XMFLOAT2 position = sprite->GetPosition();
        position.x += 0.1f;
        sprite->SetPosition(position);
        sprite->Update();

        //3Dオブジェクト更新
        object3d_1->Update();
        object3d_2->Update();
        object3d_3->Update();

     
       

        //描画前処理
        dxCommon->PreDraw();

#pragma region 最初のシーンの描画    
        spriteCommon->PreDraw();
        sprite->Draw();
        spriteCommon->PostDraw();

        //3Dオブジェクト描画前処理
        Object3d::PreDraw(dxCommon->GetCommandList());

        //3Dオブジェクトの描画
        object3d_1->Draw();
        object3d_2->Draw();
        object3d_3->Draw();

        //3Dオブジェクト描画後処理
        Object3d::PostDraw();
#pragma endregion  最初のシーンの描画 

       //描画後処理
        dxCommon->PostDraw();
        
    }


#pragma region 最初のシーン終了
    delete sprite;
    sprite = nullptr;
#pragma endregion 

#pragma region 基盤システムの終了
    delete spriteCommon;
    spriteCommon = nullptr;

    delete input;
    input = nullptr;

    delete dxCommon;
    dxCommon = nullptr;

    
    //3Dモデル解放
    delete model_1;
    delete model_2;
    //3Dオブジェクト解放
    delete object3d_1;
    delete object3d_2;
    delete object3d_3;

   //WindowsAPIの終了処理
    winApp->Finalize();
    //WindowsAPIの解放
    delete winApp;
    winApp = nullptr;
#pragma endregion 基盤システムの終了
    return 0;
}
