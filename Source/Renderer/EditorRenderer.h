#pragma once

#include <glm/vec2.hpp>

class Map;
class EditorController;

class EditorRenderer
{
public:
	EditorRenderer( const EditorController& controller, const Map& map );

	void Render() const;

private:
	static void RenderBackground();

	static void RenderGrid();

	void RenderMapCells() const;

	void RenderHoveredCell() const;

	void RenderCell( glm::ivec2 coords ) const;

	void RenderPlayerSpawn() const;

	void RenderRays() const;

private:
	const EditorController& m_controller;
	const Map& m_map;
};
