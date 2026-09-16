#pragma once

#include "Map.h"

#include <cstdint>
#include <memory>
#include <functional>

struct SDL_Window;
struct SDL_Renderer;
struct IScene;
class Input;

class Game
{
public:
	static void Init();

	static Game& GetInstance();

	static void Destroy();

	void Run();

	void Quit()
	{
		m_shouldRun = false;
	}

	template<typename T>
	void RequestSceneChange();

	[[nodiscard]]
	SDL_Renderer* GetRenderer() const
	{
		return m_renderer;
	}

	[[nodiscard]]
	float GetDeltaTime() const
	{
		return m_deltaTime;
	}

	[[nodiscard]]
	const Input& GetInput() const
	{
		return *m_input.get();
	}

private:
	Game();

	~Game();

	void Update();

	void Render();

	void UpdateDeltaTime();

	void ProcessEvents();

	void HandleCommonActions();

	void ToggleFullscreen();

private:
	bool m_shouldRun = false;
	bool m_isFullscreen = false;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	float m_deltaTime = 0.0f;
	float m_perfFrequency = 0.0f;
	uint64_t m_lastPerfCounter = 0;

	std::unique_ptr<Input> m_input;

	std::unique_ptr<IScene> m_scene;
	std::function<void()> m_requestedSceneChange;

	Map m_map;
};

template<typename T>
void Game::RequestSceneChange()
{
	m_requestedSceneChange = [this]()
	{
		m_scene = std::make_unique<T>( m_map );
	};
}
