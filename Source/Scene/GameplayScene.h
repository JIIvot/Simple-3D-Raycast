#pragma once

#include "IScene.h"
#include "Controller/GameplayController.h"
#include "Renderer/GameplayRenderer.h"

class GameplayScene final : public IScene
{
public:
	GameplayScene( Map& map );

	void Update() override
	{
		m_controller.Update();
	}

	void Render() override
	{
		m_renderer.Render();
	}

private:
	GameplayController m_controller;
	GameplayRenderer m_renderer;
};
