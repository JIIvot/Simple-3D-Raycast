#include "GameplayController.h"
#include "Game.h"
#include "Input.h"
#include "Scene/EditorScene.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <numbers>

static constexpr float c_lerpError = 0.01f;

static constexpr float c_angleLerpSpeed = 10.0f;
static constexpr float c_angleRotateAmount = 90.0f;

static constexpr float c_positionError = 0.1f;
static constexpr float c_walkSpeed = 10.0f;

static constexpr float c_headHeightMeters = 1.8f;
static constexpr float c_headBobAmplitudeMeters = 0.1f;
static constexpr float c_headBobSpeed = std::numbers::pi_v<float> / ( c_cellSizeMeters / c_walkSpeed );
static constexpr float c_headBobReturnLerpSpeed = 5.0f;

[[nodiscard]]
static constexpr glm::vec2 GetCellCenterPosition( glm::ivec2 coords )
{
	return { c_cellSizeMeters * ( coords.x + 0.5f ), c_cellSizeMeters * ( coords.y + 0.5f ) };
}

[[nodiscard]]
static float NormalizeAngle( float angleDeg )
{
	float normalizedAngle = std::fmodf( angleDeg, 360.0f );
	if ( normalizedAngle < 0.0f )
	{
		normalizedAngle += 360.0f;
	}

	return normalizedAngle;
}

GameplayController::GameplayController( const Map& map )
	: m_map( map )
	, m_targetCoords( map.GetSpawnCoords() )
	, m_position( GetCellCenterPosition( m_targetCoords ) )
	, m_headHeight( c_headHeightMeters )
{
}

void GameplayController::Update()
{
	ProcessInput();
	UpdateAngle();
	UpdateTargetCoords();
	UpdatePosition();
	UpdateHeadHeight();
}

void GameplayController::ProcessInput()
{
	Game& game = Game::GetInstance();
	const Input& input = game.GetInput();

	m_inputVector.x = 0;
	m_inputVector.y = 0;

	/* Switch scene action (G) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_G ) )
	{
		game.RequestSceneChange<EditorScene>();
	}

	/* Rotate clock wise action (E) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_E ) )
	{
		m_targetAngle += c_angleRotateAmount;
	}

	/* Rotate counter clock wise action (Q) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_Q ) )
	{
		m_targetAngle -= c_angleRotateAmount;
	}

	/* Move forward action (W) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_W ) )
	{
		m_inputVector.y += 1;
	}

	/* Move backward action (S) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_S ) )
	{
		m_inputVector.y -= 1;
	}

	/* Move right action (D) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_D ) )
	{
		m_inputVector.x += 1;
	}

	/* Move left action (A) */
	if ( input.IsKeyJustPressed( SDL_SCANCODE_A ) )
	{
		m_inputVector.x -= 1;
	}
}

void GameplayController::UpdateAngle()
{
	if ( m_angle == m_targetAngle )
	{
		return;
	}

	const float t = 1.0f - std::expf( -c_angleLerpSpeed * Game::GetInstance().GetDeltaTime() );
	m_angle = std::lerp( m_angle, m_targetAngle, t );

	if ( std::fabsf( m_angle - m_targetAngle ) <= c_lerpError )
	{
		m_angle = m_targetAngle;
	}
}

void GameplayController::UpdateTargetCoords()
{
	if ( m_inputVector.x == 0 && m_inputVector.y == 0 )
	{
		return;
	}

	const int32_t direction = std::lround( NormalizeAngle( m_targetAngle ) / 90.0f );
	glm::ivec2 desiredCoords = m_targetCoords;

	switch ( direction )
	{
	case 0:
		desiredCoords.x += m_inputVector.y;
		desiredCoords.y += m_inputVector.x;
		break;

	case 1:
		desiredCoords.x -= m_inputVector.x;
		desiredCoords.y += m_inputVector.y;
		break;

	case 2:
		desiredCoords.x -= m_inputVector.y;
		desiredCoords.y -= m_inputVector.x;
		break;

	case 3:
		desiredCoords.x += m_inputVector.x;
		desiredCoords.y -= m_inputVector.y;
		break;
	}

	if ( Map::IsInBounds( desiredCoords ) && m_map.GetCellType( desiredCoords ) == eCellType_None )
	{
		m_targetCoords = desiredCoords;
	}
}

void GameplayController::UpdatePosition()
{
	const glm::vec2 targetPosition = GetCellCenterPosition( m_targetCoords );
	if ( targetPosition == m_position )
	{
		return;
	}

	const float deltaTime = Game::GetInstance().GetDeltaTime();
	const glm::vec2 positionDelta = targetPosition - m_position;
	const glm::vec2 direction = glm::normalize( positionDelta );

	m_position += c_walkSpeed * deltaTime * direction;
	m_walkingTime += deltaTime;

	if ( glm::length2( positionDelta ) <= c_positionError * c_positionError )
	{
		m_position = targetPosition;
		m_walkingTime = 0.0f;
	}
}

void GameplayController::UpdateHeadHeight()
{
	if ( m_walkingTime != 0.0f )
	{
		m_headHeight = c_headHeightMeters + c_headBobAmplitudeMeters * sinf( c_headBobSpeed * m_walkingTime );
		return;
	}

	if ( m_headHeight == c_headHeightMeters )
	{
		return;
	}
	
	const float t = 1.0f - std::expf( -c_headBobReturnLerpSpeed * Game::GetInstance().GetDeltaTime() );
	m_headHeight = std::lerp( m_headHeight, c_headHeightMeters, t );

	if ( std::fabsf( m_headHeight - c_headHeightMeters ) <= c_lerpError )
	{
		m_headHeight = c_headHeightMeters;
	}
}
