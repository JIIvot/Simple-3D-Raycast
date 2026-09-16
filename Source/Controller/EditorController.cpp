#include "EditorController.h"
#include "Constants.h"
#include "Game.h"
#include "Input.h"
#include "Scene/GameplayScene.h"

#include <SDL3/SDL_render.h>

static constexpr float c_raysAngleRotationSpeed = 8.0f;

void EditorController::Update()
{
	UpdateHoveredCell();
	ProcessInput();
}

void EditorController::UpdateHoveredCell()
{
	const Game& game = Game::GetInstance();

	const glm::vec2 windowMousePosition = game.GetInput().GetMousePosition();
	SDL_RenderCoordinatesFromWindow( game.GetRenderer(), windowMousePosition.x, windowMousePosition.y, &m_mousePosition.x, &m_mousePosition.y);

	const bool isValidX = m_mousePosition.x >= 0 && m_mousePosition.x < c_editorWidthPixels;
	const bool isValidY = m_mousePosition.y >= 0 && m_mousePosition.y < c_editorHeightPixels;

	if ( !isValidX || !isValidY )
	{
		m_isMousePositionValid = false;
		return;
	}

	m_hoveredCellCoords.x = static_cast<int32_t>( m_mousePosition.x / c_cellSizePixels );
	m_hoveredCellCoords.y = static_cast<int32_t>( m_mousePosition.y / c_cellSizePixels );
	m_isMousePositionValid = true;
}

void EditorController::ProcessInput()
{
	Game& game = Game::GetInstance();
	const Input& input = game.GetInput();

	/* Switch scene action (G) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_G ) )
	{
		game.RequestSceneChange<GameplayScene>();
	}

	/* Reset action (LSHIFT + R) */
	if ( input.IsKeyPressed( SDL_SCANCODE_LSHIFT ) && input.IsKeyJustPressed( SDL_SCANCODE_R ) )
	{
		Reset();
	}

	/* Toggle rays action (F) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_F ) )
	{
		m_hasRaysEnabled = !m_hasRaysEnabled;
	}

	if ( !m_isMousePositionValid )
	{
		return;
	}

	/* Rotate rays action (MOUSE WHEEL) */
	if ( m_hasRaysEnabled )
	{
		m_raysAngle -= input.GetMouseScrollDelta() * c_raysAngleRotationSpeed;
	}

	/* Place spawn action (P) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_P ) )
	{
		m_map.SetSpawnCoords( m_hoveredCellCoords );
		return;
	}

	if ( m_map.GetSpawnCoords() == m_hoveredCellCoords )
	{
		return;
	}

	/* Place wall action (LMB) */
	if ( input.IsButtonPressed( SDL_BUTTON_LEFT ) )
	{
		m_map.SetCellType( m_hoveredCellCoords, eCellType_Wall );
	}

	/* Remove wall action (RMB) */
	if ( input.IsButtonPressed( SDL_BUTTON_RIGHT ) )
	{
		m_map.SetCellType( m_hoveredCellCoords, eCellType_None );
	}
}

void EditorController::Reset()
{
	glm::ivec2 coords;
	for ( coords.x = 0; coords.x < c_mapWidthCells; ++coords.x )
	{
		for ( coords.y = 0; coords.y < c_mapHeightCells; ++coords.y )
		{
			m_map.SetCellType( coords, eCellType_None );
		}
	}
}
