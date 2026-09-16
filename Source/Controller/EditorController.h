#pragma once

#include <glm/vec2.hpp>

class Map;

class EditorController
{
public:
	EditorController( Map& map )
		: m_map( map )
	{
	}

	void Update();

	[[nodiscard]]
	bool IsMousePositionValid() const
	{
		return m_isMousePositionValid;
	}

	[[nodiscard]]
	glm::vec2 GetMousePosition() const
	{
		return m_mousePosition;
	}

	[[nodiscard]]
	glm::ivec2 GetHoveredCellCoords() const
	{
		return m_hoveredCellCoords;
	}

	[[nodiscard]]
	bool HasRaysEnabled() const
	{
		return m_hasRaysEnabled;
	}

	[[nodiscard]]
	float GetRaysAngle() const
	{
		return m_raysAngle;
	}

private:
	void UpdateHoveredCell();

	void ProcessInput();

	void Reset();

private:
	Map& m_map;

	bool m_isMousePositionValid = false;
	glm::vec2 m_mousePosition = { 0.0f, 0.0f };
	glm::ivec2 m_hoveredCellCoords = { 0, 0 };

	bool m_hasRaysEnabled = false;
	float m_raysAngle = 0.0f;
};
