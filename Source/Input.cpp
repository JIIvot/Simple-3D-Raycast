#include "Input.h"

#include <SDL3/SDL_keyboard.h>

Input::Input()
	: m_currKeys( SDL_GetKeyboardState( &m_numKeys ) )
	, m_lastKeys( m_numKeys )
{
}

void Input::Update()
{
	m_mouseScrollDelta = 0.0f;

	m_lastButtonFlags = m_currButtonFlags;
	m_currButtonFlags = SDL_GetMouseState( &m_mousePosition.x, &m_mousePosition.y );

	memcpy( m_lastKeys.data(), m_currKeys, m_numKeys );
}
