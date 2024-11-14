#include "Framework.h"

void Framework::Initialize() {
    engine_ = std::make_unique<Engine>();
    engine_->Initialize();
}

void Framework::Update() {
    engine_->Update();
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
        Draw();
    }
}
