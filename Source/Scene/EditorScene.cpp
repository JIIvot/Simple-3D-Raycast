#include "EditorScene.h"

EditorScene::EditorScene( Map& map )
	: IScene( map )
	, m_controller( map )
	, m_renderer( m_controller, map )
{
}
