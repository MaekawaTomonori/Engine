#include "Framework.h"

#include "System/System.h"

void Framework::Run() {
    System::Log(Logger::Level::INFO, "Framework Run");
	engine = std::make_unique<Engine>();
    //engine->SetTitle("LE2C_22_マエカワ_トモノリ_ぼっくすしょっと");
    engine->Initialize();

    scene_ = std::make_unique<SceneManager>();

    /*
     * Enable EngineDebugMode
     * エンジンの動作を確認するためのデバッグモードを有効にします
     * デバッグモード有効時、ゲームの処理が呼び出されません
     * 有効になっている場合はコメントアウトもしくは削除してください
     */
	//engine->EnableDebug();
    //engine->SetBackColor({0,0,0,1});

	Initialize();


    //MainLoop
    while (engine->IsActive()){
        //Update
        engine->Update();

        if (!engine->IsDebug()){
            Update();
            scene_->Update();
        }

        engine->Draw();
        if (!engine->IsDebug()){
            Draw();
            scene_->Draw();
        }

        engine->EndFrame();
    }

    Finalize();
	engine->Finalize();
}
