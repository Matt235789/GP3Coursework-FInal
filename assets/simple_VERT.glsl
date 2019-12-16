#version 400

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 normal;


out vec3 position;

out vec2 texCoords0;


uniform mat4 MVP;

void main()
{
	
	gl_Position = MVP * vec4(pos, 1.0f);
	texCoords0 = texCoords;
	
}