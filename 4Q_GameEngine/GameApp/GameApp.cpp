#include "pch.h"
#include "GameApp.h"

GameApp::GameApp(HINSTANCE hInstance)
	:Engine(hInstance)
{

}

GameApp::~GameApp()
{

}

bool GameApp::Initialize(UINT Width, UINT Height)
{
	const bool result = __super::Initialize(Width, Height);
	if (!result)
		return result;


	return true;
}

void GameApp::Update()
{
	__super::Update();
}

void GameApp::Render()
{
	__super::Render();
}
