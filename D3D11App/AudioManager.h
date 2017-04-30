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
	void play(const std::string& alias, float volume = 1.0f, float pitch = 0.0f);
private:
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffect>> m_soundEffects;
};
