#pragma once
#include <unordered_map>

#include "../GameApp/core/inc/fmod.hpp"

#include "ISingleton.h"

namespace ECS
{
	class Entity;
}

class SoundManager : public ISingleton<SoundManager>
{
public:
	SoundManager() = default;
	virtual ~SoundManager() override;

	void Initialize();
	void Update() const;
	void CreateSound(const char* name, bool isLoop);
	void PlayBackSound(const char* name);
	void SetPaused(const char* name, bool isPlaying);
	void SetVolume(const char* name, float vol);
	void RemoveChannel(const char* name);
	void StopSound(const char* name);

private:
	FMOD::System* m_System = nullptr;
	std::unordered_map<const char*, FMOD::Channel*> m_Channels;
	std::unordered_map<const char*, FMOD::Sound*> m_Sounds;
	std::vector<FMOD::ChannelGroup*> m_ChannelGroups;
	void* m_ExtraDriverData = nullptr;
	FMOD_RESULT m_Result = FMOD_OK;

	int m_MaxChannel = 32;
	std::string m_ResourcePath = "../Resource/Sound/";
};

