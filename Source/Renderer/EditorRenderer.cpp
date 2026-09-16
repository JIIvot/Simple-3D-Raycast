#include "EditorRenderer.h"
#include "Controller/EditorController.h"
#include "Game.h"
#include "CellType.h"

#include <SDL3/SDL_render.h>
#include <cstdint>

static constexpr int32_t c_gridSizePixels = 2;
static constexpr int32_t c_spawnSizePixels = 10;

static constexpr SDL_Color c_backgroundColor = { 21, 21, 21, 255 };
static constexpr SDL_Color c_gridColor = { 65, 65, 65, 255 };
static constexpr SDL_Color c_hoveredColor = { 255, 255, 255, 80 };
static constexpr SDL_Color c_rayColor = { 94, 233, 233, 255 };
static constexpr SDL_Color c_spawnColor = { 8, 178, 59, 255 };

static constexpr SDL_Color c_cellColors[eCellType_Count] = {
	{ 0, 0, 0, 0 },    // None, will be skipped by the renderer anyway
	{ 218, 36, 36, 255 } // Wall
};

EditorRenderer::EditorRenderer( const EditorController& controller, const Map& map )
	: m_controller( controller )
	, m_map( map )
{
	SDL_SetRenderLogicalPresentation( Game::GetInstance().GetRenderer(), c_editorWidthPixels, c_editorHeightPixels, SDL_LOGICAL_PRESENTATION_LETTERBOX );
}

void EditorRenderer::Render() const
{
	RenderBackground();
	RenderMapCells();
	RenderPlayerSpawn();

	if ( !m_controller.IsMousePositionValid() )
	{
		RenderGrid();
		return;
	}

	if ( m_controller.HasRaysEnabled() )
	{
		RenderRays();
		return;
	}

	RenderHoveredCell();
	RenderGrid();
}

void EditorRenderer::RenderBackground()
{
	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();

	SDL_SetRenderDrawColor( renderer, c_backgroundColor.r, c_backgroundColor.g, c_backgroundColor.b, c_backgroundColor.a );
	SDL_RenderFillRect( renderer, nullptr );
}

void EditorRenderer::RenderGrid()
{
	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();
	SDL_SetRenderDrawColor( renderer, c_gridColor.r, c_gridColor.g, c_gridColor.b, c_gridColor.a );

	/* Vertical lines */
	SDL_FRect rect = {
		.x = 0,
		.w = c_editorWidthPixels,
		.h = c_gridSizePixels
	};

	for ( int32_t y = 1; y < c_mapHeightCells; ++y )
	{
		rect.y = y * c_cellSizePixels - static_cast<float>( c_gridSizePixels ) / 2;
		SDL_RenderFillRect( renderer, &rect );
	}

	/* Horizontal lines */
	rect.y = 0;
	rect.w = c_gridSizePixels;
	rect.h = c_editorHeightPixels;

	for ( int32_t x = 1; x < c_mapWidthCells; ++x )
	{
		rect.x = x * c_cellSizePixels - static_cast<float>( c_gridSizePixels ) / 2;
		SDL_RenderFillRect( renderer, &rect );
	}
}

void EditorRenderer::RenderMapCells() const
{
	glm::ivec2 coords;
	for ( coords.x = 0; coords.x < c_mapWidthCells; ++coords.x )
	{
		for ( coords.y = 0; coords.y < c_mapHeightCells; ++coords.y )
		{
			RenderCell( coords );
		}
	}
}

void EditorRenderer::RenderHoveredCell() const
{
	const glm::ivec2 hoveredCoords = m_controller.GetHoveredCellCoords();
	const SDL_FRect rect = {
		.x = static_cast<float>( hoveredCoords.x * c_cellSizePixels ),
		.y = static_cast<float>( hoveredCoords.y * c_cellSizePixels ),
		.w = static_cast<float>( c_cellSizePixels ),
		.h = static_cast<float>( c_cellSizePixels )
	};

	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();
	SDL_SetRenderDrawColor( renderer, c_hoveredColor.r, c_hoveredColor.g, c_hoveredColor.b, c_hoveredColor.a );
	SDL_RenderFillRect( renderer, &rect );
}

void EditorRenderer::RenderCell( glm::ivec2 coords ) const
{
	const CellType cellType = m_map.GetCellType( coords );
	if ( cellType == eCellType_None )
	{
		return;
	}

	const SDL_Color& color = c_cellColors[static_cast<int32_t>( cellType )];
	const SDL_FRect rect = {
		.x = static_cast<float>( coords.x * c_cellSizePixels ),
		.y = static_cast<float>( coords.y * c_cellSizePixels ),
		.w = static_cast<float>( c_cellSizePixels ),
		.h = static_cast<float>( c_cellSizePixels )
	};

	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
	SDL_RenderFillRect( renderer, &rect );
}

void EditorRenderer::RenderPlayerSpawn() const
{
	const glm::ivec2 coords = m_map.GetSpawnCoords();
	const SDL_FRect rect = {
		.x = coords.x * c_cellSizePixels + ( c_cellSizePixels - c_spawnSizePixels ) * 0.5f,
		.y = coords.y * c_cellSizePixels + ( c_cellSizePixels - c_spawnSizePixels ) * 0.5f,
		.w = c_spawnSizePixels,
		.h = c_spawnSizePixels
	};

	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();
	SDL_SetRenderDrawColor( renderer, c_spawnColor.r, c_spawnColor.g, c_spawnColor.b, c_spawnColor.a );
	SDL_RenderFillRect( renderer, &rect );
}

void EditorRenderer::RenderRays() const
{
	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();

	const glm::vec2 mousePosition = m_controller.GetMousePosition();
	const glm::vec2 startPosition = mousePosition / c_editorPixelsPerMeter;

	float angle = m_controller.GetRaysAngle() - c_horizontalFov * 0.5f;
	for ( int32_t i = 0; i < c_gameWidthPixels; ++i )
	{
		const Ray ray = m_map.CastRay( startPosition, angle );
		const glm::vec2 endPosition = ray.endPosition * c_editorPixelsPerMeter;

		SDL_SetRenderDrawColor( renderer, c_rayColor.r, c_rayColor.g, c_rayColor.b, c_rayColor.a );
		SDL_RenderLine( renderer, mousePosition.x, mousePosition.y, endPosition.x, endPosition.y );

		angle += c_angleBetweenRays;
	}
}
