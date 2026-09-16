#pragma once

#include <cstdint>
#include <vector>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_mouse.h>
#include <glm/vec2.hpp>

class Input
{
public:
	Input();

	void Update();

	void OnMouseScrolled( float amount )
	{
		m_mouseScrollDelta += amount;
	}

	[[nodiscard]]
	bool IsKeyPressed( SDL_Scancode code ) const
	{
		return m_currKeys[code];
	}

	[[nodiscard]]
	bool IsKeyJustPressed( SDL_Scancode code ) const
	{
		return m_currKeys[code] && !m_lastKeys[code];
	}

	[[nodiscard]]
	bool IsButtonPressed( uint32_t button ) const
	{
		return m_currButtonFlags & SDL_BUTTON_MASK( button );
	}

	[[nodiscard]]
	bool IsButtonJustPressed( uint32_t button ) const
	{
		return ( m_currButtonFlags & SDL_BUTTON_MASK( button ) ) && !( m_lastButtonFlags & ( SDL_BUTTON_MASK( button ) ) );
	}

	[[nodiscard]]
	glm::vec2 GetMousePosition() const
	{
		return m_mousePosition;
	}

	[[nodiscard]]
	float GetMouseScrollDelta() const
	{
		return m_mouseScrollDelta;
	}

private:
	int32_t m_numKeys;
	const bool* m_currKeys;
	std::vector<uint8_t> m_lastKeys;

	SDL_MouseButtonFlags m_currButtonFlags = 0;
	SDL_MouseButtonFlags m_lastButtonFlags = 0;
	glm::vec2 m_mousePosition = glm::vec2( 0.0f );
	float m_mouseScrollDelta = 0.0f;
};
