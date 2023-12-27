#pragma once
#include "../Engine/Engine.h"

class GameApp : public Engine
{
public:
	GameApp(HINSTANCE hInstance);
	virtual ~GameApp() override;

	bool Initialize(UINT Width, UINT Height) override;
	virtual void Update() override;
	virtual void Render() override;
};