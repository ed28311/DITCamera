#include "displayer.hpp"

DITCameraTool::Tool::Display::Display()
{
	#ifdef isWindowsBlock
	debugHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	#endif
}

DITCameraTool::Tool::Display::Display(std::string debugString)
{
	debugMode = debugString;
	#ifdef isWindowsBlock
	debugHandle = GetStdHandle(STD_OUTPUT_HANDLE);;
	#endif
}


std::string DITCameraTool::Tool::Display::_debugPrintFormat(std::string debugString)
{
	if (debugString == (std::string)("INFO"))
	{
		if (!isWindows)
		{
			return "\x1b[;33;1m";
		}
		else
		{
			#ifdef isWindowsBlock
			SetConsoleTextAttribute(debugHandle, 14);
			#endif
			return "";
		}
	}
	else if (debugString == (std::string)("WARNING"))
	{
		if (!isWindows)
		{
			return "\x1b[;31;1m";
		}
		else
		{
			#ifdef isWindowsBlock
			SetConsoleTextAttribute(debugHandle, 12);
			#endif
			return "";
		}
	}
	else
	{
		throw std::invalid_argument("Wrong _debugPrintFormat(debugMode) format.");
	}
}

std::string DITCameraTool::Tool::Display::_debugEndFormat()
{
	if (isWindows)
	{
		#ifdef isWindowsBlock
		SetConsoleTextAttribute(debugHandle, 15);
		#endif
		return "";
	}
	else
	{
		return "\033[0m";
	}
}
void DITCameraTool::Tool::Display::printVariable(std::string variableName, std::string variable)
{
	std::string debugString = _debugPrintFormat(debugMode);
	std::cout << debugString << variableName;
	std::string endString = _debugEndFormat();
	std::cout << endString << " = " << variable << std::endl;
}

void DITCameraTool::Tool::Display::printVariable(std::string variableName, int variable)
{
	std::string debugString = _debugPrintFormat(debugMode);
	std::cout << debugString << variableName;
	std::string endString = _debugEndFormat();
	std::cout << endString << " = " << variable << std::endl;
}

void DITCameraTool::Tool::Display::printVariable(std::string variableName, float variable)
{
	std::string debugString = _debugPrintFormat(debugMode);
	std::cout << debugString << variableName;
	std::string endString = _debugEndFormat();
	std::cout << endString << " = " << variable << std::endl;
}

void DITCameraTool::Tool::Display::printVairable(std::string variableName, std::vector<std::string> variable)
{
	std::string debugString = _debugPrintFormat(debugMode);
	std::cout << debugString << variableName;
	std::string endString = _debugEndFormat();
	std::cout << endString << " = [" << std::endl;
	for (std::string element : variable)
	{
		std::cout << element << ", ";
	}
	std::cout << "\b\b]" << std::endl;
}

void DITCameraTool::Tool::Display::printVariable(std::string variableName, DITCameraTool::Config config)
{
	json algorithmConf = config.GetAlgorithmConf();
	json globalConf = config.GetGlobalConf();
	std::string debugString = _debugPrintFormat(debugMode);
	std::cout << debugString << variableName;
	std::string endString = _debugEndFormat();
	std::cout << endString << ":" << std::endl;
	debugString = _debugPrintFormat(debugMode);
	std::cout << debugString << "algorithmConfig";
	endString = _debugEndFormat();
	std::cout << endString << " = " << algorithmConf.dump(4) << std::endl;
	debugString = _debugPrintFormat(debugMode);
	std::cout << debugString << "globalConfig";
	endString = _debugEndFormat();
	std::cout << endString << " = " << globalConf.dump(4) << std::endl;
	debugString = _debugPrintFormat(debugMode);
};
void DITCameraTool::Tool::Display::cprintf(std::string printString)
{

};
