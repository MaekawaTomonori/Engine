#include "Adapter.h"

void Adapter::Initialize() {
    engine_ = std::make_unique<Engine>();
    engine_->Initialize();

	/*
     * Enable EngineDebugMode
     * エンジンの動作を確認するためのデバッグモードを有効にします
     * デバッグモード有効時、ゲームの処理が呼び出されません
     * 有効になっている場合はコメントアウトもしくは削除してください
     */
	engine_->EnableDebug();
}

void Adapter::Update() const {
    engine_->Update();
}

void Adapter::Draw() const {
    engine_->Draw();
}

void Adapter::EndFrame() const {
    engine_->EndFrame();
}

void Adapter::Finalize() const {
    engine_->Finalize();
}

bool Adapter::IsActive() const {
    return engine_->IsActive();
}

bool Adapter::IsDebug() const {
    return engine_->IsDebug();
}

void Adapter::SetFPSLimit(const int fps) const {
    engine_->SetFPSLimit(fps);
}

void Adapter::SetTitle(const std::string& title) const {
    engine_->SetTitle(title);
}
