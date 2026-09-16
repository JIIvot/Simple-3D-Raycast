#pragma once

#include <glm/vec2.hpp>

class Map;

class GameplayController
{
public:
	GameplayController( const Map& map );

	void Update();

	[[nodiscard]]
	glm::vec2 GetPosition() const
	{
		return m_position;
	}

	[[nodiscard]]
	float GetAngle() const
	{
		return m_angle;
	}

	[[nodiscard]]
	float GetHeadHeight() const
	{
		return m_headHeight;
	}

private:
	void ProcessInput();

	void UpdateAngle();

	void UpdateTargetCoords();

	void UpdatePosition();

	void UpdateHeadHeight();

private:
	const Map& m_map;

	glm::ivec2 m_targetCoords;
	glm::vec2 m_position;

	float m_angle = 0.0f;
	float m_targetAngle = 0.0f;

	float m_headHeight;
	float m_walkingTime = 0.0f;

	glm::ivec2 m_inputVector = { 0, 0 };
};
