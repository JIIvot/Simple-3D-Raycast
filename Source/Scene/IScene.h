#pragma once

class Map;

struct IScene
{
	IScene( Map& map )
	{
	}

	virtual ~IScene() = default;

	virtual void Update() = 0;

	virtual void Render() = 0;
};
