#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"

#include"SpriteCommon.h"
#include"Sprite.h"

#include"Object3d.h"


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

    //3Dオブジェクト生成
    Object3d* object3d = Object3d::Create();

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

        object3d->Update();

       /* DirectX::XMFLOAT2 size = sprite->GetSize();
        size.y += 1.0f;
        sprite->SetSize(size);*/
       

        //描画前処理
        dxCommon->PreDraw();

#pragma region 最初のシーンの描画    
        spriteCommon->PreDraw();
        sprite->Draw();
        spriteCommon->PostDraw();

        //3Dオブジェクト描画前処理
        Object3d::PreDraw(dxCommon->GetCommandList());

        //3Dオブジェクトの描画
        object3d->Draw();

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

    delete object3d;

   //WindowsAPIの終了処理
    winApp->Finalize();
    //WindowsAPIの解放
    delete winApp;
    winApp = nullptr;
#pragma endregion 基盤システムの終了
    return 0;
}
