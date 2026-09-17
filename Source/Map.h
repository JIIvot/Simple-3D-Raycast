#pragma once

#include "CellType.h"
#include "Constants.h"
#include "Ray.h"

#include <array>
#include <glm/vec2.hpp>

class Map
{
public:
	[[nodiscard]]
	static constexpr bool IsInBounds( glm::ivec2 coords )
	{
		return coords.x >= 0 && coords.x < c_mapWidthCells && coords.y >= 0 && coords.y < c_mapHeightCells;
	}

	[[nodiscard]]
	Ray CastRay( glm::vec2 startPosition, float angleDeg ) const;

	[[nodiscard]]
	CellType GetCellType( glm::ivec2 coords ) const
	{
		return m_cells[coords.x][coords.y];
	}

	void SetCellType( glm::ivec2 coords, CellType type )
	{
		m_cells[coords.x][coords.y] = type;
	}

	[[nodiscard]]
	glm::ivec2 GetSpawnCoords() const
	{
		return m_spawnCoords;
	}

	void SetSpawnCoords( glm::ivec2 coords );

private:
	std::array<std::array<CellType, c_mapHeightCells>, c_mapWidthCells> m_cells;
	glm::ivec2 m_spawnCoords = { 0, 0 };
};
