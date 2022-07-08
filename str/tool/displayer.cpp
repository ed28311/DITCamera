#include "displayer.hpp"

DITCameraTool::Tool::Display::Display()
{
	#ifdef isWindowsToken
	debug_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	#endif
}

DITCameraTool::Tool::Display::Display(std::string debug_string)
{
	debug_mode = debug_string;
	#ifdef isWindowsToken
	debug_handle = GetStdHandle(STD_OUTPUT_HANDLE);;
	#endif
}


std::string DITCameraTool::Tool::Display::DebugPrintFormat(std::string debug_string)
{
	if (debug_string == (std::string)("INFO"))
	{
		if (!isWindows)
		{
			return "\x1b[;33;1m";
		}
		else
		{
			#ifdef isWindowsToken
			SetConsoleTextAttribute(debug_handle, 14);
			#endif
			return "";
		}
	}
	else if (debug_string == (std::string)("WARNING"))
	{
		if (!isWindows)
		{
			return "\x1b[;31;1m";
		}
		else
		{
			#ifdef isWindowsToken
			SetConsoleTextAttribute(debug_handle, 12);
			#endif
			return "";
		}
	}
	else
	{
		throw std::invalid_argument("Wrong DebugPrintFormat(debug_mode) format.");
	}
}

std::string DITCameraTool::Tool::Display::DebugEndFormat()
{
	if (isWindows)
	{
		#ifdef isWindowsToken
		SetConsoleTextAttribute(debug_handle, 15);
		#endif
		return "";
	}
	else
	{
		return "\033[0m";
	}
}
void DITCameraTool::Tool::Display::PrintVariable(std::string variable_name, std::string variable)
{
	std::string debug_string = DebugPrintFormat(debug_mode);
	std::cout << debug_string << variable_name;
	std::string end_string = DebugEndFormat();
	std::cout << end_string << " = " << variable << std::endl;
}

void DITCameraTool::Tool::Display::PrintVariable(std::string variable_name, int variable)
{
	std::string debug_string = DebugPrintFormat(debug_mode);
	std::cout << debug_string << variable_name;
	std::string end_string = DebugEndFormat();
	std::cout << end_string << " = " << variable << std::endl;
}

void DITCameraTool::Tool::Display::PrintVariable(std::string variable_name, float variable)
{
	std::string debug_string = DebugPrintFormat(debug_mode);
	std::cout << debug_string << variable_name;
	std::string end_string = DebugEndFormat();
	std::cout << end_string << " = " << variable << std::endl;
}

void DITCameraTool::Tool::Display::PrintVairable(std::string variable_name, std::vector<std::string> variable)
{
	std::string debug_string = DebugPrintFormat(debug_mode);
	std::cout << debug_string << variable_name;
	std::string end_string = DebugEndFormat();
	std::cout << end_string << " = [" << std::endl;
	for (std::string element : variable)
	{
		std::cout << element << ", ";
	}
	std::cout << "\b\b]" << std::endl;
}

void DITCameraTool::Tool::Display::PrintVariable(std::string variable_name, DITCameraTool::Config config)
{
	json algorithm_config = config.GetAlgorithmConf();
	json global_config = config.GetGlobalConf();
	std::string debug_string = DebugPrintFormat(debug_mode);
	std::cout << debug_string << variable_name;
	std::string end_string = DebugEndFormat();
	std::cout << end_string << ":" << std::endl;
	debug_string = DebugPrintFormat(debug_mode);
	std::cout << debug_string << "algorithmConfig";
	end_string = DebugEndFormat();
	std::cout << end_string << " = " << algorithm_config.dump(4) << std::endl;
	debug_string = DebugPrintFormat(debug_mode);
	std::cout << debug_string << "globalConfig";
	end_string = DebugEndFormat();
	std::cout << end_string << " = " << global_config.dump(4) << std::endl;
	debug_string = DebugPrintFormat(debug_mode);
};
void DITCameraTool::Tool::Display::CPrintf(std::string printString)
{

};
