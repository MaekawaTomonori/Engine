#include "EngineDebug.h"

#include "Engine.h"
#include "Object/Particle/ParticleManager.h"
#include "System/Loader.h"
#include "System/Input/Input.h"
#include "System/Sound/Audio.h"

EngineDebug::EngineDebug(Engine* engine): engine_(engine) {
}

void EngineDebug::Initialize() {
    Loader::Texture("c.png");
    //Loader::LoadTexture("MonsterBall.png");
    //Loader::LoadTexture("circle.png");

    Loader::Model("terrain");

    //Loader::Audio("Alarm01.wav");

    key = ParticleManager::GetInstance()->RegisterKey(ParticleDimension::PARTICLE2D, "c.png");
    ParticleManager::GetInstance()->EditConfig(key).
        SetCount(1).
        SetPosition({640,360}).
        SetDirection({0,0}).
        SetRandomizeDirection(false).
        SetSpeed({0,0}).
        SetGravity({0,0}).
        SetLifetime(static_cast<int>(10000000000)).
        SetMin({0,0}).
        SetMax({10,01}).
        SetColor({1,1,1,1}).
        SetRandomizeColor(false).
		Emit();

    model_ = std::make_unique < Model >();
    model_->Initialize();
    model_->SetMesh("terrain");
    model_->SetTransform({
        {1, 1, 1},
        {0, 3.14f, 0 },
        {}
    });
}

void EngineDebug::Update() {
    if (Input::GetInstance()->TriggerKey(DIK_F11)){
        engine_->ToggleFullscreen();
    }

    if (Input::GetInstance()->TriggerKey(DIK_RETURN)){
        System::Log("Log");
    }

    model_->Update();
}

void EngineDebug::Draw() const {
    model_->Draw();
}

void EngineDebug::Finalize() {
    //sprite_.reset();
    //model_.reset();
}
