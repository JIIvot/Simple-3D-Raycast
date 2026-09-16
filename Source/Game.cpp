#include "Game.h"
#include "Constants.h"
#include "Input.h"
#include "Scene/EditorScene.h"

#include <SDL3/SDL.h>

static constexpr char c_gameTitle[] = "Raycast-Game";

static Game* s_instance;

void Game::Init()
{
	s_instance = new Game;
}

Game& Game::GetInstance()
{
	return *s_instance;
}

void Game::Destroy()
{
	delete s_instance;
	s_instance = nullptr;
}

Game::Game()
{
	if ( !SDL_Init( SDL_INIT_VIDEO ) )
	{
		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, c_gameTitle, "Failed to initialize SDL3!", nullptr );
		return;
	}

	m_window = SDL_CreateWindow( c_gameTitle, 1280, 720, SDL_WINDOW_RESIZABLE );
	if ( !m_window )
	{
		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, c_gameTitle, "Failed to create a window!", nullptr );
		return;
	}

	m_renderer = SDL_CreateRenderer( m_window, nullptr );
	if ( !m_renderer )
	{
		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, c_gameTitle, "Failed to create a renderer!", nullptr );
		return;
	}

	SDL_SetRenderVSync( m_renderer, 1 );
	SDL_SetRenderDrawBlendMode( m_renderer, SDL_BLENDMODE_BLEND );

	m_perfFrequency = static_cast<float>( SDL_GetPerformanceFrequency() );
	m_input = std::make_unique<Input>();
	m_shouldRun = true;
}

Game::~Game()
{
	m_scene.reset();
	m_input.reset();

	SDL_DestroyRenderer( m_renderer );
	SDL_DestroyWindow( m_window );
	SDL_Quit();
}

void Game::Update()
{
	UpdateDeltaTime();

	m_input->Update();
	ProcessEvents();
	HandleCommonActions();

	if ( m_requestedSceneChange )
	{
		m_requestedSceneChange();
		m_requestedSceneChange = nullptr;
	}

	m_scene->Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
	SDL_RenderClear( m_renderer );

	m_scene->Render();

	SDL_RenderPresent( m_renderer );
}

void Game::UpdateDeltaTime()
{
	const uint64_t currCounter = SDL_GetPerformanceCounter();
	m_deltaTime = static_cast<float>( currCounter - m_lastPerfCounter ) / m_perfFrequency;
	m_lastPerfCounter = currCounter;
}

void Game::ProcessEvents()
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_EVENT_QUIT:
			Quit();
			break;

		case SDL_EVENT_MOUSE_WHEEL:
			m_input->OnMouseScrolled( event.wheel.y );
			break;
		}
	}
}

void Game::HandleCommonActions()
{
	if ( m_input->IsKeyJustPressed( SDL_SCANCODE_ESCAPE ) )
	{
		Quit();
	}

	if ( m_input->IsKeyJustPressed( SDL_SCANCODE_F11 ) )
	{
		ToggleFullscreen();
	}
}

void Game::ToggleFullscreen()
{
	m_isFullscreen = !m_isFullscreen;
	SDL_SetWindowFullscreen( m_window, m_isFullscreen );
}

void Game::Run()
{
	RequestSceneChange<EditorScene>();
	m_lastPerfCounter = SDL_GetPerformanceCounter();

	while ( m_shouldRun )
	{
		Update();
		Render();
	}
}
