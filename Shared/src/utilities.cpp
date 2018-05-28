#include "utilities.h"

namespace dev {

	void Log::setLevel(Level level){
		m_Log_Level = level;
	}

	void Log::error(const char * message){
		if (m_Log_Level >= LevelError)
			std::cout << "[ERROR]: " << message << std::endl;
	}

	void Log::warning(const char * message) {
		if (m_Log_Level >= LevelWarning)
			std::cout << "[WARNING]: " << message << std::endl;
	}

	void Log::info(const char * message) {
		if (m_Log_Level >= LevelInfo)
			std::cout << "[INFO]: " << message << std::endl;
	}
	
	Log log;
	

















}