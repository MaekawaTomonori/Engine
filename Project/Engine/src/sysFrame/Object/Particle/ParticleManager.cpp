#include "ParticleManager.h"

#include <cassert>
#include <ranges>

#include "Emitter2d.h"
#include "ParticleCommon.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

ParticleManager* ParticleManager::instance_ = nullptr;
std::once_flag ParticleManager::flag_;

void ParticleManager::ConfigEditor::SetConfig() const {
	GetInstance()->SetConfig(key_, config_);
}

ParticleManager::ConfigEditor::ConfigEditor(ParticleKey key) :key_(std::move(key)) {
	config_ = {
		.count = 1,
		.position = {0, 0},
		.direction = {0, 0},
		.randomizeDirection = false,
		.speed = {0, 0},
		.gravity = {0, 0},
		.lifetime = 50,
		.min = {0, 0},
		.max = {0, 0},
		.color = {1, 1, 1, 1},
		.randomizeColor = false
	};
}

ParticleManager::ConfigEditor::ConfigEditor(ParticleKey key, const EmitterConfig& config):key_(std::move(key)), config_(config) {}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::operator=(const EmitterConfig& config) {
	config_ = config;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetCount(uint32_t count) {
	config_.count = count;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetPosition(const Vector2& position) {
	config_.position = position;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetDirection(const Vector2& direction) {
	config_.direction = direction;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetSpeed(const Vector2& speed) {
	config_.speed = speed;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetGravity(const Vector2& gravity) {
	config_.gravity = gravity;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetLifetime(int lifetime) {
	config_.lifetime = lifetime;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetMin(const Vector2& min) {
	config_.min = min;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetMax(const Vector2& max) {
	config_.max = max;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetRandomizeColor(bool randomizeColor) {
	config_.randomizeColor = randomizeColor;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetRandomizeDirection(bool randomizeDirection) {
	config_.randomizeDirection = randomizeDirection;
	return *this;
}

ParticleManager::ConfigEditor& ParticleManager::ConfigEditor::SetColor(const Vector4& color) {
	config_.color = color;
	return *this;
}

void ParticleManager::ConfigEditor::Emit() const {
	SetConfig();
	GetInstance()->Emit(key_);
}

void ParticleManager::Create() {
    instance_ = new ParticleManager();
    SingletonFinalizer::AddFinalizer(&Destroy);
}

void ParticleManager::Destroy() {
    delete instance_;
    instance_ = nullptr;
    System::Log(Log::Level::INFO, "ParticleManager Disabled");
}

ParticleManager* ParticleManager::GetInstance() {
    std::call_once(flag_, Create);
    assert(instance_);
    return instance_;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SRVManager* srvManager) {
    dxCommon_ = dxCommon;
    srvManager_ = srvManager;

    pCommon_ = std::make_unique<ParticleCommon>(dxCommon_);
    pCommon_->Initialize();
}

void ParticleManager::Update() const {
    for (const auto& particle : particles_ | std::views::values){
        particle->Update();
    }
}

void ParticleManager::Draw() const {
    for (const auto& particle : particles_ | std::views::values){
        particle->Draw();
    }
}

ParticleKey ParticleManager::RegisterKey(ParticleDimension dimension, const std::string& texture) {
    ParticleKey key {dimension, texture};
	Register(key);

    return key;
}

void ParticleManager::SetConfig(const ParticleKey& key, const EmitterConfig& config) {
    particles_[key]->SetConfig(config);
}

ParticleManager::ConfigEditor& ParticleManager::EditConfig(const ParticleKey& key) {
	if (!configs_.contains(key)) {
		Register(key);
	}

	return *configs_[key].get();
}

void ParticleManager::Emit(const ParticleKey& key) {
    particles_[key]->Emit();
}

void ParticleManager::Register(const ParticleKey& key) {
	switch (key.dimension) {
	case ParticleDimension::PARTICLE2D:
		particles_[key] = std::make_unique<Emitter2d>(pCommon_.get(), srvManager_);
		break;
	case ParticleDimension::PARTICLE3D:
		break;
	}
	particles_[key]->Initialize();
	particles_[key]->SetTexture(key.texture);
	configs_[key] = std::make_unique<ConfigEditor>(key);
}
