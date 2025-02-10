#include "Fade.h"

#include "Object/Sprite/Sprite.h"
#include "System/Loader.h"

void Fade::InitIn() {
    Loader::Texture("BlackFilter.png");
    filterSprite_ = std::make_unique<Sprite>();
    filterSprite_->Initialize("BlackFilter.png");
    filterColor_ = {1.0f,1.0f,1.0f,1.0f};
}

bool Fade::UpdateIn() {
    //明るくなる
    if (filterColor_.w > 0.0f){
        filterColor_.w -= 0.01f;  
    } else{
        filterColor_.w = 0.0f;
    }

    return filterColor_.w > 0.0f;
}

void Fade::InitOut() {
	Loader::Texture("BlackFilter.png");
    filterSprite_ = std::make_unique<Sprite>();
	filterSprite_->Initialize("BlackFilter.png");
	filterColor_ = { 1.0f,1.0f,1.0f,0.0f };
}

bool Fade::UpdateOut() {
    //暗くなる
	if (filterColor_.w < 1.0f) {
        filterColor_.w += 0.01f; 
    } else {
        filterColor_.w = 1.0f;
    }

    return filterColor_.w <= 1.0f;
}

void Fade::Draw() {
    filterSprite_->SetColor(filterColor_);
    filterSprite_->Update();
    filterSprite_->Draw();
}
