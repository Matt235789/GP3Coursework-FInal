#pragma once
#include "pch.h"
#include "Common.h"

class Texture
{
private:
	GLuint m_texture;
	std::string m_directory;

	static GLuint Load(const std::string& directory);
public:
	Texture(std::string path);
	void Bind();

};
