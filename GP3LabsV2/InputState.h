#pragma once
#include "pch.h"
#include "Common.h"

struct InputState
{
	InputState()
	{
		keys.fill(false);
	}

	std::array<char, 400> keys;
	glm::ivec2 mousePos;

};