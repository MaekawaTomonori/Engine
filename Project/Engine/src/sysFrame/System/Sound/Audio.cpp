#define MINIAUDIO_IMPLEMENTATION
#include "Audio.h"

#include <cassert>
#include <fstream>
#include <threads.h>

#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"


AudioManager* AudioManager::instance_ = nullptr;
std::once_flag AudioManager::onceFlag_;

AudioManager* AudioManager::GetInstance() {
    call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void AudioManager::Create() {
    instance_ = new AudioManager;
    SingletonFinalizer::AddFinalizer(&Finalize);
}

void AudioManager::Initialize() {
	auto* engine = new ma_engine;
    if (ma_engine_init(nullptr, engine) != MA_SUCCESS){
        System::Log(Log::Level::ERR, "Failed to initialize audio engine");
        return;
    }

    engine_.reset(engine);

    System::Log(Log::Level::INFO, "Audio Enable");   
}

void AudioManager::Finalize() {
    delete instance_;
    instance_ = nullptr;
    System::Log(Log::Level::INFO, "Audio Disable");
}


AudioManager::SoundHandle::SoundHandle(uint32_t handle, AudioManager* manager): handle(handle), manager(manager) {
}

AudioManager::SoundHandle& AudioManager::SoundHandle::SetVolume(float volume) {
    manager->SetVolume(handle, volume);
    return *this;
}

AudioManager::SoundHandle& AudioManager::SoundHandle::SetPitch(float pitch) {
    manager->SetPitch(handle, pitch);
    return *this;
}

void AudioManager::SoundHandle::Play() const {
    manager->Play(handle);
}

void AudioManager::SoundHandle::Stop() const {
    manager->Stop(handle);
}

void AudioManager::SoundHandle::Pause() const {
    manager->Pause(handle);
}

void AudioManager::SoundHandle::Resume() const {
    manager->Resume(handle);
}

AudioManager::SoundHandle& AudioManager::SoundHandle::Loop(bool loop) {
	manager->SetLoop(handle, loop);
	return *this;
}

AudioManager::SoundHandle AudioManager::Load(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(mutex);
    const std::string path = folderPath_ + filePath;

    // default
    folderPath_ = "Assets/Sound/";

    uint32_t soundDataHandle = nextHandle++;
    soundFilePaths[soundDataHandle] = path;

    auto soundInstance = std::make_unique<ma_sound>();
    if (ma_sound_init_from_file(engine_.get(), path.c_str(), 0, nullptr, nullptr, soundInstance.get()) != MA_SUCCESS){
        return {0, this};
    }
    uint32_t soundInstanceHandle = nextHandle++;
    soundInstances[soundInstanceHandle] = std::move(soundInstance);

    return {soundInstanceHandle, this};
}

AudioManager::~AudioManager() {
    ma_engine_uninit(engine_.get());
}

void AudioManager::Play(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex);
    const auto itr = soundInstances.find(handle);
    if (itr != soundInstances.end()){
        ma_sound_start(itr->second.get());
    }
}

void AudioManager::Stop(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex);
    const auto itr = soundInstances.find(handle);
    if (itr != soundInstances.end()){
        ma_sound_stop(itr->second.get());
    }
}

void AudioManager::Pause(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex);
    const auto itr = soundInstances.find(handle);
    if (itr != soundInstances.end()){
        ma_sound_stop(itr->second.get());
    }
}

void AudioManager::Resume(uint32_t handle) {
    std::lock_guard<std::mutex> lock(mutex);
    const auto itr = soundInstances.find(handle);
    if (itr != soundInstances.end()){
        ma_sound_start(itr->second.get());
    }
}

void AudioManager::SetVolume(uint32_t handle, float volume) {
    std::lock_guard<std::mutex> lock(mutex);
    const auto itr = soundInstances.find(handle);
    if (itr != soundInstances.end()){
        ma_sound_set_volume(itr->second.get(), volume);
    }
}

void AudioManager::SetPitch(uint32_t handle, float pitch) {
    std::lock_guard<std::mutex> lock(mutex);
    const auto itr = soundInstances.find(handle);
    if (itr != soundInstances.end()){
        ma_sound_set_pitch(itr->second.get(), pitch);
    }
}

void AudioManager::SetLoop(uint32_t handle, bool loop) {
	std::lock_guard<std::mutex> lock(mutex);
	const auto itr = soundInstances.find(handle);
	if (itr != soundInstances.end()) {
		ma_sound_set_looping(itr->second.get(), loop);
	}
}
