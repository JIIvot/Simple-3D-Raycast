#include "GameplayRenderer.h"
#include "Controller/GameplayController.h"
#include "Game.h"
#include "MathUtil.h"

#include <SDL3/SDL_render.h>
#include <glm/geometric.hpp>

static constexpr float c_cellHeightMeters = 2.7f;

static constexpr SDL_Color c_floorColor = { 30, 9, 13, 255 };
static constexpr SDL_Color c_ceilingColor = { 24, 49, 167, 255 };
static constexpr SDL_Color c_wallColor = { 218, 36, 36, 255 };

static void RenderFloor()
{
	static constexpr SDL_FRect c_rect = {
		.x = 0.0f,
		.y = static_cast<float>( c_gameHeightPixels ) * 0.5f,
		.w = c_gameWidthPixels,
		.h = static_cast<float>( c_gameHeightPixels ) * 0.5f
	};

	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();
	SDL_SetRenderDrawColor( renderer, c_floorColor.r, c_floorColor.g, c_floorColor.b, c_floorColor.a );
	SDL_RenderFillRect( renderer, &c_rect );
}

static void RenderCeiling()
{
	static constexpr SDL_FRect c_rect = {
		.x = 0.0f,
		.y = 0.0f,
		.w = c_gameWidthPixels,
		.h = static_cast<float>( c_gameHeightPixels ) * 0.5f
	};

	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();
	SDL_SetRenderDrawColor( renderer, c_ceilingColor.r, c_ceilingColor.g, c_ceilingColor.b, c_ceilingColor.a );
	SDL_RenderFillRect( renderer, &c_rect );
}

GameplayRenderer::GameplayRenderer( const GameplayController& controller, const Map& map )
	: m_controller( controller )
	, m_map( map )
{
	SDL_SetRenderLogicalPresentation( Game::GetInstance().GetRenderer(), c_gameWidthPixels, c_gameHeightPixels, SDL_LOGICAL_PRESENTATION_LETTERBOX );
}

void GameplayRenderer::Render() const
{
	RenderCeiling();
	RenderFloor();
	RenderMap();
}

void GameplayRenderer::RenderMap() const
{
	SDL_Renderer* const renderer = Game::GetInstance().GetRenderer();

	const glm::vec2 position = m_controller.GetPosition();
	const float angle = m_controller.GetAngle();
	const float headHeight = m_controller.GetHeadHeight();

	float angleDelta = -c_horizontalFov * 0.5f;
	for ( int32_t i = 0; i < c_gameWidthPixels; ++i )
	{
		const Ray ray = m_map.CastRay( position, angle + angleDelta );
		if ( !ray.isHit )
		{
			angleDelta += c_angleBetweenRays;
			continue;
		}

		const float distance = glm::length( ray.endPosition - position ) * std::cosf( MathUtil::ToRadians( angleDelta ) );
		const float viewHeight = distance * c_halfVerticalFovTan;
		const float coverageFactor = std::min( 1.0f, c_cellHeightMeters / viewHeight );
		const float colorFactor = std::max( 0.3f, coverageFactor );

		const SDL_FRect rect = {
			.x = static_cast<float>( i ),
			.y = std::max( 0.0f, c_gameHeightPixels / viewHeight * ( viewHeight * 0.5f + headHeight - c_cellHeightMeters ) ),
			.w = 1,
			.h = coverageFactor * c_gameHeightPixels
		};

		const SDL_Color color = {
			.r = static_cast<uint8_t>( c_wallColor.r * colorFactor ),
			.g = static_cast<uint8_t>( c_wallColor.g * colorFactor ),
			.b = static_cast<uint8_t>( c_wallColor.b * colorFactor ),
			.a = c_wallColor.a
		};

		SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
		SDL_RenderFillRect( renderer, &rect );

		angleDelta += c_angleBetweenRays;
	}
}
