#include "PlayScene.h"

#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Effect/ParticleManager.h"
#include "Framework/Engine.h"
#include "Object/Model/Model.h"
#include "Object/Sprite/Sprite.h"
#include "Effect/Emitter.h"

PlayScene::~PlayScene() {
}

void PlayScene::Initialize() {
    //UserInit
    textureManager = TextureManager::GetInstance();
    textureManager->Load("uvChecker.png");
    textureManager->Load("monsterBall.png");
    textureManager->Load("circle.png");

    modelManager = ModelManager::GetInstance();
    modelManager->Load("plane");
    modelManager->Load("terrain");

    //sprite = std::make_unique<Sprite>(SpriteCommon::GetInstance().get());
    //sprite->Initialize("uvChecker.png");

    model = std::make_unique<Model>(ModelCommon::GetInstance());
    model->Initialize();
    model->SetMesh("terrain");

    emitter_ = ParticleManager::GetInstance()->Emit(
        {1,1,1,
        0,0,0,
        0, 0, 10
        });
}

void PlayScene::Update() {
    //sprite->Update();
    model->Update();
    //emitter_->Update();
}

void PlayScene::Draw() {
    model->Draw();
    //emitter_->Draw();
    //sprite->Draw();
}
