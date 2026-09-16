#pragma once

#include "IScene.h"
#include "Controller/EditorController.h"
#include "Renderer/EditorRenderer.h"

class EditorScene final : public IScene
{
public:
	EditorScene( Map& map );

	void Update() override
	{
		m_controller.Update();
	}

	void Render() override
	{
		m_renderer.Render();
	}

private:
	EditorController m_controller;
	EditorRenderer m_renderer;
};
