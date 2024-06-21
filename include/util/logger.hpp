#pragma once

#include <cstdio>

class Logger
{
	public:

		/* log if input log level is equal or smaller to log level set */
		template <typename... Args>
		static void log(unsigned int logLevel, Args ... args)
		{
			if (logLevel <= _logLevel)
			{
			std::printf(args ...);
			}
		}

		static void setLogLevel(unsigned int inLogLevel)
		{
			inLogLevel <= 9 ? _logLevel = inLogLevel : _logLevel = 9;
		}

	private:
		static unsigned int _logLevel;
};