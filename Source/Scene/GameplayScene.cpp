#include "GameplayScene.h"

GameplayScene::GameplayScene( Map& map )
	: IScene( map )
	, m_controller( map )
	, m_renderer( m_controller, map )
{
}
