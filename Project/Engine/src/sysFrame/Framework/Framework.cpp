#include "Framework.h"

void Framework::Run() {
	engine_ = std::make_unique<Adapter>();
    engine_->Initialize();

    scene_ = std::make_unique<SceneManager>();

	Initialize();

    //MainLoop
    while (engine_->IsActive()){
        //Update
        engine_->Update();

        if (!engine_->IsDebug()){
            Update();
            scene_->Update();
        }

        engine_->Draw();
        if (!engine_->IsDebug()){
            Draw();
            scene_->Draw();
        }

        engine_->EndFrame();
    }

    Finalize();
	engine_->Finalize();
}
