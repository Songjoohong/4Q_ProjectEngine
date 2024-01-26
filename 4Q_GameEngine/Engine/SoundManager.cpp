#include "pch.h"
#include "SoundManager.h"

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
	FMOD::Channel* newChannel = nullptr;

	const std::string finalPath = m_ResourcePath + name;
	m_Result = m_System->createSound(finalPath.c_str(), isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF, nullptr, &newSound);
	assert(m_Result == FMOD_OK);
	m_Sounds[name] = newSound;
	m_Channels[name] = newChannel;
}

void SoundManager::PlayBackSound(const char* name)
{
	m_Result = m_System->playSound(m_Sounds[name], nullptr, false, &m_Channels[name]);
	assert(m_Result == FMOD_OK);
}

