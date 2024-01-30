#include "pch.h"
#include "SoundManager.h"

SoundManager::~SoundManager()
{
	m_System->release();
}

void SoundManager::Initialize()
{
	m_Result = System_Create(&m_System);
	assert(m_Result == FMOD_OK);

	m_Result = m_System->init(m_MaxChannel, FMOD_INIT_NORMAL, m_ExtraDriverData);
	assert(m_Result == FMOD_OK);
}

void SoundManager::Update() const
{
	m_System->update();
}

void SoundManager::CreateSound(const char* name, const bool isLoop)
{
	FMOD::Sound* newSound = nullptr;
	const std::string finalPath = m_ResourcePath + name;
	m_Result = m_System->createSound(finalPath.c_str(), isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF, nullptr, &newSound);
	assert(m_Result == FMOD_OK);
	m_Sounds[name] = newSound;
	
}

void SoundManager::PlayBackSound(const char* name)
{
	auto it = m_Channels.find(name);
	if(it != m_Channels.end())
	{
		m_Result = m_System->playSound(m_Sounds[name], nullptr, false, &m_Channels[name]);
		assert(m_Result == FMOD_OK);
		return;
	}
	FMOD::Channel* newChannel = nullptr;
	m_Result = m_System->playSound(m_Sounds[name], nullptr, false, &newChannel);
	assert(m_Result == FMOD_OK);
	m_Channels[name] = newChannel;
}

void SoundManager::SetPaused(const char* name, bool isPlaying)
{
	m_Channels[name]->setPaused(!isPlaying);
}

void SoundManager::SetVolume(const char* name, const float vol)
{
	m_Channels[name]->setVolume(vol);
}

void SoundManager::RemoveChannel(const char* name)
{
	m_Channels[name]->stop();
}



