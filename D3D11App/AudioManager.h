#pragma once

#include <memory>
#include <unordered_map>
#include "DirectXTK/Audio.h"

#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment(lib, "lib/DirectXTKAudioWin8D.lib")
#else
	#pragma comment(lib, "lib/DirectXTKAudioWin8.lib")
#endif

class AudioManager {
public:
	AudioManager();
	void update();
	void load(const std::string& filePath, const std::string& alias);
	void playMulti(const std::string& alias, float volume = 1.0f, float pitch = 0.0f);
	void play(const std::string& alias, bool loop = false, float volume = 1.0f, float pitch = 0.0f);
	void pause(const std::string& alias);
	void stop(const std::string& alias);
	//void fadein(const std::string& alias);
	//void fadeout(const std::string& alias);
private:
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffect>> m_soundEffects;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> m_soundEffectsInstance;
};
