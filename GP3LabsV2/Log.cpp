#include "pch.h"
#include "Log.h"
#include "Application.h"



char selectedType = 'a';

void Log::Debug(const std::string& msg, errType logType, const char* file, int line)
{

	static int count = 0;

	if (count == 0) {
		std::cout << "What type of logs do you want to see? a = all, e = error, w = warning, t = trace n = none (default is all)" << std::endl;

		//std::cin >> selectedType;
		count++;
	}




	if (selectedType == 'a') {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (selectedType == 'e' && logType == errType::ERROR) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (selectedType == 't' && logType == errType::TRACE) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (selectedType == 'w' && logType == errType::WARNING) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (selectedType == 'n') {

	}
}

bool Log::CheckGLError(const char* file, int line)
{
	GLenum err = glGetError();
	bool errorFound = false;
	while (err != GL_NO_ERROR)
	{

		switch (err) {
		case(GL_NO_ERROR):
			break;

		case (GL_INVALID_ENUM):
			//LOG_DEBUG(err);
			break;
		}



		std::cout << "GL ERROR(" << file << " ," << line << "): " <<
			glewGetErrorString(err) << std::endl;
		errorFound = true;
	}


	return errorFound;
}
