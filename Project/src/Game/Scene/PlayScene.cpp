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

    modelManager = ModelManager::GetInstance();
    modelManager->Load("plane.obj");
    modelManager->Load("axis.obj");

	sprite = std::make_unique<Sprite>(SpriteCommon::GetInstance().get());
    sprite->Initialize("uvChecker.png");

    //model = std::make_unique<Model>(ModelCommon::GetInstance().get());
    //model->Initialize();
    //model->SetMesh("plane.obj");

    emitter_ = ParticleManager::GetInstance()->Emit();
}

void PlayScene::Update() {
	sprite->Update();
    //model->Update();
    emitter_->Update();
}

void PlayScene::Draw() {
	//model->Draw();
    emitter_->Draw();
	sprite->Draw();
}
