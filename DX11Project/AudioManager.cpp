// -----------------------------------------------
// File : AudioManager.cpp
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#include "AudioManager.h"

#include "UtilStr.h"

AudioManager::AudioManager() {
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	DirectX::AUDIO_ENGINE_FLAGS flags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	flags = flags | DirectX::AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<DirectX::AudioEngine>(flags);
}

void AudioManager::update()
{
	if (!m_audioEngine->Update()) {
		if (m_audioEngine->IsCriticalError()) {
			//throw
		}
	}
}

void AudioManager::load(const std::string& filePath, const std::string& alias)
{
	m_soundEffects[alias] = std::make_unique<DirectX::SoundEffect>(m_audioEngine.get(), s2ws(filePath).c_str());
	m_soundEffectsInstance[alias] = m_soundEffects[alias]->CreateInstance();
}

void AudioManager::playMulti(const std::string& alias, float volume, float pitch)
{
	assert(m_soundEffects.count(alias) != 0);
	m_soundEffects[alias]->Play(volume, pitch, 0);
}

void AudioManager::play(const std::string& alias, bool loop, float volume, float pitch)
{
	assert(m_soundEffects.count(alias) != 0);

	//ポーズ中なら再開する
	if (m_soundEffectsInstance[alias]->GetState() == DirectX::SoundState::PAUSED) {
		m_soundEffectsInstance[alias]->Resume();
		return;
	}

	m_soundEffectsInstance[alias]->SetVolume(volume);
	m_soundEffectsInstance[alias]->SetPitch(pitch);
	m_soundEffectsInstance[alias]->Play(loop);
}

void AudioManager::pause(const std::string& alias)
{
	assert(m_soundEffects.count(alias) != 0);
	if (m_soundEffectsInstance[alias]->GetState() == DirectX::SoundState::PLAYING) {
		m_soundEffectsInstance[alias]->Pause();
	}
}

void AudioManager::stop(const std::string& alias)
{
	assert(m_soundEffects.count(alias) != 0);
	if (m_soundEffectsInstance[alias]->GetState() == DirectX::SoundState::PLAYING) {
		m_soundEffectsInstance[alias]->Stop();
	}
}

void AudioManager::setVolume(const std::string& alias, float volume)
{
	assert(m_soundEffects.count(alias) != 0);
	if (m_soundEffectsInstance[alias]->GetState() == DirectX::SoundState::PLAYING) {
		m_soundEffectsInstance[alias]->SetVolume(volume);
	}
}
