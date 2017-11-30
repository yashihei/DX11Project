// -----------------------------------------------
// File : AudioManager.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <memory>
#include <unordered_map>
#include <DirectXTK/Audio.h>

class AudioManager {
public:
	AudioManager();
	void update();
	void load(const std::string& filePath, const std::string& alias);
	void playMulti(const std::string& alias, float volume = 1.0f, float pitch = 0.0f);
	void play(const std::string& alias, bool loop = false, float volume = 1.0f, float pitch = 0.0f);
	void pause(const std::string& alias);
	void stop(const std::string& alias);
	void setVolume(const std::string& alias, float volume);
	void clear()
	{
		m_soundEffects.clear();
		m_soundEffectsInstance.clear();
	}
private:
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffect>> m_soundEffects;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> m_soundEffectsInstance;
};
