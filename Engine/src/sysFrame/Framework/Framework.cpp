#include "Framework.h"

#include "System/System.h"

void Framework::Run() {
	engine = std::make_unique<Engine>();
    engine->Initialize();

    /*
     * Enable EngineDebugMode
     * エンジンの動作を確認するためのデバッグモードを有効にします
     * 通常の動作をさせる場合はコメントアウトしてください
     * デバッグモード有効時、ゲームの処理が呼び出されません
     */
    engine->EnableDebug();

	Initialize();
    
    //MainLoop
    while (engine->IsActive()){
        //Update
        engine->Update();

        if (!engine->IsDebug()){
            Update();
        }

        engine->Draw();
        if (!engine->IsDebug()){
            Draw();
        }

        engine->EndFrame();
    }

    Finalize();
    engine->Finalize();
}
