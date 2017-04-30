#include "AudioManager.h"

#include "Utility.h"

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

void AudioManager::load(const std::string & filePath, const std::string & alias)
{
	auto soundEffect = std::make_unique<DirectX::SoundEffect>(m_audioEngine.get(), s2ws(filePath).c_str());
	m_soundEffects[alias] = std::move(soundEffect);
}

void AudioManager::play(const std::string& alias, float volume, float pitch)
{
	assert(m_soundEffects.count(alias) != 0);
	m_soundEffects[alias]->Play(volume, pitch, 0);
}
