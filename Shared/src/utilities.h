#pragma once
#include <iostream>

namespace dev {

	class Log {
	public:
		enum Level { LevelError = 0, LevelWarning, LevelInfo };
	private:
		Level m_Log_Level = LevelInfo;

	public:
		void setLevel(Level level);
		void error(const char* message);
		void warning(const char* message);
		void info(const char* message);
	};

	extern Log log;

}