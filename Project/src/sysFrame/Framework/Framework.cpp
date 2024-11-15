#include "Framework.h"

void Framework::Initialize() {
    engine_ = std::make_unique<Engine>();
    engine_->Initialize();

    scene_ = std::make_unique<SceneManager>();
}

void Framework::Update() {
    engine_->Update();
    scene_->Update();
    endRequest_ = engine_->IsEndRequest();
}

bool Framework::IsEndRequest() const {
    return endRequest_;
}

void Framework::Run() {
    Initialize();
    while (true){
        Update();
        if (IsEndRequest()){
            break;
        }
        engine_->PreDraw();
        Draw();
        engine_->PostDraw();
    }
}
