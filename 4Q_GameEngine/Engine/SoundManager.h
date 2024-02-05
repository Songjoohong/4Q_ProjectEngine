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
	void CreateSound(std::string name, bool isLoop);
	void PlayBackSound(std::string name);
	void SetPaused(std::string name, bool isPlaying);
	void SetVolume(std::string name, float vol);
	void RemoveChannel(std::string name);
	void StopSound(std::string name);

private:
	FMOD::System* m_System = nullptr;
	std::unordered_map<std::string, FMOD::Channel*> m_Channels;
	std::unordered_map<std::string, FMOD::Sound*> m_Sounds;
	std::vector<FMOD::ChannelGroup*> m_ChannelGroups;
	void* m_ExtraDriverData = nullptr;
	FMOD_RESULT m_Result = FMOD_OK;

	int m_MaxChannel = 32;
	std::string m_ResourcePath = "../Resource/Sound/";
};

