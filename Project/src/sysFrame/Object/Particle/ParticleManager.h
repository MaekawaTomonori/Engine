#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "System/Math/Vector2.h"

struct EmitterConfig;
class ParticleCommon;
class Emitter2d;

enum class ParticleDimension{
	PARTICLE2D,
	PARTICLE3D
};

struct ParticleKey{
	ParticleDimension dimension;
    std::string texture;

	bool operator==(const ParticleKey& key) const {
        return dimension == key.dimension && texture == key.texture;
	}
};

namespace std {
	template<>
	struct std::hash<ParticleKey> {
		std::size_t operator()(const ParticleKey& key) const noexcept {
			return std::hash<int>()(static_cast<int>(key.dimension)) ^ (std::hash<std::string>()(key.texture) << 1);
		}
	};
}

struct EmitterConfig{
	uint32_t count;
	Vector2 position;

	Vector2 direction;
	bool randomizeDirection;

	// min max
	Vector2 speed;

	Vector2 gravity;

	int lifetime;

	Vector2 min;
	Vector2 max;

	Vector4 color;
	bool randomizeColor;

};
class ParticleManager{
public:
	class ConfigEditor{
		ParticleKey key_;
		EmitterConfig config_{};

		void SetConfig() const;
	public:
		ConfigEditor(ParticleKey key);
		ConfigEditor(ParticleKey key, const EmitterConfig& config);
		ConfigEditor& operator=(const EmitterConfig& config);
		ConfigEditor& SetCount(uint32_t count);
		ConfigEditor& SetPosition(const Vector2& position);
		ConfigEditor& SetDirection(const Vector2& direction);
		ConfigEditor& SetSpeed(const Vector2& speed);
		ConfigEditor& SetGravity(const Vector2& gravity);
		ConfigEditor& SetLifetime(int lifetime);
		ConfigEditor& SetMin(const Vector2& min);
		ConfigEditor& SetMax(const Vector2& max);
		ConfigEditor& SetRandomizeColor(bool randomizeColor);
		ConfigEditor& SetRandomizeDirection(bool randomizeDirection);
		ConfigEditor& SetColor(const Vector4& color);

		void Emit() const;
	};
private:
    DirectXCommon* dxCommon_ = nullptr;
    SRVManager* srvManager_ = nullptr;

	std::unordered_map<ParticleKey, std::unique_ptr<Emitter2d>> particles_;
	std::unordered_map<ParticleKey, std::unique_ptr<ConfigEditor>> configs_;

	std::unique_ptr<ParticleCommon> pCommon_;

private:
	static ParticleManager* instance_;
	static std::once_flag flag_;

	static void Create();
	static void Destroy();

	ParticleManager() = default;
	~ParticleManager() = default;

public:
	
	static ParticleManager* GetInstance();
    ParticleManager(const ParticleManager&) = delete;
    ParticleManager& operator=(const ParticleManager&) = delete;


	void Initialize(DirectXCommon* dxCommon, SRVManager* srvManager);
    void Update() const;
	void Draw() const;

	ParticleKey RegisterKey(ParticleDimension dimension, const std::string& texture);
	ConfigEditor& EditConfig(const ParticleKey& key);
    void Emit(const ParticleKey& key);

private:
    void SetConfig(const ParticleKey& key, const EmitterConfig& config);
	void Register(const ParticleKey& key);
};