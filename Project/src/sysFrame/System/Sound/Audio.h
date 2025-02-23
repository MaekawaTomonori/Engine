#pragma once
#include <memory>
#include <unordered_map>
#include <mutex>
#include <string>

#include "miniaudio/miniaudio.h"

class AudioManager{
    AudioManager() = default;
    ~AudioManager();
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    static AudioManager* instance_;
    static std::once_flag onceFlag_;

    static void Create();
    static void Finalize();

    std::unique_ptr<ma_engine> engine_;
    std::unordered_map<uint32_t, std::string> soundFilePaths;
    std::unordered_map<uint32_t, std::unique_ptr<ma_sound>> soundInstances;
    uint32_t nextHandle = 1;
    std::mutex mutex;

    std::string folderPath_ = "Assets/Sound/";

    void Play(uint32_t handle);
    void Stop(uint32_t handle);
    void Pause(uint32_t handle);
    void Resume(uint32_t handle);
    void SetVolume(uint32_t handle, float volume);
    void SetPitch(uint32_t handle, float pitch);
	void SetLoop(uint32_t handle, bool loop);

    public:
    class SoundHandle{
        public:
        SoundHandle(uint32_t handle, AudioManager* manager);

        SoundHandle& SetVolume(float volume);
        SoundHandle& SetPitch(float pitch);
        SoundHandle& Loop(bool loop);
        void Play() const;
        void Stop() const;
        void Pause() const;
        void Resume() const;

        private:
        uint32_t handle; 
        AudioManager* manager;
    };

    void Initialize();
    SoundHandle Load(const std::string& filePath);

    static AudioManager* GetInstance();
};
