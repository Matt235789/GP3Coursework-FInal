#pragma once

#ifdef _DEBUG
#define LOG_DEBUG(msg , logType) Log::Debug(msg, logType, __FILE__, __LINE__)
#define GL_ATTEMPT(func) func; \
		if(Log::CheckGLError(__FILE__, __LINE__)) __debugbreak()
#define CHECK_GL_ERROR() Log::CheckGLError(__FILE__, __LINE__)
#else//shouldn't really be outputting to console if this is a release build
#define LOG_DEBUG(msg)
#define GL_ATTEMPT(func) func
#define CHECK_GL_ERROR()
#endif

#include <string>

//​TODO:​ implement different log levels

enum errType
{
	TRACE, ERROR, WARNING
};


class Log
{
private:
	Log();

public:
	static void Debug(const std::string& msg, errType logType, const char* file, int line);
	static bool CheckGLError(const char* file, int line);
	///const static char fff = 'n';
};