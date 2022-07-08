#ifndef PRINTVARIABLE_LOAD
#include <iostream>
#include <vector>
#include "../config_object.hpp"
#ifndef WindowsDetermined
#define WindowsDetermined
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define isWindowsToken
#include "windows.h"
static const bool isWindows = true;
#else
static const bool isWindows = false;
#endif
#endif
#define PRINTVARIABLE_LOAD


/*
Description:
	Tool controls output color by warning level.
*/
namespace DITCameraTool 
{
	namespace Tool
	{
		class Display 
		{
		public:
			Display();
			Display(std::string);
			#ifdef isWindowsToken
			HANDLE debug_handle;
			#endif
			std::string debug_mode = "INFO";
			void CPrintf(std::string);
			void PrintVariable(std::string, std::string);
			void PrintVariable(std::string, int);
			void PrintVariable(std::string, float);
			void PrintVairable(std::string, std::vector<std::string>);
			void PrintVariable(std::string, DITCameraTool::Config);
		private:
			std::string DebugPrintFormat(std::string);
			std::string DebugEndFormat();

		};
	}
}

#define _PrintVariable(variable) DITCameraTool::Tool::Display().PrintVariable((std::string)#variable, (variable))
#endif