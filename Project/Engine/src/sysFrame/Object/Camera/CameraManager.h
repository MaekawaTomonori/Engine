#pragma once
#include <mutex>
#include <string>
#include <unordered_map>

#include "Camera.h"

class CameraManager{
    static CameraManager* instance_;
    static std::once_flag onceFlag_;

    std::unordered_map<std::string, std::unique_ptr<Camera>> cameras_;
    Camera* active_ = nullptr;

    // ImGui 
    uint16_t noname = 0;
    bool initialize = false;
    int currentIndex = 0;
	std::vector<std::string> names;

public:
	static CameraManager* GetInstance();

    void Initialize();
    void Update();

    Camera* GetCamera() const;

    Camera* Add(const std::string& name);
    Camera* Active(const std::string& name);

private:
	CameraManager() = default;
	~CameraManager();
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

	static void Create();
    static void Destroy();

    void Load();
    void Save();
};


