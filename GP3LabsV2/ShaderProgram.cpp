#include "pch.h"
#include "ShaderProgram.h"
#include "Log.h"

ShaderProgram::ShaderProgram(const std::string& vShaderFile, const std::string& fShaderFile)
{
	m_vShader = new Shader(vShaderFile, VERTEX_SHADER);
	m_fShader = new Shader(fShaderFile, FRAGMENT_SHADER);
	CHECK_GL_ERROR();

	m_program = glCreateProgram();
	glAttachShader(m_program, m_vShader->Get());
	glAttachShader(m_program, m_fShader->Get());

	Link();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}

void ShaderProgram::BindAttribLoc(GLuint loc, const char* name)
{
	glBindAttribLocation(m_program, loc, name);
	CHECK_GL_ERROR();
}

void ShaderProgram::Link()
{
	//link shaders and attribs
	glLinkProgram(m_program);
	CheckForLinkErrors();
	CHECK_GL_ERROR();

	//if they were linked w no longer need them
	delete m_vShader;
	delete m_fShader;
}

void ShaderProgram::Use()
{
	//TODO : Only bind program if it isn't already bound
	static GLuint* progPointer;
	if (progPointer != &m_program) {
		progPointer = &m_program;
		glUseProgram(*progPointer);
	}

}

bool ShaderProgram::CheckForLinkErrors()
{
	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint length = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

		char* msg = (char*)malloc(length);
		glGetProgramInfoLog(m_program, length, &length, msg);
		free(msg);

		glDeleteProgram(m_program);
		return true;
	}
	return false;
}