#include "Game.h"

int main()
{
	Game::Init();
	Game::GetInstance().Run();
	Game::Destroy();
}
