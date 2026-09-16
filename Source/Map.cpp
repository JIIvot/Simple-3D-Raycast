#include "Map.h"
#include "MathUtil.h"

#include <glm/gtx/norm.hpp>
#include <cmath>

static constexpr int32_t c_rayLengthCells = 100;

Ray Map::CastRay( glm::vec2 startPosition, float angleDeg ) const
{
	Ray ray = {
		.isHit = false,
		.endPosition = startPosition
	};

	const float angleRad = MathUtil::ToRadians( angleDeg );
	const float angleSin = std::sinf( angleRad );
	const float angleCos = std::cosf( angleRad );
	const float angleTan = angleSin / angleCos;

	glm::ivec2 cellStep = { 1, 1 };
	glm::ivec2 coordStep = { 1, 1 };

	if ( angleCos < 0.0f )
	{
		cellStep.x = 0;
		coordStep.x = -1;
	}

	if ( angleSin < 0.0f )
	{
		cellStep.y = 0;
		coordStep.y = -1;
	}

	glm::ivec2 coords = {
		static_cast<int32_t>( ray.endPosition.x / c_cellSizeMeters ),
		static_cast<int32_t>( ray.endPosition.y / c_cellSizeMeters )
	};

	int32_t cellsVisited = 0;
	while ( !ray.isHit && cellsVisited < c_rayLengthCells )
	{
		const float horizontalDeltaX = ( coords.x + cellStep.x ) * c_cellSizeMeters - ray.endPosition.x;
		const float horizontalDeltaY = horizontalDeltaX * angleTan;

		const float verticalDeltaY = ( coords.y + cellStep.y ) * c_cellSizeMeters - ray.endPosition.y;
		const float verticalDeltaX = verticalDeltaY / angleTan;

		const float horizontalDeltaSquared = horizontalDeltaX * horizontalDeltaX + horizontalDeltaY * horizontalDeltaY;
		const float verticalDeltaSquared = verticalDeltaX * verticalDeltaX + verticalDeltaY * verticalDeltaY;

		if ( horizontalDeltaSquared < verticalDeltaSquared )
		{
			ray.endPosition.x += horizontalDeltaX;
			ray.endPosition.y += horizontalDeltaY;
			coords.x += coordStep.x;
		}
		else
		{
			ray.endPosition.x += verticalDeltaX;
			ray.endPosition.y += verticalDeltaY;
			coords.y += coordStep.y;
		}

		ray.isHit = !IsInMapBounds( coords ) || GetCellType( coords ) != eCellType_None;
		++cellsVisited;
	}

	return ray;
}

void Map::SetSpawnCoords( glm::ivec2 coords )
{
	SetCellType( coords, eCellType_None );
	m_spawnCoords = coords;
}
