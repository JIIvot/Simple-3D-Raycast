#pragma once

class Map;
class GameplayController;

class GameplayRenderer
{
public:
	GameplayRenderer( const GameplayController& controller, const Map& map );

	void Render();

private:
	void RenderFloor();

	void RenderCeiling();

	void RenderMap();

private:
	const GameplayController& m_controller;
	const Map& m_map;
};
